/*
    Program Descriptor: "Car Part Order Management System" 
                        
    Program Details: 
        - As an employee in the IT department of a car parts firm, you have been tasked to
        - produce the delivery docket for each van, products are packed in the vans in weight order
        - The lightest first, heaviest last.

    Author: Aleksy Szopinski - C24378306

*/
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Symbolic Names
#define PRODUCTNAMELENGTH 50
#define ENGINECODELENGTH 50
#define FILENAMELENGTH 20
#define BUFFERSIZE 1000

//Struct Templates
struct batchDateTime
{
    int minute;
    int hour;
    int day;
    int month;
};

struct carPart 
{
    int lineCode;
    int batchCode;
    struct batchDateTime BDT;
    int productID;
    char productName[PRODUCTNAMELENGTH];
    char targetEngineCode[ENGINECODELENGTH];
    int binNum;
    float weight;
};

struct van
{
    int totalItems;
    float totalWeight;
};

//Function definitions

//functions for merge sort
void merge_sort(struct carPart *,int records);
void merge_sort_recursion(struct carPart *, int l, int r);
void merge_sorted_arrays(struct carPart *, int l, int m, int r);

//utilities
void copyOrder(struct carPart *dest,struct carPart *src);
void printStruct(struct carPart *ptr,int length);
int countOrders(int numOfFiles,char **filenames);
int readFileIntoStruct(char **filenames, struct carPart *carPartPtr, int numOfFiles);

//functions for finding item of particular weight
int binary_search(struct carPart *, float e, int l, int r);
int findRepeatsForward(struct carPart *carPartPtr, float e, int index,int records);
int findRepeatsBackward(struct carPart *carPartPtr, float e, int index);
struct carPart* compareTimes(struct carPart *p1, struct carPart *p2);


