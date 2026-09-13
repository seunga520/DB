# Project 2
20221337_Project2/
├── docs/
│   ├── logical_schema.png
│   ├── physical_schema.png
│   └── project_report.pdf
├── database/
│   ├── schema.sql
│   └── sample_data.sql
└── src/
    └── main.cpp
    └── README.md

## 1. Development Environment

### Operating System
- macOS Tahoe 26.5.1

### Hardware
- MacBook Air (M1, 2020)
- Apple M1
- Memory: 8GB

### Database
- MySQL Server
- MySQL Workbench 8.0.47 Community Edition

### Programming Language
- C++17

### Compiler
- Apple Clang 21.0.0
- Target: arm64-apple-darwin25.5.0

### Development Tools
- Visual Studio Code
- MySQL Workbench

### Database Connection
- MySQL C API (`mysql.h`)
- Database Name: `car_company`


## 2. MySQL C API (mysqlclient) Setup

This project uses the MySQL C API (`mysql.h`) to connect to the MySQL database.

Compile the program using the following command:

```bash
g++ main.cpp -o main \
-I/usr/local/mysql/include \
-L/usr/local/mysql/lib \
-lmysqlclient \
-Wl,-rpath,/usr/local/mysql/lib
```

### Description

- `-I/usr/local/mysql/include`
  - Path to MySQL header files (`mysql.h`)

- `-L/usr/local/mysql/lib`
  - Path to MySQL libraries

- `-lmysqlclient`
  - Link the MySQL Client Library

- `-Wl,-rpath,/usr/local/mysql/lib`
  - Configure runtime library path

## 3. Program Execution

### Step 1. Create Database

### Step 2. Compile the Program

### Step 3. Run the Program

프로그램이 MySQL 데이터베이스에 연결된 후 메인 메뉴가 출력된다.

## 4. Program Usage

프로그램 실행 후 다음과 같은 메뉴가 출력된다.

===== Car Company Database =====

1. Sales Trends
2. Defective Part Tracking
3. Top Brands by Revenue
4. Top Brands by Unit Sales
5. Seasonal Sales Patterns
6. Dealer Inventory Efficiency
7. Supplier Coverage
0. Exit

Select:

원하는 기능의 번호를 입력한 후 Enter를 누르면 해당 쿼리가 실행된다.

### Query Functions

| Menu  | Description |
| 1     | Analyze sales trends over the past 3 years by brand, year, month, week, gender, and income range |
| 2     | Track defective parts and identify affected vehicles and customers |
| 3     | Find the top 2 brands by revenue in the past year |
| 4     | Find the top 2 brands by unit sales in the past year |
| 5     | Identify the month in which a specific body style sells best |
| 6     | Find dealers with the longest average inventory holding period |
| 7     | Find the supplier that provides parts for the largest number of distinct models |
| 0     | Exit the program |

### Example
