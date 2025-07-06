# 🚌 Bus Management System (RDBMS)

This project implements a relational database system to manage bus operations. It covers data storage, manipulation, and reporting for buses, drivers, routes, schedules, and bookings, demonstrating practical skills in SQL and database design.

## 📌 Features

✅ Database Structure

- **Tables:** `Bus`, `Drivers`, `Route`, `Bookings`, `Schedule`
- Each table is linked using primary and foreign keys to maintain relationships between the tables.

✅ Perform SQL queries for:
- Data retrieval and analysis
- Aggregations like count, average, and sum
- Filtering and sorting
- Joining tables to generate comprehensive reports

✅ Support for advanced SQL operations:
- Grouping with `GROUP BY`
- Aggregations like `COUNT()`, `AVG()`, `MAX()`, `SUM()`
- Set operations with `UNION`, `INTERSECT`, `EXCEPT`
- String and numeric functions like `SUBSTRING()`, `ROUND()`, `UPPER()`

✅ Generate insights such as:
- Total bookings per bus or per day
- Driver assignments and contact details
- Buses sorted by capacity or route distance
- Schedules for buses between specific cities

## 📷 Example Queries

> **Get drivers for AC buses only:**
> 
SELECT D.driver_name, D.contact_number, B.bus_name
FROM Drivers D
JOIN Bus B ON D.bus_id = B.bus_id
WHERE B.bus_type = 'AC';


>**List total bookings per bus:**
>
SELECT B.bus_name, COUNT(Bo.booking_id) AS total_bookings
FROM Bus B
JOIN Bookings Bo ON B.bus_id = Bo.bus_id
GROUP BY B.bus_name;
>
>
> **Find buses with maximum capacity:**
>
SELECT * FROM Bus
WHERE capacity = (SELECT MAX(capacity) FROM Bus);

-----
## 📜 License
© 2025 Himani Sharma. All rights reserved.
This project is provided for viewing purposes only. No permission is granted to copy, modify, distribute, or use this code in any form without explicit written consent from the owner.
