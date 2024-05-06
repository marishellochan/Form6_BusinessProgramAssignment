* Input file(s):
    - Product List
    - Employee List
    - Delivery Time Table
 * Output file(s):
    - Customer Orders
 * Techniques used:
    - File I/O
    - Data Structures
    - Binary Search
    - Selection Sort
    - Arrays
    - Sequential, Conditional and Repetitive Logic
 ***********************************************************/

#include <stdio.h>      /* Used to access printf(), scanf();*/
#include <stdlib.h>     /* Used to access ..... */
#include <string.h>     /* Used to access ..... */
#define NUM_ITEMS 20
#define TOTAL_EMP 20
#define NUM_RECORDS 6
#define ORDER_LIMIT 30


struct Product_Item //Structure to store Product Details
{
    int Item_ID;
    char Item_Name[35];
    float Item_Price;
    int Stock_Availability;
};

struct Staff //Structure to store Employee Details
{
    int Employee_ID;
    int Employee_Passcode;
    char Employee_FirstName[35];
    char Employee_LastName[50];
};

struct TimeSlot //Structure to store Delivery Details
{
    int Delivery_Count;
    int Order_Number[2];
    char Customer_Name[2][35];
    char Area[2][50];
};

struct Delivery //Structure to store Deliveries for each Time Slot
{
    struct TimeSlot Time[6]; //Times from 1 to 5; array starting from 1 instead of 0
};

struct Orders //Structure to store the Items Ordered
{
    int Order_Item_ID;
    char Order_itemName[20];
    int Quantity;
};

struct Customer_Order //Structure to store the Customer Orders
{
    int Customer_Order_Number;
    char Customer_First_Name[25];
    char Customer_Last_Name[30];
    char Delivery_Area[50];
    struct Orders ord[6];
    int Order_Count;
};


//Global Structure Definitions
struct Customer_Order Customer_Orders[100]; //
struct Delivery weekDay[NUM_RECORDS]; //Monday to Friday
struct Product_Item Item[NUM_ITEMS];
struct Staff Employee[TOTAL_EMP];

//Global variable definitions
int Employee_IS_Logged_In;
int Number_Of_Products_Read;
int Number_Of_Customers;





/*
 * Function Name: Sort Product Structure by ID
 * Input Parameters: none
 * Returns: none
 * Function Purpose: To sort all the items in the Product Structure by their Item ID
*/

void sortProductStructByID()
{
   int i,j,swapped;
   struct Product_Item temp;

   for(i = 0; i < NUM_ITEMS-1; i++) {
      swapped = 0;

      for(j = 0; j < NUM_ITEMS-1-i; j++)
      {

        if ( Item[j].Item_ID > Item[j+1].Item_ID)
        {
            temp = Item[j];
            Item[j] = Item[j+1];
            Item[j+1] = temp;

            swapped = 1;
         }
      }
   }
}


/*
 * Function Name: Sort Product Structure by Name
 * Input Parameters: none
 * Returns: none
 * Function Purpose: To sort all the Items in the Product Structure in order by their names
*/

void sortProductStructByName()
{
   int i,j,swapped;
   struct Product_Item temp;

   for(i = 0; i < NUM_ITEMS-1; i++) {
      swapped = 0;

      for(j = 0; j < NUM_ITEMS-1-i; j++)
      {

        if( strcasecmp( Item[j].Item_Name ,  Item[j+1].Item_Name) > 0)
        {
            temp = Item[j];
            Item[j] = Item[j+1];
            Item[j+1] = temp;

            swapped = 1;
         }
      }
   }
}

/*
 * Function Name: Binary Search With Name for Price
 * Input Parameters: Item Name
 * Returns: Item Price
 * Function Purpose: To use the Item name entered by the user to find the price of the Item
*/
float binarySearchWithNameForPrice(char key[20]){
    //Search list lo to hi for an item 'key
   int lowIndex = 0;
   int highIndex = Number_Of_Products_Read - 1;
   int middle;

   sortProductStructByName();

   while (lowIndex <= highIndex)
   {


       middle = (lowIndex + highIndex) / 2;

       if ( strcasecmp( key, Item[middle].Item_Name ) == 0)
       {
            return Item[middle].Item_Price;
       }
       else if ( strcasecmp( key, Item[middle].Item_Name ) < 0)
       {
           highIndex = middle - 1;
       }
       else
       {
            lowIndex = middle + 1;
       }
   }
   if (lowIndex > highIndex)
       return -1;
}



/*
 * Function Name: Read Product Item Names
 * Input Parameters: none
 * Returns: the amount of records read
 * Function Purpose: To read the item details of all the Products from the Product List file
*/
int readProductItems()
{
    char filename [] = "ProductList.txt";

    FILE * inputProductItems; //Use a variable name that makes sense

    inputProductItems = fopen(filename,"r");

    if (inputProductItems == NULL)
    {
        char errorMessage[100];
        strcat(errorMessage, filename);
        strcat(errorMessage, " not found!");
        fprintf(stderr, "%s", errorMessage);
        exit(1);
    }

    int t = 0;
    //for (t = 1; t < NUM_ITEMS; t = t + 1)
    //{



    while( !feof(inputProductItems) )
    {
        fscanf(inputProductItems, "%d", &Item[t].Item_ID);
        fscanf(inputProductItems, "%s", &Item[t].Item_Name);
        fscanf(inputProductItems, "%f", &Item[t].Item_Price);
        fscanf(inputProductItems, "%d", &Item[t].Stock_Availability );
        t++;
    }
    // }

    fclose(inputProductItems);
    return t; //the amount of records read
}


