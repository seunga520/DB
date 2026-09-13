
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <mysql.h>

/* connection setting */
static const char* DB_HOST   = "127.0.0.1";
static const char* DB_USER   = "admin";
static const char* DB_PASS   = "1234";          
static const char* DB_NAME   = "car_company";
static unsigned    DB_PORT   = 0;

static MYSQL* conn = nullptr;

void printLine(int width = 40) {
    std::cout << std::string(width, '-') << "\n";
}

void printTitle(const std::string& title) {
    printLine();
    std::cout << "  " << title << "\n";
    printLine();
}

void runAndPrint(const std::string& sql) {
    if (mysql_query(conn, sql.c_str())) {
        std::cerr << "[ERROR] " << mysql_error(conn) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    if (!res) {
        if (mysql_field_count(conn) == 0)
            std::cout << "  Rows affected: " << mysql_affected_rows(conn) << "\n";
        else
            std::cerr << "[ERROR] " << mysql_error(conn) << "\n";
        return;
    }

    int numFields = mysql_num_fields(res);
    MYSQL_FIELD* fields = mysql_fetch_fields(res);

    std::vector<std::string> colNames(numFields);
    std::vector<int> widths(numFields);
    for (int i = 0; i < numFields; i++) {
        colNames[i] = fields[i].name ? fields[i].name : "";
        widths[i] = std::max((int)colNames[i].size(), 10);
    }

    std::vector<std::vector<std::string>> rows;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        std::vector<std::string> r;
        unsigned long* lengths = mysql_fetch_lengths(res);
        for (int i = 0; i < numFields; i++) {
            std::string val = row[i] ? std::string(row[i], lengths[i]) : "NULL";
            r.push_back(val);
            widths[i] = std::max(widths[i], (int)val.size());
        }
        rows.push_back(r);
    }
    mysql_free_result(res);  

    std::cout << "  ";
    for (int i = 0; i < numFields; i++)
        std::cout << std::left << std::setw(widths[i] + 2) << colNames[i];
    std::cout << "\n  ";
    for (int i = 0; i < numFields; i++)
        std::cout << std::string(widths[i] + 2, '-');
    std::cout << "\n";

    for (auto& r : rows) {
        std::cout << "  ";
        for (int i = 0; i < numFields; i++)
            std::cout << std::left << std::setw(widths[i] + 2) << r[i];
        std::cout << "\n";
    }
    if (rows.empty()) std::cout << "  (no results)\n";
    std::cout << "  [" << rows.size() << " row(s)]\n";
}

std::string getInput(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

std::string escapeStr(const std::string& s) {
    std::string buf(s.size() * 2 + 1, '\0');
    unsigned long len = mysql_real_escape_string(conn, &buf[0], s.c_str(), s.size());
    buf.resize(len);
    return buf;
}

/* Connection */
bool connectDB() {
    conn = mysql_init(nullptr);
    if (!conn) { std::cerr << "mysql_init() failed\n"; return false; }

    if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS,DB_NAME, DB_PORT, nullptr, 0)) {
        std::cerr << "ERROR: Cannot connect " << mysql_error(conn) << "\n";
        mysql_close(conn);
        conn = nullptr;
        return false;
    }
    mysql_set_character_set(conn, "utf8mb4");
    std::cout << "  Connected to MySQL (" << DB_HOST << ":" << DB_PORT
              << " / " << DB_NAME << ")\n";
    return true;
}