int main()
{
    //Intial Variables 
    //structures
    struct carPart *carPartPtr;

    //files
    FILE *fp;
    char filename[FILENAMELENGTH];
    char filenamebuffer[FILENAMELENGTH];
    int numOfOrders = 0;
    int numOfFiles = 0;

    //reading files into struct
    char buffer[BUFFERSIZE];
    char *data;

    printf("\nCar Parts Order Management System\n");
    printf("How many files are we working with today?\n");
    scanf("%d", &numOfFiles);

    //Store the filenames the user would like to process
    //step 1: allocate array of pointers to strings
    char **filenames = (char**) calloc(numOfFiles, sizeof(char*));
    
    if(filenames == NULL) 
    {
        printf("First stage of allocating array of filenames unsuccesful\n");
    }
    else
    {
        printf("First stage of allocating array of filenames successful\n");
    }

    //step 2: allocate space for each string
    for(int i = 0;i < numOfFiles;i++)
    {
        *(filenames + i) = (char *)calloc(FILENAMELENGTH,sizeof(char));

        if(*(filenames + i) == NULL) 
        {
            printf("Second Stage: Allocating Filename %d Unsuccesful\n",i+1);
        }
        else
        {
            printf("Second Stage: Allocating filename %d succesful\n",i+1);
        }
    }

    //get filenames from user
    for(int i = 0;i < numOfFiles;i++)
    {
        printf("\nEnter filename %d:\n",i+1);
        scanf("%s", filename);
        strcpy(*(filenames + i),filename);
    }

    //count number of orders inside CSV files and dynamically allocate an appropriate array.
    numOfOrders = countOrders(numOfFiles,filenames);

    printf("\nDetected %d orders, allocating memory\n",numOfOrders);

    //allocate memory appropriately for number of parts in files
    carPartPtr = calloc(numOfOrders,sizeof(struct carPart));

    if(carPartPtr == NULL)
    {
        printf("\nMemory Allocation Unsuccessful, ending program...\n");
        
        return 1; //return failure, end program.
    }
    else
    {
        printf("\nMemory Allocation Successful\n");
    }

    //read data from files into struct array
    int records = readFileIntoStruct(filenames, carPartPtr, numOfFiles);

    if(records == -1)//if previous function returns error, end program.
    {
        return 1;
    }

    free(filenames);//We no longer need the filenames as the contents have been copied to structure array.
     
    //Sort struct array by weight
    merge_sort(carPartPtr, records);

    //task 1 pre-process data and sort by weight
    //and task 2 merge orders from files into single dispatch list
    //done

    //Task 3 - Provide a user interface to search for earliest occurence
    //of a product with a particular weight.
    //i.e., binary search

    //user interface
    int choice;
    float weight;
    int index;
    int itemsFound = 0;
    int repeatIndex;

    do
    {
        printf("Your data has been pre-processed and merged into a single list\n");
        printf("1: Print the dispatch list\n");
        printf("2: Search for earliest occurence of a product with a particular weight\n");
        printf("3: Print Summary\n");
        printf("4: Print List to File\n");
        printf("0: End Program\n");
        scanf("%d",&choice);
        if(choice == 1)
        {
            printf("\nDispatch List:\n");

            //print sorted structure array
            printStruct(carPartPtr,records);
        }
        else if(choice == 2)
        {
            printf("\nEnter Particular Weight:\n");
            scanf("%f",&weight);

            index = binary_search(carPartPtr,weight,0,records);

            if(index == -1)
            {
                printf("\nCould not find item\n");

                continue; //go back to main menu
            }
            
            int startIndex = findRepeatsBackward(carPartPtr,weight,index);
            int endIndex = findRepeatsForward(carPartPtr,weight,index, records);
            int totalItems = (endIndex - startIndex);

            printf("\nWe have found %d items with matching weight between index %d and index %d\n",totalItems , startIndex, endIndex);

            struct carPart *tempCarPartArraySearch = calloc(totalItems,sizeof(struct carPart));

            for(int i = 0; i < totalItems; i++)
            {
                copyOrder((tempCarPartArraySearch + i),(carPartPtr + startIndex));
                startIndex++;
            }

            printf("\nItems of Matching Weight:\n");
            struct carPart *earliestOccurencePtr;
            printStruct(tempCarPartArraySearch,totalItems);

            //Find earliest occurence
            printf("\nEarliest Occurence:\n");
            earliestOccurencePtr = compareTimes((tempCarPartArraySearch), (tempCarPartArraySearch + 1));
            for(int i = 1; i < totalItems;i++)
            {
                earliestOccurencePtr = compareTimes((earliestOccurencePtr), (tempCarPartArraySearch + i + 1));
            }
            printStruct(earliestOccurencePtr, 1);
        }
        else if(choice == 3)
        {
            //print summary of products included in each van (4 vans) to screen
            //For each van
            //1. Ask for Weight Limit for vans
            float weightLimit = 0;
            float totalWeight = 0;
            int numOfVans = 0;

            //creating an array which will hold the number of items in each van for the purpose
            //of allocating memory for the 2D array that will hold all the items for each van
            printf("\nHow many vans are availible?\n");
            scanf("%d",&numOfVans);

            struct van *Vans = calloc(numOfVans, sizeof(struct van)); 

            for(int i = 0;i < records;i++)
            {
                totalWeight += (carPartPtr + i)->weight;
            }

            printf("\nTotal weight of all items: %f\n", totalWeight);
            printf("\nEnter Weight Limit for Vans:\n");
            scanf("%f",&weightLimit);

            int j = 0;

            //2. Calculate which parts go into which van by index
            for(int i = 0; i < numOfVans;i++)
            {
                (Vans + i)->totalWeight = 0;

                while(((Vans + i)->totalWeight + (carPartPtr + j)->weight) <= weightLimit)
                {
                    (Vans + i)->totalWeight += (carPartPtr + j)->weight;
                    (Vans + i)->totalItems++;
                    j++;

                }

                printf("\nTotal Items in Van %d = %d\n", i + 1,(Vans + i)->totalItems);
                printf("\nTotal Items that fit into vans: %d\n",j);
                printf("\nTotal Items left over: %d\n",records - j);
            }

            //DMA a 2D array that will hold the vans and their items
            struct carPart **vanContents = calloc(numOfVans,sizeof(struct carPart *));
            for(int i = 0;i < numOfVans;i++)
            {
                *(vanContents + i) = calloc((Vans + i)->totalItems,sizeof(struct carPart));
            }

            int startIndex = 0;
            for(int i = 0;i < numOfVans;i++)
            {
                for(int j = 0;j < ((Vans + i)->totalItems);j++)
                {
                    //(*(*(vanContents + i)+j) = (carPartPtr + k)
                    struct carPart *dest = (*(vanContents + i) + j);
                    struct carPart *src = (carPartPtr + startIndex + j);
                    copyOrder(dest, src);
                }
                startIndex += (Vans + i)->totalItems;
            }

            //3. Print the total items in each van and their total weight.
            for(int i = 0; i < numOfVans;i++)
            {
                printf("\nVan %d:\n",i + 1);
                printf("Total items: %d\n",(Vans + i)->totalItems);
                printf("Total Weight: %.2f\n",(Vans + i)->totalWeight);
            }
            //4. Let the user choose if they want to see a list of all items in each van
            char displaylistchoice;
            printf("Would you like to display a list of items for each van? Y/N");
            
            scanf(" %c",&displaylistchoice);
            if(displaylistchoice == 'y' || displaylistchoice == 'Y')
            {
                for(int i = 0;i < numOfVans;i++)
                {
                    printf("\nVan %d:\n", i + 1);
                    //printStruct(*(vanContents + i),(Vans + i)->totalItems);
                    for(int j = 0;j < (Vans + i)->totalItems;j++)
                    {
                        //printStruct((*(vanContents + i)+j),(Vans + i)->totalItems);
                        printf("Line Code[%d]: %d, Batch Code: %d, Minute: %d, Hour: %d, Day: %d, Month: %d, ProductID: %d, Product Name: %s,Target Engine Code: %s, Bin Num: %d, Weight: %.2f\n",
                            j + 1,
                            (*(vanContents + i)+j)->lineCode,
                            (*(vanContents + i)+j)->batchCode,
                            (*(vanContents + i)+j)->BDT.minute,
                            (*(vanContents + i)+j)->BDT.hour,
                            (*(vanContents + i)+j)->BDT.day,
                            (*(vanContents + i)+j)->BDT.month,
                            (*(vanContents + i)+j)->productID,
                            (*(vanContents + i)+j)->productName,
                            (*(vanContents + i)+j)->targetEngineCode,
                            (*(vanContents + i)+j)->binNum,
                            (*(vanContents + i)+j)->weight
                        );
                        printf("\n");
                    }
                }
            }
            free(Vans);
            free(vanContents);
            
        }
        else if(choice == 4)
        {
            //print list to file
        }
    } while (choice != 0);
    

    free(carPartPtr);

    return 0; //return success, end program

}//end main()