/*
 * Function Name: Read Employee Staff
 * Input Parameters: none
 * Returns: the amount of records read
 * Function Purpose: To read all the information from the Employee List File
*/
int readEmployeeStaff()
{
    char filename [] = "EmployeeList.txt";

    FILE * inputEmployeeStaff; //Use a variable name that makes sense

    inputEmployeeStaff = fopen(filename,"rw");

    if (inputEmployeeStaff == NULL)
    {
        char errorMessage[100];
        strcat(errorMessage, filename);
        strcat(errorMessage, " not found!");
        fprintf(stderr, "%s", errorMessage);
        exit(1);
    }

    int e = 1;

    while( !feof(inputEmployeeStaff) )
    {
        fscanf(inputEmployeeStaff, "%d", &Employee[e].Employee_ID);
        fscanf(inputEmployeeStaff, "%d", &Employee[e].Employee_Passcode);
        fscanf(inputEmployeeStaff, "%s", &Employee[e].Employee_FirstName);
        fscanf(inputEmployeeStaff, "%s", &Employee[e].Employee_LastName );
        e++;
    }
    // }

    return e; //the amount of records read
}


/*
 * Function Name: Read and Load Structures
 * Input Parameters: none
 * Returns: none
 * Function Purpose: To read and load all the structures to be used in the program
*/
void readAndLoadStructs()
{
    char filename [] = "Delivery-Timetable.txt";

    FILE * inputFileX; //Use a variable name that makes sense

    inputFileX = fopen(filename,"rw");

    if (inputFileX == NULL)
    {
        char errorMessage[100];
        strcat(errorMessage, filename);
        strcat(errorMessage, " not found!");
        fprintf(stderr, "%s", errorMessage);
        exit(1);
    }

    int i, j;
    for (i = 1; i < NUM_RECORDS; i = i + 1) //Mon - Friday
        for (j = 1; j < 6; j = j + 1)      //Each hour of the day
            weekDay[i].Time[j].Delivery_Count = 0; //Set the delivery count to 0

    while( !feof(inputFileX) )
    {
        int timeOfDay;
        char dayOfTheWeek;

        fscanf(inputFileX," %c",&dayOfTheWeek);

        switch(dayOfTheWeek)
        {
        case 'M': //Monday
            fscanf(inputFileX, "%d", &timeOfDay);

            if (weekDay[1].Time[timeOfDay].Delivery_Count == 0)
            {
                //Add the record
                fscanf(inputFileX, "%d", &weekDay[1].Time[timeOfDay].Order_Number[0]);
                fscanf(inputFileX, "%s", &weekDay[1].Time[timeOfDay].Customer_Name[0]);
                fscanf(inputFileX, "%s", &weekDay[1].Time[timeOfDay].Area[0]);

                //Update Delivery Count for the time
                weekDay[1].Time[timeOfDay].Delivery_Count++;

                //printf("--- %d %s\n",weekDay[1].Time[timeOfDay].Order_Number[0], weekDay[1].Time[timeOfDay].Customer_Name[0]);
            }
            else if (weekDay[1].Time[timeOfDay].Delivery_Count == 1)
            {
                //Add the record
                fscanf(inputFileX, "%d", &weekDay[1].Time[timeOfDay].Order_Number[1]);
                fscanf(inputFileX, "%s", &weekDay[1].Time[timeOfDay].Customer_Name[1]);
                fscanf(inputFileX, "%s", &weekDay[1].Time[timeOfDay].Area[1]);
                //Update Delivery Count for the time
                weekDay[1].Time[timeOfDay].Delivery_Count++;
            }
            else
            {
                //Time period is filled
                int orderNumTemp;
                char custNameTemp[35];
                char custArea[50];
                fscanf(inputFileX, "%d", &orderNumTemp);
                fscanf(inputFileX, "%s", &custNameTemp);
                fscanf(inputFileX, "%s", &custArea);

                printf("Could not add: M %d %d %s %s\n",timeOfDay, orderNumTemp, custNameTemp, custArea);

            }
            break;
        case 'T': //Tuesday
            fscanf(inputFileX, "%d", &timeOfDay);

            if (weekDay[2].Time[timeOfDay].Delivery_Count == 0)
            {
                //Add the record
                fscanf(inputFileX, "%d", &weekDay[2].Time[timeOfDay].Order_Number[0]);
                fscanf(inputFileX, "%s", &weekDay[2].Time[timeOfDay].Customer_Name[0]);
                fscanf(inputFileX, "%s", &weekDay[2].Time[timeOfDay].Area[0]);
                //Update Delivery Count for the time
                weekDay[2].Time[timeOfDay].Delivery_Count++;
            }
            else if (weekDay[2].Time[timeOfDay].Delivery_Count == 1)
            {
                //Add the record
                fscanf(inputFileX, "%d", &weekDay[2].Time[timeOfDay].Order_Number[1]);
                fscanf(inputFileX, "%s", &weekDay[2].Time[timeOfDay].Customer_Name[1]);
                fscanf(inputFileX, "%s", &weekDay[2].Time[timeOfDay].Area[1]);
                //Update Delivery Count for the time
                weekDay[2].Time[timeOfDay].Delivery_Count++;
            }
            else
            {
                //Time period is filled
                int orderNumTemp;
                char custNameTemp[35];
                char custArea[50];
                fscanf(inputFileX, "%d", &orderNumTemp);
                fscanf(inputFileX, "%s", &custNameTemp);
                fscanf(inputFileX, "%s", &custArea);

                printf("Could not add: T %d %d %s %s\n",timeOfDay, orderNumTemp, custNameTemp, custArea);

            }
            break;
        case 'W': //Wednesday
            fscanf(inputFileX, "%d", &timeOfDay);

            if (weekDay[3].Time[timeOfDay].Delivery_Count == 0)
            {
                //Add the record
                fscanf(inputFileX, "%d", &weekDay[3].Time[timeOfDay].Order_Number[0]);
                fscanf(inputFileX, "%s", &weekDay[3].Time[timeOfDay].Customer_Name[0]);
                fscanf(inputFileX, "%s", &weekDay[3].Time[timeOfDay].Area[0]);
                //Update Delivery Count for the time
                weekDay[3].Time[timeOfDay].Delivery_Count++;
            }
            else if (weekDay[3].Time[timeOfDay].Delivery_Count == 1)
            {
                //Add the record
                fscanf(inputFileX, "%d", &weekDay[3].Time[timeOfDay].Order_Number[1]);
                fscanf(inputFileX, "%s", &weekDay[3].Time[timeOfDay].Customer_Name[1]);
                fscanf(inputFileX, "%s", &weekDay[3].Time[timeOfDay].Area[1]);
                //Update Delivery Count for the time
                weekDay[3].Time[timeOfDay].Delivery_Count++;
            }
            else
            {
                //Time period is filled
                int orderNumTemp;
                char custNameTemp[35];
                char custArea[50];
                fscanf(inputFileX, "%d", &orderNumTemp);
                fscanf(inputFileX, "%s", &custNameTemp);
                fscanf(inputFileX, "%s", &custArea);

                printf("Could not add: W %d %d %s %s\n",timeOfDay, orderNumTemp, custNameTemp, custArea);

            }
            break;
        case 'H': //Thursday
            fscanf(inputFileX, "%d", &timeOfDay);

            if (weekDay[4].Time[timeOfDay].Delivery_Count == 0)
            {
                //Add the record
                fscanf(inputFileX, "%d", &weekDay[4].Time[timeOfDay].Order_Number[0]);
                fscanf(inputFileX, "%s", &weekDay[4].Time[timeOfDay].Customer_Name[0]);
                fscanf(inputFileX, "%s", &weekDay[4].Time[timeOfDay].Area[0]);
                //Update Delivery Count for the time
                weekDay[4].Time[timeOfDay].Delivery_Count++;
            }
            else if (weekDay[4].Time[timeOfDay].Delivery_Count == 1)
            {
                //Add the record
                fscanf(inputFileX, "%d", &weekDay[4].Time[timeOfDay].Order_Number[1]);
                fscanf(inputFileX, "%s", &weekDay[4].Time[timeOfDay].Customer_Name[1]);
                fscanf(inputFileX, "%s", &weekDay[4].Time[timeOfDay].Customer_Name[1]);
                //Update Delivery Count for the time
                weekDay[4].Time[timeOfDay].Delivery_Count++;
            }
            else
            {
                //Time period is filled
                int orderNumTemp;
                char custNameTemp[35];
                char custArea[50];
                fscanf(inputFileX, "%d", &orderNumTemp);
                fscanf(inputFileX, "%s", &custNameTemp);
                fscanf(inputFileX, "%s", &custArea);

                printf("Could not add: H %d %d %s %s\n",timeOfDay, orderNumTemp, custNameTemp, custArea);

            }
            break;
        case 'F': //Friday
            fscanf(inputFileX, "%d", &timeOfDay);

            if (weekDay[5].Time[timeOfDay].Delivery_Count == 0)
            {
                //Add the record
                fscanf(inputFileX, "%d", &weekDay[5].Time[timeOfDay].Order_Number[0]);
                fscanf(inputFileX, "%s", &weekDay[5].Time[timeOfDay].Customer_Name[0]);
                fscanf(inputFileX, "%s", &weekDay[5].Time[timeOfDay].Area[0]);
                //Update Delivery Count for the time
                weekDay[5].Time[timeOfDay].Delivery_Count++;
            }
            else if (weekDay[5].Time[timeOfDay].Delivery_Count == 1)
            {
                //Add the record
                fscanf(inputFileX, "%d", &weekDay[5].Time[timeOfDay].Order_Number[1]);
                fscanf(inputFileX, "%s", &weekDay[5].Time[timeOfDay].Customer_Name[1]);
                fscanf(inputFileX, "%s", &weekDay[5].Time[timeOfDay].Area[1]);
                //Update Delivery Count for the time
                weekDay[5].Time[timeOfDay].Delivery_Count++;
            }
            else
            {
                //Time period is filled
                int orderNumTemp;
                char custNameTemp[35];
                char custArea[50];
                fscanf(inputFileX, "%d", &orderNumTemp);
                fscanf(inputFileX, "%s", &custNameTemp);
                fscanf(inputFileX, "%s", &custArea);

                printf("Could not add: F %d %d %s %s\n",timeOfDay, orderNumTemp, custNameTemp, custArea);
            }

            break;
        default:
            printf("Not a valid day of the week\n");

        }

    }
}



