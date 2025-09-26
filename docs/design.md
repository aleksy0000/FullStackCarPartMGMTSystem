# Core Features

|Feature           | Description                                          | How it fits into this project
------------------------------------------------------------------------------------------------------------------
|Inventory CRUD    | Add, update, delete, list car parts                  | Already in the old code; 
|                  |                                                      | refactor into modules
------------------------------------------------------------------------------------------------------------------
|Order Management  | Place, cancel, and track orders                      | Use Order struct; 
|                  |                                                      | update stock automatically
------------------------------------------------------------------------------------------------------------------
|Stock Tracking    | Real-time quantity tracking, low-stock alerts        | Add field threshold in Part; 
|                  |                                                      | generate CLI alert or dashboard
------------------------------------------------------------------------------------------------------------------
|Search / Filter   | Search parts by name, category, part number          | Implement via functions + 
|                  |                                                      | optional SQL queries
------------------------------------------------------------------------------------------------------------------
|Sorting & Reports | Sort by price, weight, stock; generate basic reports | Use merge sort / DB queries; 
|                  |                                                      | export CSV or display in dashboard
------------------------------------------------------------------------------------------------------------------

# Advanced Features

| Feature            | Description                                          | Implementation Idea                       
------------------------------------------------------------------------------------------------------------------------
|Categories & Tags | Group parts by type (engine, suspension, electronics) | Add category field in Part;               |
|                  |                                                       | filter/search in frontend                 |
------------------------------------------------------------------------------------------------------------------------
| User Roles       | Admin vs Staff vs Customer                            | Role-based access control;                |
|                  |                                                       | authentication                            |
------------------------------------------------------------------------------------------------------------------------
| Order History & Analytics | Track orders over time, total revenue, frequent parts | Aggregate queries in DB          |
|                           |                                                       | display charts in frontend       |
------------------------------------------------------------------------------------------------------------------------
| REST API           | Expose backend functionality via endpoints      | Semester 2 goal; GET /parts, POST /orders, etc.
-----------------------------------------------------------------------------------------------------------------------
| Database Abstraction | Swap CSV → SQL backend seamlessly              | Modular db.c handles all CRUD operations  
------------------------------------------------------------------------------------------------------------------------
