/*
   This simple "Library System" program is based on an existing practice project from the internet
   Since I've rewritten so much of it I consider this my "own creation".
*/

 

#include <windows.h>			
#include <stdio.h>			
#include <conio.h>			//getch() is used a few times
#include <stdlib.h>			
#include <string.h>                  	
#include <ctype.h>                   	                 
#include <time.h>			

#define RETURNTIME 15
#define STNAME_LENGTH 32		//length of char stname[] in struct books
#define NAME_LENGTH 32			//length of char name[] in struct books
#define AUTHOR_LENGTH 32		//length of char Author[] in struct books
#define STR_LENGTH 6		        //length of the var temp_str array

//prototyping of the functions
void gotoxy (int x, int y);
void clearInput(void);
void mainMenu(void);
void addBooks(void);
void deleteBooks(void);
void searchBooks(void);
void editBooks(void);
void issueBooks(void);
void viewBooks(void);
long strToLong(char *str);		//instead of using scanf for numeric input
float strToFloat(char *str);
void returnFunc(int menu_nr, char *menu_name);
void openFile(void);
void saveToFile(void);
int  checkId(long id);
int checkName(char *name);
int Password(int n);
void issueRecord(void);
void closeApplication(void);

FILE *fp;

//global variables
char *filename="librabry.dat";											//used in openFile() and saveToFile()
char password[10]={"cproject"};											//used in Password()
char categories[][20]={"Computer","Sports","Self-Help","Nature","Science","(Auto)biography"};		        //used in addBooks()
COORD coord = {0, 0};												//Upper left corner -> used by function void gotoxy(void)

// structures used in the program
struct date {
	int mm;
	int dd;
	int yy;
};

struct date empty = {0}; 	//used in issueBooks -> remove from list

struct books {
	long id;
	char stname[STNAME_LENGTH];
	char name[NAME_LENGTH];
	char Author[AUTHOR_LENGTH];
	long quantity;
	float Price;
	long count;
	long rackno;
	char *cat;
	char borrow;
	struct date issued;
	struct date duedate;
	struct books *next;
};
struct books *start, *temp, *new, *search_temp;		

int main()
{
	int attempt=4;
	int pass=0;
	
	while(attempt>0)
	{
		clearInput();
		if((Password(attempt))==0)
		{
			pass=1;
			break;
		}
		else
			attempt--;
	}
	if(pass==1)
		mainMenu();
	else
		closeApplication();

	return(0);	
}