/* QUERY 1 – Sales Trends */
void query1_salesTrends() {
    printTitle("QUERY 1 – Sales Trends (past 3 years)");

    // by Year & Brand
    std::cout << "\n1.1 Sales by Brand and Year\n";
    runAndPrint(R"(
        SELECT
            b.name                          AS brand,
            YEAR(st.sale_date)              AS year,
            COUNT(*)                        AS units,
            FORMAT(SUM(st.sale_price),2)    AS total_revenue
        FROM Sales_Transactions st
        JOIN Vehicles v   ON st.VIN = v.VIN
        JOIN Models   m   ON v.model_ID = m.model_ID
        JOIN Brands   b   ON m.brand_ID = b.brand_ID
        WHERE st.sale_date >= DATE_SUB(CURDATE(), INTERVAL 3 YEAR)
        GROUP BY b.name, YEAR(st.sale_date)
        ORDER BY year DESC, total_revenue DESC
    )");

    // by Month
    std::cout << "\n1.2 Sales by Brand and Month\n";
    runAndPrint(R"(
        SELECT
            b.name                          AS brand,
            DATE_FORMAT(st.sale_date,'%Y-%m') AS YM,
            COUNT(*)                        AS units,
            FORMAT(SUM(st.sale_price),2)    AS total_revenue
        FROM Sales_Transactions st
        JOIN Vehicles v ON st.VIN = v.VIN
        JOIN Models   m ON v.model_ID = m.model_ID
        JOIN Brands   b ON m.brand_ID = b.brand_ID
        WHERE st.sale_date >= DATE_SUB(CURDATE(), INTERVAL 3 YEAR)
        GROUP BY b.name, DATE_FORMAT(st.sale_date,'%Y-%m')
        ORDER BY YM DESC, b.name
    )");

    // by Week
    std::cout << "\n1.3 Sales by Brand and Week (ISO)\n";
    runAndPrint(R"(
        SELECT
            b.name                              AS brand,
            DATE_FORMAT(st.sale_date,'%x-W%v')  AS YW,
            COUNT(*)                            AS units,
            FORMAT(SUM(st.sale_price),2)        AS total_revenue
        FROM Sales_Transactions st
        JOIN Vehicles v ON st.VIN = v.VIN
        JOIN Models   m ON v.model_ID = m.model_ID
        JOIN Brands   b ON m.brand_ID = b.brand_ID
        WHERE st.sale_date >= DATE_SUB(CURDATE(), INTERVAL 3 YEAR)
        GROUP BY b.name, DATE_FORMAT(st.sale_date,'%x-W%v')
        ORDER BY YW DESC, brand
    )");

    // by Gender
    std::cout << "\n1.4 Sales by Brand and Buyer Gender\n";
    runAndPrint(R"(
        SELECT
            b.name                          AS brand,
            c.gender,
            COUNT(*)                        AS units,
            FORMAT(SUM(st.sale_price),2)    AS total_revenue
        FROM Sales_Transactions st
        JOIN Vehicles  v ON st.VIN = v.VIN
        JOIN Models    m ON v.model_ID = m.model_ID
        JOIN Brands    b ON m.brand_ID = b.brand_ID
        JOIN Customers c ON st.customer_ID = c.customer_ID
        WHERE st.sale_date >= DATE_SUB(CURDATE(), INTERVAL 3 YEAR)
        GROUP BY b.name, c.gender
        ORDER BY brand, c.gender
    )");

    // by Income Range
    std::cout << "\n1.5 Sales by Brand and Buyer Income Range\n";
    runAndPrint(R"(
        SELECT
            b.name AS brand,
            CASE
                WHEN c.annual_income <  50000 THEN 'Under $50K'
                WHEN c.annual_income <  80000 THEN '$50K-$80K'
                WHEN c.annual_income < 120000 THEN '$80K-$120K'
                ELSE 'Over $120K'
            END                             AS income_range,
            COUNT(*)                        AS units,
            FORMAT(SUM(st.sale_price),2)    AS total_revenue
        FROM Sales_Transactions st
        JOIN Vehicles  v ON st.VIN = v.VIN
        JOIN Models    m ON v.model_ID = m.model_ID
        JOIN Brands    b ON m.brand_ID = b.brand_ID
        JOIN Customers c ON st.customer_ID = c.customer_ID
        WHERE st.sale_date >= DATE_SUB(CURDATE(), INTERVAL 3 YEAR)
        GROUP BY b.name, income_range
        ORDER BY brand, income_range
    )");
}

