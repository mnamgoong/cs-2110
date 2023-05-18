/**
 * CS 2110 - Spring 2023 - Homework #9
 *
 * @author Michelle Namgoon
 *
 * list.c: Complete the functions!
 */

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!-IMPORTANT-!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * For any function that must use malloc, if malloc returns NULL, the function
 * itself should return NULL if needs to return something (or return 1 if
 * the function returns a int).
 */

// Do not add ANY additional includes!!!
#include "list.h"

/* You should NOT have any global variables. */

/* The create_node function is static because this is the only file that should
   have knowledge about the nodes backing the linked List. */
static User *create_user(char *name, UserType type, UserUnion data);
static int create_student(int num_classes, double *grades, Student *dataOut); //completed for you
static int create_instructor(double salary, Instructor *dataOut); //completed for you
static Node *create_node(char *name, UserType type, UserUnion data);
static int user_equal(const User *user1, const User *user2);

/** create_user
 *
 * Helper function that creates a User by allocating memory for it on the heap
 * and initializing with the passed in data. You MUST create a deep copy of
 * the data union. This means malloc'ing space for copies of each field in
 * UserUnion data.
 *
 * If malloc returns NULL, you should return NULL as well.
 *
 * If creating a student/instructor fails, you must free everything you've allocated
 * and return NULL
 *
 * You should call create_student() or create_instructor() in this function
 * to create a deep copy of the UserUnion depending on the UserType
 *
 * (hint: you cannot just assign UserUnion data to the data part of user)
 *
 * @param the fields of the User struct
 * @return a User, return NULL if malloc fails
 */
static User *create_user(char *name, UserType type, UserUnion data)
{
    // allocate space for a new user
    User *newUser = (User *) malloc(sizeof(User));
    if (newUser == NULL) {
        free(newUser);
        return NULL;
    }

    // allocate space for the user's name
    if (name != NULL) {
        char *newUserName = (char *) malloc(sizeof(char) * (strlen(name) + 1));
        if (newUserName == NULL) {
            free(newUser);
            return NULL;
        }
        strcpy(newUserName, name);
        newUser->name = newUserName;
    } else {
        newUser->name = NULL;
    }

    // set the user's type
    newUser->type = type;

    // set the user's data
    int success;
    if (type == STUDENT) {
        success = create_student(data.student.num_classes, data.student.grades, &(newUser->data.student));
    } else { // instructor
        success = create_instructor(data.instructor.salary, &(newUser->data.instructor));
    }

    if (success == 1) { // malloc failed
        free(newUser);
        return NULL;
    } else {
        return newUser;
    }
}

/** create_student
 *
 * Helper function that creates a Student and allocates memory for the grade array on the heap.
 *
 * THIS FUNCTION IS IMPLEMENTED FOR YOU
 *
 * @param the fields of the Student struct, and an existing pointer to a student
 * @return 1 if malloc fails, 0 otherwise.
 */
static int create_student(int num_classes, double *grades, Student *dataOut)
{
    /***do not change anything in this function***/
    dataOut->num_classes = num_classes;
    dataOut->grades = NULL;
    if (grades != NULL) {
        if (!(dataOut->grades = (double *) malloc(sizeof(double)*num_classes))) return 1;
        memcpy(dataOut->grades, grades, sizeof(double)*num_classes);
    }
    return 0;
}

/** create_instructor
 *
 * Helper function that creates an Instructor.
 *
 * THIS FUNCTION IS IMPLEMENTED FOR YOU
 *
 * @param the fields of the Instructor struct, and an existing pointer to an instructor
 * @return 1 if malloc fails, 0 otherwise. (this function should always return 0 since
 * you won't need to malloc anything)
 */
static int create_instructor(double salary, Instructor *dataOut)
{
    /***do not edit anything in this function***/
    dataOut->salary = salary; //yes that's all this function does
    return 0;
}

