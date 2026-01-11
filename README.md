# MySimpleDB

**MySimpleDB** is a lightweight, custom database system that stores tables as files on disk. It is designed for **learning purposes** and **small projects**.

---

## Features

- **Create databases and tables**  
- **Add rows to tables**  
- **Read tables**  
- **Store data in a simple text-based format** (`.tbl`)  
- **Support for column types** (`INT`, `CHAR`, etc.)

---

## Storage Format

### Meta file (`.meta`)
Each database has a **`.meta` file** that stores metadata, including:  
- The **database name**  

### Table file (`.tbl`)
Each table is stored as a **`.tbl` file**:  
1. The **first line** contains the **column names**, separated by semicolons (`;`).  
2. The **second line** contains the **column types**, also separated by semicolons (`;`).  
3. The following lines contain the **data rows**.  

**Example:**  
A table might have columns **id**, **name**, and **age** with types **INT**, **CHAR**, and **INT**.  
The rows would contain the values for each record, such as:  
- `1, Bohdan, 20`  
- `2, Anna, 25`  
