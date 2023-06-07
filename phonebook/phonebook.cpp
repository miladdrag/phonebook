#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <windows.h>
#include <sstream>
#include <set>

using namespace std;
enum Color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7
};

void setColor(Color textColor, Color backgroundColor) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD color = ((backgroundColor & 0x0F) << 5) | (textColor & 0x0F);
    SetConsoleTextAttribute(consoleHandle, color);
}
bool isNumber(const string& is_num)
{
    for (char const& ch : is_num) {
        if (isdigit(ch) == 0)
            return false;
    }
    return true;
}
void addContact(const string& fileName) {
    ofstream file(fileName, ios::app);

    if (file.is_open()) {
        bool isPhoneNumber = true;
        bool isIdNumber = true;
        string name, phoneNumber, studentId;
        setColor(GREEN, BLACK);
        cout << "\t\t\t\t\tEnter name: ";
        getline(cin >> ws, name);
        //cin.ignore();
        cout << "\t\t\t\t\tEnter phone number: ";
        getline(cin, phoneNumber);
        if (!isNumber(phoneNumber)) {
            setColor(RED, BLACK);
            cerr << "\t\t\t\t\tInvalid phone number. Please enter digits only." << endl;
            file.close();
            return;
        }
        //cin.ignore();

        cout << "\t\t\t\t\tEnter student ID: ";
        getline(cin, studentId);
        if (!isNumber(studentId)) {
            setColor(RED, BLACK);
            cerr << "\t\t\t\t\tInvalid phone number. Please enter digits only." << endl;
            file.close();
            return;
        }
        //cin.ignore();

        ifstream readFile(fileName); // باز کردن فایل برای خواندن موارد قبلی

        string line;
        bool isDuplicate = false;

        while (getline(readFile, line)) {
            string existingPhoneNumber, existingStudentId;

            // جدا کردن مقادیر از هر خط
            size_t comma1 = line.find(',');
            size_t comma2 = line.find(',', comma1 + 1);

            existingPhoneNumber = line.substr(comma1 + 1, comma2 - comma1 - 1);
            existingStudentId = line.substr(comma2 + 1);

            // چک کردن تکراری بودن شماره تلفن یا شماره دانشجویی
            if (existingPhoneNumber == phoneNumber || existingStudentId == studentId) {
                isDuplicate = true;
                break;
            }
        }

        readFile.close();

        if (isDuplicate == true) {
            setColor(RED, BLACK);
            cout << "\t\t\t\t\tPhone number or student ID is already exist." << endl;
        }
        else {

            file << name << ',' << phoneNumber << ',' << studentId << endl;
            cout << "\t\t\t\t\tContact added successfully." << endl;
        }

        file.close();
    }
    else {
        cerr << "\t\t\t\t\tError opening file." << endl;
    }
}
void searchByStudentId(const string& fileName, const string& studentId) {

    ifstream file(fileName);

    if (file.is_open()) {
        string line;
        bool found = false;

        while (getline(file, line)) {
            size_t comma1 = line.find(',');
            size_t comma2 = line.find(',', comma1 + 1);

            string existingStudentId = line.substr(comma2 + 1);

            if (existingStudentId == studentId) {
                string name = line.substr(0, comma1);
                string phoneNumber = line.substr(comma1 + 1, comma2 - comma1 - 1);
                setColor(GREEN, BLACK);
                cout << "\t\t\t\t\tName: " << name << endl;
                cout << "\t\t\t\t\tPhone number: " << phoneNumber << endl;

                found = true;
                break;
            }
        }

        file.close();

        if (!found) {
            setColor(RED, BLACK);
            cout << "\t\t\t\t\tNo contact found with the provided student ID.\n" << endl;
        }
    }
    else {
        setColor(RED, BLACK);
        cerr << "Error opening file.\n" << endl;
    }
}

