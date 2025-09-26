//#include "../include/inventory.h"
//#include "../include/order.h"
#include "../include/db.h"
//Database Operations

////////////////////////////////////////////
/***************************************** *
*                                          *
*           CSV OPERATIONS                 *
*                                          *
* *****************************************/
////////////////////////////////////////////

int csv_handler(){
    char filename[MEDIUMLENGTH];
    int numberOfFiles;
    int numberOfRecords;
    int recordChoice;

    printf("How many files are we working with today?");
    scanf("%d", &numberOfFiles);

    //store the filenames the user would like to process (this will be a drag and drop system later on)
    //step 1: allocate array of pointers to strings
    char **filenames = (char**) calloc(numberOfFiles, sizeof(char*));

    //step 2: allocate space for each string
    for(int i = 0;i < numberOfFiles;i++){
        *(filenames + i) = (char *)calloc(30,sizeof(char));

        if(*(filenames + i) == NULL){
            printf("Second Stage: Allocating filename %d unsuccesful\n", i + 1);
        }else{
            //carry on
        }
    }
    //get filenames from user (this will be a drag and drop system later on)
    for(int i = 0;i < numberOfFiles;i++){

        printf("\nEnter filename %d:\n",i + 1);
        scanf("%s", filename);
        strcpy(*(filenames + i), filename);
    }

    //count number of records inside CSV files and dynamically allocate an appropriate array.
    //Originally this was just number of orders, but for this larger project I need it to be more flexible
    //So we will count the number of records, and the user will then choose if these are orders, parts etc.
    numberOfRecords = countRecords(numberOfFiles, filenames);

    printf("\nDetected %d records, please choose type of record:\n", numberOfRecords);
    printf("1 for car parts, 2 for orders, 3 for customer\n");
    scanf("%d", &recordChoice);

    //allocate memory depending on type of file we are working with
    if(recordChoice == 1){
        CarPart *carPartPtr = calloc(numberOfRecords, sizeof(CarPart));
            if(carPartPtr == NULL){printf("calloc fail"); return 1;}

        int j = 0;
        for(int i = 0;i < numberOfFiles;i++){

            j = j + readCSV(*(filenames + i), carPartPtr, CAR_PART, j);

        }

    }else if(recordChoice == 2){
        Order *orderPtr = calloc(numberOfRecords, sizeof(Order));
            if(orderPtr == NULL){printf("calloc fail"); return 1;}
            
        int j = 0;
        for(int i = 0;i < numberOfFiles;i++){

            j = j + readCSV(*(filenames + i), orderPtr, ORDER, j);

        }
    }else if(recordChoice == 3){
        Customer *customerPtr = calloc(numberOfRecords, sizeof(Customer));
            if(customerPtr == NULL){printf("calloc fail"); return 1;}

        int j = 0;

        for(int i = 0;i < numberOfFiles;i++){

            j = j + readCSV(*(filenames + i), customerPtr, CUSTOMER, j);

        }
    }
    else{
        printf("\nYour choice doesn't match any case\n");
    }

    free(filenames);//as records have been read into appropriate struct array, we don't need these anymore.
}   

//this will return the index it ended on, so we can start from there on the next file
int readCSV(const char* filename, void *array, CSVType type, int startIndex ){
    int j = 0;

    FILE *fp = fopen(filename, "r");
    if(!fp){
        printf("Error opening file: %s\n", filename);

        return 0;
    }

    j = startIndex;//start instering at this index
    char line[256];

    while(fgets(line, sizeof(line), fp) != NULL){
        //remove newline if present
        line[strcspn(line, "\r\n")] = 0;

        //calculate pointer to the current struct
        void* currentItem = (char*)array + j * 
            (type == CAR_PART ? sizeof(CarPart) : type == ORDER ? sizeof(Order) : sizeof(Customer));

        //call parser based on type
        switch(type){
            case CAR_PART:
                parseCarPart(line, currentItem);
                break;
            case ORDER: 
                parseOrder(line, currentItem);
                break;
            case CUSTOMER:
                parseCustomer(line, currentItem);
                break;
        }

        j++;
    }

    fclose(fp);

    return j;
}

void parseDateTime(char* dateStr, char* timeStr, DateTime* dt){
    sscanf(dateStr, "%d-%d-%d", &dt->year, &dt->month, &dt->day);
    sscanf(timeStr, "%d:%d", &dt->hour, &dt->minute);
}

void parseCarPart(char* line, void* item){
    CarPart* part = (CarPart*)item;

    char addedDate[11], addedTime[6], lastDate[11], lastTime[6];

    sscanf(line, "%d,%49[^,],%29[^,],%d,%lf",
            &part->id,
            part->name,
            part->category,
            &part->quantity,
            &part->price,
            addedDate, addedTime,
            lastDate, lastTime
            );

    parseDateTime(addedDate, addedTime, &part->added_date);
    parseDateTime(lastDate, lastTime, &part->last_updated);

}

void parseOrder(char* line, void* item){
    Order* order = (Order*)item;

    char orderDate[11], orderTime[6];

    sscanf(line, "%d,%d,%d,%d,%lf",
            &order->id,
            &order->customer_id,
            &order->part_id,
            &order->quantity,
            &order->total_price,
            orderDate, orderTime
           );

    parseDateTime(orderDate, orderTime, &order->order_time);
}

void parseCustomer(char* line, void* item){
    Customer* customer = (Customer*)item;

    sscanf(line, "%d,%49[^,],%99[^,],%29[^,],%49[^,]",
           &customer->id,
            customer->name,
            customer->address,
            customer->phone,
            customer->email
           );
}

int countRecords(int numberOfFiles, char **filenames){
    int numberOfRecords = 0;
    int current_line;
    char c;
    FILE *fp;

    for(int i = 0;i < numberOfFiles;i++){
        current_line = 0;

        fp = fopen(*(filenames + i), "r");

        if(fp == NULL){

            printf("Error opening files %s", *(filenames + i));

            return 1; //return failure, end program
        }else{
            printf("File %s opened successfully\n", *(filenames + i));

            //count number of lines in CSV file
            do{
                c = fgetc(fp);
                if(c == '\n'){
                    current_line++;
                }
            }while(c != EOF);

            numberOfRecords = numberOfRecords + current_line - 1; //subtract 1 for header file
            
            fclose(fp);//close file
            printf("File %s has %d orders\n", *(filenames + i), current_line - 1);
            printf("%d orders detected so far...\n",numberOfRecords);
        }
    }

    return numberOfRecords;
}


