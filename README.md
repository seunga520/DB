# DB
# Car Company Database System

## 1. Project Overview

This project implements a **Car Company Database Management System** using MySQL and C/C++.

The database was designed by transforming an E-R model into a relational schema, normalizing the relations, and implementing the resulting physical schema in MySQL. The C/C++ application connects to the database and provides a menu-driven interface for executing several automobile sales and inventory queries.

## 2. Development Environment

* Database: MySQL 8.0 or later
* Database Design Tool: MySQL Workbench
* Programming Language: C/C++
* Compiler: GCC
* Database Connectivity: MySQL C API / ODBC

## 3. Database Setup

Before running the application, create and populate the MySQL database using the provided SQL files.

1. Start the MySQL server.
2. Execute `schema.sql` to create the database tables and constraints.
3. Execute `sample_data.sql` to insert the sample data.
4. Check that the database has been created successfully.

Example:

```bash
mysql -u <username> -p < schema.sql
mysql -u <username> -p < sample_data.sql
```

## 4. Compile and Run

Compile the source code with GCC/G++ and the required MySQL libraries.

Example:

```bash
g++ main.cpp -o car_company -lmysqlclient
```

Run the program:

```bash
./car_company
```

Make sure that the MySQL server is running and that the database connection information in the source code is configured correctly before execution.

## 5. Main Features

The program provides a menu-driven interface for executing the following database queries:

1. **Sales Trends**

   * Displays sales trends for different brands over the past three years.
   * Sales can be analyzed by year, month, week, customer gender, and income range.

2. **Defective Part Tracking**

   * Finds vehicles containing a specified defective part.
   * Displays the VIN and customer information for affected vehicles.

3. **Top Brands by Revenue**

   * Displays the top two brands based on total sales revenue during the past year.

4. **Top Brands by Unit Sales**

   * Displays the top two brands based on the number of vehicles sold during the past year.

5. **Seasonal Sales Patterns**

   * Identifies the month in which a selected vehicle body style has the highest sales.

6. **Dealer Inventory Efficiency**

   * Finds dealers that keep vehicles in inventory for the longest average period.

7. **Supplier Coverage**

   * Identifies the supplier that provides parts for the largest number of distinct vehicle models.

## 6. Project Structure

```text
StudentID_Project2/
├── docs/
│   ├── logical_schema.png
│   ├── physical_schema.png
│   └── project_report.pdf
├── database/
│   ├── schema.sql
│   └── sample_data.sql
└── src/
    ├── main.cpp (or main.c)
    ├── database.h
    └── README.md
```