void searchByPhoneNumber(const std::string& fileName, const std::string& phoneNumber) {
    std::ifstream file(fileName);

    if (file.is_open()) {
        std::string line;
        bool found = false;

        while (std::getline(file, line)) {
            size_t comma1 = line.find(',');
            size_t comma2 = line.find(',', comma1 + 1);

            std::string existingPhoneNumber = line.substr(comma1 + 1, comma2 - comma1 - 1);

            if (existingPhoneNumber == phoneNumber) {
                std::string name = line.substr(0, comma1);
                std::string studentId = line.substr(comma2 + 1);
                setColor(GREEN, BLACK);
                std::cout << "\t\t\t\t\tName: " << name << std::endl;
                std::cout << "\t\t\t\t\tStudent ID: " << studentId << std::endl;

                found = true;
                break;
            }
        }

        file.close();

        if (!found) {
            setColor(RED, BLACK);
            std::cout << "\t\t\t\t\tNo contact found with the provided phone number.\n" << std::endl;
        }
    }
    else {
        setColor(RED, BLACK);
        std::cerr << "Error opening file.\n" << std::endl;
    }
}
void removeContact(const string& fileName, const string& searchValue) {
    ifstream readFile(fileName);
    ofstream tempFile("temp.txt");

    if (readFile.is_open() && tempFile.is_open()) {
        string line;
        bool found = false;

        while (getline(readFile, line)) {
            size_t comma1 = line.find(',');
            size_t comma2 = line.find(',', comma1 + 1);

            string existingPhoneNumber = line.substr(comma1 + 1, comma2 - comma1 - 1);
            string existingStudentId = line.substr(comma2 + 1);

            if (existingPhoneNumber != searchValue && existingStudentId != searchValue) {
                tempFile << line << endl;
            }
            else {
                found = true;
            }
        }

        readFile.close();
        tempFile.close();

        if (found) {
            if (remove(fileName.data()) != 0) {
                setColor(RED, BLACK);
                cerr << "Error deleting contact.\n" << endl;
                return;
            }

            if (rename("temp.txt", fileName.data()) != 0) {
                setColor(RED, BLACK);
                cerr << "Error deleting contact.\n" << endl;
                return;
            }

            setColor(GREEN, BLACK);
            cout << "\t\t\t\t\tContact removed successfully." << endl;
        }

        else {
            setColor(RED, BLACK);
            cout << "\t\t\t\t\tNo contact found with the value.\n" << endl;
        }
    }
    else {
        setColor(RED, BLACK);
        cerr << "\t\t\t\t\tError opening file.\n" << endl;
    }
}
void editContact(const string& fileName, const string& searchId) {
    ifstream readFile(fileName);
    ofstream tempFile("temp.txt");

    if (readFile.is_open() && tempFile.is_open()) {
        string line;
        bool found = false;
        set<string> uniqueContacts;

        while (getline(readFile, line)) {
            size_t comma1 = line.find(',');
            size_t comma2 = line.find(',', comma1 + 1);

            string existingId = line.substr(comma2 + 1);

            uniqueContacts.insert(existingId);
            tempFile << line << endl;
        }

        readFile.close();
        tempFile.close();

        if (uniqueContacts.find(searchId) != uniqueContacts.end()) {
            ifstream tempReadFile("temp.txt");
            ofstream updatedFile(fileName);

            if (tempReadFile.is_open() && updatedFile.is_open()) {
                string tempLine;
                bool foundContact = false;

                while (getline(tempReadFile, tempLine)) {
                    size_t comma1 = tempLine.find(',');
                    size_t comma2 = tempLine.find(',', comma1 + 1);

                    string existingId = tempLine.substr(comma2 + 1);

                    if (existingId == searchId) {
                        string name, phoneNumber, studentId;

                        cout << "\t\t\t\t\tEnter new name: ";
                        getline(cin >> ws, name);

                        cout << "\t\t\t\t\tEnter new phone number: ";
                        getline(cin, phoneNumber);
                        if (!isNumber(phoneNumber)) {
                            setColor(RED, BLACK);
                            cerr << "\t\t\t\t\tInvalid phone number. Please enter digits only." << endl;
                            tempReadFile.close();
                            updatedFile.close();
                            remove("temp.txt");
                            return;
                        }

                        cout << "\t\t\t\t\tEnter new student ID: ";
                        getline(cin, studentId);
                        if (!isNumber(studentId)) {
                            setColor(RED, BLACK);
                            cerr << "\t\t\t\t\tInvalid student ID. Please enter digits only." << endl;
                            tempReadFile.close();
                            updatedFile.close();
                            remove("temp.txt");
                            return;
                        }

                        bool isDuplicate = false;
                        for (const string& contact : uniqueContacts) {
                            size_t comma1 = contact.find(',');
                            size_t comma2 = contact.find(',', comma1 + 1);
                            string existingPhoneNumber = contact.substr(comma1 + 1, comma2 - comma1 - 1);
                            string existingStudentId = contact.substr(comma2 + 1);

                            if (existingPhoneNumber == phoneNumber || existingStudentId == studentId) {
                                isDuplicate = true;
                                break;
                            }
                        }

                        if (isDuplicate) {
                            setColor(RED, BLACK);
                            cerr << "\t\t\t\t\tA contact with the same phone number or student ID already exists." << endl;
                            tempReadFile.close();
                            updatedFile.close();
                            remove("temp.txt");
                            return;
                        }

                        updatedFile << name << "," << phoneNumber << "," << studentId << endl;
                        foundContact = true;
                    }
                    else {
                        updatedFile << tempLine << endl;
                    }
                }

                tempReadFile.close();
                updatedFile.close();

                if (foundContact) {
                    remove("temp.txt");
                    cout << "\t\t\t\t\tContact updated successfully." << endl;
                }
                else {
                    remove("temp.txt");
                    setColor(RED, BLACK);
                    cout << "\t\t\t\t\tNo contact found with the provided ID." << endl;
                }
            }
            else {
                setColor(RED, BLACK);
                cerr << "\t\t\t\t\tError opening file." << endl;
            }
        }
        else {
            setColor(RED, BLACK);
            cout << "\t\t\t\t\tNo contact found with the provided ID." << endl;
            remove("temp.txt");
        }
    }
    else {
        cerr << "Error opening file." << endl;
    }
}