/*
 * Function Name:Display Product
 * Input Parameters: number of products read
 * Returns: none
 * Function Purpose: To display the product menu to the user
*/
void displayProduct(int numRead)
{

    printf("\t |**************************************************************************|\n");
    printf("\t |************************************ PRODUCTS ****************************|\n");
    printf("\t |__________________________________________________________________________|\n\n");

   int t;

    printf("\t Item ID        Item Name    Item Price      Item Status\n");
    for(t = 0; t < numRead ; t= t + 1)
    {
        printf("\t %4d %19s %10.2f",Item[t].Item_ID, Item[t].Item_Name, Item[t].Item_Price );

        if(Item[t].Stock_Availability == 0)
        {
            printf("        OUT OF STOCK \n");
        }
        else
        {
            printf("         %5d \n",Item[t].Stock_Availability);
        }
    }
    printf("\n");
}


/*
 * Function Name: Read Customer Order File
 * Input Parameters: none
 * Returns: the amount of orders present
 * Function Purpose: Read the customer order details from the customer order file
*/
int readCustomerOrderFile()
{
    char filename [] = "Customer-orders.txt";
    FILE * inputCustomerOrders; //Use a variable name that makes sense
    int i = 0, j = 0, k;
    int countOrders = 0;

    inputCustomerOrders = fopen(filename,"r");

    if (inputCustomerOrders == NULL)
    {
        char errorMessage[100];
        strcat(errorMessage, filename);
        strcat(errorMessage, " not found!");
        fprintf(stderr, "%s", errorMessage);
        exit(1);
    }

    while( !feof(inputCustomerOrders) )
    {
        if (countOrders < ORDER_LIMIT)
        {
            int flag_NewOrder = 1; //

            int temp;

            fscanf(inputCustomerOrders,"%d",&temp);

            //Check for an existing presence
            //Go through the list from loc 0 to whatever is the present amount aka countOrders
            for (k = 0; k < countOrders; k++)
            {
                if ( Customer_Orders[k].Customer_Order_Number == temp )//found
                {
                    flag_NewOrder = 0;

                    char stringTemp[50];

                    fscanf(inputCustomerOrders,"%s",&stringTemp);
                    fscanf(inputCustomerOrders,"%s",&stringTemp);
                    fscanf(inputCustomerOrders,"%s",&stringTemp);

                    fscanf(inputCustomerOrders,"%d",
                           &Customer_Orders[k].ord[ Customer_Orders[k].Order_Count ].Order_Item_ID);

                    fscanf(inputCustomerOrders,"%s",
                           &Customer_Orders[k].ord[ Customer_Orders[k].Order_Count ].Order_itemName);

                    fscanf(inputCustomerOrders,"%d",
                           &Customer_Orders[k].ord[ Customer_Orders[k].Order_Count ].Quantity);

                    Customer_Orders[k].Order_Count++;
                }
            }
            if (flag_NewOrder == 1)
            {
                Customer_Orders[countOrders].Customer_Order_Number = temp;
                fscanf(inputCustomerOrders,"%s",&Customer_Orders[countOrders].Customer_First_Name);
                fscanf(inputCustomerOrders,"%s",&Customer_Orders[countOrders].Customer_Last_Name);
                fscanf(inputCustomerOrders,"%s",&Customer_Orders[countOrders].Delivery_Area);
                fscanf(inputCustomerOrders,"%d",&Customer_Orders[countOrders].ord[ Customer_Orders[countOrders].Order_Count ].Order_Item_ID);
                fscanf(inputCustomerOrders,"%s",&Customer_Orders[countOrders].ord[ Customer_Orders[countOrders].Order_Count ].Order_itemName);
                fscanf(inputCustomerOrders,"%d",&Customer_Orders[countOrders].ord[ Customer_Orders[countOrders].Order_Count ].Quantity);
                Number_Of_Customers++;
                Customer_Orders[countOrders].Order_Count++;
                countOrders++;
            }

        }//end if
        else break;
    }//End while loop

    fclose(inputCustomerOrders);
    return countOrders;
    //Print Test

}//End Function- read Customer orders