int readFileIntoStruct(char **filenames, struct carPart *carPartPtr, int numOfFiles)
{
    FILE *fp;
    int read = 0;
    int records = 0;

    for(int i = 0;i < numOfFiles;i++)
    {

        fp = fopen(*(filenames + i), "r");

        if(fp == NULL)
        {
            printf("Error opening file %s\n", *(filenames + i));
            return 1; //return failure, end program.
        }
        else
        {
            printf("File %s opened successfully\n", *(filenames + i));

            //Read data from text files into carPart structure

            //skip first line containing headings
            char skipHeadings[BUFFERSIZE];
            fgets(skipHeadings,sizeof(skipHeadings),fp);

            //format: lineCode,batchCode,batchDateTime(minute,hour,day,month),productID,productName,targetEngineCode,binNum,weight
            do
            {
                read = fscanf(fp,
                            "%d,%d,%d-%d-%d-%d,%d,%49[^,],%49[^,],%d,%f\n\n",
                            &(carPartPtr + records)->lineCode,
                            &(carPartPtr + records)->batchCode,
                            &(carPartPtr + records)->BDT.minute,
                            &(carPartPtr + records)->BDT.hour,
                            &(carPartPtr + records)->BDT.day,
                            &(carPartPtr + records)->BDT.month,
                            &(carPartPtr + records)->productID,
                            (carPartPtr + records)->productName,
                            (carPartPtr + records)->targetEngineCode,
                            &(carPartPtr + records)->binNum,
                            &(carPartPtr + records)->weight
                            );
                //printf("%d\n",read);

                if(read == 11) records++;

                if(read != 11 && !feof(fp)) 
                {
                    printf("File Format Incorrect\n");

                    return -1; //failure, end program
                } 

                if(ferror(fp))
                {
                    printf("Error reading file.\n");

                    return -1; //failure, end program
                }
            } while (!feof(fp));

            fclose(fp);//close file

            printf("\n%d records read so far.\n\n",records);
        }
    }//end for loop for reading files into structure

    return records;
}

