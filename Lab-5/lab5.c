/* Jillian Coveney
 * COEN 11
 * Lab 5 
 * Tuesday 2:15pm
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NODE struct node

struct node
{
	char mainName[20];
	int groupSize;
	NODE *next;
};

NODE *head = NULL;
NODE *tail = NULL;

void insert(void);
void delete(void);
void list(void);
void searchSize(void);
int checkDuplicate(char *str);

int main(){
	int input;

	while(1)		//function that will loop forever until user exits
	{		
		printf("Surf Lesson Waitlist!\n");
		printf("1 - Insert name.\n");
		printf("2 - Delete a guest.\n");
		printf("3 - List names.\n");
		printf("4 - Search for a size.\n");
		printf("0 - Exit this waiting list.\n");
	
		scanf("%d", &input);		//asks user to input their choice and stores it at the address of input
		
		switch(input)
		{
		case 0: 						//quits the program
			printf("Thank you and come again!\n");	
			return 0;
		case 1:
			insert();
			break;
		case 2:
			delete();
			break;
		case 3: 
			list();
			break;
		case 4:
			searchSize();
			break;
		default:
			printf("Invalid entry. Please try again.\n");
			break;
		}
	}
}
void insert()
{
	int i;
	char thisMainName[20];		//array for that receives their name;
	int thisGroupSize = 0; 		//integer variable that recieves the size
	NODE *p;
	p = (NODE*)malloc(sizeof (NODE));		//malloc is used to assign memory to be created and returns a pointer to the allocated space

	printf("Enter a name for your group: ");
	scanf("%s", thisMainName);
	i = checkDuplicate(thisMainName);
	if(i == 1)		//if duplicate is found then return to main function
	{
		return;
	}	
	printf("Enter size of the group: ");
	scanf("%d", &thisGroupSize);
	printf("\n");					
	strcpy(p->mainName, thisMainName);
	p->groupSize = thisGroupSize;		//the pointer is accessing the structure member groupSize
	if(head == NULL)
	{
		p->next = NULL;
		tail = head = p;
		return;
	}
	else
	{
		p->next = NULL;
		tail->next = p;
		tail = p;
		return;
	}
}
void delete(){
	int deleteSize;
	int i;
	int j;
	int flag = 0;		//flag is 1 if there is a size found that fits the opening
	NODE *p, *q;		// pointer q will trail p in traversing the list
	p = q = head;
	
	printf("What is the size of the opening?\n");	
	scanf("%d", &deleteSize);

	while(p != NULL)
	{
		if(deleteSize >=  p->groupSize)
		{	
			printf("%s with a group size of %d can have a lesson.\n", p->mainName, p->groupSize);
			deleteSize -= p->groupSize;
			flag = 1;

			if (p == head)		//size is found in the first node
			{
				head = p->next;
				free(p);
				q = p = head;	
			}
			else if (p == tail)		//size is found in the last node
			{
				tail = q;
				q->next = NULL;
				free(p);	
			}
			else if (head == tail)		//delete the only node in list
			{
				tail = head = NULL;
				free(p);	
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
	if(flag == 0) 
	{
		printf("Sorry, no groups on the list to fit the size.\n");
		printf("\n");
		return;
	}
}
void list()
{
	NODE *p;
	p = head;
	int flag = 0;
	while(p != NULL)
	{
		flag = 1;
		printf("List (Name, Group Size)\n");
		printf("%s, %d\n", p->mainName, p->groupSize);
		printf("\n");
		p = p->next;
	}	
	if(flag == 0)
	{
		printf("Sorry, there are no groups on the list\n");
		printf("\n");
		return;
	}
}
void searchSize(){
	NODE *p;
	p = head;
	int findSize;	
	
	printf("What size group would you like to see?\n");
	scanf("%d", &findSize);

	printf("The following groups are less than or equal to %d\n", findSize);
	while (p != NULL)
	{
		if(p->groupSize <= findSize)
		{
			printf("%s, %d\n", p->mainName, p->groupSize);
		}
		p = p->next;
	}
	printf("\n");
	return;
}
int checkDuplicate(char *str)		//receives a pointer to the string with that group's main name they input
{
	NODE *p;
	p = head;
	int i = 0;
	while(p != NULL)
	{
		if(strcmp(str, p->mainName) !=0) 	//if no duplicate, continue traversing array
		{ 
			p = p->next;
		}
		else
		{
			printf("Sorry, that name exists already! Try a different one.\n");
			printf("\n");
			return 1;		//returns 1 to the insert function to indicate name is already in list
		}
	}
	return 0;		//returns 0 to allow the insertion of the new group
}
