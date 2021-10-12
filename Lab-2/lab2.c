/* Jillian Coveney
 * COEN 11
 * Lab 2 
 * Tuesday 2:15pm
 */

#include <stdio.h>
#include <string.h>

int groupCount = 0;
int groupSize [10];
char mainName [10][20];

void insert(void);
void delete(void);
void list(void);

int main(){
	int input;

	while(1){

		printf("Surf Lesson Waitlist!\n");
		printf("1 - Insert name.\n");
		printf("2 - Delete a guest.\n");
		printf("3 - List names.\n");
		printf("0 - Exit this waiting list.\n");
	
		scanf("%d", &input);
		
		switch(input)
		{
		case 0: 
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
		default:
			printf("Invalid entry. Please try again.\n");
			break;
		}
	}
}
void insert()
{
	int i;
	char thisMainName[20];
	int thisGroupSize = 0; 
	
	if(groupCount >= 10)
	{
		printf("Sorry, the wait list is full.\n");
		printf("\n");
		return;
	}
	printf("Enter a name for your group: ");
	scanf("%s", thisMainName);
	
	printf("Enter size of the group: ");
	scanf("%d", &thisGroupSize);
	printf("\n");					
	if(thisGroupSize > 10) 
	{
		printf("Sorry, that is too many people.\n");
		printf("\n");
		return;
	}

	strcpy(mainName[groupCount], thisMainName);
	groupSize[groupCount] = thisGroupSize;
	groupCount++;
}
void delete(){
	int deleteSize;
	int i;
	int j;
	int flag = 0;

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
		for(i = 0; i < groupCount; i++)
		{
			if(deleteSize >=  groupSize[i])
			{
				flag = 1;
				printf("%s with a group size of %d can have a lesson.\n", mainName[i], groupSize[i]);
				deleteSize -= groupSize[i];
				printf("\n");
				for(j = i; j < groupCount; j++)
				{
					strcpy(mainName[j], mainName[j+1]);
					groupSize[j] = groupSize[j+1];
				}	
				groupCount--;
				i--;
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
 
	printf("There's %d names on the waiting list:\n", groupCount);
	printf("\n");
	printf("Name, Party Size\n");
	for(i = 0; i < groupCount; i++){
		printf("%s, %d\n", mainName[i], groupSize[i]);
	}
	printf("\n");
}
