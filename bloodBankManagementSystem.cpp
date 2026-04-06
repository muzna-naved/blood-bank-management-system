#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
using namespace std;

struct Node
{ // Linked List Node
    string bloodGroup;
    int units;
    int year;
    int month;
    int day;
    Node *next;
};

struct QueueNode
{
    string name;     // patient name
    string blood;    // blood type
    int age;         // patient age
    QueueNode *next; // pointer to next request in queue
};

struct BSTNode
{
    string name;
    string bloodGroup;
    int id;
    int age;
    float weight;
    bool hasDisease;
    int monthsSinceLastDonation;
    BSTNode *left;
    BSTNode *right;
};

struct StackNode
{
    string patientName;
    string bloodType;
    int units;
    int remainingUnits;   
    int year;
    int month;
    int day;
    StackNode *next;
};

class BloodBank
{
public:
    Node *head;
    QueueNode *front;
    QueueNode *rear;
    BSTNode *root;
    StackNode *top;
    int nextId;

    BloodBank()
    { // constructor
        head = NULL;
        front = rear = NULL;
        root = NULL;
        top = NULL;
        nextId = 1;
    }

    //-------------------------LINKED LIST--------------------------------

    void addBloodUnit()
    { // add blood unit function
        cout << "----- ADD BLOOD UNIT -----" << endl;
        cout << "--------------------------" << endl;
        int y, m, d;
        string bG;
        int qty;

        cout << "Enter Collection Year: ";
        cin >> y;
        cout << "Enter Collection Month: ";
        cin >> m;
        cout << "Enter Collection Day: ";
        cin >> d;

        time_t now = time(0);      // current time
        tm *ltm = localtime(&now); // convert to local time

        int cday = ltm->tm_mday;
        int cmonth = 1 + ltm->tm_mon;    // we added 1 because tm_mon stores january as 0, and so on
        int cyear = 1900 + ltm->tm_year; // tm_year stores years passed since 1900, (it stores 126 for the year 2026), so we added 1900

        // basic range check
        if (m < 1 || m > 12 || d < 1 || d > 31)
        {
            cout << "Please enter a valid date." << endl;
            return;
        }

        // future date check
        if (y > cyear ||
            (y == cyear && m > cmonth) ||
            (y == cyear && m == cmonth && d > cday))
        {

            cout << "Collection date cannot be in the future." << endl;
            return;
        }

        cout << "Enter Blood Group: ";
        cin >> bG;
        cout << "Enter Quantity: ";
        cin >> qty;

        Node *newNode = new Node();
        newNode->bloodGroup = bG;
        newNode->units = qty;
        newNode->year = y;
        newNode->month = m;
        newNode->day = d;
        newNode->next = NULL;

        // if inventory is empty
        if (head == NULL)
        {
            head = newNode; // newNode becomes head
        }
        else
        {
            // make temp pointer
            Node *temp = head;         // temp points to head
            while (temp->next != NULL) // traverse the list until reaches NULL
            {
                temp = temp->next; // move to next
            }
            temp->next = newNode;
        }
        cout << "Blood unit added successfully in the inventory!" << endl;
    }

    void viewBloodInventory()
    { // display blood inventory fucntion
        cout << "----- BLOOD INVENTORY -----" << endl;
        cout << "---------------------------" << endl;

        if (head == NULL)
        {
            cout << "The inventory is empty." << endl;
            return;
        }
        else
        {
            Node *temp = head; // temp points to head
            while (temp != NULL)
            { // traverse until reaches NULL
                cout << "Blood Group: " << left << setw(10) << temp->bloodGroup << "Units: " << temp->units << "     Date:" << temp->day << "/" << temp->month << "/" << temp->year << endl;
                temp = temp->next; // move to next
            }
        }
    }

    Node *searchBloodGroup()
    { // serach blood group function
        string bg;
        cout << "Enter blood group to search: ";
        cin >> bg;

        if (head == NULL)
        {
            cout << "Inventory is empty!" << endl;
            return NULL;
        }

        Node *temp = head;
        while (temp != NULL)
        { // traverse until reaches NULL
            if (temp->bloodGroup == bg)
            {
                int units = temp->units;
                cout << "Blood group found. Available units: " << units << endl;
                return temp;
            }
            temp = temp->next;
        }
        cout << "BLOOD GROUP NOT FOUND." << endl;
        return NULL;
    }

    void checkLowStock()
    { // check low stock fucntion
        if (head == NULL)
        { // when inventory is empty
            cout << "Inventory is empty." << endl;
            return;
        }

        bool found = false;
        Node *temp = head;

        while (temp != NULL)
        { // traverse until reaches NULL
            if (temp->units < 3)
            {
                cout << "BLOOD GROUP: " << temp->bloodGroup << " UNITS: " << temp->units << " - Stock is LOW!" << endl;
                found = true;
            }
            temp = temp->next; // move to next
        }

        if (!found) // if blood group not found
            cout << "No Blood Group is low in stock!" << endl;
    }

