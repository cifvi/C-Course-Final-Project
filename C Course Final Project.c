#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 20											// גודל שם פרטי ומשפה למערכים זמניים כקבוע

typedef struct {
	int year;
	char month;
	char day;
}Date;

typedef struct {
	long long id;
	char* name;
	char* family;
	Date birthdate;
	long long PartnerId;
	long long MotherId;
	long long FatherId;
	char NumOfChildern;
	long long* ChildrenPtr;
}Person;

typedef struct {
	Person* persons;
	int sizeMgr;
	int numOfPersons;
}mgr_db;

void person_print(Person* p); // מדפיסה את כל הפרטים על האדם
void db_init(mgr_db* mgr);// מקימה את מסד הנתונים
void person_add(mgr_db* mgr);// מוסיפה אדם למוסד בנתונים בעזרת סט פרסון
Person* id_search(mgr_db* mgr, long long id); //מחזירה מבציע על אדם לפי התעודת זהות
void person_search(const mgr_db* mgr);// נעזרת בחיפוש איידי כדי למצוא אדם ונעזרת בהדפסת אדם כדי להדפיס את פרטיו
void SetPerson(Person* p);// קולטת פרטים של אדם חדש ומגדירה
void parents_search(const mgr_db* mgr);// נעזרת בחיפשו איידי והדפסה כדי להדפיס את כל פרטיו של הוריי האדם
void delete_person(mgr_db* mgr); // מוחק אדם ואת 
void delete_relatives(mgr_db* mgr, Person* p); //מוחק את הפרטים על האדם בעזרת פונקציות עזר שמחוקת  מהוריו את הילד
void delete_Child_From_His_Parents(Person* ParentPtr, Person* child); // מוחקת את הילד מרשימת הילדים של ההורה
void get_gen(const mgr_db* mgr); //בודקת כמה דורות יש לאדם שהוקלד
void search_by_name(const mgr_db* mgr);// מדפיס את כל האנשים עם אותו שם פרטי ומשפחה 
void print_db(const mgr_db* mgr); // מדפיס את כל מסד הנתונים
void Quit(mgr_db* mgr);
Person* FakeRealloc(mgr_db* mgr, Person NewPerson); //יוצרת מערך אנשים גדול ב1 משחררת את הזיכרון הישן ומעתיקה את החדש
char menu();


void main()
{
	char choice = 0;
	mgr_db mgr;
	db_init(&mgr);
	while (choice != '8')
	{
		choice = menu();
		switch (choice)
		{
		case'1':
			person_add(&mgr);
			break;
		case '2':
			person_search(&mgr);
			break;
		case '3':
			parents_search(&mgr);
			break;
		case '4':
			delete_person(&mgr);
			break;
		case '5':
			get_gen(&mgr);
			break;
		case '6':
			print_db(&mgr);
			break;
		case '7':
			search_by_name(&mgr);
			break;
		case '8':
			Quit(&mgr);
			break;
		}
	}
}

char menu()
{
	char choice;
	printf("\n\nDatabase System Menu:\n");
	printf("1. Add person\n2. Search a person\n3. Search Parents\n4. Delete a person\n5. Get generation\n6. Print database\n7. Search by name\n8. Quit: ");
	scanf(" %c", &choice);
	printf("\n");
	if (choice > '8' || choice < '1')
	{
		printf("Wrong input, please enter a number between 1-8:\n");
		menu();
	}
	else
	{
		printf("You selected option number %c\n", choice);
		return choice;
	}
}

void person_print(Person* p) // מדפיס את כל בנתונים על האדם
{
	printf("ID: %lld\n", p->id);
	printf("Name: %s\n", p->name);
	printf("Family: %s\n", p->family);
	printf("Birth Date: %d/%d/%d\n", p->birthdate.day, p->birthdate.month, p->birthdate.year);
	printf("The partner ID is: %lld\n", p->PartnerId);
	printf("The mother ID is: %lld\n", p->MotherId);
	printf("The father ID is: %lld\n", p->FatherId);
	if (p->NumOfChildern == 0)
	{
		printf("The person has no children!\n\n");
	}
	else
	{
		for (int i = 0; i < p->NumOfChildern; i++)
		{
			printf("The ID for the %d child is: %lld\n", i + 1, p->ChildrenPtr[i]);
		}
		printf("\n");
	}
}

