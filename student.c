#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student.h" 


Student *createStudent(Student *next, char last[], char first[]);
Student *searchStudent(Student *head, char last[], char first[]);
Grade *createGrade(char gradeName[], double grade);
Grade *searchGrade(Student *student, char gradeName[]);
Grade *findLastGrade(Grade *head);
int compareStudent(Student *ptr1, Student *ptr2);
int compareGrade(Grade *ptr1, Grade *ptr2);
void removeAllGrades(Student *headStudentList, Student *pStudent);
/////////////////////////////////////////////////////////////////////////////////////////////
// add a student to the list of students
//	   1. Make sure a student by that name does not exist (print an error message and do nothing if it exists)
//	   2. Add a new student with that name (add-at-front)

Student* addStudent(Student *headStudentList, char last[], char first[])
{
    if (searchStudent(headStudentList, last, first) != NULL) {
        printf("\nERROR: %s, %s already exists\n", last, first);
        return headStudentList;
    }

    Student *pNew = (Student*)malloc(sizeof(Student));

    if (headStudentList == NULL) { //if list is empty
        pNew = createStudent(NULL, last, first);
        return pNew;
    }

    pNew = createStudent(headStudentList, last, first);
    
    return pNew;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// add a grade to the specified student
//   1. Make sure a student by that name exists (so you can add grade to it)
//   2. If the specifed grade already exists, update the grade's value to the new value
//   3. Otherwise, add the grade to the end of the student's grade list (add-at-end)

void addGrade(Student *headStudentList, char last[], char first[], char gradeName[], double value) 
{
    Student *pStudent = searchStudent(headStudentList, last, first);

    if (pStudent == NULL) {
        printf("\nERROR: %s, %s does not exist\n", last, first);
        return;
    }

    Grade *pGrade = searchGrade(pStudent, gradeName);

    if (pGrade != NULL) { // if the grade already exists
        pGrade->value = value;
    }

    else { //grade does not exist
        pGrade = createGrade(gradeName, value);
        if (pStudent->headGradeList != NULL) { // if there is at least one grade
            Grade *pLast = findLastGrade(pStudent->headGradeList);
            pLast->next = pGrade;
        }
        else {
            pStudent->headGradeList = pGrade;
        }
        pGrade->next = NULL;
    }

    return;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// return the number of students in the list
// Iterate through the student list to count the number of students, return that count

int count(Student *headStudentList) {
    int i = 0;
    for (Student *ptr = headStudentList; ptr != NULL; ptr = ptr->next) {
        i++;
    }
	return i;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// print all the grades for a given student
//	1. Make sure a student with that name exists
//	2. Print the name of that student
//	3. Print all the "grade name: grade value" pairs for that student

void printStudent(Student *headStudentList, char last[], char first[])
{
    Student *pStudent = searchStudent(headStudentList, last, first);
    Grade *pGrade = (Grade*)malloc(sizeof(Grade));
    
    if (pStudent == NULL) {
        printf("\nERROR: Student named %s %s does not exist", first, last);
        return;
    }
    printf("\nStudent Name: %s, %s\n", last, first);
    for (pGrade = pStudent->headGradeList; pGrade != NULL; pGrade = pGrade->next) {
        printf("%-3s : %g\n", pGrade->name, pGrade->value);
    }
	return;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// print all the students
// For each student
//	   a. Print the name of that student
//	   b. Print all the "grade name: grade value" pairs for that student

void print(Student *headStudentList)
{
    for (Student *ptr = headStudentList; ptr != NULL; ptr = ptr->next) {
        printStudent(headStudentList, ptr->lastName, ptr->firstName);
    }
	return;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// add a student (in alphabetical order)
//	1. Make sure a student by that name does not exist 
//	2. Add a student with that name (add-in-order)

Student *addStudentOrdered(Student *headStudentList, char last[], char first[])
{
    if (searchStudent(headStudentList, last, first) != NULL) {
        printf("\nERROR: Student %s, %s already exists\n", last, first);
        return headStudentList;
    }

    Student *pNew = (Student*)malloc(sizeof(Student));
    pNew = createStudent(NULL, last, first);

    if (headStudentList == NULL) { //case 1: if list is empty
        return pNew;
    }

    //case 2: person is first in list, add at front

    if (compareStudent(pNew, headStudentList) > 0) {
        pNew->next = headStudentList;
        return pNew;
    }

    //search
    for (Student *ptr = headStudentList; ptr != NULL; ptr = ptr->next) {
        if (ptr->next == NULL) { //case 3: end of list, add at end
            ptr->next = pNew;
            return headStudentList;
        }
        if (compareStudent(pNew, ptr->next) > 0) {
            pNew->next = ptr->next;
            ptr->next = pNew;
            return headStudentList;
        }
    }

	return NULL;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// adds a grade to the specified student in order
//	1. Make sure a student by that name exists (so you can add a grade to the student)
//	2. If the grade already exists, update the value
//	3. Otherwise, add the grade to the student (add-in-order)

void addGradeOrdered(Student *headStudentList, char last[], char first[], char gradeName[], double value)
{
    Student *pStudent = (Student*)malloc(sizeof(Student));
    pStudent = searchStudent(headStudentList, last, first);

    if (pStudent == NULL) {
        printf("\nERROR: Student %s, %s does not exist\n", last, first);
        return;
    }

    Grade *pGrade = (Grade*)malloc(sizeof(Grade));
    pGrade = searchGrade(pStudent, gradeName);

    if (pGrade != NULL) { // if the grade already exists
        pGrade->value = value;
        return;
    }

    pGrade = createGrade(gradeName, value);

    if (pStudent->headGradeList == NULL) { //case 1: list is empty
        pStudent->headGradeList = pGrade;
        pGrade->next = NULL;
        return;
    }

    //case2: grade is first in list, add at front;

    if (compareGrade(pGrade, pStudent->headGradeList) > 0) {
        pGrade->next = pStudent->headGradeList;
        pStudent->headGradeList = pGrade;
        return;
    }

    //search
    for (Grade *ptr = pStudent->headGradeList; ptr != NULL; ptr = ptr->next) {
        if (ptr->next == NULL) { //case 3: end of list, add at end
            ptr->next = pGrade;
            pGrade->next = NULL;
            return;
        }
        if (compareGrade(pGrade, ptr->next) > 0) {
            pGrade->next = ptr->next;
            ptr->next = pGrade;
            return;
        }
    }

    return;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// remove a grade from a student
//	1. Make sure a student with that name exists
//	2. Make sure the grade exists in that student
//	3. Remove the grade from that student

void removeGrade(Student *headStudentList, char last[], char first[], char gradeName[])
{
    Student *pStudent = searchStudent(headStudentList, last, first);

    if (pStudent == NULL) {
        printf("\nERROR: %s, %s does not exist.\n", last, first);
        return;
    }

    Grade *pGrade = searchGrade(pStudent, gradeName);

    if (pGrade == NULL) {
        printf("\nERROR: %s is not a grade for %s, %s\n", gradeName, last, first);
        return;
    }
    if (pStudent->headGradeList == pGrade) { // delete the first one
        Grade *pDel = pStudent->headGradeList;
        pStudent->headGradeList = pStudent->headGradeList->next;
        free(pDel);
        return;
    }

    //search
    for (Grade *ptr = pStudent->headGradeList; ptr != NULL; ptr = ptr->next) {
        if (ptr->next == pGrade) {
            Grade *pDel = ptr->next;
            ptr->next = ptr->next->next;
            free(pDel);
            return;
        }
    }
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// remove a student
//	1. Make sure a student with that name exists
//	2. Remove that student and all the grades of that student 

Student *removeStudent(Student *headStudentList, char last[], char first[])
{
    Student *pStudent = searchStudent(headStudentList, last, first);

    if (pStudent == NULL) {
        printf("\nERROR: %s, %s does not exist.\n", last, first);
        return headStudentList;
    }

    removeAllGrades(headStudentList, pStudent);

    if (pStudent == headStudentList) {
        Student *pDel = headStudentList;
        headStudentList = headStudentList->next;
        free(pDel);
        return headStudentList;
    }

    //search
    for (Student *ptr = headStudentList; ptr != NULL; ptr = ptr->next) {
        if (ptr->next == pStudent) {
            Student *pDel = ptr->next;
            ptr->next = ptr->next->next;
            free(pDel);
            return headStudentList;
        }
    }
	return NULL;
}

Student *searchStudent(Student *head, char last[], char first[])
{
    for (Student *ptr = head; ptr != NULL; ptr = ptr->next) {
        if ((strcmp(ptr->firstName, first) == 0) && (strcmp(ptr->lastName, last) == 0)) {
                return ptr;
                }
    }
    return NULL;
}

Student *createStudent(Student *next, char last[], char first[])
{
    Student *ptr = malloc(sizeof(Student));
    ptr->firstName = (char*)malloc(sizeof(strlen(first + 1)));
    ptr->lastName = (char*)malloc(sizeof(strlen(last + 1)));

    strcpy(ptr->firstName, first);
    strcpy(ptr->lastName, last);
    ptr->next = next;
    ptr->headGradeList = NULL;
    
    return ptr;
}

Grade *createGrade(char gradeName[], double grade)
{
    Grade *pNew = (Grade*)malloc(sizeof(Grade));

    strcpy(pNew->name, gradeName);
    pNew->value = grade;

    return pNew;
    
}

Grade *searchGrade(Student *pStudent, char gradeName[])
{
    Grade *pGrade = (Grade*)malloc(sizeof(Grade));
    for (pGrade = pStudent->headGradeList; pGrade != NULL; pGrade = pGrade->next) {
        if (strcmp(pGrade->name, gradeName) == 0) return pGrade;
    }
    return NULL;
}

Grade *findLastGrade(Grade *head)
{

    Grade *ptr = (Grade*)malloc(sizeof(Grade));
    for (ptr = head; ptr != NULL; ptr = ptr->next) {
        if (ptr->next == NULL) return ptr;
    }
    return NULL;
}

int compareStudent(Student *ptr1, Student *ptr2)
{
    //returns 1 if person1 goes first
    //returns -1 if person2 goes first
    //returns 0 if they are the same

    if (strcmp(ptr1->lastName, ptr2->lastName) < 0) return 1;
    if (strcmp(ptr1->lastName, ptr2->lastName) > 0) return -1;
    if (strcmp(ptr1->firstName, ptr2->firstName) < 0) return 1;
    if (strcmp(ptr1->firstName, ptr2->firstName) > 0) return -1;

    return 0;
}

int compareGrade(Grade *ptr1, Grade *ptr2)
{
    //returns 1 if grade1 goes first
    //returns -1 if grade2 goes first
    //returns 0 if they are the same
    char cat1, cat2;
    int num1, num2;

    sscanf(ptr1->name, "%c%d", &cat1, &num1);
    sscanf(ptr2->name, "%c%d", &cat2, &num2);

    if (cat1 < cat2) return 1; //grade 1 goes first
    if (cat2 < cat1) return -1; //grade2 goes first

    if (num1 < num2) return 1; //grade1 goes first
    if (num2 < num1) return -1; //grade2 goes first

    return 0; //grades are the same
}

void removeAllGrades(Student *headStudentList, Student *pStudent)
{
    for (Grade *ptr = pStudent->headGradeList; ptr != NULL; ptr = ptr->next) {
        removeGrade(headStudentList, pStudent->lastName, pStudent->firstName, ptr->name);
    }
    return;
}