/*
 * Function Name: Find Product
 * Input Parameters: size of array, Item ID
 * Returns: the location of the Item
 * Function Purpose: The Search the array that holds all the products to find one product
*/
int FindProduct(int size, int ID)
{

    int loc;
    int highindex=size;
    int lowindex=0;
    int middleindex;

    while(lowindex<=highindex)
    {
        middleindex=(lowindex+highindex)/2;

        if(ID == Item[middleindex].Item_ID)
        {
            loc=middleindex;
            break;
        }

        if(ID < Item[middleindex].Item_ID)
        {
            highindex=middleindex-1;
        }
        else
        {
            lowindex=middleindex+1;
        }
    }

    if (lowindex>highindex)
    {
        loc=-1;
    }

    return loc;
}


/*
 * Function Name: Generate Order ID
 * Input Parameters: None
 * Returns: Order ID
 * Function Purpose: To create an order ID by random for an order
*/
int generate_orderid()
{
    int i=0;
    int j=0;
    int count=0;
    int same=1;
    int num;

    while(same == 1)
    {
        int lower=100, upper=999;
        srand (time(NULL));
        num=(rand() % (upper-lower + 1)) + lower;

        for(i=1; i<6; i++)
        {
            for (j = 1; j < 6; j = j + 1)
            {
                for(count=0; count<2; count ++)
                {
                    if(weekDay[i].Time[j].Order_Number[count] == num)
                    {
                        printf("\n\tDay %d at Time %d:00pm has the same order number %d as generated : %d",i,j,weekDay[i].Time[j].Order_Number[count],num);
                        printf("\n\t The number will be generated again");
                        same = 1;
                    }
                    else
                    {
                        same=0;
                        break;
                    }
                    if (same == 0) break;
                }//end for
                if (same == 0) break;
            }//end for
            if (same == 0) break;
        }//end for
    }//end while
    return num;
}



/*
 * Function Name: Delivery Times Available
 * Input Parameters: none
 * Returns: none
 * Function Purpose: To display the Delivery days and times available to the user
*/
void deliveryTimesAvailable()
{
    int i, j;
    printf("\t\t|-------------------------------------------------------------------------------------| \n");
    printf("\t\t|********************** Days and Times Available For Delivery *********************** |\n");
    printf("\t\t|************************************************************************************ |\n");
    printf("\n \t\t DAY \t\t TIME\n");
    for (i = 1; i < NUM_RECORDS; i = i + 1) //Mon - Friday
    {
        if (i==1)
        {
            printf("\n \t\t Monday \t");
        }
        if (i==2)
        {
            printf("\n \t\t Tuesday \t");
        }
        if (i==3)
        {
            printf("\n \t\t Wednesday \t");
        }
        if (i==4)
        {
            printf("\n \t\t Thursday \t");
        }
        if (i==5)
        {
            printf("\n \t\t Friday \t");
        }
        for (j = 1; j < 6; j = j + 1)
        {
            //Two deliveries per hour
            //if((weekDay[i].Time[j].Delivery_Count == 0)||(weekDay[i].Time[j].Delivery_Count == 1))
            //Check for less than 2 rather than 0 then 1
            if((weekDay[i].Time[j].Delivery_Count < 2))
            {
                printf("%d:00 pm \t",j);
            }

        }
        printf("\n");
    }
    printf("\n\t\t************************************************************************************\n");
    printf("\t\t------------------------------------------------------------------------------------\n");

}