void db_init(mgr_db* mgr) // יוצרת מערך דינמי של אנשים לפי מספר האנשים
{
	int sizeppl;
	printf("Please insert your desired population size: ");
	scanf("%d", &sizeppl);
	while (sizeppl <= 0)
	{
		printf("The number is invalid, please enter a positive number\n");
		printf("Please insert your desired population size: ");
		scanf("%d", &sizeppl);
	}

	mgr->sizeMgr = sizeppl;
	mgr->numOfPersons = 0;
	mgr->persons = (Person*)malloc(mgr->sizeMgr * sizeof(Person));
	if (mgr->persons == NULL)
	{
		printf("System is out of memory. \n");
		exit(0);
	}
	printf("Done successfully\n");
}

void person_add(mgr_db* mgr)												// מוסיפה אדם למבנה
{
	Person newPerson;
	SetPerson(&newPerson);
	if (mgr->numOfPersons == 0)												// רלוונטי רק לבן אדם הראשון שמוזן	
	{
		mgr->persons[0] = newPerson;
		mgr->numOfPersons = 1;
		return;
	}
	if (mgr->numOfPersons < mgr->sizeMgr)									//יש מקום במערך לעוד אנשים
	{
		if (newPerson.id > mgr->persons[mgr->numOfPersons - 1].id)			// במידה והאדם החדש בעל הת''ז הגבוה ביותר
		{
			mgr->persons[mgr->numOfPersons] = newPerson;
			mgr->numOfPersons++;
		}
		else
		{
			for (int i = 0; i < mgr->numOfPersons; i++)
			{
				if (mgr->persons[i].id > newPerson.id)						//ברגע שהחדש קטן ממישהו הוא מכניס אותו ומריץ את כולם קדימה באחד
				{
					for (int j = (mgr->numOfPersons - 1); j >= i; j--)
					{
						mgr->persons[j + 1] = mgr->persons[j];
					}
					mgr->persons[i] = newPerson;
					i = mgr->numOfPersons;
				}

			}
			mgr->numOfPersons++;
		}
	}

	else																	//אין מקום לעוד אנשים, נצטרך להקצות זיכרון חדש ולהעביר את כל התנונים אליו
	{
		mgr->persons = FakeRealloc(mgr, newPerson);
		mgr->numOfPersons++;
		mgr->sizeMgr = mgr->numOfPersons;
	}
	printf("Person added successfully.\n");
}
Person* FakeRealloc(mgr_db* mgr, Person newPerson)
{
	Person* tmp = (Person*)malloc((mgr->numOfPersons + 1) * sizeof(Person));
	if (tmp == NULL)
	{
		printf("System is out of memory.");
		exit(0);
	}
	if (mgr->persons[mgr->numOfPersons-1].id < newPerson.id)					//מקרה בו הת.ז החדשה היא הכי גדולה
	{
		for (int i = 0; i < mgr->numOfPersons; i++)
		{
			tmp[i] = mgr->persons[i];
		}
		tmp[mgr->numOfPersons] = newPerson;
		free(mgr->persons);
		return tmp;
	}
	else																   //מקרה בו צריך להוסיף את הת.ז החדשה בתוך המערך בצורה ממוינת
	{
		for (int i = 0; i < mgr->numOfPersons; i++)
		{
			if (mgr->persons[i].id > newPerson.id)
			{
				for (int j = i; j < mgr->numOfPersons; j++)
				{
					tmp[j + 1] = mgr->persons[j];
				}
				tmp[i] = newPerson;
				i = mgr->numOfPersons;
			}
			else
			{
				tmp[i] = mgr->persons[i];
			}
		}
		free(mgr->persons);
		return tmp;
	}
}