int countOrders(int numOfFiles,char **filenames)
{
    int numOfOrders = 0;
    int current_line;
    char c;
    FILE *fp;

    for(int i = 0;i < numOfFiles;i++)
    {
        current_line = 0;

        fp = fopen(*(filenames + i), "r");

        if(fp == NULL)
        {
            printf("Error opening file %s\n", *(filenames + i));
            return 1; //return failure, end program.
        }
        else
        {
            printf("File %s opened successfully\n", *(filenames + i));

            //Count number of lines in file
            do
            {
                c = fgetc(fp);
                if(c == '\n')
                {
                    current_line++;
                }
            } while (c != EOF); //count lines in file

            numOfOrders = numOfOrders + current_line - 1; //subtract 1 for header line

            fclose(fp); //close file
            printf("File %s has %d orders\n", *(filenames + i), current_line - 1);
            printf("%d Orders detected so far...\n", numOfOrders);
        }
    }

    return numOfOrders;
}

void printStruct(struct carPart *ptr, int length)
{
    for(int i = 0; i < length;i++)
    {
        printf("Line Code[%d]: %d, Batch Code: %d, Minute: %d, Hour: %d, Day: %d, Month: %d, ProductID: %d, Product Name: %s,Target Engine Code: %s, Bin Num: %d, Weight: %.2f\n",
            i,
            (ptr + i)->lineCode,
            (ptr + i)->batchCode,
            (ptr + i)->BDT.minute,
            (ptr + i)->BDT.hour,
            (ptr + i)->BDT.day,
            (ptr + i)->BDT.month,
            (ptr + i)->productID,
            (ptr + i)->productName,
            (ptr + i)->targetEngineCode,
            (ptr + i)->binNum,
            (ptr + i)->weight);
        printf("\n");
    }
}

void copyOrder(struct carPart *dest,struct carPart *src)
{
    dest->lineCode = src->lineCode;
    dest->batchCode = src->batchCode;
    dest->BDT.minute = src->BDT.minute;
    dest->BDT.hour = src->BDT.hour;
    dest->BDT.day = src->BDT.day;
    dest->BDT.month = src->BDT.month;
    dest->productID = src->productID;
    strcpy(dest->productName,src->productName);
    strcpy(dest->targetEngineCode,src->targetEngineCode);
    dest->binNum = src->binNum;
    dest->weight = src->weight;
}

void merge_sort(struct carPart *carPartPtr,int records)
{
    merge_sort_recursion(carPartPtr, 0, records - 1);
}

void merge_sort_recursion(struct carPart *carPartPtr, int l, int r)
{
    if(l < r)
    {
        int m = l + (r - l) / 2;

        merge_sort_recursion(carPartPtr, l, m);
        merge_sort_recursion(carPartPtr, m + 1, r);

        merge_sorted_arrays(carPartPtr, l, m, r);
    }
    else
    {
        return;
    }
}