/* QUERY 2 – Defective Part Tracking */
void query2_defectiveParts() {
    printTitle("QUERY 2 – Defective Part Tracking");

    std::string supplierName = getInput("  Supplier name (e.g. Getrag): ");
    std::string partType     = getInput("  Part type    (e.g. Transmission): ");
    std::string dateFrom     = getInput("  Production date FROM (YYYY-MM-DD): ");
    std::string dateTo       = getInput("  Production date TO   (YYYY-MM-DD): ");
    std::string plantName    = getInput("  Specific plant name (leave blank for all): ");

    std::string sSupplier = escapeStr(supplierName);
    std::string sPart     = escapeStr(partType);
    std::string sFrom     = escapeStr(dateFrom);
    std::string sTo       = escapeStr(dateTo);
    std::string sPlant    = escapeStr(plantName);

    std::string plantFilter = "";
    if (!sPlant.empty())
        plantFilter = " AND sp.name = '" + sPlant + "'";

    std::string sql = R"(
        SELECT DISTINCT
            vp.VIN,
            m.model_name,
            b.name                  AS brand,
            vp.production_date,
            sp.name                 AS supplier_plant,
            COALESCE(c.name,'(unsold)') AS customer,
            COALESCE(c.phone,'')    AS phone
        FROM Vehicle_part vp
        JOIN Produces      pr ON vp.supplier_plant_ID = pr.supplier_plant_ID
                              AND vp.part_type_ID     = pr.part_type_ID
        JOIN Part_Types    pt ON pr.part_type_ID      = pt.part_type_ID
        JOIN Supplier_Plants sp ON vp.supplier_plant_ID = sp.supplier_plant_ID
        JOIN Suppliers     s  ON sp.supplier_ID       = s.supplier_ID
        JOIN Vehicles      v  ON vp.VIN               = v.VIN
        JOIN Models        m  ON v.model_ID            = m.model_ID
        JOIN Brands        b  ON m.brand_ID            = b.brand_ID
        LEFT JOIN Sales_Transactions st ON st.VIN      = vp.VIN
        LEFT JOIN Customers c ON st.customer_ID        = c.customer_ID
        WHERE s.name  = ')" + sSupplier + R"('
          AND pt.part_type = ')" + sPart + R"('
          AND vp.production_date BETWEEN ')" + sFrom + R"(' AND ')" + sTo + R"('
    )" + plantFilter;

    runAndPrint(sql);
}


/* QUERY 3 – Top 2 Brands by Revenue (past year) */   
void query3_topBrandsByRevenue() {
    printTitle("QUERY 3 – Top 2 Brands by Revenue (past year)");
    runAndPrint(R"(
        SELECT
            b.name                          AS brand,
            COUNT(*)                        AS units_sold,
            FORMAT(SUM(st.sale_price),2)    AS total_revenue
        FROM Sales_Transactions st
        JOIN Vehicles v ON st.VIN = v.VIN
        JOIN Models   m ON v.model_ID = m.model_ID
        JOIN Brands   b ON m.brand_ID = b.brand_ID
        WHERE st.sale_date >= DATE_SUB(CURDATE(), INTERVAL 1 YEAR)
        GROUP BY b.name
        ORDER BY SUM(st.sale_price) DESC
        LIMIT 2
    )");
}

/* QUERY 4 – Top 2 Brands by Unit Sales (past year) */
void query4_topBrandsByUnits() {
    printTitle("QUERY 4 – Top 2 Brands by Unit Sales (past year)");
    runAndPrint(R"(
        SELECT
            b.name                          AS brand,
            COUNT(*)                        AS units_sold,
            FORMAT(SUM(st.sale_price),2)    AS total_revenue
        FROM Sales_Transactions st
        JOIN Vehicles v ON st.VIN = v.VIN
        JOIN Models   m ON v.model_ID = m.model_ID
        JOIN Brands   b ON m.brand_ID = b.brand_ID
        WHERE st.sale_date >= DATE_SUB(CURDATE(), INTERVAL 1 YEAR)
        GROUP BY b.name
        ORDER BY units_sold DESC
        LIMIT 2
    )");
}

/* QUERY 5 – Seasonal Sales Patterns */
void query5_seasonalSales() {
    printTitle("QUERY 5 – Seasonal Sales Patterns");

    std::string bodyStyle = getInput("  Body style (e.g. Convertible, SUV, Sedan): ");
    std::string sStyle    = escapeStr(bodyStyle);

    std::string sql = R"(
        SELECT
            MONTH(st.sale_date)             AS month_num,
            MONTHNAME(st.sale_date)         AS month_name,
            COUNT(*)                        AS units_sold
        FROM Sales_Transactions st
        JOIN Vehicles v ON st.VIN = v.VIN
        JOIN Models   m ON v.model_ID = m.model_ID
        WHERE m.body_style = ')" + sStyle + R"('
        GROUP BY MONTH(st.sale_date), MONTHNAME(st.sale_date)
        ORDER BY units_sold DESC, month_num
    )";
    runAndPrint(sql);
}

