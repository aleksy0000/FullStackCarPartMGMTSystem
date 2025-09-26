#include "inventory.h"
//Data Structures for order management 

//Order
typedef struct {
    int id;                     // Unique OrderID
    int customer_id;            //link to customer
    int part_id;                //link to carpart
    int quantity;                // quantity ordered
    double total_price;         //total cost = quantity * CarPart.price
    DateTime order_time;        //order date and time
    char status[SHORTLENGTH];   //"Pending", "Shipped", "Completed"
} Order;

//Customer
typedef struct {
    int id;         //unique customer id
    char name[MEDIUMLENGTH];  //Customers name
    char address[LONGLENGTH]; //Customers address
    char phone[SHORTLENGTH];  //Customers phone number
    char email[MEDIUMLENGTH]; //Customers Email
} Customer;

//Supplier
typedef struct {
    int id;
    char name[MEDIUMLENGTH];
    char contact[MEDIUMLENGTH];
    char address[LONGLENGTH];
    DateTime last_ordered;  // Optional: last order date
} Supplier;