void merge_sorted_arrays(struct carPart *carPartPtr, int l, int m, int r)
{
    int left_length = m - l + 1;
    int right_length = r - m;

    struct carPart *tempCarPartArrayLeft = calloc(left_length, sizeof(struct carPart));
    /*if(tempCarPartArrayLeft == NULL)
    {
        printf("Allocating Memory for Temp Array Left Failed...");

    }
    else
    {
        printf("Allocated %d memory structs for Temp Array Left Successfully",left_length);
    }*/

    struct carPart *tempCarPartArrayRight = calloc(right_length, sizeof(struct carPart));

    /*if(tempCarPartArrayRight == NULL)
    {
        printf("Allocating Memory for Temp Array Right Failed...");
    }
    else
    {
        printf("Allocated %d memory structs for Temp Array Right Successfully",right_length);
    }*/

    int i, j, k;

    //copy appropriate structure array element inside left temporary array
    for(int i = 0;i < left_length;i++)
    {

        copyOrder((tempCarPartArrayLeft + i),(carPartPtr + (l + i)));
        
    }

    //copy appropriate structure array element inside right temporary array
    for(int i = 0; i < right_length; i++)
    {

        copyOrder((tempCarPartArrayRight + i),(carPartPtr + (m + 1 + i)));

    }

    //merge left and right temp arrays into 1 sorted array.
    //i keeps track of position inside left temporary array
    //j keeps track of position inside right temporary array
    //k keeps track of position inside permanent structure array
    for(i = 0, j = 0, k = l; k <= r; k++)
    {

        if(i < left_length && (j >= right_length || (tempCarPartArrayLeft + i)->weight <= (tempCarPartArrayRight + j)->weight))
        {

            copyOrder((carPartPtr + k),(tempCarPartArrayLeft + i));
            i++;
        }
        else
        {

            copyOrder((carPartPtr + k),(tempCarPartArrayRight + j));
            j++;
            
        }

    }

    //printf("\nFreeing Temp Array Left\n");
    free(tempCarPartArrayLeft);
    //printf("\nFreeing Temp Array Right\n");
    free(tempCarPartArrayRight);
}


//binary search functions (modifed to handle multiple items of particular weight)
int binary_search(struct carPart *carPartPtr, float e, int l, int r)
{
    int mid = l + (r - l) / 2;

    if(l > r) return -1; //couldn't find the particular weight

    if((carPartPtr + mid)->weight==e)
    {
        return mid;
    }
    else if((carPartPtr + mid)->weight > e)
    {
        return binary_search(carPartPtr, e, l, mid - 1);
    }
    else if((carPartPtr + mid)->weight < e)
    {
        return binary_search(carPartPtr, e, mid + 1, r);
    }
}

int findRepeatsForward(struct carPart *carPartPtr, float e, int index,int records)
{
    while((index + 1 < records)&&((carPartPtr + (index + 1))->weight==e))
    {
        index++;
    }

    return index + 1;
    
}

int findRepeatsBackward(struct carPart *carPartPtr, float e, int index)
{
    while((index - 1 >= 0)&&(carPartPtr + (index - 1))->weight==e)
    {
        index--;
    }
    
    return index;
}

struct carPart* compareTimes(struct carPart *p1, struct carPart *p2)
{
    if(p1->BDT.month > p2->BDT.month)
    {
        return p2;
    }
    else if(p1->BDT.month < p2->BDT.month)
    {
        return p1;
    }
    else//if same
    {
        if(p1->BDT.day > p2->BDT.day)
        {
            return p2;
        }
        else if(p1->BDT.day < p2->BDT.day)
        {
            return p1;
        }
        else//if same
        {
            if(p1->BDT.hour > p2->BDT.hour)
            {
                return p2;
            }
            else if(p1->BDT.hour < p2->BDT.hour)
            {
                return p1;
            }
            else//if same
            {
                if(p1->BDT.minute > p2->BDT.minute)
                {
                    return p2;
                }
                else if(p1->BDT.minute < p2->BDT.minute)
                {
                    return p1;
                }
                else
                {
                    return p1;
                }
            }
        }
    }
}