/** create_node
  *
  * Helper function that creates a Node by allocating memory for it on the heap.
  * Be sure to set its next pointer to NULL.
  *
  * Remember that you need to malloc both the node itself and the User
  * that's contained in the struct. You might want to call create_user instead of
  * malloc'ing again.
  *
  * If malloc returns NULL for either of them, you should return NULL to indicate failure.
  * In the case that you successfully malloc one of them but fail to malloc the other one,
  * you should free the one you successfully malloc'd.
  *
  * @param the fields of the User struct
  * @return a Node
  */
static Node* create_node(char *name, UserType type, UserUnion data)
{
    // allocate space for a new node
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (newNode == NULL) {
        free(newNode);
        return NULL;
    }

    // allocate space for a new user
    User *newUser = create_user(name, type, data);
    if (newUser == NULL) {
        free(newUser);
        return NULL;
    }

    // set node
    newNode->data = newUser;
    newNode->next = NULL;

    return newNode;
}

/** student_equal
 *
 * Helper function to compare two Student structs. You may find it ueful to call this helper
 * function from the user_equal function that you have to implement.
 *
 * @param the two Student structs to be compared
 * @return 1 if equal, 0 otherwise
 */
static int student_equal(const Student *student1, const Student *student2)
{
    /***do not edit anything in this function***/
    if (student1->num_classes != student2->num_classes) return 0;
    if (student1->grades != student2->grades)
    {
        if (student1->grades == NULL || student2->grades == NULL) return 0;
        if (memcmp(student1->grades, student2->grades, student1->num_classes * sizeof(double))) {
            return 0;
        }
    }
    return 1;
}

/** user_equal
 * Helper function to help you compare two user structs.
 *
 * If the name, type, and union fields are all equal, you should return 1.
 * Otherwise, return 0.
 *
 * NOTE: struct members that are pointers may be null!
 * If two users both have null for a certain pointer, the pointers are
 * considered equal.
 * However, if either of the input users is NULL, you should return 0.
 *
 * You may find it useful to call the student_equal function defined above.
 *
 * Make sure you're using the right function when comparing the name.
 * Remember that you are allowed to use functions from string.h
 *
 * This is meant to be used as a helper function in 'contains'; it is not tested
 * by the autograder.
 *
 * @param the two User structs to be compared
 * @return 1 if equal, 0 otherwise
 */
