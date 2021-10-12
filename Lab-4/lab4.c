/* Jillian Coveney
 * COEN 11
 * Lab 4 
 * Tuesday 2:15pm
 */

#include <stdio.h>
#include <string.h>

typedef union
{
	char phone[15];
	int minAge;
	float avgAge;
}EXTRA;

typedef struct waitlist
{
	char mainName[20];
	int groupSize;	
	int maxAge;
	EXTRA extraInfo;	
}INFO;

int groupCount = 0;	 //number of groups on the waitlist.
INFO array[10];		//the actual waitlist can fit a max of 10 groups. Space for an array of 10 structures. 

void insert(void);
void delete(void);
void list(void);
void searchSize(void);
int checkDuplicate(char *str);

int main(){
	int input;

	while(1)	//function that will loop forever until user exits
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
		case 0: 	//quits the program
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
	char thisMainName[20];		//array that receives their name;
	int thisGroupSize; 		//integer variable that recieves the size
	INFO *partyp = &array[groupCount];		//the pointer party is pointing to next open spot on waitlist.
	int thisExtraInfo = 0;		//integer variable that recieves their max age
	char parentNumber[15];
	int thisMinAge;	
	float thisAvgAge;
	
	if(groupCount >= 10)
	{
		printf("Sorry, the wait list is full.\n");
		printf("\n");
		return;
	}
	printf("Enter a name for your group: ");
	scanf("%s", thisMainName);
	i = checkDuplicate(thisMainName);
	if(i == 1)		//if duplicate is found then return to main function
	{
		return;
	}	
	printf("Enter size of the group: ");
	scanf("%d", &thisGroupSize);
	printf("What is the oldest age in the group?\n");
	scanf("%d", &thisExtraInfo);
	partyp -> maxAge = thisExtraInfo;	
	if(thisExtraInfo < 18)
	{
		printf("Please enter a parent or gaurdian phone number.\n"); 
		scanf("%s", parentNumber);
		strcpy(partyp -> extraInfo.phone, parentNumber);
	}
	if(thisExtraInfo > 65)
	{ 
		printf("Please enter the youngest age in the group.\n");
		scanf("%d", &thisMinAge);
		partyp -> extraInfo.minAge = thisMinAge;
	}
	if(thisExtraInfo >= 18 && thisExtraInfo <= 65) 
	{ 
		printf("Please enter the average age of the group.\n");
		scanf("%f", &thisAvgAge);
		partyp -> extraInfo.avgAge = thisAvgAge;
	}
	printf("\n");					
	strcpy(partyp->mainName, thisMainName);
	partyp -> groupSize = thisGroupSize;		 //the pointer is accessing the structure member groupSize
	groupCount++;
	return;
}
void delete(){
	int deleteSize;
	int i;
	int j;
	int flag = 0;		//tells you if a value has been found that fits the delete size
	INFO *partyp = array;		//pointer used to traverse the group sizes 
	INFO *partyq = array;		//pointer used to copy and shift a group's information up a spot in the array

	if(groupCount == 0)
	{
		printf("There are no groups on the waitlist.\n");
		printf("\n");
		return;
	}	
	printf("What is the size of the opening?\n");	
	scanf("%d", &deleteSize);

	while(groupCount != 0)
	{
		for(i = 0; i < groupCount; i++, partyp++)
		{
			if(deleteSize >=  partyp->groupSize)
			{
				flag = 1;
				printf("%s with a group size of %d can have a lesson.\n", partyp->mainName, partyp->groupSize);
				deleteSize -= partyp->groupSize;
				printf("\n");
				for(j = i; j < groupCount - 1; j++)		//starts from the next group on list
				{
					partyq[j] = partyq[j+1];	//copy the next group into the previous group's spot
				}	
				groupCount--;		//one less group becase they were deleted
				i--;
				partyp--;
			}		
			if(deleteSize == 0)
			{
				return;
			};
		}
		break;
	}
	if(flag == 0)
	{ 
		printf("Sorry, the groups sizes are too big for the opening.\n");
		printf("\n");
		return;
	}
}
void list(){
	int i;
 	INFO *partyp = array;
	
	printf("There's %d names on the waiting list:\n", groupCount);
	printf("\n");
	printf("Name, Group Size, Max Age, Extra Info\n");
	for(i = 0; i < groupCount; i++, partyp++)
	{
		printf("%s, %d, %d, ", partyp->mainName, partyp->groupSize, partyp->maxAge);
		if(partyp->maxAge < 18)			//series of conditionals for where to store the info in the union
		{
			printf("%s\n", partyp->extraInfo.phone);
		}
		else if(partyp->maxAge > 65)
		{
			printf("%d\n", partyp->extraInfo.minAge);
		}
		else
		{
			printf("%f\n", partyp->extraInfo.avgAge);
		}
	}
	printf("\n");
	return;
}
void searchSize(){
	INFO *partyp = array;		//pointer used to traverse the group sizes
	int i;
	int findSize;	
	
	printf("What size group would you like to see?\n");
	scanf("%d", &findSize);

	printf("The following groups are less than or equal to %d\n", findSize);
	for(i = 0; i < groupCount; i++, partyp++)
	{
		if(partyp->groupSize <= findSize)
		{
			printf("%s, %d, %d, ", partyp->mainName, partyp->groupSize, partyp->maxAge);
			if(partyp->maxAge < 18) 		//series of conditionals for where to store the info in the union
			{
				printf("%s\n", partyp->extraInfo.phone);
			}
			else if(partyp->maxAge > 65)
			{
				printf("%d\n", partyp->extraInfo.minAge);
			}
			else
			{
				printf("%f\n", partyp->extraInfo.avgAge);
			}
		}
	}
	printf("\n");
	return;
}
int checkDuplicate(char *str)		//receives a pointer to the string with that group's main name they input
{
	INFO *partyp = array;
	int i = 0;
	while(i < groupCount)
	{
		if(strcmp(str, partyp->mainName) !=0) 	//if no duplicate, continue traversing array
		{ 
			i++;
			partyp++;
		}
		else
		{
			printf("Sorry, that name exists already! Try a different one.\n");
			printf("\n");
			return 1;	//returns 1 to the insert function to indicate name is already in list
		}
	}
	return 0;	//returns 0 to allow the insertion of the new group
}