void SetPerson(Person* p)
{
	printf("Enter the personal data: \n");
	printf("ID: ");
	scanf("%lld", &p->id);
	while (p->id <= 0)
	{
		printf("The ID must be postive number. Please insert your correct ID: ");
		scanf("%lld", &p->id);
	}
	char tmp[SIZE];
	printf("First Name: ");
	fseek(stdin, 0, SEEK_END);
	gets(tmp);
	p->name = (char*)calloc((strlen(tmp) + 1), sizeof(char));
	if (p->name == NULL)
	{
		printf("System is out of memory.\n\n");
		exit(0);
	}
	strcpy(p->name, tmp);
	printf("Family name: ");
	fseek(stdin, 0, SEEK_END);
	gets(tmp);
	p->family = (char*)calloc(strlen(tmp) + 1, sizeof(char));
	if (p->family == NULL)
	{
		printf("System is out of memory.\n\n");
		exit(0);
	}
	strcpy(p->family, tmp);

	printf("Birth month(1-12): ");
	scanf("%d", &p->birthdate.month);
	while (p->birthdate.month > 12 || p->birthdate.month < 1)
	{
		printf("The month must be in the range of 1-12! please try again: ");
		scanf("%d", &p->birthdate.month);
	}
	printf("Day of birth(1-30): ");
	scanf("%d", &p->birthdate.day);
	while (p->birthdate.day > 30 || p->birthdate.day < 1)
	{
		printf("The day must be between 1-30! please try again: ");
		scanf("%d", &p->birthdate.day);
	}
	printf("Birth year: ");
	scanf("%d", &p->birthdate.year);
	while (p->birthdate.year <= 0)
	{
		printf("The year must be postive! please try again: ");
		scanf("%d", &p->birthdate.year);
	}
	printf("Father ID: ");
	scanf("%lld", &p->FatherId);
	while (p->FatherId < 0)
	{
		printf("The father's ID must be postive. (Insert the number 0 incase you have no father) ");
		scanf("%lld", &p->FatherId);
	}
	printf("Mother id: ");
	scanf("%lld", &p->MotherId);
	while (p->MotherId < 0)
	{
		printf("The mother's ID must be postive. (Insert the number 0 incase you have no mother ");
		scanf("%lld", &p->MotherId);
	}
	printf("Partner id: ");
	scanf("%lld", &p->PartnerId);
	while (p->PartnerId < 0)
	{
		printf("The partner's ID must be postive. (Insert the number 0 incase you have no partner) ");
		scanf("%lld", &p->PartnerId);
	}
	printf("Number of children: ");
	scanf("%d", &p->NumOfChildern);
	while (p->NumOfChildern < 0)
	{
		printf("The number of children must be postive. (Insert 0 if you have no children)");
		scanf("%lld", &p->NumOfChildern);
	}
	if (p->NumOfChildern > 0)
	{
		p->ChildrenPtr = (long long*)malloc(p->NumOfChildern * sizeof(long long));
		if (p->ChildrenPtr == NULL)
		{
			printf("System is out of memory.\n\n");
			exit(0);
		}
		for (int i = 0; i < p->NumOfChildern; i++)
		{
			printf("Enter ID for the %d child: ", i + 1);
			scanf("%lld", &p->ChildrenPtr[i]);
			while (p->ChildrenPtr[i] <= 0)
			{
				printf("Invalid input. Please enter a valid ID: \n");
				printf("Enter ID for the %d child: ", i + 1);
				scanf("%lld", &p->ChildrenPtr[i]);
			}
		}
	}
	else
		p->ChildrenPtr = NULL;
}

Person* id_search(mgr_db* mgr, long long id)// בודקת אם קיים בנאדם כזה, לבדוק אם איידי זה מספר או מערך
{

	if (id >= mgr->persons[mgr->numOfPersons / 2].id)					//על מנצ לייעל את החיפוש, הפונקציה בודקת את הת.ז המבוקש  בחצי המערך ומשם כמות הבדיקות קטנה יותר
		for (int i = mgr->numOfPersons / 2; i < mgr->numOfPersons; i++)
		{
			if (id == mgr->persons[i].id)
				return &mgr->persons[i];
		}
	else
	{
		for (int i = 0; i < mgr->numOfPersons / 2; i++)
		{
			if (id == mgr->persons[i].id)
				return &mgr->persons[i];
		}
	}
	return NULL;
}

