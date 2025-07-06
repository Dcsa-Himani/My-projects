//BANK MANAGEMENT SYSTEM
//A program by HIMANI SHARMA

#include <iostream>
#include <fstream>
#include <cstring>  
using namespace std;

#define ADMIN_PIN "123456"

const int SIZE=31; 

class Account 
{
    int acc_no;
    char name[SIZE];  
    double balance;
public:
    Account()
    {
        acc_no = 0;
        balance = 0.0;
        name[0] ='\0';  
    }

    void getaccountnumber(int accountnumber) 
    { acc_no = accountnumber; }
    int putaccountnumber() const 
    { return acc_no; }

    void getname(const char* accName) 
    { strcpy(name,accName);}
	const char* putname() const 
    { return name; }

    void getbalance(double bal) 
    { balance = bal; }
    double putbalance() const 
    { return balance; }
    
    void createaccount();
    bool deposit(int,double);
    bool withdraw(int,double);
    void transfer();
    void modify();
    void deleteaccount();
    void displayaccount();
    void displayAllaccount();
    void viewTransactionHistory();
};


    void Account :: createaccount() 
    {
        cout << "Enter Account Number: ";
        cin >> acc_no;
        
        bool accountFound = false;
        fstream file("BankAccounts.txt", ios::binary | ios::in | ios::out);
        
        Account acct1;
		while (file.read((char*) &acct1, sizeof(acct1))) 
		{
            if (acct1.putaccountnumber() == acc_no) 
			{
				accountFound = true;
				break;
			}
		}
		file.close();
		
		if(accountFound)
		{
			cout << "Account already exists !!"<<endl;
            return;	
		}
		else
		{
			cin.ignore(); 
        	cout << "Enter Account Holder Name (max "<< sizeof(name)-1 <<" characters): ";
        	cin.getline(name, sizeof(name));  

        	cout << "Enter Initial Balance: ";
        	cin >> balance;
        
        	ofstream outFile("BankAccounts.txt", ios::app | ios::binary);
        	if (!outFile) 
			{
            	cout << "Error opening file!" << endl;
            	return;
        	}

        	outFile.write((char*) this, sizeof(*this));
        	outFile.close();

        	cout << "Account created successfully!" << endl;
			
			ofstream log("TransactionHistory.txt", ios::app);
			log << "\nCreation => A new account created with Acc No.  : " << acc_no  <<endl << endl;
			log.close();	
		}        
    }


    bool Account :: deposit(int inputacc,double deposit) 
	{
        bool accountFound = false;

        fstream file("BankAccounts.txt", ios::in | ios::out | ios::binary);
        if (!file) 
		{
            cout << "Error opening file!" << endl;
            return false;
        }

        Account acct1;
        while (file.read((char*) &acct1, sizeof(acct1))) 
		{
            if (acct1.putaccountnumber() == inputacc) 
			{
                acct1.getbalance(acct1.putbalance() + deposit);
                
                file.seekp(-sizeof(acct1), ios::cur);
                file.write((char*) &acct1, sizeof(acct1));
                accountFound = true;
                break;
            }
        }

        file.close();
        if (!accountFound) 
		{
            cout << "Account not found!" << endl;
        }

        return accountFound;
    }

    
    bool Account :: withdraw(int inputacc,double withdrawal) 
	{
        bool accountFound = false;
        bool sufbal =true;

        fstream file("BankAccounts.txt", ios::in | ios::out | ios::binary);
        if (!file) 
		{
            cout << "Error opening file!" << endl;
            return false;
        }

        Account acct1;
        while (file.read((char*) &acct1, sizeof(acct1))) 
		{
            if (acct1.putaccountnumber() == inputacc) 
			{
                if (acct1.putbalance() >= withdrawal) 
				{
                    acct1.getbalance(acct1.putbalance() - withdrawal);
                    
                    file.seekp(-sizeof(acct1), ios::cur); 
                    file.write((char*) (&acct1), sizeof(acct1));
                    accountFound = true;
                    break;
                } 
				else 
				{
                    cout << "Insufficient balance!" << endl;
                    accountFound = true;
                    sufbal=false;
                    break;
                }
            }
        }
        
		file.close();
        if (!accountFound) 
		{
            cout << "Account not found!" << endl;
        }
        return (accountFound && sufbal);
    }

    
    void Account :: transfer()
    {
    	int acc1;
    	int acc2;
        double transfer;
        bool account1Found=false;
        bool account2Found=false;
        
        cout << "FROM Account Number: ";
        cin >> acc1;
        cout << "TO Account Number: ";
        cin >> acc2;
        cout << "Enter Amount to be transferred : ";
        cin >> transfer;
        
        fstream file("BankAccounts.txt", ios::in | ios::out | ios::binary);
        if (!file) 
		{
            cout << "Error opening file!" << endl;
            return;
        }

        Account acct1;
        while (file.read((char*) &acct1, sizeof(acct1))) 
		{
            if (acct1.putaccountnumber() == acc1) 
                account1Found = true;
			if (acct1.putaccountnumber() == acc2) 
                account2Found = true;
        }

        file.close();
        
        if(!account1Found)
        {
        	cout<<"Account no. "<<acc1<<" not found \nTransfer Failed !!";
        	return;
		}
		if(!account2Found)
        {
        	cout<<"Account no. "<<acc2<<" not found \nTransfer Failed !!";
        	return;
		}
        
        Account acct;
        acct.withdraw(acc1,transfer);
        acct.deposit(acc2,transfer);
        cout<<"Transfer successful !\n";
        
        ofstream log("TransactionHistory.txt", ios::app);
		log << "Transfer => Amount : " << transfer << " transferred From Acc No. : " << acc1 <<" To Acc No. : "<<acc2 << endl;
		log.close();
	}
	
	
	void Account :: modify()
	{
		int inputacc;
        bool accountFound = false;
        bool modify=false;

        cout << "Enter Account Number: ";
        cin >> inputacc;

        fstream file("BankAccounts.txt", ios::binary | ios::in | ios::out);
        if (!file) 
		{
            cout << "Error opening file!" << endl;
            return;
        }

        Account acct1;
        while (file.read((char*) &acct1, sizeof(acct1))) 
		{
            if (acct1.putaccountnumber() == inputacc) 
			{
				int choice,newaccno;
				char newname[SIZE];
				double newbalance;
                cout << "Choose what you want to modify : 1.Name  2.Balance 3.Account No.\n";
                cin>>choice;
                switch(choice)
                {
                	case 1:cout<<"Current name = "<<acct1.name<<endl;
                		   cin.ignore();
						   cout<<"Enter new Name :-";
                		   cin.getline(newname,sizeof(newname));
                		   strcpy(acct1.name,newname);
                		   file.seekp(-sizeof(acct1), ios::cur); 
                    	   file.write((char*) (&acct1), sizeof(acct1));
                    	   cout<<"Name modified successfullly!";
                    	   modify=true;
                		   break;
                	case 2:cout<<"Current balance = "<<acct1.balance<<endl;
						   cout<<"Enter new Balance :-";
                		   cin>>newbalance;
                		   acct1.balance=newbalance;
                		   file.seekp(-sizeof(acct1), ios::cur); 
                    	   file.write((char*) (&acct1), sizeof(acct1));
                    	   cout<<"Balance modified successfullly!";
                    	   modify=true;
                		   break;
                	case 3:cout<<"Current Account no. = "<<acct1.acc_no<<endl;
						   cout<<"Enter new Account no. :-";
                		   cin>>newaccno;
                		   acct1.acc_no=newaccno;
                		   file.seekp(-sizeof(acct1), ios::cur); 
                    	   file.write((char*) (&acct1), sizeof(acct1));
                    	   cout<<"Account No. modified successfullly!";
                    	   modify=true;
                		   break;
                	default:
                		cout<<"Incorrect choice.Choose 1, 2 or 3.";
				}
                accountFound = true;
                break;
            }
        }

        file.close();

        if (!accountFound) 
		{
            cout << "Account not found!" << endl;
        }
        if(modify)
        {
        	ofstream log("TransactionHistory.txt", ios::app);
			log << "\nModification => Acc No. : " << acc_no <<" is modified." <<endl << endl;
			log.close();	
		}        
		
	}
    
    void Account::deleteaccount()
    {
    	int inputacc;
    	bool accountDeleted=false;
    	cout << "Enter Account Number to delete: ";
    	cin >> inputacc;

    	char confirm;
    	cout << "Are you sure you want to delete this account? (y/n): ";
    	cin >> confirm;
    	if (confirm != 'y' && confirm != 'Y') return;

    	ifstream inFile("BankAccounts.txt", ios::binary);
    	ofstream outFile("Temp.txt", ios::binary);
    	
    	Account acct1;
    	while (inFile.read((char*)&acct1, sizeof(acct1))) 
    	{
        	if (acct1.putaccountnumber() != inputacc) 
        	{
            	outFile.write((char*)&acct1, sizeof(acct1));
        	} 
        	else 
        	{
            	accountDeleted = true;
        	}
   		}

    	inFile.close();
    	outFile.close();

    	remove("BankAccounts.txt");
    	rename("Temp.txt", "BankAccounts.txt");

   		if (accountDeleted)
   		{
        	cout << "Account deleted successfully!\n";
        	
        	ofstream log("TransactionHistory.txt", ios::app);
			log << "\nDeletion => Acc No. : " << acc_no <<" is deleted." <<endl << endl;
			log.close();
		}
    	else
        	cout << "Account not found !!\n";
	}	
	
	
    void Account :: displayaccount() 
	{
        int inputacc;
        bool accountFound = false;

        cout << "Enter Account Number: ";
        cin >> inputacc;

        ifstream inFile("BankAccounts.txt", ios::binary);
        if (!inFile) 
		{
            cout << "Error opening file!" << endl;
            return;
        }

        Account acct1;
        while (inFile.read((char*) &acct1, sizeof(acct1))) 
		{
            if (acct1.putaccountnumber() == inputacc) 
			{
                cout << "\nAccount Details:\n";
                cout << "Account Number: " << acct1.putaccountnumber() << endl;
                cout << "Account Holder: " << acct1.putname() << endl;
                cout << "Balance: " << acct1.putbalance() << endl;
                accountFound = true;
                break;
            }
        }

        inFile.close();

        if (!accountFound) 
		{
            cout << "Account not found!" << endl;
        }
    }
    
    
    void Account :: displayAllaccount() 
	{
        ifstream inFile("BankAccounts.txt", ios::binary);
        if (!inFile) 
		{
            cout << "Error opening file!" << endl;
            return;
        }

        Account acct1;
        cout<<"All accounts details :- \n";
        while (inFile.read((char*) &acct1, sizeof(acct1))) 
		{ 
                cout << "Acct No. "<< acct1.putaccountnumber() << " => "<< acct1.putname() << " [ Balance : ";
                cout << acct1.putbalance() << " ]"<<endl;
        }

        inFile.close();
    }
    
    void Account::viewTransactionHistory()
	{
    	ifstream file("TransactionHistory.txt");
    	if (!file)
    	{
        	cout << "No transaction history available." << endl;
        	return;
    	}

    	string line;
    	cout << "\nTransaction History :- \n";
    
   		while (getline(file, line))
    	{
        	cout << line << endl;
    	}

    	file.close();
	}
	
	
	bool verifyPIN() 
	{
    	string input;
    	int attempts = 4;

    	while (attempts != 0) 
    	{
        	system("cls");
    	    cout << "\n\n";
       		cout << "========================================\n";
       		cout << "          BANK MANAGEMENT LOGIN        \n";
        	cout << "========================================\n\n";
        	cout << "      Enter 6-digit PIN to continue     \n";
        	cout << "      Attempts left: " << attempts << "\n\n";
        	cout << "========================================\n";
        	cout << "Enter PIN: ";
        	cin >> input;

        	if (input == ADMIN_PIN) 
        	{
            	cout << "\nAccess Granted. Welcome Admin!\n";
            	system("pause");
            	system("cls");
            	return true;
        	} 
        	else 
        	{
        		if(attempts>1)
            	{
					cout << "\nIncorrect PIN! Please try again.\n";
            		system("pause");
        	 	}
        	 	attempts--;
        	 }
    	}

    	cout << "\nToo many incorrect attempts. Access denied !!\n";
    	return false;
	}	