/* QUERY 6 – Dealer Inventory Efficiency */
void query6_dealerInventory() {
    printTitle("QUERY 6 – Dealer Inventory Efficiency (longest avg hold time)");
    runAndPrint(R"(
        SELECT
            d.name                                      AS dealer,
            d.address,
            COUNT(i.VIN)                                AS vehicles_handled,
            ROUND(AVG(
                DATEDIFF(
                    COALESCE(st.sale_date, CURDATE()),
                    i.received_date
                )
            ), 1)                                       AS avg_days_in_inventory
        FROM Inventory i
        JOIN Dealers d ON i.dealer_ID = d.dealer_ID
        LEFT JOIN Sales_Transactions st ON st.VIN = i.VIN
                                       AND st.dealer_ID = i.dealer_ID
        GROUP BY d.dealer_ID, d.name, d.address
        ORDER BY avg_days_in_inventory DESC
    )");
}

/* QUERY 7 – Supplier Coverage */
void query7_supplierCoverage() {
    printTitle("QUERY 7 – Supplier Coverage (most distinct models across brands)");
    runAndPrint(R"(
        SELECT
            s.name                          AS supplier,
            COUNT(DISTINCT v.model_ID)      AS distinct_models,
            COUNT(DISTINCT m.brand_ID)      AS distinct_brands,
            GROUP_CONCAT(DISTINCT b.name ORDER BY b.name SEPARATOR ', ') AS brands_covered
        FROM Vehicle_part vp
        JOIN Supplier_Plants sp ON vp.supplier_plant_ID = sp.supplier_plant_ID
        JOIN Suppliers       s  ON sp.supplier_ID       = s.supplier_ID
        JOIN Vehicles        v  ON vp.VIN               = v.VIN
        JOIN Models          m  ON v.model_ID            = m.model_ID
        JOIN Brands          b  ON m.brand_ID            = b.brand_ID
        GROUP BY s.supplier_ID, s.name
        ORDER BY distinct_models DESC, distinct_brands DESC
        LIMIT 5
    )");
}

/* MENU */
void displayMenu() {
    std::cout << "\n";
    printLine(60);
    std::cout << "   Car Company Database – Main Menu\n";
    printLine(60);
    std::cout << "   1. Sales Trends (by year/month/week/gender/income)\n";
    std::cout << "   2. Defective Part Tracking\n";
    std::cout << "   3. Top 2 Brands by Revenue (past year)\n";
    std::cout << "   4. Top 2 Brands by Unit Sales (past year)\n";
    std::cout << "   5. Seasonal Sales Patterns\n";
    std::cout << "   6. Dealer Inventory Efficiency\n";
    std::cout << "   7. Supplier Coverage\n";
    std::cout << "   0. Exit\n";
    printLine(60);
    std::cout << "   Choice: ";
}

int getUserChoice() {
    std::string line;
    std::getline(std::cin, line);
    try { return std::stoi(line); }
    catch (...) { return -1; }
}

/* MAIN */
int main() {
    std::cout << "\n========================================\n";
    std::cout << "  Car Company DB  –  Project 2\n";
    std::cout << "  CSE4110 / AIE4055  Spring 2026\n";
    std::cout << "========================================\n\n";

    if (const char* e = getenv("DB_PASS"))  DB_PASS = e;
    if (const char* e = getenv("DB_USER"))  DB_USER = e;
    if (const char* e = getenv("DB_HOST"))  DB_HOST = e;

    if (!connectDB()) return 1;

    while (true) {
        displayMenu();
        int choice = getUserChoice();
        std::cout << "\n";
        switch (choice) {
            case 1: query1_salesTrends();      break;
            case 2: query2_defectiveParts();   break;
            case 3: query3_topBrandsByRevenue();break;
            case 4: query4_topBrandsByUnits(); break;
            case 5: query5_seasonalSales();    break;
            case 6: query6_dealerInventory();  break;
            case 7: query7_supplierCoverage(); break;
            case 0:
                std::cout << "  Goodbye\n";
                mysql_close(conn);
                return 0;
            default:
                std::cout << "  Invalid choice. Please enter 0-7.\n";
        }
    }
}