static int user_equal(const User *user1, const User *user2)
{
    if (user1 == NULL || user2 == NULL) { // check if either are NULL
        return 0;
    }

    if ((user1->name == NULL && user2->name == NULL) || (strcmp(user1->name, user2->name) == 0)) { // name are both NULL or equal
        if (user1->type == STUDENT && user2->type == STUDENT) { // both student
            return student_equal(&(user1->data.student), &(user2->data.student));
        } else if (user1->type == INSTRUCTOR && user2->type == INSTRUCTOR) { // both instructor
            if (user1->data.instructor.salary == user2->data.instructor.salary) { // salary are equal
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

/** create_list
 *
 * Creates a LinkedList by allocating memory for it on the heap.
 * Be sure to initialize size to zero and head to NULL.
 *
 * If malloc returns NULL, you should return NULL to indicate failure.
 *
 * @return a pointer to a new struct list or NULL on failure
 */
LinkedList *create_list(void)
{
    LinkedList *newList = (LinkedList *) malloc(sizeof(LinkedList));
    if (newList == NULL) {
        free(newList);
        return NULL;
    }

    newList->head = NULL;
    newList->size = 0;

    return newList;
}

/** push_front
 *
 * Adds the element at the front of the LinkedList.
 *
 * @param list a pointer to the LinkedList structure.
 * @param      the fields of the User struct
 * @return 1 if the LinkedList is NULL or if allocating the new node fails, 0 if
 * successful.
 */
int push_front(LinkedList *list, char *name, UserType type, UserUnion data)
{
    if (list == NULL) { // LinkedList is NULL
        return 1;
    }

    Node *newNode = create_node(name, type, data);
    if (newNode == NULL) {
        free(newNode);
        return 1;
    }

    newNode->next = list->head;
    list->head = newNode;
    list->size = list->size + 1;

    return 0;
}

/** push_back
 *
 * Adds the element to the back of the LinkedList.
 *
 * @param list a pointer to the LinkedList structure.
 * @param      the fields of the User struct
 * @return 1 if the LinkedList is NULL or if allocating the new node fails, 0 if
 * successful.
 */
int push_back(LinkedList *list, char *name, UserType type, UserUnion data)
{
    if (list == NULL) { // LinkedList is NULL
        return 1;
    }

    if (list->size == 0) { // just add if empty list
        push_front(list, name, type, data);
    } else {
        Node *curr = list->head;
        // find the last node
        for (int i = 0; i < list->size - 1; i++) { 
            curr = curr->next;
        } // curr is now the second to last element
        Node *newNode = create_node(name, type, data);
        if (newNode == NULL) {
            free(newNode);
            return 1;
        }
        curr->next = newNode;
        list->size = list->size + 1;
    }

    return 0;
}

/** add_at_index
 *
 * Add the element at the specified index in the LinkedList. This index must lie in
 * the inclusive range [0,size].
 * For example, if you have no elements in the LinkedList,
 * you should be able to add to index 0 but no further.
 * If you have two elements in the LinkedList,
 * you should be able to add to index 2 but no further.
 *
 * @param list a pointer to the LinkedList structure
 * @param index 0-based, starting from the head in the inclusive range
 *              [0,size]
 * @param the fields of the User struct
 * @return 1 if the index is out of bounds or the LinkedList is NULL or malloc fails
 *         (do not add the data in this case)
 *         otherwise (on success) return 0
 */
int add_at_index(LinkedList *list, int index, char *name, UserType type, UserUnion data)
{
    if (list == NULL) { // LinkedList is NULL
        return 1;
    }

    if (index < 0 || index > list->size) { // index is out of bounds
        return 1;
    }

    if (index == 0 || list->size == 0) { // add to front if index = 0 or list is emmpty
        push_front(list, name, type, data);
    } else if (index == list->size) {
        push_back(list, name, type, data);
    } else {
        Node *newNode = create_node(name, type, data);
        if (newNode == NULL) {
            free(newNode);
            return 1;
        }

        if (index == 1) {
            newNode->next = list->head->next;
            list->head->next = newNode;
        } else {
            Node *curr = list->head;
            for (int i = 0; i < index; i++) { 
                curr = curr->next;
            } // now curr is at index

            newNode->next = curr->next;
            curr->next = newNode;
        }
        list->size = list->size + 1;
    }
    
    return 0;
}

/** get
 *
 * Gets the data at the specified index in the LinkedList
 *
 * @param list a pointer to the LinkedList structure
 * @param index 0-based, starting from the head.
 * @param dataOut A pointer to a pointer used to return the data from the
 *        specified index in the LinkedList or NULL on failure.
 * @return 1 if dataOut is NULL or index is out of range of the LinkedList or
 *         the LinkedList is NULL, 0 (on success) otherwise
 */
int get(LinkedList *list, int index, User **dataOut)
{
    if (dataOut == NULL) { // dataOut is NULL
        return 1;
    }

    if (list == NULL || list->size == 0) { // LinkedList is NULL or empty
        *dataOut = NULL;
        return 1;
    }

    if (index < 0 || index >= list->size) { // index is out of bounds
        return 1;
    }
    
    if (index == 0) {
        *dataOut = list->head->data;
    } else {
        Node *curr = list->head;
        for (int i = 0; i < index; i++) {
            curr = curr->next;
        } // now curr is at index
        *dataOut = curr->data;
    }

    return 0;
}

/** contains
  *
  * Traverses the LinkedList, trying to see if the LinkedList contains some
  * data. We say the list contains some input if there exists some node with
  * equal data as the input.
  *
  * You should use 'user_equal' here to compare the data. Note that pointers are
  * allowed to be null!
  *
  * If there are multiple pieces of data in the LinkedList which are equal to
  * the "data" parameter, return the one at the lowest index.
  *
  *
  * @param list a pointer to the LinkedList structure
  * @param data The data, to see if it exists in the LinkedList
  * @param dataOut A pointer to a pointer used to return the data contained in
  *                the LinkedList or NULL on failure
  * @return int    0 if dataOut is NULL, the list is NULL, or the list
  *                does not contain data, else 1
  */
int contains(LinkedList *list, User *data, User **dataOut)
{
    if (dataOut == NULL) { // dataOut is NULL
        return 0;
    }

    if (list == NULL || list->size == 0) { // LinkedList is NULL or empty
        *dataOut = NULL;
        return 0;
    }
    
    Node *curr = list->head;
    for (int i = 0; i < list->size; i++) {
        if (user_equal(curr->data, data) == 1) {
            *dataOut = curr->data;
            return 1;
        }
        curr = curr->next;
    }

    *dataOut = NULL;
    return 0;
}

/** pop_front
  *
  * Removes the Node at the front of the LinkedList, and returns its data to the program user.
  *
  * @param list a pointer to the LinkedList.
  * @param dataOut A pointer to a pointer used to return the data in the first
  *                Node or NULL if the LinkedList is NULL or empty
  * @return 1 if dataOut is NULL (the LinkedList is NULL or empty), else (on success) 0
  */
int pop_front(LinkedList *list, User **dataOut)
{
    if (dataOut == NULL) { // dataOut is NULL
        return 1;
    }

    if (list == NULL || list->size == 0) { // LinkedList is NULL or empty
        *dataOut = NULL;
        return 1;
    }

    Node *temp = list->head;
    *dataOut = list->head->data;
    list->head = list->head->next;
    list->size = list->size - 1;

    free(temp);

    return 0;
}

/** pop_back
  *
  * Removes the Node at the back of the LinkedList, and returns its data to the program user.
  *
  * @param list a pointer to the LinkedList.
  * @param dataOut A pointer to a pointer used to return the data in the last
  *                Node or NULL if the LinkedList is NULL or empty
  * @return 1 if dataOut is NULL (the LinkedList is NULL or empty), else (on success) 0
  */
int pop_back(LinkedList *list, User **dataOut)
{
    if (dataOut == NULL) { // dataOut is NULL
        return 1;
    }

    if (list == NULL || list->size == 0) { // LinkedList is NULL or empty
        *dataOut = NULL;
        return 1;
    }

    if (list->size == 1) {
        pop_front(list, dataOut);
    } else {
        Node *curr = list->head;
        while (curr->next->next != NULL) {
            curr = curr->next;
        } // now curr is the second to last element
        Node *temp = curr->next;
        *dataOut = curr->next->data;
        curr->next = NULL;
        list->size = list->size - 1;

        free(temp);
    }

    return 0;
}


/** remove_at_index
 *
 * Remove the element at the specified index in the LinkedList.
 *
 * @param list a pointer to the LinkedList structure
 * @param dataOut A pointer to a pointer used to return the data in the last
 *                Node or NULL if the LinkedList is NULL or empty
 * @param index 0-based, starting from the head in the inclusive range
 *              [0,size-1]
 * @return 1 if the index is out of bounds, the LinkedList is NULL or
 *         the dataOut is NULL
 *         otherwise return 0
 */
int remove_at_index(LinkedList *list, User **dataOut, int index)
{
    if (dataOut == NULL) { // dataOut is NULL
        return 1;
    }

    if (list == NULL) { // LinkedList is NULL
        *dataOut = NULL;
        return 1;
    }

    if (index < 0 || index > (list->size - 1)) { // index is out of bounds
        return 1;
    }

    if (index == 0) { // remove from front if index = 0
        pop_front(list, dataOut);
    } else if (index == (list->size - 1)) {
        pop_back(list, dataOut);
    } else {
        Node *curr = list->head;
        for (int i = 0; i < (index - 1); i++) {
            curr = curr->next;
        } // now curr is the element before index
        
        Node *temp = curr->next;
        *dataOut = curr->next->data;

        if (index == 1) {
            list->head->next = list->head->next->next;
        } else {
            curr->next = curr->next->next;
        }
        list->size = list->size - 1;

        free(temp);
    }
    
    return 0;
}

/** empty_list
 *
 * Empties the LinkedList. After this is called, the LinkedList should be
 * empty. This does NOT free the LinkedList struct itself, just all nodes and
 * data within. Make sure to check that the list is not NULL before
 * using it.
 *
 * Once again, the things that need to be freed after this function are:
 * - the nodes
 * - the user structs within the nodes
 * - all pointers in the user struct (and the union within)
 *
 * However, if you're using other functions you've written (which you should be),
 * those functions might take care of some of the freeing for you.
 *
 * You may call free on a char pointer as well as a struct pointer.
 *
 * If 'list' is NULL, simply return.
 *
 * @param list a pointer to the LinkedList structure
 */
void empty_list(LinkedList *list)
{
    if (list == NULL) {
        return;
    }

    User *dataOut = NULL;
    while (list->size > 0) {
        pop_front(list, &dataOut);
        if (dataOut->type == STUDENT) {
            free(dataOut->data.student.grades);
        }
        free(dataOut->name);
        free(dataOut);
    }
}

/** num_passing_all_classes
 *
 * Traverses the LinkedList, counting the number of Students that are passing
 * all of their classes. We say a student is passing if their grade is greater
 * than or equal to 60.
 *
 * You should make sure your code only counts students, as instructors cannot be
 * passing or failing a class!
 *
 * If a student is not taking any classes, they are NOT considered to be passing
 * all their classes.
 *
 * @param list a pointer to the LinkedList structure
 * @param dataOut A pointer to int used to return the count of students passing all
 *                of their classes
 *                or -1 if the LinkedList is NULL or empty
 * @return 1 if the LinkedList is NULL or empty, else (on success) 0
 */
int num_passing_all_classes(LinkedList *list, int *dataOut)
{
    if (list == NULL || list->size == 0) { // LinkedList is NULL or empty
        *dataOut = -1;
        return 1;
    }

    User *person = NULL;
    int studentsPassing = 0;
    
    for (int i = 0; i < list-> size; i++) {
        get(list, i, &person);
        if (person->type == STUDENT) {
            double *grades = person->data.student.grades;
            int classes = person->data.student.num_classes;
            if (classes != 0) {
                int classesPassed = 0;
                for (int j = 0; j < classes; j++) {
                    if (grades[j] >= 60) {
                        classesPassed++;
                    }
                }
                if (classesPassed == classes) {
                    studentsPassing++;
                    
                }
            }
        }
    }

    *dataOut = studentsPassing;
    return 0;
}

/** get_average_salary
 *
 * Traverses the LinkedList, computing the average of all instructor salaries.
 *
 * You should make sure your code only looks at instructors, as students do not have salaries!
 *
 * If there are no instructors in the list, place a 0 at dataOut.
 *
 * @param list a pointer to the LinkedList structure
 * @param dataOut A pointer to double used to return the average salary of the instructors
 *                or -1 if the LinkedList is NULL or empty
 * @return 1 if the LinkedList is NULL or empty, else (on success) 0
 */
int get_average_salary(LinkedList *list, double *dataOut)
{
    if (list == NULL || list->size == 0) { // LinkedList is NULL or empty
        *dataOut = -1;
        return 1;
    }

    int instructors = 0;
    double salaries = 0;
    *dataOut = 0;
    User *curr = NULL;
    for (int i = 0; i < list->size; i++) {
        get(list, i, &curr);
        if (curr->type == INSTRUCTOR) {
            instructors++;
            salaries += curr->data.instructor.salary;
        }
    }
    
    if (instructors > 0) {
        *dataOut = salaries / instructors;
    } else {
        *dataOut = 0;
    }
    
    return 0;
}
