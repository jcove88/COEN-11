/* Jillian Coveney
 * COEN 11
 * Lab 9 
 * Tuesday 2:15pm
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define ENTRY struct entry
#define LIST struct list

//node with only name and the party size 
struct entry
{
	char mainName[20];
	int groupSize;
	ENTRY *next;
};

//linked list with head and tail
//each node depending on size has a head and tail initially pointing to null
struct list 
{
	ENTRY *head;
	ENTRY *tail;
};

//global
pthread_mutex_t mutex;
LIST lists[4] = {{NULL, NULL}, {NULL, NULL}, {NULL, NULL}, {NULL, NULL}};	//array of linked lists

//user functions
void insert(char *inputName, int inputSize);		//user inserts a name and group size
void delete(int deleteSize);				//removes guests from list
void list(void);					//shows the list
void searchSize(void);					//shows groups on the list with size less than or equal to request
int checkDuplicate(char *inputName);			//checks if name is already in list
void clearList(ENTRY *p);				//frees all nodes and quits program
void readFile(char *fileName);				//reads a file and reinserts any existing info
void saveFile(char *fileName);				//save information into a file
void back_lists(ENTRY *p);				//recursive function that shows the list in reverse order
void back_names(ENTRY *p);				//recursive function that shows the names in list backwards
void *auto_save(void *arg);				//saves info to binary file every 15 seconds
void read_auto_save(char *fileName);			//reads info from binary file

int main(int argc, char *argv[]){		//argc is the number or arguments and argv is the file name
	int i, input, inputSize, deleteSize;
	char inputName[20];
	ENTRY *p;
	pthread_t thread;		//creating thread
	pthread_mutex_init(&mutex, NULL);		//initializing mutex
	pthread_create(&thread, NULL, auto_save, (void *) argv[2]);
 
	if(argc != 3)
	{
		printf("You forgot to include the name of some of the files!\n");
		return 1;
	}
	readFile(argv[1]);
	
	while(1)		//function that will loop forever until user exits
	{		
		printf("Surf Lesson Waitlist!\n");
		printf("1 - Insert name.\n");
		printf("2 - Delete a guest.\n");
		printf("3 - List names.\n");
		printf("4 - Search for a size.\n");
		printf("5 - Show lists in reverse.\n");
		printf("6 - Show names in the lists reversed.\n");
		printf("7 - Read contents from the binary file.\n");
		printf("0 - Exit this waiting list.\n");
	
		scanf("%d", &input);		//asks user to input their choice and stores it at the address of input
		
		switch(input)
		{
		case 0: 						//quits the program
			pthread_mutex_lock(&mutex);
			pthread_cancel(thread);
			pthread_mutex_unlock(&mutex);
			saveFile(argv[1]);
			printf("Thank you and come again!\n");	
			for(i = 0; i < 4; i++)
			{
				p = lists[i].head;
				clearList(p);
			}
			return 0;
		case 1:
			printf("Enter a name for the group: \n");
			scanf("%s", inputName);
			if(checkDuplicate(inputName) == 0) 
			{
				printf("Enter the size of the party: \n");
				scanf("%d", &inputSize);
				insert(inputName, inputSize);
			}
			break;
		case 2:
			printf("What is the size of the opening?\n");
			scanf("%d", &deleteSize);
			delete(deleteSize);
			break;
		case 3: 
			list();
			break;
		case 4:
			searchSize();
			break;
		case 5: 
			printf("Name\tGroup Size\n");
			printf("--------------------\n");
			for(i = 4; i >= 0; i--){
				p = lists[i].head;
				back_lists(p);
			}
			printf("\n");
			break;
		case 6:
			printf("Name\n");
			printf("---------\n");
			for(i = 0; i < 4; i++)
			{
				p = lists[i].head;
				back_names(p);
			}
			printf("\n");
			break;
		case 7:
			printf("Reading from binary file...\n");
			read_auto_save(argv[2]);
			break;
		default:
			printf("Invalid entry. Please try again.\n");
			break;
		}
	}
}
void insert(char *inputName, int inputSize)
{	
	pthread_mutex_lock(&mutex);
	int i;
	ENTRY *p;		//temporary node
	p = (ENTRY*)malloc(sizeof(ENTRY));		//malloc is used to assign memory to be created and returns a pointer to the allocated space
	strcpy(p->mainName, inputName);
	p->groupSize = inputSize;
	
	switch(inputSize)
	{
		case 1:
		case 2: 
			i = 0;
			break;
		case 3:
		case 4:
			i = 1;
			break;
		case 5:
		case 6:
			i = 2;
			break;
		default:		
			i = 3;
			break;
	}
	if(lists[i].head == NULL)		//add when list is empty
	{
		lists[i].head = lists[i].tail = p;
		p->next = NULL;
	}
	else			//add to end of list
	{
		lists[i].tail->next = p;
		lists[i].tail = p;
		p->next = NULL;
	}
	pthread_mutex_unlock(&mutex);
	return;
}
void delete(int deleteSize){
	int i;
	int j;
	int flag = 0;		//flag is 1 if there is a size found that fits the opening
	ENTRY *p, *q;		// pointer q will trail p in traversing the list
	
	//this determines which node p and q are pointing to in the array of linked lists	
	if(deleteSize == 1 || deleteSize == 2)
	{
		i = 0;
	}
	else if(deleteSize == 3 || deleteSize == 4)
	{
		i = 1;
	}
	else if(deleteSize == 5 || deleteSize == 6)
	{
		i = 2;
	}
	else if(deleteSize >= 7)
	{ 
		i = 3;
	}
	
	pthread_mutex_lock(&mutex);
	//this loops through the lists containing those sizes and deletes the right ones
	for(j = 0; j <= i; j++)
	{	
		p = q = lists[j].head;
		while(p != NULL)
		{	
			if(deleteSize >=  p->groupSize)
			{
				printf("%s with a group size of %d can have a lesson.\n", p->mainName, p->groupSize);
			
				deleteSize -= p->groupSize;
				flag = 1;
		
				if(p == NULL)		//size not found
				{
					printf("Sorry, the size wasn't found");
					printf("\n");
					continue;
				}
				if (p == lists[j].head)		//size is found in the first node
				{
					p = lists[j].head;
					lists[j].head = p->next;
					free(p);
					p = lists[j].head;
				}
				else if (p == lists[j].tail)		//size is found in the last node
				{
					lists[j].tail = q;
					q->next = NULL;
					free(p);	
					p = NULL;
				}
				else if (lists[j].head == lists[j].tail)		//delete the only node in list
				{
					lists[j].tail = lists[j].head = NULL;
					free(p);	
					p = NULL;
				}	
				else		//size is in a middle node
				{	
					q->next = p->next;
					free(p);
					p = q->next;
				}
				printf("\n");
			}	
			else
			{
				q = p;
				p = p->next;
			}
		}
	}
	if (flag == 0) 
	{
		printf("Sorry, no groups on the list to fit the size.\n");
		printf("\n");
		return;
	}
	pthread_mutex_unlock(&mutex);
}
void list()
{
	int i;
	int flag = 0;
	int x = 1;		//x is incremented for each list to display the list number
	ENTRY *p = lists[i].head;
	for (i = 0; i < 4; i++)
	{	
		printf("List %d (Name, Group Size)\n", x);
		p = lists[i].head;
		while(p != NULL)
		{
			flag = 1;
			printf("%s, %d\n", p->mainName, p->groupSize);
			p = p->next;
		}
		printf("\n");
		x++;
	}	
	if(flag == 0)
	{
		printf("Sorry, there are no groups on the list\n");
		return;
	}
	printf("\n");
}
void searchSize(){
	ENTRY *p;
	p = (ENTRY*)malloc(sizeof (ENTRY));
	int findSize;	
	int i;

	printf("What size group would you like to see?\n");
	scanf("%d", &findSize);

	printf("The following groups are less than or equal to %d\n", findSize);
	for(i = 0; i < 4; i++)
	{
		p = lists[i].head;
		while (p != NULL)
		{
			if(p->groupSize <= findSize)
			{
				printf("%s, %d\n", p->mainName, p->groupSize);
			}
			p = p->next;
		}
	}
	printf("\n");
	return;
}
//compares the input name with existing list names for a duplicate
int checkDuplicate(char *inputName)		
{
	ENTRY *p;
	int i;
	for(i = 0; i < 4; i++)
	{
		p = lists[i].head;	
		while(p != NULL)
		{
			if(strcmp(inputName, p->mainName) !=0) 	//if no duplicate, continue traversing array
			{ 
				p = p->next;
			}
			else
			{
				printf("Sorry, that name exists already! Try a different one.\n");
				printf("\n");
				return 1;		//returns 1 to the insert function to indicate name is already in list
			}
		}		//returns 0 to allow the insertion of the new group
	}
	return 0;
}
void clearList(ENTRY *p)
{
	if (p == NULL)
	{
		return;
	}
	clearList(p->next);
	free(p);
	printf("The list has been deleted!\n");
	return;
}
//reads the accepted file name and passes any information to the insert function
void readFile(char *fileName)
{
	char fileMainName[20];
	int fileGroupSize;
	FILE *fp;
	
	fp = fopen(fileName, "r");
	
	if(fp == NULL)		//txt file is empty
	{
		return;
	}
	else
	{
		printf("Beginning read\n");	
		while(fscanf(fp, "%s%d", fileMainName, &fileGroupSize) == 2)		//Ryan Cain said i could ignore the fseek because we were having issues
		{
			printf("inserting %s, %d ", fileMainName, fileGroupSize);
			insert(fileMainName, fileGroupSize);
		}
		printf("\n");
	}
	fclose(fp);
}
//save information into file
void saveFile(char *fileName)
{
	FILE *fp;
	ENTRY *p;
	int i;
	
	fp = fopen(fileName, "w");
		
	while(fp == NULL)
	{
		printf("Sorry, cannot open the file.\n");
		return;
	}
	for(i = 0; i < 4; i++)
	{ 
		p = lists[i].head;
		while(p != NULL)
		{
			fprintf(fp, "%s\t%d\n", p->mainName, p->groupSize);
			p = p->next;
		}
	}
	fclose(fp);
}
//reverse all the lists
void back_lists(ENTRY *p)
{
	if(p == NULL)		//empty list or end of each list	
	{
		return;
	}
	back_lists(p->next);
	printf("%s\t%d\n", p->mainName, p->groupSize);
	return;
}

//reverse the individual names in the lists
void reverseWord(char *array);
void reverseWord(char *array)
{
	if(*array == '\0')
	{
		return;
	}
	reverseWord(array+1);
	printf("%c", *array);
}
void back_names(ENTRY *p)
{
	if(p == NULL)
	{
		return;
	}
	reverseWord(p->mainName);
	printf("\n");
	back_names(p->next);
	return;
}
//auto saves the contents of list into binary file every 15 sec.
void *auto_save(void *arg)
{
	FILE *fp;
	ENTRY *p;
	char *fileName = (char *)arg;
	int i;	
	while(1)
	{	
		printf("Autosaving...\n");
		pthread_mutex_lock(&mutex);
		fp = fopen(fileName, "wb");
		if(fp == NULL)
		{
			printf("Error in reading binary file.\n");
			pthread_mutex_unlock(&mutex);
			return;
		}
		for(i = 0; i < 4; i++)
		{	
			p = lists[i].head;
			while(p != NULL)
			{
				fwrite(p, sizeof(ENTRY), 1, fp);
				p = p->next;
			}
		}
		fclose(fp);
		printf("Successful autosave to bin file.\n");
		pthread_mutex_unlock(&mutex);
		sleep(15);
	}	
}
//reads the contents from the binary file
void read_auto_save(char *fileName)
{
	FILE *fp;
	ENTRY p;
	pthread_mutex_lock(&mutex);
	
	fp = fopen(fileName, "rb");
	
	if(fp == NULL)
	{
		printf("Error in reading binary file.\n");
		pthread_mutex_unlock(&mutex);
		return;
	}
	printf("Name\tParty Size\n");
	while(fread(&p, sizeof(ENTRY), 1, fp) == 1)
	{
		printf("%s\t%d\n", p.mainName, p.groupSize);
	}
	fclose(fp);
	pthread_mutex_unlock(&mutex);
	return;
}