    void deleteExpiredBlood()
    { // delete expire blood function
        if (head == NULL)
        {
            cout << "Inventory is empty" << endl;
            return;
        }

        time_t now = time(0);      // current time
        tm *ltm = localtime(&now); // convert to local time

        int cday = ltm->tm_mday;
        int cmonth = 1 + ltm->tm_mon;
        int cyear = 1900 + ltm->tm_year;

        Node *temp = head;
        Node *prev = NULL;
        bool deleted = false;

        while (temp != NULL)
        {
            bool isexpired = false;
            tm bloodTime = {};
            bloodTime.tm_year = temp->year - 1900;
            bloodTime.tm_mon = temp->month - 1;
            bloodTime.tm_mday = temp->day;

            time_t blood_date = mktime(&bloodTime);

            double diff = difftime(now, blood_date) / (60 * 60 * 24); // days

            if (diff > 30)
                // condition for expiry (assuming blood expires in 30 days)
                isexpired = true;

            if (isexpired)
            {
                Node *todelete = temp;

                if (temp == head)
                { // if expired node is head
                    head = head->next;
                    temp = head;
                }
                else
                {
                    // if expired node is middle or last
                    prev->next = temp->next;
                    temp = temp->next;
                }
                delete todelete; // removes the node from memory
                deleted = true;
            }
            else
            {
                // move to next node if not expired
                prev = temp;
                temp = temp->next;
            }
        }
        if (deleted)
            cout << "Expired blood units deleted successfully!" << endl;
        else
            cout << "No expired blood units found." << endl;
    }

    //--------------------------END OF LINKED LIST------------------------

    //--------------------------QUEUE-------------------------------------

    void enqueuePatientRequest()
    {                                         // function to add a new patient request in the queue
        QueueNode *newNode = new QueueNode(); // create a new node for patient request

        cout << "\n==== Blood Request Form ====\n";
        cout << "Enter Patient Name: ";
        cin.ignore();
        getline(cin, newNode->name);
        cout << "Enter Required Blood Type: ";
        cin >> newNode->blood;
        cout << "Enter Patient Age: ";
        cin >> newNode->age;

        newNode->next = NULL; // new node will be last node so next is null

        if (rear == NULL)
        {                           // if queue is empty
            front = rear = newNode; // front and rear both point to new node
        }
        else
        {
            rear->next = newNode; // link new node after rear
            rear = newNode;       // update rear
        }
        cout << "\nPatient request added successfully.\n";
    }

    void dequeuePatientRequest()
    { // function to Dequeue patient request
        if (front == NULL)
        { // if queue is empty
            cout << "\n No pending requests.\n";
            return;
        }

        QueueNode *temp = front; // store front node in temp
        front = front->next;     // move front to next request

        if (front == NULL)
        { // if queue becomes empty after removal
            rear = NULL;
        }

        string name = temp->name; // store data in variables
        string blood = temp->blood;
        int age = temp->age;
        delete temp; // free memory

        cout << "\n--- Processing Request ---\n"; // display patient info
        cout << "Name       : " << name << endl;
        cout << "Blood Type : " << blood << endl;
        cout << "Age        : " << age << endl;
        cout << "--------------------------\n";

        Node *found = head;
        while (found != NULL)
        {
            if (found->bloodGroup == blood)
                break;

            found = found->next;
        }

        if (found != NULL)
        { // if blood found in inventory
            int year = found->year;
            int month = found->month;
            int day = found->day;

            if (found->units > 0)
            {
                found->units--;
                pushIssuedBlood(name, blood, 1, found->units, year, month, day);
            }
            else
            {
                cout << "Blood group found but no units available.\n";
                return;
            }

            cout << "Status: Request fulfilled.\n";
            cout << "Remaining units of " << blood << " : " << found->units << endl;
        }
        else
        {
            cout << "Status: No match in inventory, try using a compatible type (if any) or check other blood banks if urgent.\n";
        }
    }

    void cancelRequest()
    { // function to cancel the next patient request
        if (front == NULL)
        { // if queue is empty
            cout << "No request to cancel\n";
            return;
        }

        QueueNode *temp = front; // temporary pointer to store first node
        front = front->next;     // move front to next request

        if (front == NULL)
        { // if queue becomes empty after deletion
            rear = NULL;
        }
        delete temp; // delete the cancelled request
        cout << "Next patient request cancelled\n";
    }