int main() 
{
    Account user;
    int choice;
  
  	if (!verifyPIN()) 
	{
        cout << "\nExiting program....." << endl;
        return 0;
    }
    
    while (1) 
    {
        cout << "\n---------------------------\n";
		cout << "Bank Management System\n";
        cout << "---------------------------\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Transfer Money\n";
        cout << "5. Modify Account\n";
        cout << "6. Delete Account\n";
        cout << "7. Display Single Account Details\n";
        cout << "8. Display All Account Details\n";
        cout << "9. View Transaction History\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) 
        {
            case 1:
                user.createaccount();
                break;
            case 2:
            	int inputacc;
            	double deposit;
            	bool accountFound;
            	cout << "Enter Account Number: ";
        		cin >> inputacc;
        		cout << "Enter Deposit Amount: ";
        		cin >> deposit;
                accountFound=user.deposit(inputacc,deposit);
                if (accountFound) 
				{
            		cout << "Deposit successful!" << endl;
            		
            		ofstream log("TransactionHistory.txt", ios::app);
					log << "Deposit => Amount : " << deposit << " deposited from Acc No. : " << inputacc << endl;
					log.close();
        		} 
				else 
				{
            		cout << "Deposit Failed !!" << endl;
        		}
                break;
            case 3:
            	double withdrawal;
            	bool success;
            	cout << "Enter Account Number: ";
        		cin >> inputacc;
        		cout << "Enter Withdrawl Amount: ";
        		cin >> withdrawal;
                success=user.withdraw(inputacc,withdrawal);
                 if (success) 
				{
            		cout << "Withdrawal successful!" << endl;
            		
            		ofstream log("TransactionHistory.txt", ios::app);
					log << "Withdraw => Amount : " << withdrawal << " withdrawn from Acc No. : " << inputacc << endl;
					log.close();
        		} 
        		else
        		{
        			cout << "Withdrawal failed !!" << endl;
				}
                break;
            case 4:
                user.transfer();
                break;
            case 5:
                user.modify();
                break;
            case 6:
                user.deleteaccount();
                break;
            case 7:
                user.displayaccount();
                break;
            case 8:
            	user.displayAllaccount();
                break;
            case 9:
            	system("cls");
            	user.viewTransactionHistory();
            	break;
            case 10:
                cout << "Exiting the program...\n";
                exit(0);
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}
//A program by HIMANI SHARMA