void person_search(const mgr_db* mgr)// קולטת תז אם קיים מדפיסה את פרטיו של האדם
{
	long long tmpId;
	printf("Please insert an ID: ");
	scanf("%lld", &tmpId);
	while (tmpId <= 0)
	{
		printf("Invalid input. Please insert the ID you wish to search (positive numbers only).");
		scanf("%lld", &tmpId);
	}
	Person* byId = id_search(mgr, tmpId);
	if (byId == NULL)
	{
		printf("There is no person with such ID within the database.\n");
	}
	else
	{
		person_print(byId);
	}
}

void parents_search(const mgr_db* mgr)
{
	long long tmpId;
	printf("Insert the ID for the person you want to find root data about: ");
	scanf("%lld", &tmpId);
	while (tmpId <= 0)
	{
		printf("Invalid input. Please insert the ID you wish to search (positive numbers only).");
		scanf("%lld", &tmpId);
	}
	Person* byId = id_search(mgr, tmpId);
	Person* fatherptr = NULL;
	Person* motherptr = NULL;
	if (byId == NULL)
	{
		printf("There is no such person within the database.\n");
		return;
	}
	else
	{
		if (byId->MotherId != 0 && byId->FatherId != 0)
		{
			fatherptr = id_search(mgr, byId->FatherId);
			motherptr = id_search(mgr, byId->MotherId);
			if (fatherptr == NULL)
			{
				printf("The person with the ID: %lld, has no parental data (father).\n", byId->id);
			}
			else
			{
				person_print(fatherptr);
			}
			if (motherptr == NULL)
			{
				printf("The person with the ID: %lld, has no parental data (mother). \n", byId->id);
			}
			else
			{
				person_print(motherptr);
			}
		}
		else if (byId->MotherId == 0 && byId->FatherId == 0)
			printf("The person with the ID %lld has no parents.\n", byId->id);
		else if (byId->MotherId == 0)
		{
			printf("The ID %lld has no mother, and the data about his father is: \n", byId);
			fatherptr = id_search(mgr, byId->FatherId);
			if (fatherptr == NULL)
				printf("We have no data of %lld 's father.\n", byId->id);
			else
				person_print(fatherptr);
		}
		else					//קיימת רק אמא	
		{
			printf("The ID %lld has no father, and the data about his mother is: \n", byId);
			motherptr = id_search(mgr, byId->MotherId);
			if (motherptr == NULL)
				printf("We have no data of %lld 's mother.\n", byId->id);
			else
				person_print(motherptr);

		}
	}
}

void delete_person(mgr_db* mgr)
{
	long long idDelete;
	printf("Insert the ID of the person you wish to delete: ");
	scanf("%lld", &idDelete);
	while (idDelete <= 0)
	{
		printf("Invalid input. Please insert the ID you wish to delete (positive numbers only).");
		scanf("%lld", &idDelete);
	}
	Person* ptoDelete = id_search(mgr, idDelete);
	if (ptoDelete == NULL)
	{
		printf("There is no data to delete, no such person.\n");
		return;
	}

	else
	{
		delete_relatives(mgr, ptoDelete); //מוחק את כל הפרטים שקשורים אליו
		Person* temp = (Person*)malloc((mgr->numOfPersons - 1) * sizeof(Person)); //מערך אנשים חדש קטן באחד מהמקורי
		for (int i = 0; i < mgr->numOfPersons - 1; i++)
		{
			if (mgr->persons[i].id == idDelete)
			{
				for (int j = i; j < mgr->numOfPersons - 1; j++)
				{
					temp[j] = mgr->persons[j + 1];
				}
				i = (mgr->numOfPersons - 1);
			}
			else
			{
				temp[i] = mgr->persons[i];
			}
		}
		mgr->numOfPersons--;
		free(mgr->persons); //מוחק את הישן
		mgr->persons = temp; //מעדכן לחדש
		printf("Person deleted successfully.\n");
	}
}