/*
 * Function Name: ID Validation
 * Input Parameters: size of Array, Employee ID
 * Returns: Return the location of the ID
 * Function Purpose: To validate that the Employee ID entered by the user is the same as one in the file
*/
int Id_Validation(int size, int id)
{

    int loc;
    int lowindex=1;
    int highindex=size;
    int middleindex;

    while(lowindex<=highindex)
    {
        middleindex=(lowindex+highindex)/2;

        if(id == Employee[middleindex].Employee_ID)
        {
            loc = middleindex;
            break;
        }
        if(id < Employee[middleindex].Employee_ID)
        {
            highindex=middleindex-1;
        }
        else
        {
            lowindex=middleindex+1;
        }
    }

    if (lowindex>highindex)
    {
        loc=-1;
    }
    return loc;
}

/*
 * Function Name: Passcode Validation
 * Input Parameters: Location in the array, Passcode
 * Returns: 0 or -1
 * Function Purpose: To ensure that the passcode entered by the user is the same as one in the file
*/
int PasscodeValidation(int loc, int passcode)
{
    if(Employee[loc].Employee_Passcode == passcode)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/*
 * Function Name: Employee Login
 * Input Parameters: none
 * Returns: none
 * Function Purpose: Allows the employee to login into the system before access employee specific options
*/
int EmployeeLogin()
{

    int ID;
    int passcode;
    int Array_Loc=-1;


    while(Array_Loc == -1)
    {
        printf("\t |**************************************************************************|\n");
        printf("\t |**************************** EMPLOYEE LOGIN ******************************|\n");
        printf("\t |__________________________________________________________________________|\n\n\n");

        printf("\t -------------------------------------------------------------------------- \n");
        printf("\t Enter Employee ID Number: ");

        fflush(stdin);
        scanf("%d",&ID);
        printf("\n");

        Array_Loc = Id_Validation(TOTAL_EMP,ID);

        if (Array_Loc != -1)
        {
            printf("\t Employee ID is Valid \n\n ");

            // test the passcode
            int passcode_valid = -1;

            do
            {
                printf("\t Please enter your passcode: ");
                fflush(stdin);
                scanf("%d",&passcode);
                printf("\n");

                passcode_valid = PasscodeValidation(Array_Loc, passcode);

                if(passcode_valid >-1)
                {
                    printf("\t *** Successful Employee Login !!! *** \n\n");
                    Employee_IS_Logged_In = 1;
                    break;
                }
                else
                {
                    printf("\t ----------------------------------------------------------------------------------| \n");
                    printf("\t |****************************Incorrect Passcode***********************************|\n");
                    printf("\t |                                                                                 |\n");
                    printf("\t | !!!!!!!!!!!!!!!!!!!!!!!!!!!Please try again!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!| \n");
                    printf("\t |_________________________________________________________________________________|\n\n\n");
                }
            }
            while(passcode_valid == -1);

            printf("\t ******************************************************************************\n");
            printf("\t ******************************************************************************\n");
            printf("\t **********************************Employee Information************************\n");
            printf("\t  Employee ID: %d \n",Employee[Array_Loc].Employee_ID);
            printf("\t  Employee Passcode: %d \n",Employee[Array_Loc].Employee_Passcode);
            printf("\t  Employee First Name: %s \n",Employee[Array_Loc].Employee_FirstName);
            printf("\t  Employee Last Name: %s \n",Employee[Array_Loc].Employee_LastName);
            break;
        }

        else
        {
            printf("\t|------------------------------------------------------------------------|\n");
            printf("\t|****************************INVALID EMPLOYEE ID*************************|\n");
            printf("\t|                                                                        |\n");
            printf("\t| !!!!!!!!!!!!!!!!!!!!!!!!!!!Please try again!!!!!!!!!!!!!!!!!!!!!!!!!!!!| \n");
            printf("\t|________________________________________________________________________|\n\n");

        }
    }
}

/*
 * Function Name: Sales Report
 * Input Parameters: none
 * Returns: none
 * Function Purpose: To generate a Sales Report for the user to see
*/
void Salesreport()
{
    int i, k;

    printf("\t |***************************************************************************|\n");
    printf("\t |---------------------------------------------------------------------------|\n");
    printf("\t |*****************************Pure Organic868**********************************|\n");
    printf("\t ********************************************************************************\n\n");

    printf("\t                                Sales Report\n");
    printf("\t      F Name    L Name         Area    Orders   Item ID        Item Name  Quantity\n\n");

    for (i = 0; i < Number_Of_Customers; i++)
    {
        printf("\t %10s %10s %13s   %6d\n", Customer_Orders[i].Customer_First_Name,
                                Customer_Orders[i].Customer_Last_Name,
                                Customer_Orders[i].Delivery_Area,
                                Customer_Orders[i].Order_Count);

         for (k = 0; k < Customer_Orders[i].Order_Count; k++ )
         {
            printf("\t\t\t\t\t\t           %4d   %14s       %3d\n",Customer_Orders[i].ord[k].Order_Item_ID,
                                Customer_Orders[i].ord[k].Order_itemName,
                                Customer_Orders[i].ord[k].Quantity);
         }
    }
}


/*
 * Function Name: CalcBill
 * Input Parameters: first name, last name, new user
 * Returns: none
 * Function Purpose: Calculates to bill for a customer based on their orders
*/



void calcBill(char firstname[], char lastname[], int newUser)
{
    int i, k, orderFound = 0;
    char fname[20], lname[20];
    float sum, subTotal;

    if  (newUser == 1)
    {
        printf("Enter the Customer's first name: ");
        fflush(stdin);
        scanf("%s",fname);
        printf("\n");

        printf("Enter the Customer's last name: ");
        fflush(stdin);
        scanf("%s",lname);
        printf("\n");
    }
    else
    {
        strcpy(fname, firstname);
        strcpy(lname, lastname);
    }


    printf("\t                                Invoice\n\n");
    printf("\t      F Name    L Name         Area    Orders   Item ID        Item Name  Quantity   Price\n\n");

    for (i = 0; i < Number_Of_Customers; i++)
    {
        sum = 0;
        subTotal = 0;
        if ( strcasecmp(fname, Customer_Orders[i].Customer_First_Name) == 0
            && strcasecmp(lname, Customer_Orders[i].Customer_Last_Name) == 0 )
        {
            printf("\t %10s %10s %13s   %6d\n", Customer_Orders[i].Customer_First_Name,
                                Customer_Orders[i].Customer_Last_Name,
                                Customer_Orders[i].Delivery_Area,
                                Customer_Orders[i].Order_Count);

            for (k = 0; k < Customer_Orders[i].Order_Count; k++ )
             {
                printf("\t\t\t\t\t\t           %4d   %14s       %3d",Customer_Orders[i].ord[k].Order_Item_ID,
                                    Customer_Orders[i].ord[k].Order_itemName,
                                    Customer_Orders[i].ord[k].Quantity);
                float tempPrice = binarySearchWithNameForPrice(Customer_Orders[i].ord[k].Order_itemName);
                if (tempPrice != -1)
                {
                    printf("   %.2f\n",tempPrice);
                    subTotal = Customer_Orders[i].ord[k].Quantity * tempPrice;
                    sum = sum + subTotal;
                }
                else{ printf("\n");}
             }
             printf("\n\t\t\t\t\t\t\t\t\t\t  Total: %.2f\n",sum );
             orderFound = 1;
             break;
        }
    }//end for
    if (orderFound == 0)
    {
        printf("\n\t **************************************************\n");
        printf("\t No order was found for this name...\n");
        printf("\t Please Try Again!!!!!!!!!\n");
    }

}

/*
 * Function Name: Stock Availability Report
 * Input Parameters: number of items
 * Returns: none
 * Function Purpose: To generate a Stock Availability Report of the Products for the user to see
*/
void Stock_Availability_Report(int numItems)
{
    int t;
    printf("\t --------------------------------- *Pure Organics* --------------------------\n");
    printf("\t ____________________________________________________________________________\n");
    printf("\t ----------------------------------------------------------------------------\n");
    printf("\t ***************************** Stock Availability Report ******************* \n");
    printf("\t ****************************************************************************\n");
    printf("\t ITEM CODE          ITEM NAME     STOCK \n");

    for(t = 1; t > numItems ; t= t + 1)
    {
        printf("\t    %4d   %18s     %4d      \n\n  ",Item[t].Item_ID,Item[t].Item_Name,Item[t].Stock_Availability);

    }
    printf("\t ----------------------------------------------------------------------------\n");
    printf("\t ____________________________________________________________________________\n\n");
}
/*
 * Function Name: Delivery Report
 * Input Parameters: TimeSlot Structure
 * Returns: none
 * Function Purpose: To generate a Delivery Report of all the customers that ordered items
*/
void Delivery_Report(struct TimeSlot*Time)
{
    int i, k, j, x, y, z, dayNumber;
    char dayChar, dayName[20];

    printf("\t Enter the day of the week. E.g M for Monday: ");
    fflush(stdin);
    scanf("%c",&dayChar);
    printf("\n");

    dayChar = toupper(dayChar);

    switch(dayChar)
    {
        case 'M': dayNumber = 1; strcpy(dayName, "Monday"); break;
        case 'T': dayNumber = 2; strcpy(dayName, "Tuesday"); break;
        case 'W': dayNumber = 3; strcpy(dayName, "Wednesday"); break;
        case 'H': dayNumber = 4; strcpy(dayName, "Thursday"); break;
        case 'F': dayNumber = 5; strcpy(dayName, "Friday"); break;
    }

    printf("\t |***************************************************************************|\n");
    printf("\t |---------------------------------------------------------------------------|\n");
    printf("\t |*****************************Pure Organic868**********************************|\n");
    printf("\t ********************************************************************************\n\n");

    printf("\t                           Delivery Report: %s\n", dayName);
    printf("\t      F Name    L Name         Area    Orders   Item ID        Item Name  Quantity     Time\n\n");


    for (i = 1; i <= 5; i = i + 1) //Weekday
    {
        if (dayNumber == i)
        {


             for (j = 1; j <= 5; j = j + 1)//Time of day
            {
                for (k = 0; k < 2; k = k + 1) //First or Second Order
                {
                for (x = 0; x < Number_Of_Customers; x++)
                    {
                    if(weekDay[i].Time[j].Order_Number[k] == Customer_Orders[x].Customer_Order_Number)
                    {
                        printf("\t %10s %10s %13s   %6d\n", Customer_Orders[x].Customer_First_Name,
                                Customer_Orders[x].Customer_Last_Name,
                                Customer_Orders[x].Delivery_Area,
                                Customer_Orders[x].Order_Count);

                         for (y = 0; y < Customer_Orders[x].Order_Count; y++ )
                         {
                            printf("\t\t\t\t\t\t           %4d   %14s       %3d    %d:00 PM\n",Customer_Orders[x].ord[y].Order_Item_ID,
                                                Customer_Orders[x].ord[y].Order_itemName,
                                                Customer_Orders[x].ord[y].Quantity,
                                                j);
                         }
                    }
                }
            }

        }
        }

    }
}

/*
 * Function Name: Employee Program
 * Input Parameters: none
 * Returns: none
 * Function Purpose: To display the menu for the Employees to perform their operations
*/
void Employee_Program_Menu()
{

    int Keep_Looping = 1;

    while (Keep_Looping)
        if (Employee_IS_Logged_In)
        {
            int emp_option;

            printf("\t ***************************************************************************\n");
            printf("\t ***************************************************************************\n");
            printf("\t *Employee Menu*\n\n");
            printf("\t Press 1: Generate Customer Sales Report\n");
            printf("\t Press 2: Generate Stock Availability Report\n");
            printf("\t Press 3: Generate Customer order and Delivery Day Report\n");
            printf("\t Press 4: Exit Login and Return to Main Menu\n");

            printf("\t: ");
            fflush(stdin);
            scanf("%d",&emp_option);
            printf("\n");

            switch(emp_option)
            {
            case 1:
                Salesreport();
                break;

            case 2:
                Stock_Availability_Report(Number_Of_Products_Read);
                break;

            case 3:
                Delivery_Report(weekDay[1].Time);
                break;
            case 4:
                Keep_Looping = 0;
                break;

            default:
                printf("\t No valid option entered... Returning to previous menu.\n");
                break;
            }//end inner switch
        }
        else
        {
            printf("\tPlease login before attempting to use this menu...\n\n");
            Employee_IS_Logged_In = EmployeeLogin();
        }

}
/*
 * Function Name: Process Order
 * Input Parameters: none
 * Returns: none
 * Function Purpose: To process the orders made by the customer when they choose to place an order
*/
void Process_Order()
{

    sortProductStructByID();

    displayProduct(Number_Of_Products_Read);

    int Product_Location, Item_ID, Item_Quantity;

    int cancel = 0, At_Least_One_Item_Purchased = 0, ask_Again, Buy_Again, Temp_Cus_Order_Number;

    int Time_Entered, Day_Number, Delivery_Day_Time_Good = 1;

    char Yes_No_Response[25], Day_Entered, Day_Name [20];








    do
    {
        Buy_Again = 0;

        //Item ID
        do
        {
            if (Product_Location == -1)
                printf("\t Item ID not found...\n");

            printf("\t Enter a valid Item ID for your order or 999 to exit this menu: ");
            fflush(stdin);
            scanf("%d", &Item_ID);
            printf("\n");

            if (Item_ID == 999)
            {
                cancel = 1;
                break;
            }

            Product_Location = FindProduct(Number_Of_Products_Read, Item_ID);
            Customer_Orders[Number_Of_Customers].ord[ Customer_Orders[Number_Of_Customers].Order_Count ].Order_Item_ID = Item_ID;
            strcpy( Customer_Orders[Number_Of_Customers].ord[ Customer_Orders[Number_Of_Customers].Order_Count].Order_itemName ,  Item[Product_Location].Item_Name);

        }while (Product_Location == -1);




        //Item Quantity
        if ( !cancel )
        {
            printf("\t Item Name: %s\n",Item[Product_Location].Item_Name);
            printf("\t Stock Availability: %d\n\n",Item[Product_Location].Stock_Availability);


            do
            {

                if (Item[Product_Location].Stock_Availability == 0)
                {
                    printf("\t Item is out of stock... \n");
                    break;
                }

                printf("\t Enter the desired quantity or 999 to exit this menu: ");
                fflush(stdin);
                scanf("%d",&Item_Quantity);
                printf("\n");


                if (Item_Quantity == 999)
                {
                    cancel = 1;
                    break;
                }
                if (Item_Quantity < 0)
                {
                    printf("\t Quantity not valid. Choose a greater amount...\n");
                }
                else if (Item_Quantity > 0 && Item_Quantity <= Item[Product_Location].Stock_Availability)
                {
                     Item[Product_Location].Stock_Availability = Item[Product_Location].Stock_Availability - Item_Quantity;
                     At_Least_One_Item_Purchased = 1;

                     Customer_Orders[Number_Of_Customers].ord[Customer_Orders[Number_Of_Customers].Order_Count++].Quantity = Item_Quantity;
                     printf("\t Success...\n");
                     break;
                }
                else //Quantity desires exceeds current capacity
                {
                    printf("\t Quantity exceeds capacity. Choose a lesser amount...\n");
                }
            }while (Item_Quantity > Item[Product_Location].Stock_Availability);
        }

        //Take another order?
        ask_Again = 1;
        if ( !cancel )
        do
        {
            printf("\t Do you wish to purchase another item? Y/N: ");
            fflush(stdin);
            scanf("%s",&Yes_No_Response);
            printf("\n");

            if ( strcmp( Yes_No_Response, "y") == 0 ||
                 strcmp( Yes_No_Response, "Y") == 0 )
            {
                ask_Again = 0;
                Buy_Again = 1;
            }
            else if ( strcmp( Yes_No_Response, "n") == 0  ||
                 strcmp( Yes_No_Response, "N") == 0 )
           {
               ask_Again = 0;
               Buy_Again = 0;
           }

        }
        while ( ask_Again == 1 );


    }while (Buy_Again == 1); //End control do while


    //At least one item was purchased so record name and delivery info
    if (At_Least_One_Item_Purchased)
    {

        Customer_Orders[Number_Of_Customers].Customer_Order_Number = generate_orderid();

        printf("\t Enter your first name: ");
        fflush(stdin);
        scanf("%s", &Customer_Orders[Number_Of_Customers].Customer_First_Name);
        printf("\n");

        printf("\t Enter your last name: ");
        fflush(stdin);
        scanf("%s", &Customer_Orders[Number_Of_Customers].Customer_Last_Name);
        printf("\n");

        printf("\t Enter your delivery area: ");
        fflush(stdin);
        scanf("%s", &Customer_Orders[Number_Of_Customers].Delivery_Area);
        printf("\n");

        Customer_Orders[Number_Of_Customers].Customer_Order_Number = generate_orderid();

        //Delivery day and time
        deliveryTimesAvailable();


        do
        {
            int isValid = 1;
            do{
                 printf("\t ____________________________________________________________________\n");
                 printf("\t  Press M for Monday \n");
                 printf("\t  Press T for Tuesday \n");
                 printf("\t  Press W for Wednesday \n");
                 printf("\t  Press H for Thursday \n");
                 printf("\t  Press F for Friday \n");
                 printf("\t  ____________________________________________________________________\n");
                 printf("\t Please Enter desired Day of the Week: ");

                fflush(stdin);
                scanf("%c",&Day_Entered);
                printf("\n");

                Day_Entered = toupper(Day_Entered);

                if ( Day_Entered != 'M' && Day_Entered != 'T' &&
                     Day_Entered != 'W' && Day_Entered != 'H' &&
                     Day_Entered != 'F' )
                {
                    isValid = 0;
                    printf("\n\t Please choose a valid day!\n");
                }
                else
                {
                    switch(Day_Entered)
                    {
                        case 'M': Day_Number = 1; strcpy(Day_Name, "Monday"); break;
                        case 'T': Day_Number = 2; strcpy(Day_Name, "Tuesday");  break;
                        case 'W': Day_Number = 3; strcpy(Day_Name, "Wednesday");  break;
                        case 'H': Day_Number = 4; strcpy(Day_Name, "Thursday");  break;
                        case 'F': Day_Number = 5; strcpy(Day_Name, "Friday");  break;
                    }
                    isValid = 1;
                }


            }while (isValid == 0);


            printf("\n\tPlease enter a Time that is Available on Monday between 1 to 5 (pm): ");
            fflush(stdin);
            scanf("%d",&Time_Entered);
            printf("\n");

            if( Time_Entered < 0 || Time_Entered > 5 )
            {
                printf("\t This time value is not valid...\n");
                printf("\t Try again.\n");
                Delivery_Day_Time_Good = 0;
            }
            else
            {
                if ( weekDay[Day_Number].Time[Time_Entered].Delivery_Count < 2)
                {

                    if (weekDay[Day_Number].Time[Time_Entered].Delivery_Count == 0)
                    {
                        //Add the record
                        weekDay[Day_Number].Time[Time_Entered].Order_Number[0] = Customer_Orders[Number_Of_Customers].Customer_Order_Number ;

                        //Update Delivery Count for the time
                        weekDay[Day_Number].Time[Time_Entered].Delivery_Count++;
                    }
                    else if (weekDay[Day_Number].Time[Time_Entered].Delivery_Count == 1)
                    {
                        //Add the record
                        weekDay[Day_Number].Time[Time_Entered].Order_Number[1] = Customer_Orders[Number_Of_Customers].Customer_Order_Number ;;

                        //Update Delivery Count for the time
                        weekDay[Day_Number].Time[Time_Entered].Delivery_Count++;
                    }
                    Delivery_Day_Time_Good = 1;


                }//end if
                else
                {
                    printf("\n\t There is no more space on %s at time %d:00pm for delivery...\n", Day_Name, Time_Entered);
                    printf("\t Please try again...\n\n");
                    Delivery_Day_Time_Good = 0;
                }
            }//end else
        }while (Delivery_Day_Time_Good == 0);

        if (Delivery_Day_Time_Good == 1)
        {
            printf("\t Success...\n");

             char tempFName [20];
             char tempLName [20];

             strcpy(tempFName, Customer_Orders[Number_Of_Customers].Customer_First_Name);
             strcpy(tempLName, Customer_Orders[Number_Of_Customers].Customer_Last_Name);
            //Customer_Orders[Number_Of_Customers].Order_Count++;
            Number_Of_Customers++;

            calcBill(tempFName, tempLName,0);
        }


    }//End At_Least_One_Item_Purchased

}

/*
 * Function Name: Exit MY Program
 * Input Parameters: None
 * Returns: Nothing
 * Function Purpose: To allow the user to exit the program
*/
void Exit_My_Program()
{
    printf("\t-----------------------------------------------------------------------\n");
    printf("\t***********************************************************************\n");
    printf("\t-----------------------------------------------------------------------\n\n");
    printf("\t-------------Thank you for Shopping with Pure Organics 868-------------\n");
    printf("\t----------------------Please Purchase from Us again!-------------------\n\n");
    printf("\t-----------------------------------------------------------------------\n");
    printf("\t***********************************************************************\n");
    printf("\t-----------------------------------------------------------------------\n");

}

/*
 * Function Name: Main
 * Input Parameters: none
 * Returns: none
 * Function Purpose: To display the main menu for the users and perform their request.
*/
int main()
{

    readAndLoadStructs();

    int numEmployeeRead = readEmployeeStaff();

    int menuOption = 0;

    int location, isValid = 0;

    Number_Of_Customers = 0;

    Number_Of_Products_Read = readProductItems();  //The amount of records read

    readCustomerOrderFile();

    Employee_IS_Logged_In = 0;

    while (menuOption!=999)
    {
        printf("\t                                  ********** \n\n");
        printf("\t|-----------------------------------------------------------------------------|\n");
        printf("\t|-----------------------------* Pure Organics 868 *---------------------------|\n");
        printf("\t|-----------------------------* 100 Natural Blends *--------------------------|\n");
        printf("\t|---------------------- All the Nutrients your Skin Needs --------------------|\n");
        printf("\t|_____________________________________________________________________________|\n\n");
        printf("\t\t\t\t\t  **********\n\n");
        printf("\t _____________________________________________________________________________\n");

       do
       {
            printf("\t |*MAIN MENU*                                                                 |\n");
            printf("\t -----------------------------------------------------------------------------\n");
            printf("\t Press 1:     Make an Order \n");
            printf("\t Press 2:     Check your Order \n");
            printf("\t Press 3:     Employee Login \n");
            printf("\t Press 4:     Employee Specific Operations \n");
            printf("\t Press 999:   Exit Menu \n");
            printf("\t _____________________________________________________________________________\n\n");
            printf("\t Please Enter your Option: ");

            fflush(stdin);
            int result = scanf("%d", &menuOption);
            printf("\n");

            if ( result )
            {
                 if  ( (menuOption > 0 && menuOption < 5)  || menuOption == 999  )
                    isValid = 1;
            }
            else
            {
                printf("\t Please enter a valid value...\n");
            }
       }while (isValid == 0);


        switch(menuOption)
        {
        case 1:
            Process_Order();
            break;

        case 2:
            calcBill("","",1);
            break;

        case 3:
            Employee_IS_Logged_In = EmployeeLogin();
            break;

        case 4:
            Employee_Program_Menu();
            break;

        case 999:
            Exit_My_Program();
            break;

        default:
            printf("\t Root Menu: Invalid entry...\n");
        }//end switch
    }//end while
    return 0;
}//end main
