//Symbolic names
#define SHORTLENGTH 30
#define MEDIUMLENGTH 50
#define LONGLENGTH 100

//Data Structures for Inventory i.e., parts
typedef struct {
    int year;    // e.g., 2025
    int month;   // 1-12
    int day;     // 1-31
    int hour;    // 0-23
    int minute;  // 0-59
    int second;  // 0-59
} DateTime;

typedef struct {
    int id;
    char name[MEDIUMLENGTH];
    char category[SHORTLENGTH];
    int quantity;
    double price;
    DateTime added_date;    // When the part was added
    DateTime last_updated;  // Last stock update
} CarPart;