    void viewPendingRequests()
    { // function to view pending requests
        if (front == NULL)
        { // if queue is empty
            cout << "\n No pending requests.\n";
            return;
        }

        QueueNode *temp = front; // start from front of queue
        int count = 1;

        while (temp != NULL)
        {                             // traverse until end of queue
            string name = temp->name; // store current node data in variables
            string blood = temp->blood;
            int age = temp->age;

            // display current request
            cout << count << ". " << name << " | " << blood << " | Age: " << age << endl;
            count++;           // increment
            temp = temp->next; // move to next request
        }
    }

    //--------------------------END OF QUEUE------------------------------

    //-----------------------------BST------------------------------------

    void addDonor()
    {
        BSTNode *newNode = new BSTNode;

        cout << "\n------DONOR APPLICATION FORM------\n"
             << endl;
        cout << "Enter Donor Name: ";
        cin.ignore();
        getline(cin, newNode->name);

        cout << "Enter Blood Type: ";
        cin >> newNode->bloodGroup;

        cout << "Enter Age: ";
        cin >> newNode->age;

        cout << "Enter Weight: ";
        cin >> newNode->weight;

        cout << "Months since last donation: ";
        cin >> newNode->monthsSinceLastDonation;

        cout << "\nMajor Diseases List:\n";
        cout << "\t1. Hepatitis\n";
        cout << "\t2. HIV/AIDS\n";
        cout << "\t3. Cancer\n";
        cout << "\t4. Tuberculosis\n";
        cout << "\t5. Heart Disease\n";
        cout << "\t6. Other serious disease\n";
        cout << "Have you ever had any major disease from the above list? (1 = Yes / 0 = No): ";
        cin >> newNode->hasDisease;
        cout << endl;

        // now eligibility check/ health screening
        if (newNode->age < 18)
        {
            cout << "Rejected: Age must be 18+\n";
            delete newNode;  
            return;
        }

        if (newNode->weight < 50)
        {
            cout << "Rejected: Weight must be >= 50\n";
            delete newNode;  
            return;
        }

        if (newNode->monthsSinceLastDonation < 3)
        {
            cout << "Rejected: Wait 3 months\n";
            delete newNode;  
            return;
        }

        if (newNode->hasDisease)
        {
            cout << "Rejected due to disease.\n";
            delete newNode;  
            return;
        }

        newNode->id = nextId++;
        newNode->left = NULL;
        newNode->right = NULL;

        // insert into BST
        if (root == NULL)
        {
            root = newNode;
        }

        else
        {
            BSTNode *temp = root;
            BSTNode *parent = NULL;

            while (temp != NULL)
            {
                parent = temp;

                if (newNode->id < temp->id)
                    temp = temp->left;
                else
                    temp = temp->right;
            }

            if (newNode->id < parent->id)
                parent->left = newNode;
            else
                parent->right = newNode;
        }

        cout << "Donor added successfully. ID = " << newNode->id << endl;
    }

    BSTNode *findMin(BSTNode *node)
    {

        while (node->left != NULL)
        {
            node = node->left;
        }

        return node;
    }

    BSTNode *deleteDonorHelper(BSTNode *node, int id)
    {

        if (node == NULL)
        {
            cout << "Donor not found.\n";
            return node;
        }

        if (id < node->id)
            node->left = deleteDonorHelper(node->left, id);

        else if (id > node->id)
            node->right = deleteDonorHelper(node->right, id);

        else
        {

            cout << "Donor deleted: " << node->name << endl;

            // no child
            if (node->left == NULL && node->right == NULL)
            {
                delete node;
                return NULL;
            }

            // one child
            else if (node->left == NULL)
            {
                BSTNode *temp = node->right;
                delete node;
                return temp;
            }

            else if (node->right == NULL)
            {
                BSTNode *temp = node->left;
                delete node;
                return temp;
            }

            // two children
            BSTNode *temp = findMin(node->right);

            node->id = temp->id;
            node->name = temp->name;
            node->bloodGroup = temp->bloodGroup;
            node->age = temp->age;
            node->weight = temp->weight;
            node->hasDisease = temp->hasDisease;
            node->monthsSinceLastDonation = temp->monthsSinceLastDonation;

            node->right = deleteDonorHelper(node->right, temp->id);
        }

        return node;
    }

    void deleteDonor()
    {

        if (root == NULL)
        {
            cout << "No donors available.\n";
            return;
        }

        int id;
        cout << "Enter Donor ID to delete: ";
        cin >> id;

        root = deleteDonorHelper(root, id);
    }

    void searchById(BSTNode *node, int id)
    {
        if (node == NULL)
        {
            cout << "Donor not found!" << endl;
            return;
        }

        if (id == node->id)
        {
            cout << "Found: ID " << node->id
                 << ", Name: " << node->name
                 << ", Blood Group: " << node->bloodGroup << endl;
            return;
        }

        if (id < node->id)
            searchById(node->left, id);
        else
            searchById(node->right, id);
    }

