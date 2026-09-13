DROP DATABASE IF EXISTS car_company;
CREATE DATABASE car_company;
USE car_company;

CREATE TABLE Manufacturing_Plants (
	manufacturing_plant_ID INT PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    location VARCHAR(255)
);
CREATE TABLE Brands (
    brand_ID INT PRIMARY KEY,
    name VARCHAR(100) NOT NULL
);
CREATE TABLE Customers (
    customer_ID INT PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    address VARCHAR(255),
    gender CHAR(1),
    phone VARCHAR(20),
    annual_income DECIMAL(12,2)
);
CREATE table Dealers (
	dealer_ID INT primary key,
    name VARCHAR(50) NOT NULL,
    address VARCHAR(255)
);
CREATE TABLE Models (
    model_ID INT PRIMARY KEY,
    brand_ID INT NOT NULL,
    model_name VARCHAR(100),
    year INT,
    base_price DECIMAL(12,2),
    body_style VARCHAR(50),

    FOREIGN KEY (brand_ID)
        REFERENCES Brands(brand_ID)
);
CREATE TABLE Vehicles (
	VIN CHAR(17) PRIMARY KEY,
    model_ID INT NOT NULL,
    current_status VARCHAR(20),
    manufacturing_plant_ID INT NOT NULL,
    manufacturing_date DATE,
    color VARCHAR(20),
    FOREIGN KEY (model_ID)
		REFERENCES Models(model_ID),
    FOREIGN KEY (manufacturing_plant_ID)
		REFERENCES Manufacturing_Plants(manufacturing_plant_ID)
);
CREATE TABLE Suppliers (
	supplier_ID int PRIMARY KEY,
    name VARCHAR(50)
);
CREATE TABLE Supplier_Plants (
	supplier_plant_ID INT PRIMARY KEY,
    supplier_ID INT,
    name VARCHAR(50),
    location VARCHAR(255),
    
    FOREIGN KEY (supplier_ID)
		REFERENCES Suppliers(supplier_ID)
);
CREATE TABLE Part_Types (
	part_type_ID INT PRIMARY KEY,
    part_type VARCHAR(50)
);
CREATE TABLE Produces (
	supplier_plant_ID INT,
    part_type_ID INT,
    
    PRIMARY KEY (supplier_plant_ID,part_type_ID),
    FOREIGN KEY (supplier_plant_ID)
		REFERENCES Supplier_Plants(supplier_plant_ID),
	FOREIGN KEY (part_type_ID)
		REFERENCES Part_Types(part_type_ID)
);
CREATE TABLE Vehicle_part (
	supplier_plant_ID INT,
    part_type_ID INT,
    VIN CHAR(17),
    production_date date,
    quantity INT,
    
    PRIMARY KEY (supplier_plant_ID, part_type_ID,VIN,production_date),
    FOREIGN KEY (supplier_plant_ID, part_type_ID)
		REFERENCES Produces(supplier_plant_ID, part_type_ID),
	FOREIGN KEY (VIN)
		REFERENCES Vehicles(VIN)
);
create table Sales_Transactions (
	sale_ID INT PRIMARY KEY,
    VIN CHAR(17) NOT NULL,
    customer_ID INT NOT NULL,
    dealer_ID INT NOT NULL,
    sale_date DATE,
    payment_method VARCHAR(50),
    sale_price DECIMAL(12,2),
    
    FOREIGN KEY (VIN)
		REFERENCES Vehicles(VIN),
    FOREIGN KEY (customer_ID)
		REFERENCES Customers(customer_ID),
	FOREIGN KEY (dealer_ID)
		REFERENCES Dealers(dealer_ID)
);
CREATE TABLE Inventory (
	dealer_ID INT NOT NULL,
    VIN CHAR(17) NOT NULL,
    received_date date,
    
    PRIMARY KEY(dealer_ID,VIN),
    FOREIGN KEY (dealer_ID)
		REFERENCES Dealers(dealer_ID),
	FOREIGN KEY (VIN)
		REFERENCES Vehicles(VIN)
);

    