void showContacts(const string& fileName, int numEntries) {
    ifstream readFile(fileName);

    if (readFile.is_open()) {
        cout << "Contact List:" << endl;

        string line;
        int count = 0;

        while (getline(readFile, line) && count < numEntries) {
            size_t comma1 = line.find(',');
            size_t comma2 = line.find(',', comma1 + 1);

            string name = line.substr(0, comma1);
            string phoneNumber = line.substr(comma1 + 1, comma2 - comma1 - 1);
            string studentId = line.substr(comma2 + 1);

            cout << "Name: " << name << endl;
            cout << "Phone Number: " << phoneNumber << endl;
            cout << "Student ID: " << studentId << endl;
            cout << "---------------------------" << endl;

            count++;
        }

        readFile.close();
    }
    else {
        setColor(RED, BLACK);
        cerr << "Error opening file." << endl;
    }
}


int main() {
    int choice;
    int subchoice;
    bool continueProgram = true;
    while (continueProgram) {
        system("cls");
        cout << "\t\t\t\t\t----------------------------\n";
        cout << "\t\t\t\t\t|*|*|-|1.Add Contact        |-|*| \n";
        cout << "\t\t\t\t\t|*|*|-|2.Remove Contact     |-|*|\n";
        cout << "\t\t\t\t\t|*|*|-|3.Search Contact     |-|*|\n";
        cout << "\t\t\t\t\t|*|*|-|4.Our Contacts       |-|*|\n";
        cout << "\t\t\t\t\t|*|*|-|5.Edit Contact(ID)   |-|*|\n";
        cout << "\t\t\t\t\t|*|*|-|0.Exit               |-|*|\n";
        cout << "\t\t\t\t\t----------------------------\n";
        cout << "\t\t\t\t\t|*|*|-|choose an opreation   |-|*| \n";
        cout << "\t\t\t\t\t----------------------------------\n";



        cin >> choice;

        if (choice == 1) {
            string fileName = "contacts.txt";
            addContact(fileName);
        }
        else if (choice == 2) {
            setColor(GREEN, BLACK);
            cout << "\t\t\t\t\t----------------------------\n";
            cout << "\t\t\t\t\t|*|*|-|1.delete by ID          |-|*|\n";
            cout << "\t\t\t\t\t|*|*|-|2.delete by phone number|-|*|\n";
            cout << "\t\t\t\t\t----------------------------\n";
            cin >> subchoice;

            if (subchoice == 1) {
                string fileName = "contacts.txt";
                string deleteIDNumber;
                cout << "\t\t\t\t\tEnter the ID to delete:\n ";
                cin >> deleteIDNumber;
                removeContact(fileName, deleteIDNumber);


            }
            else if (subchoice == 2) {
                string fileName = "contacts.txt";
                string deletePhoneNumber;
                cout << "\t\t\t\t\tEnter the phone number to delete:\n ";
                cin >> deletePhoneNumber;
                removeContact(fileName, deletePhoneNumber);
            }
            else
            {
                cout << "\t\t\t\t\tplease enter correct operation.\n ";
            }
        }
        else if (choice == 3) {
            cout << "\t\t\t\t\t----------------------------\n";
            cout << "\t\t\t\t\t|*|*|-|1.search by ID          |-|*|\n";
            cout << "\t\t\t\t\t|*|*|-|2.search by phone number|-|*|\n";
            cout << "\t\t\t\t\t----------------------------\n";
            cin >> subchoice;
            if (subchoice == 1) {
                string fileName = "contacts.txt";
                string searchIDNumber;
                cout << "\t\t\t\t\tEnter the ID to search: \n";
                cin >> searchIDNumber;
                searchByStudentId(fileName, searchIDNumber);
            }
            else if (subchoice == 2) {
                string fileName = "contacts.txt";
                string searchPhoneNumber;
                cout << "\t\t\t\t\tEnter the phone number to search: \n";
                cin >> searchPhoneNumber;
                searchByPhoneNumber(fileName, searchPhoneNumber);
            }
            else
            {
                cout << "\t\t\t\t\tplease enter correct operation. \n";
            }
        }
        else if (choice == 4) {

            string fileName = "contacts.txt";
            int how_much;
            cout << "\t\t\t\t\thow much\n ";
            cin >> how_much;
            showContacts(fileName, how_much);
        }
        else if (choice == 5)
        {
            setColor(GREEN, BLACK);
            string fileName = "contacts.txt";
            string phoneNumberEdit;
            cout << "\t\t\t\t\tEnter the Id to edit: \n";
            cin >> phoneNumberEdit;
            editContact(fileName, phoneNumberEdit);
        }
        else if (choice == 0) {
            cout << "\t\t\t\t\tExiting program\n";
            continueProgram = false;
        }
        else
        {
            cout << "\t\t\t\t\tchoose right operation\n";
        }
        if (continueProgram) {
            int continueChoice;
            cout << "\t\t\t\t\tDo you want to continue? (1: Continue, 0: Exit):\n ";
            cin >> continueChoice;

            if (continueChoice != 1) {
                continueProgram = false; // خروج از حلقه و پایان برنامه
            }
        }
    }
    return 0;

}