void delete_relatives(mgr_db* mgr, Person* p) //מוחק את כל הפרטים שקשורים אליו
{
	if (p->PartnerId != 0)
	{
		Person* tmppartner = id_search(mgr, p->PartnerId);
		if (tmppartner == NULL)
		{
			printf("We have no data about the partner.\n");
		}
		else
		{
			tmppartner->PartnerId = 0;
			printf("Your partner's data has updated successfully.\n");
		}
	}
	if (p->NumOfChildern != 0) //בודק אם יש לבנאדם שמחקנו ילדים
	{
		for (int i = 0; i < p->NumOfChildern; i++) //עובר על כל מערך הילדים
		{
			Person* tmpChildrenptr = id_search(mgr, p->ChildrenPtr[i]);
			if (tmpChildrenptr == NULL)
			{
				printf("We have no data about the %d child.\n", i + 1);
			}
			else                             // מעדכן את אמא או אבא בהתאמה ל0
			{
				if (tmpChildrenptr->FatherId == p->id)
				{
					tmpChildrenptr->FatherId = 0;
					printf("Data has successfully updated.(%d child)\n", i + 1);
				}
				else
				{
					tmpChildrenptr->MotherId = 0;
					printf("Data has successfully updated.(%d child)\n", i + 1);
				}

			}
		}
	}

	if (p->FatherId == 0 && p->MotherId == 0)
		printf("No parents to update\n");
	else if (p->FatherId != 0 && p->MotherId != 0)
	{
		Person* motherPtr = id_search(mgr, p->MotherId);
		if (motherPtr == NULL)
			printf("No data to update\n");
		else
			delete_Child_From_His_Parents(motherPtr, p);
		Person* FatherPtr = id_search(mgr, p->FatherId);
		if (FatherPtr == NULL)
			printf("No data to update\n");
		else
			delete_Child_From_His_Parents(FatherPtr, p);
	}
	else if (p->FatherId != 0)
	{
		Person* FatherPtr = id_search(mgr, p->FatherId);
		if (FatherPtr == NULL)
			printf("No data to update\n");
		else
			delete_Child_From_His_Parents(FatherPtr, p);
	}
	else															//במידה ויש רק אמא
	{
		Person* motherPtr = id_search(mgr, p->MotherId);
		if (motherPtr == NULL)
			printf("No data to update\n");
		else
			delete_Child_From_His_Parents(motherPtr, p);
	}


}

void delete_Child_From_His_Parents(Person* ParentPtr, Person* child)
{
	long long* tmpForChild = (long long*)calloc((ParentPtr->NumOfChildern - 1), sizeof(long long));
	for (int i = 0; i < (ParentPtr->NumOfChildern - 1); i++)
	{
		if (child->id = ParentPtr->ChildrenPtr[i])
		{
			for (int j = i; j < ParentPtr->NumOfChildern - 1; j++)
			{
				tmpForChild[j] = ParentPtr->ChildrenPtr[j + 1];
			}
			i = ParentPtr->NumOfChildern - 1;
		}
		else
		{
			tmpForChild[i] = ParentPtr->ChildrenPtr[i];
		}
	}
	free(ParentPtr->ChildrenPtr);
	ParentPtr->ChildrenPtr = tmpForChild;
	ParentPtr->NumOfChildern--;
	printf("Data updated successfully\n");
}