    void searchDonor()
    {
        int id;
        cout << "Enter ID to search: ";
        cin >> id;

        searchById(root, id);
    }

    void displayDonors(BSTNode *node)
    {
        if (node == NULL)
            return;

        displayDonors(node->left);

        cout << "ID          : " << node->id << endl;
        cout << "Name        : " << node->name << endl;
        cout << "Blood Group : " << node->bloodGroup << endl;
        cout << "-----------------------------\n";

        displayDonors(node->right);
    }

    //--------------------------END OF BST--------------------------------

    //-----------------------------STACK----------------------------------

void pushIssuedBlood(string name, string blood, int units, int remaining, int year, int month, int day)
    {
        /*This function will be called from linked list when they remove an issued
        blood record from inventory so they pass that record into stack(this function)*/

        StackNode *newNode = new StackNode; // create new node
        newNode->patientName = name;
        newNode->bloodType = blood;
        newNode->units = units;
        newNode->remainingUnits = remaining; 
        newNode->year = year;
        newNode->month = month;
        newNode->day = day;

        newNode->next = top; // stack insertion
        top = newNode;
        cout << "Blood issue recorded successfully." << endl;
    }

   void popLastIssuedBlood()
    {
        if (top == NULL)
        {
        cout << "No issue records available.\n";
        return;
        }

        StackNode *temp = top;

        cout << "Removing last issued record of " << temp->patientName << endl;

        // Restore inventory
        Node *inv = head;
        while (inv != NULL)
        {
            if (inv->bloodGroup == temp->bloodType)
            {
            inv->units += temp->units;
            break;
            }
        inv = inv->next;
        }

        top = top->next;
        delete temp;
    }

    void viewRecentIssues()
    { // display function
        if (top == NULL)
        { // if no records available
            cout << "No issue records available.\n";
            return;
        }

        StackNode *temp = top;
        cout << "\n------Recent Blood Issues------\n";

        while (temp != NULL)
        { // loop to display all records
            cout << "Patient      : " << temp->patientName << endl;
            cout << "Blood Type   : " << temp->bloodType << endl;
            cout << "Units Issued : " << temp->units << endl;
           cout << "Remaining    : " << temp->remainingUnits << endl;
            cout << "Date         : " << temp->day << "/" << temp->month << "/" << temp->year << endl;
            cout << "-----------------------------\n";

            temp = temp->next;
        }
    }

    void countRecentIssues()
    { // to count records in history
        int count = 0;
        StackNode *temp = top;

        while (temp != NULL)
        {
            count++;
            temp = temp->next;
        }
        cout << "Total issued blood records: " << count << endl;
    }

    //--------------------------END OF STACK------------------------------
};

int main()
{
    BloodBank bb;
    int choice;
    do
    {
        cout << "\n-----BLOOD BANK MANAGEMENT SYSTEM-----\n";
        cout << "\n\t1. Add Donor\n\t2. Delete Donor\n\t3. Search Donor by ID\n\t4. Display all Donors" << endl;
        cout << "\n\t5. Add Patient Request\n\t6. Process Patient Request\n\t7. View Pending Requests\n\t8. Cancel Request" << endl;
        cout << "\n\t9. Add Blood Unit\n\t10. View Blood Inventory\n\t11. Search for a Blood Group\n\t12. Check Low Stock\n\t13. Remove Expired Blood" << endl;
        cout << "\n\t14. Remove last record\n\t15. View recent blood issues\n\t16. Count total blood issues" << endl;
        cout << "\n\t17. Exit\n\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            bb.addDonor();
            break;
        case 2:
            bb.deleteDonor();
            break;
        case 3:
            bb.searchDonor();
            break;
        case 4:
            bb.displayDonors(bb.root);
            break;
        case 5:
            bb.enqueuePatientRequest();
            break;
        case 6:
            bb.dequeuePatientRequest();
            break;
        case 7:
            bb.viewPendingRequests();
            break;
        case 8:
            bb.cancelRequest();
            break;
        case 9:
            bb.addBloodUnit();
            break;
        case 10:
            bb.viewBloodInventory();
            break;
        case 11:
            bb.searchBloodGroup();
            break;
        case 12:
            bb.checkLowStock();
            break;
        case 13:
            bb.deleteExpiredBlood();
            break;
        case 14:
            bb.popLastIssuedBlood();
            break;
        case 15:
            bb.viewRecentIssues();
            break;
        case 16:
            bb.countRecentIssues();
            break;
        case 17:
            cout << "Exiting...";
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    } while (choice != 17);
    cout << "Goodbye!" << endl;
    return 0;
}