//Function allows to print text on any place on the screen -> it places the cursor on a desired location on screen
void gotoxy (int x, int y)											
{
	coord.X = x; coord.Y = y; // X and Y coordinates
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


void clearInput(void)
{
	fflush(stdin);
}

void mainMenu()			
{
	char ch;
	time_t t;

	clearInput();
	openFile();	//test 12/05/2020
	system("cls");

	gotoxy(20,3);
	printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 MAIN MENU \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
	gotoxy(20,5);	
	printf("\xDB\xDB\xDB\xDB\xB2 1. Add Books   ");
	gotoxy(20,7);
	printf("\xDB\xDB\xDB\xDB\xB2 2. Delete books");
	gotoxy(20,9);
	printf("\xDB\xDB\xDB\xDB\xB2 3. Search Books");
	gotoxy(20,11);
	printf("\xDB\xDB\xDB\xDB\xB2 4. Issue Books");
	gotoxy(20,13);
	printf("\xDB\xDB\xDB\xDB\xB2 5. View Book list");
	gotoxy(20,15);
	printf("\xDB\xDB\xDB\xDB\xB2 6. Edit Book Record");
	gotoxy(20,17);
	printf("\xDB\xDB\xDB\xDB\xB2 7. Close Application");
	gotoxy(20,19);
	printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
	gotoxy(20,20);

	time(&t);
	printf("Date and time:%s\n",ctime(&t));

	gotoxy(20,21);
	printf("Enter your choice: ");
	switch(ch=getchar())
	{
		case '1':
			addBooks();
			break;
		case '2':
			deleteBooks();
			break;
		case '3':
			searchBooks();
			break;
		case '4':
			issueBooks();
			break;
		case '5':
			viewBooks();
			break;
		case '6':
			editBooks();
			break;
		case '7':
			closeApplication();
			break;
		default:
			gotoxy(10,23);
			printf("\aWrong Entry!!Please hit a key and re-enter correct option");
			getch();
			mainMenu();
	}
}
void addBooks(void)    //funtion that add books
{
	char ch;
	int n;
	long bookid, cat_id;
	char temp_str[STR_LENGTH];

	clearInput();
	system("cls");

	gotoxy(20,5);
	printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 SELECT CATEGOIES \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
	gotoxy(20,7);
	printf("\xDB\xDB\xDB\xDB\xB2 1. %s", categories[0] );
	gotoxy(20,9);
	printf("\xDB\xDB\xDB\xDB\xB2 2. %s", categories[1]);
	gotoxy(20,11);
	printf("\xDB\xDB\xDB\xDB\xB2 3. %s", categories[2]);
	gotoxy(20,13);
	printf("\xDB\xDB\xDB\xDB\xB2 4. %s", categories[3]);
	gotoxy(20,15);
	printf("\xDB\xDB\xDB\xDB\xB2 5. %s", categories[4]);
	gotoxy(20,17);
	printf("\xDB\xDB\xDB\xDB\xB2 6. %s", categories[5]);
	gotoxy(20,19);
	printf("\xDB\xDB\xDB\xDB\xB2 7. Back to main menu");
	gotoxy(20,21);
	for(n=0;n<41;n++) printf("\xB2");
	gotoxy(20,22);

	printf("Enter your choice:");
	gotoxy(39,22);
	fgets(temp_str, sizeof(temp_str), stdin);
	cat_id=strToLong(temp_str);		
	if(cat_id==7)
		mainMenu();
	
	//Set up the screen layout
	system("cls");
	gotoxy(20,3);
	printf("Enter the Information Below");
	gotoxy(20,4);
	for(n=0; n<44; n++) printf("\xB2");
	gotoxy(20,5); printf("\xB2");
	gotoxy(63,5); printf("\xB2");
	gotoxy(20,6); printf("\xB2");
	gotoxy(63,6); printf("\xB2");
	gotoxy(20,7); printf("\xB2");
	gotoxy(63,7); printf("\xB2");
	gotoxy(20,8); printf("\xB2");
	gotoxy(63,8); printf("\xB2");
	gotoxy(20,9); printf("\xB2");
	gotoxy(63,9); printf("\xB2");
	gotoxy(20,10); printf("\xB2");
	gotoxy(63,10); printf("\xB2");
	gotoxy(20,11); printf("\xB2");
	gotoxy(63,11); printf("\xB2");
	gotoxy(20,12);
	for(n=0; n<44; n++) printf("\xB2");
	gotoxy(21,5);
	printf("Category:");
	gotoxy(31,5);
	printf("%s",categories[cat_id-1]);
	
	clearInput();
	gotoxy(21,6);
	printf("Book ID:\t");
	gotoxy(30,6);
	fgets(temp_str, sizeof(temp_str), stdin);
	bookid=strToLong(temp_str);		

	//openFile();
	if(checkId(bookid) == 0)
	{
		new=(struct books *)malloc(sizeof(struct books));
		if(start==NULL)
			start=temp=new;
		else
		{
			temp=start;				//makes the first current to loop through all records
			while(temp->next != NULL)
				temp=temp->next;		//last record found
			temp->next=new	;			//save the address of nw
			temp=new;				//Set the new current
		}

	
		// Fill the new structure
		temp->id=bookid;

		clearInput();
		gotoxy(21,7);
		printf("Book Name:");

		gotoxy(33,7);
		fgets(temp->name, NAME_LENGTH-1, stdin);
		clearInput();
		gotoxy(21,8);

		printf("Author:");
		gotoxy(30,8);
		fgets(temp->Author, AUTHOR_LENGTH-1, stdin);
		clearInput();

		gotoxy(21,9);
		printf("Quantity:");
		gotoxy(31,9);
		fgets(temp_str, sizeof(temp_str), stdin);
		temp->quantity=strToLong(temp_str);			
		clearInput();

		gotoxy(21,10);
		printf("Price:");
		gotoxy(28,10);
		fgets(temp_str, sizeof(temp_str), stdin);
		temp->Price=strToFloat(temp_str);	
		clearInput();
		
		gotoxy(21,11);
		printf("Rack No:");gotoxy(30,11);
		fgets(temp_str, sizeof(temp_str), stdin);
		temp->rackno=strToLong(temp_str);	
	
		temp->borrow='N'; 			
		temp->next=NULL;		//cap the new record with a NULL pointer -> last record
	
		temp->cat=categories[cat_id-1];	

		saveToFile();
		gotoxy(21,14);
		printf("The record is sucessfully saved");
		gotoxy(21,16);
		
		printf("Add another book?[Y/N]: ");
		do
		{
			clearInput();
			switch(ch=toupper(getchar()))
			{
				case 'N':
					mainMenu();
					break;
				case 'Y':
					addBooks();
					break;
				default:
					gotoxy(21,17);
					printf("Invalid input, select Y/N: ");
			}
		}
		while(ch != 'N' || ch != 'Y');
	}
	if(checkId(bookid) == 1)
	{
		gotoxy(21,13);
		printf("\aThe book id already exists\a");
		getch();
		addBooks();
	}
}

void deleteBooks()    //function that delete items from file fp
{
	long bookid;
	char ch;
	char temp_str[STR_LENGTH];
	struct books *previous;

	system("cls");
	clearInput();

	printf("*****************************Delete Books*********************************");
	gotoxy(20,6);
	printf("\xDB\xDB\xDB\xB2 1. Delete By ID");
	gotoxy(20,10);
	printf("\xDB\xDB\xDB\xB2 2. Back to main menu");
	gotoxy(20,16);
	printf("Enter Your Choice: ");
	
	
	switch(ch=getchar())
	{
		case '1':
			//openFile();
			system("cls");
			clearInput();
			gotoxy(10,5);
			printf("Enter the Book ID to delete: ");
			fgets(temp_str, sizeof(temp_str), stdin);
			bookid=strToLong(temp_str);	

			if(checkId(bookid)==0)
			{
				gotoxy(10,10);
				printf("No record is found modify the search");
				getch();
				deleteBooks();	
			}
			gotoxy(10,7);
			printf("The book record is available");
			gotoxy(10,8);
			printf("Book name is: %s",search_temp->name);
			gotoxy(10,9);
			printf("Rack No. is: %d",search_temp->rackno);
			gotoxy(10,11);
			printf("Delete record?[Y/N]: ");
			clearInput();
			ch=toupper(getchar()); 
	
			if(ch=='N')
				deleteBooks();

			else if(ch=='Y')
			{	
				temp=start;
				while(temp != NULL)
				{	
					if(temp->id==bookid)
					{
						if(temp==start)	// special condition
							start=temp->next;
						else
							previous->next=temp->next;
						free(temp);
						gotoxy(10,14);
						saveToFile();
						printf("The record is sucessfully deleted");
						break;
					}
					else
					{
						previous=temp;
						temp=temp->next;
					}
				}	//end of while loop
				gotoxy(10,16);
				printf("Delete another record?(Y/N) ");
				do
				{
					clearInput();
					switch(ch=toupper(getchar()))
					{
						case 'N':
							mainMenu();
							break;
						case 'Y':
							deleteBooks();
							break;
						default:
							system("cls");
							gotoxy(10,13);
							printf("Invalid input, select Y/N: ");
					}	
				}
				while(ch != 'N' || ch != 'Y');
			}
			else
			{
				gotoxy(10,12);
				printf("invalid choice");
				gotoxy(10,13);
				printf("Press Enter to go back");
				getchar();
				deleteBooks();
			}
			break;
		case '2':
			mainMenu();
			break;
		default:
			gotoxy(20,18);
			printf("Invalid input...");
			getch();
			deleteBooks();
	}
}

void searchBooks()
{
	long bookid;
	int n;
	char ch;
	char temp_str[STR_LENGTH];	

	char *name;
	name=(char *)malloc(NAME_LENGTH);

	//openFile();
	system("cls");
	clearInput();

	printf("*****************************Search Books*********************************");
	gotoxy(20,6);
	printf("\xDB\xDB\xDB\xB2 1. Search By ID");
	gotoxy(20,10);
	printf("\xDB\xDB\xDB\xB2 2. Search By Name");
	gotoxy(20,14);
	printf("\xDB\xDB\xDB\xB2 3. Back to main menu");
	gotoxy( 15,20);
	printf("Enter Your Choice: ");
	
	switch(ch=getchar())
	{
		case '1':
			system("cls");
			clearInput();
			gotoxy(25,4);
			printf("****Search Books By Id****");
			gotoxy(20,5);
			printf("Enter the book id: ");
			fgets(temp_str, sizeof(temp_str), stdin);
			bookid=strToLong(temp_str);	
			gotoxy(20,7);
			printf("Searching........");
			sleep(1000);
			if(checkId(bookid)==1)
			{
				gotoxy(20,7);
				printf("The Book is available");
				gotoxy(20,8);
				for(n=0; n<44; n++) printf("\xB2");
				gotoxy(20,9);
				printf("\xB2 ID:%d",search_temp->id);gotoxy(63,9);printf("\xB2");
				gotoxy(20,10);
				printf("\xB2 Name:%s",search_temp->name);gotoxy(63,10);printf("\xB2");
				gotoxy(20,11);
				printf("\xB2 Author:%s ",search_temp->Author);gotoxy(63,11);printf("\xB2");
				gotoxy(20,12);
				printf("\xB2 Qantity:%d ",search_temp->quantity);gotoxy(63,12);printf("\xB2"); 
				gotoxy(20,13);
				printf("\xB2 Price:%.2f",search_temp->Price);gotoxy(63,13);printf("\xB2");
				gotoxy(20,14);
				printf("\xB2 Rack No:%d ",search_temp->rackno);gotoxy(63,14);printf("\xB2");
				gotoxy(20,15);
				for(n=0; n<44; n++) printf("\xB2");
				gotoxy(20,17);
				returnFunc(3, "Search Books");
			}
			if(checkId(bookid)==0)
			{	
				gotoxy(20,8);
				printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				gotoxy(20,9);printf("\xB2");  gotoxy(38,9);printf("\xB2");
				gotoxy(20,10);
				printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				gotoxy(22,9);printf("\aNo Record Found");
				gotoxy(20,17);
				printf("New search?(Y/N) ");
				clearInput();
				if((ch=toupper(getchar()))=='Y')
					searchBooks();
				else
					mainMenu();
			}
			break;	
	
		case '2':
			system("cls");
			gotoxy(25,4);
			printf("****Search Books By Name****");
			gotoxy(20,5);
			printf("Enter Book Name: ");
			clearInput();
			fgets(name, NAME_LENGTH-1, stdin);
			gotoxy(20,7);
			printf("Searching........");
			sleep(1000);
			if(checkName(name)==0)
			{
				gotoxy(20,7);
				printf("The Book is available");
				gotoxy(20,8);
				for(n=0; n<44; n++) printf("\xB2");
				gotoxy(20,9);
				printf("\xB2 ID:%d",search_temp->id);gotoxy(63,9);printf("\xB2");
				gotoxy(20,10);
				printf("\xB2 Name:%s",search_temp->name);gotoxy(63,10);printf("\xB2");
				gotoxy(20,11);
				printf("\xB2 Author:%s ",search_temp->Author);gotoxy(63,11);printf("\xB2");
				gotoxy(20,12);
				printf("\xB2 Qantity:%d ",search_temp->quantity);gotoxy(63,12);printf("\xB2");
				gotoxy(20,13);
				printf("\xB2 Price:%.2f",search_temp->Price);gotoxy(63,13);printf("\xB2");
				gotoxy(20,14);
				printf("\xB2 Rack No:%d ",search_temp->rackno);gotoxy(63,14);printf("\xB2");
				gotoxy(20,15);
				for(n=0; n<44; n++) printf("\xB2");
				gotoxy(20,17);
				returnFunc(3, "Search Books");
			}
			if(checkName(name)==1)
			{	
				sleep(1000);
				gotoxy(20,8);
				printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				gotoxy(20,9);printf("\xB2");  gotoxy(38,9);printf("\xB2");
				gotoxy(20,10);
				printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
				gotoxy(22,9);printf("\aNo Record Found");
				gotoxy(20,17);
				printf("Try another search?(Y/N) ");
				clearInput();
				if((ch=toupper(getchar()))=='Y')
					searchBooks();
				else
					mainMenu();
			}
			break;

		case '3':
			mainMenu();
			break;

		default :
			printf("Invalid input...");
			getch();
			searchBooks();
	}
}

void issueBooks(void)  //function that issue books from library
{
	char ch;
	long bookid;
	int j;
	char another='Y';
	char nullstring[]="";
	char temp_str[STR_LENGTH];

	struct tm *t;
	time_t now;
	time(&now);
	t=localtime(&now);
	
	system("cls");
	clearInput();

	printf("********************************ISSUE SECTION**************************");
	gotoxy(10,5);
	printf("\xDB\xDB\xDB\xDb\xB2 1. Issue a Book");
	gotoxy(10,7);
	printf("\xDB\xDB\xDB\xDb\xB2 2. View Issued Books");
	gotoxy(10,9);
	printf("\xDB\xDB\xDB\xDb\xB2 3. Search Issued Books");
	gotoxy(10,11);
	printf("\xDB\xDB\xDB\xDb\xB2 4. Remove Issued Books");
	gotoxy(10,13);
	printf("\xDB\xDB\xDB\xDb\xB2 5. Back to main menu");
	gotoxy(10,17);

	printf("Enter a Choice: ");
	switch(ch=getchar())
	{
		case '1':  //issue book

			system("cls");
			while(another=='Y')
			{
				system("cls");
				gotoxy(15,4);
				printf("***Issue Book section***");
				gotoxy(10,6);
				printf("Enter the Book Id: ");
				clearInput();
				fgets(temp_str, sizeof(temp_str), stdin);
				bookid=strToLong(temp_str);

				//openFile();
				if((checkId(bookid))==1) 
				{
					gotoxy(10,8);
					printf("The book record is available");
					gotoxy(10,9);
					printf("Currently, there are %d unissued copies of %s",search_temp->quantity, search_temp->name);
					

					gotoxy(10,11);
					printf("Enter name: ");
					clearInput();
					fgets(search_temp->stname, STNAME_LENGTH-1, stdin);					

					search_temp->issued.dd=t->tm_mday;
					search_temp->issued.mm=t->tm_mon+1;
					search_temp->issued.yy=t->tm_year+1900;

					search_temp->quantity-=1;
					search_temp->borrow = 'Y';

					gotoxy(10,12);
					printf("Book: %s",search_temp->name);
					gotoxy(10,13);
					printf("Issued on: %d-%d-%d",search_temp->issued.dd,search_temp->issued.mm,search_temp->issued.yy);

					search_temp->duedate.dd=search_temp->issued.dd+RETURNTIME;   //for return date
					search_temp->duedate.mm=search_temp->issued.mm;
					search_temp->duedate.yy=search_temp->issued.yy;
					if(search_temp->duedate.dd>30)
					{
						search_temp->duedate.mm+=search_temp->duedate.dd/30;
						search_temp->duedate.dd-=30;
					}
					if(search_temp->duedate.mm>12)
					{
						search_temp->duedate.yy+=search_temp->duedate.mm/12;
						search_temp->duedate.mm-=12;
					}
					gotoxy(10,15);
					printf("Return date: %d-%d-%d",search_temp->duedate.dd,search_temp->duedate.mm,search_temp->duedate.yy);
					saveToFile();
				}
				if((checkId(bookid))==0)
				{
					gotoxy(10,11);
					printf("No record found");
				}
				gotoxy(10,18);
				printf("Issue another book?(Y/N): ");
				clearInput();
				another=toupper(getchar());
			}
			issueBooks();
			break;

		case '2':  //show issued book list

			system("cls");
			//openFile();	

			j=4;
			printf("*********************************************Issued book list*********************************************\n");
			gotoxy(2,2); printf("ISSUED TO");
			gotoxy(33,2); printf("CATEGORY");
			gotoxy(46,2); printf("ID");
			gotoxy(51,2); printf("BOOK NAME");
			gotoxy(82,2); printf("ISSUED DATE");
			gotoxy(95,2); printf("RETURN DATE");
			
			temp=start;
			while(temp != NULL)
			{
				if(temp->borrow=='Y')
				{	
					gotoxy(2,j);
					printf("%s",temp->stname);
					gotoxy(33,j);
					printf("%s",temp->cat);
					gotoxy(46,j);
					printf("%d",temp->id);
					gotoxy(51,j);
					printf("%s",temp->name);
					gotoxy(82,j);
					printf("%d-%d-%d",temp->issued.dd,temp->issued.mm,temp->issued.yy );
					gotoxy(95,j);
					printf("%d-%d-%d",temp->duedate.dd,temp->duedate.mm,temp->duedate.yy);
					gotoxy(106,25);
					j++;
				}
				temp=temp->next;
			}
			gotoxy(1,25);
			returnFunc(5, "Issue Books");
			break;
			
		case '3':   //search issued books by id
			system("cls");
			//openFile();
			another='Y';
			while(another=='Y')
			{
				system("cls");
				clearInput();
				gotoxy(10,6);
				printf("Enter Book ID: ");

				fgets(temp_str, sizeof(temp_str), stdin);
				bookid=strToLong(temp_str);

				if(checkId(bookid)==1 && search_temp->borrow=='Y')
				{
					issueRecord();
					gotoxy(10,12);
					returnFunc(5, "Issue Books");
				}
				
				else
				{
					gotoxy(10,8);
					printf("No Record Found");
				}
				gotoxy(10,13);
				printf("New Search?(Y/N): ");
				clearInput();
				another=toupper(getchar());
			}
			issueBooks();
			break;
			
		case '4':  //remove issued books from list
			system("cls");
			//openFile();
			another='Y';
			while(another=='Y')
			{
				system("cls");
				gotoxy(10,5);
				printf("Enter book id to remove from the issued list: ");
				clearInput();
				fgets(temp_str, sizeof(temp_str), stdin);
				bookid=strToLong(temp_str);

				if(checkId(bookid)==1 && search_temp->borrow=='Y')
				{
					issueRecord();
					gotoxy(10,11);
					printf("Press a key to continue...");
					clearInput();
					getch();
					gotoxy(10,12);
					printf("Remove the title from the list of issued books?(Y/N): ");
					clearInput();
					if((ch=toupper(getchar()))=='Y')
					{
						strncpy(search_temp->stname, nullstring, sizeof(nullstring));
						search_temp->borrow='N';
						search_temp->issued=empty;	
						search_temp->duedate=empty;
						search_temp->quantity+=1;
						gotoxy(10,14);
						saveToFile();
						printf("Succesfully removed from the list of issued books");
					}
				}
				else
				{
					gotoxy(10,15);
					printf("No Record Found");
				}
				gotoxy(10,16);
				printf("New Search?(Y/N): ");
				clearInput();
				another=toupper(getchar());
			} // end of while-loop
			issueBooks();
			break;

		case '5':
			mainMenu();
			break;
		
		default:
			gotoxy(10,18);
			printf("\aWrong Entry!!");
			getch();
			issueBooks();
			break;
	}	//end of switch
}

void viewBooks(void)  //show the list of book persists in library
{
	int n,i,j;

	n=0;
	i=1;
	j=4;

	system("cls");
	//openFile();
	
	gotoxy(1,1);
	printf("****************************************************Book List**************************************************");
	gotoxy(3,2); printf("CATEGORY");
	gotoxy(16,2); printf("ID");
	gotoxy(22,2); printf("BOOK NAME");
	gotoxy(54,2); printf("AUTHOR");
	gotoxy(86,2); printf("QTY");
	gotoxy(93,2); printf("PRICE");
	gotoxy(106,2); printf("RackNo");



	temp=start;	//makes the first current to loop through all records
	

	while(temp != NULL)
	{	
		if(temp->borrow=='Y' || temp->borrow=='N')
		{
			gotoxy(2,j);
			printf("%s",temp->cat);
			gotoxy(16,j);
			printf("%d",temp->id);
			gotoxy(22,j);
			printf("%s",temp->name);
			gotoxy(54,j);
			printf("%s",temp->Author);
			gotoxy(86,j);
			printf("%d",temp->quantity);
			gotoxy(93,j);
			printf("%.2f",temp->Price);
			gotoxy(106,j);
			printf("%d",temp->rackno);
			printf("\n\n");

			j++;
			n=n+temp->quantity;
		}
		i++;
		temp=temp->next;
		
	}
	
	gotoxy(3,25);
	printf("Total Books = %d",n);
	gotoxy(35,25);
	clearInput();
	returnFunc(0, "Main");
}

void editBooks(void)  //edit information about book
{
	long bookid;
	char ch;
	char temp_str[STR_LENGTH];
	
	
	system("cls");
	clearInput();
	gotoxy(20,4);

	printf("****Edit Books Section****");
	gotoxy(20,6);
	printf("\xDB\xDB\xDB\xB2 1. Edit By ID");
	gotoxy(20,10);
	printf("\xDB\xDB\xDB\xB2 2. Back to main menu");
	gotoxy(20,16);
	printf("Enter Your Choice: ");
	
	
	switch(ch=getchar())
	{
		case '1':
			//openFile();
			system("cls");
			clearInput();
			gotoxy(15,6);
			printf("Enter Book Id to be edited: ");
			fgets(temp_str, sizeof(temp_str), stdin);
			bookid=strToLong(temp_str);	

			if(checkId(bookid)==1)
			{
				gotoxy(15,7);
				printf("The book is available");
				gotoxy(15,8);
				printf("Book ID:%d",search_temp->id);
				gotoxy(15,9);
				clearInput();
				printf("Enter new name: "); fgets(search_temp->name, NAME_LENGTH-1, stdin);
				gotoxy(15,10);
				clearInput();
				printf("Enter new Author: "); fgets(search_temp->Author, AUTHOR_LENGTH-1, stdin);
				gotoxy(15,11);
				clearInput();		
				printf("Enter new quantity: "); 
				fgets(temp_str, sizeof(temp_str), stdin);
				search_temp->quantity=strToLong(temp_str);	
				gotoxy(15,12);
				clearInput();
				printf("Enter new price: ");
				fgets(temp_str, sizeof(temp_str), stdin);
				search_temp->Price=strToFloat(temp_str);	
				gotoxy(15,13);
				clearInput();
				printf("Enter new rackno: "); 
				fgets(temp_str, sizeof(temp_str), stdin);
				search_temp->rackno=strToLong(temp_str);	
				gotoxy(15,14);
				saveToFile();
				printf("The record is modified");
			}
			else if(checkId(bookid)==0)
			{
				gotoxy(15,9);
				printf("No record found");
			}
			else
			{
				printf("Unsuspected error...");
				sleep(2000);
				closeApplication;
			}
			gotoxy(15,16);
			printf("Modify another Record?(Y/N) ");
			do
			{
				clearInput();
				switch(ch=toupper(getchar()))
				{
					case 'N':
						mainMenu();
						break;
					case 'Y':
						editBooks();
						break;
					default:
						gotoxy(21,17);
						printf("Invalid input, select Y/N: ");
				}
			}
			while(ch != 'N' || ch != 'Y');
			break;
		case '2':
			mainMenu();
			break;
		default:
			gotoxy(20,18);
			printf("Invalid input...");
			getch();
			editBooks();
	}
}

long strToLong(char *str)
{
	long n;
	char *eptr;

	eptr=(char *)malloc(sizeof(eptr));

	n=strtol(str, &eptr, 10);
	free(eptr); eptr=NULL;
	return(n);
}

float strToFloat(char *str)
{
	float f;
	char *eptr;

	eptr=(char *)malloc(sizeof(eptr));


	f=strtof(str, &eptr);
	free(eptr); eptr=NULL;
	return(f);	
}

void returnFunc(int menu_nr, char *menu_name)
{
	while(1)
	{
		printf(" Press ENTER to return to the %s menu", menu_name);
		if(getch()==13) //allow only use of enter
			break;
	} // end of while loop

	switch(menu_nr)
	{
		case 0:
		       	mainMenu();
			break;
		case 1:
			addBooks();
			break;
		case 2:
			deleteBooks();
			break;
		case 3:
			searchBooks();
			break;
		case 4:
			editBooks();
			break;
		case 5:
			issueBooks();
			break;
		case 6:
			viewBooks();
			break;
	}
}

void openFile(void)
{
	fp=fopen(filename, "a+");	// can be changed to "r" if the file exists
	if(fp)				// assume doesn't exist otherwise
	{
		start=(struct books *)malloc(sizeof(struct books));
		temp=start;
		while(1)
		{
			new=(struct books *)malloc (sizeof(struct books));
			fread(temp, sizeof(struct books), 1, fp);
			if(temp->next==NULL)
				break;
			temp->next=new;
			temp=new;
		} 
		fclose(fp);
	}
	else
	{
		printf("Error opening \"Database file\" %s...", filename);
		sleep(1000);
		mainMenu();
	}
}

void saveToFile(void)
{
	temp=start;
	if(temp==NULL)
	{
		printf("No data to write to file %s...\n", filename);
		printf("Nothing can be saved");
		sleep(1000);
		mainMenu();
	}
	fp=fopen(filename, "w");
	if(fp==NULL)
	{
		printf("Error opening \"Database file\" %s...", filename);
		sleep(1000);
		mainMenu();
	}
	while(temp != NULL)
	{
		fwrite(temp, sizeof(struct books), 1, fp);
		temp=temp->next;
	}
	fclose(fp);
}

int checkId(long id)  //check for book in library by Id
{

	search_temp=start;	//makes the first current to loop through all records
	while(search_temp != NULL)
	{
		if(search_temp->id==id)
			return(1);  //returns 1 if book exists
		search_temp=search_temp->next;
	}	
	return(0); 	//return 0 if book doesn't exist
}

int checkName(char *name)  //check for book in library by name
{

	search_temp=start;			//makes the first current to loop through all records
	while(search_temp != NULL)
	{
		if((strcmp(search_temp->name, name))==0)
			return(0);  //returns 0 if book exists
		search_temp=search_temp->next;
	}	
	return(1); 	//return 1 if book doesn't exist
}

int Password(int n) //checks for valid password input
{
	char d[25]="Password Protected";
	char ch;
	char input[10];
	int j, i;
	
	
		system("cls");
		clearInput();

		gotoxy(10,3); 
		for(j=0;j<58;j++) printf("\xB2");
		gotoxy(10,4); printf("\xB2");
		gotoxy(10,5); printf("\xB2");
		gotoxy(10,6); printf("\xB2");
		gotoxy(31,5); printf("Library System");
		gotoxy(67,4); printf("\xB2");
		gotoxy(67,5); printf("\xB2");
		gotoxy(67,6); printf("\xB2");
		gotoxy(10,7);
        	for(j=0;j<58;j++) printf("\xB2");

		gotoxy(10,10);
		for(j=0;j<20;j++)
		{
			Sleep(50);			
			printf("*");
		}
		for(j=0;j<strlen(d);j++)
		{
			Sleep(50);
			printf("%c",d[j]);
		}
		for(j=0;j<20;j++)
		{
			Sleep(50);
			printf("*");
		}
	
		gotoxy(15,12);
		if(n<4)
			printf("%d attempts left", n);
		gotoxy(15,15);
		printf("Enter Password:");

		
		i=0;
		while(ch!=13)
		{
			ch=getch();
			if(ch!=13 && ch!=8)
			{
				putch('*');
				input[i] = ch;
				i++;
			}
		}
		input[i] = '\0';
	
		if(strcmp(input,password)==0)
		{
			gotoxy(15,20);
			printf("Password match");
			gotoxy(17,22);
			printf("Press any key to countinue.....");
			getch();
			return(0);
		}					
		else
		{	
			gotoxy(15,16);
			printf("\aWarning!! Incorrect Password");
			gotoxy(15,18);
			getch();
			return(1);
		}
}


void issueRecord()  //display issued book's information
{
system("cls");
gotoxy(10,8);
printf("The Book has been given out to: %s",search_temp->stname);
gotoxy(10,9);
printf("Issued on: %d-%d-%d",search_temp->issued.dd,search_temp->issued.mm,search_temp->issued.yy);
gotoxy(10,10);
printf("Return Date:%d-%d-%d",search_temp->duedate.dd,search_temp->duedate.mm,search_temp->duedate.yy);
}

void closeApplication(void)
{
	int i;
	char *creator;

	creator="Roland";

	system("cls");
	gotoxy(10,3); 
	for(i=0;i<42;i++) printf("\xB2");
	gotoxy(10,4); printf("\xB2");
	gotoxy(10,5); printf("\xB2");
	gotoxy(10,6); printf("\xB2");
	gotoxy(20,5); printf("Library System");
	gotoxy(51,4); printf("\xB2");
	gotoxy(51,5); printf("\xB2");
	gotoxy(51,6); printf("\xB2");
	gotoxy(10,7);
        for(i=0;i<42;i++) printf("\xB2");

	gotoxy(10,9); printf("******************************************");
	gotoxy(10,11); printf("Created by: %s", creator);
	gotoxy(10,13); printf("******************************************");
	gotoxy(10,14); printf("******************************************");
	gotoxy(10,16); printf("Closing program.....");
	gotoxy(10,18);
	Sleep(1000);
	exit(0);
}