void get_gen(const mgr_db* mgr)
{
	long long id;
	int countgeneration = 1;														//סופר מספר דורות
	int sizeOfChildren = 0;
	int sizeOfFather = 1;
	int indexChildren;
	printf("Please insert the ID for the generation test: ");
	scanf("%lld", &id);
	Person* ptrPerson = id_search(mgr, id);
	if (ptrPerson == NULL)
	{
		printf("The is no such person within the database.");
		return;
	}

	long long* fathersArr = (long long*)malloc(sizeof(long long)); //מערך אבות דור נוכחי הראשון זה רק ראש השבט
	if (fathersArr == NULL)
	{
		printf("System is out of memory.\n\n");
		exit(0);
	}
	long long* childrenArr; //מערך כל הילדים של אותו דור
	fathersArr[0] = id;
	int flag = 1;
	while (flag)
	{
		for (int i = 0; i < sizeOfFather; i++) //עובר על כל האבות באותו דור ובודק את סך הילדים של אותו דור
		{
			id = fathersArr[i];
			ptrPerson = id_search(mgr, id);
			if (ptrPerson == NULL)
				continue;
			sizeOfChildren += ptrPerson->NumOfChildern;
		}
		if (sizeOfChildren == 0)
			flag = 0;
		else
		{
			childrenArr = (long long*)malloc(sizeOfChildren * sizeof(long long)); //יוצר מערך בגודל של כל הילדים של אותו דור
			indexChildren = 0;
			for (int i = 0; i < sizeOfFather; i++)
			{
				id = fathersArr[i];
				ptrPerson = id_search(mgr, id);
				if (ptrPerson == NULL)
					continue;
				for (int j = 0; j < ptrPerson->NumOfChildern; j++)
				{
					childrenArr[indexChildren] = ptrPerson->ChildrenPtr[j]; //מכניס את כל הילדים של אותו דור למערך
					indexChildren++;
				}
			}
			countgeneration++; //מגדיל דור
			free(fathersArr); //משחרר את מערך האבות
			fathersArr = childrenArr; //מערך האבות הופך להיות מערך הילדים של אותו דור
			sizeOfFather = sizeOfChildren;
			sizeOfChildren = 0;
		}
	}
	free(fathersArr);
	fathersArr = NULL;
	printf("The person has %d generations.\n", countgeneration);
}

void search_by_name(const mgr_db* mgr)
{

	char name[SIZE];
	char family[SIZE];
	printf("Enter the first name you want to search: ");
	fseek(stdin, 0, SEEK_END);
	gets(name);
	printf("Enter the family name you want to search: ");
	fseek(stdin, 0, SEEK_END);
	gets(family);
	int nameSize = strlen(name);
	int familySize= strlen(family);
	int countFound = 0;
	int check;

	for (int i = 0; i < mgr->numOfPersons; i++)
	{
		check = 1;
		if (strlen(mgr->persons[i].name) != nameSize) //בודק שוני בין האורכים של השמות
			continue;
		for (int j = 0; j < nameSize; j++)
		{
			if (mgr->persons[i].name[j] != name[j])
			{
				j = nameSize;
				check = 0;
			}
		}
		if (check)
		{
			if (strlen(mgr->persons[i].family) != familySize) // בודק שוני בין האורכים של השמות
				continue;
			for (int j = 0; j < familySize; j++)
			{
				if (mgr->persons[i].family[j] != family[j])
				{
					j = familySize;
					check = 0;
				}
			}
		}
		else
		{
			continue;
		}
		if (check)
		{
			countFound++;
			printf("The %d person the system found is: \n", countFound);
			person_print(&mgr->persons[i]);
		}
	}
	if (countFound == 0)
	{
		printf("No results found.\n");
	}
}

void print_db(const mgr_db* mgr)
{
	printf("The population size of our database is: %d\n", mgr->sizeMgr);
	printf("The amount of people in the database is: %d\n\n", mgr->numOfPersons);
	for (int i = 0; i < mgr->numOfPersons; i++)
	{
		printf("The data about the %d person is:\n", i + 1);
		person_print(&mgr->persons[i]);
	}
}

void Quit(mgr_db* mgr)
{
	for (int i = 0; i < mgr->numOfPersons; i++)
	{
		free(mgr->persons[i].ChildrenPtr);
		free(mgr->persons[i].name);
		free(mgr->persons[i].family);
		mgr->persons[i].ChildrenPtr = NULL;
		mgr->persons[i].name = NULL;
		mgr->persons[i].family = NULL;
	}
	free(mgr->persons);
	mgr->persons = NULL;
	printf("All the data was successfully deleted.\n");
	printf("Thank you for using our system.\n\n");
	exit(0);
}