#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>
using namespace std;

namespace AddressBook {
    struct Contact {
        string name;
        string phone;
        string email;
        string address;
    };

    class Book {
    private:
        string filename;

    public:
        Book(const string& file) : filename(file) {}

      
        void addContact() {
            ofstream file(filename, ios::app);
            if (!file) {
                cout << "Error opening file.\n";
                return;
            }

            Contact newContact;
            cout << "Enter contact details:\n";
            cout << "Name: ";
            getline(cin, newContact.name);

            while (true) {
                cout << "Phone: ";
                getline(cin, newContact.phone);

                if (isValidPhoneNumber(newContact.phone)) {
                    break;
                }

                cout << "Invalid phone number. Please enter a 10-digit number.\n";
            }

            while (true) {
                cout << "Email: ";
                getline(cin, newContact.email);

                if (isValidEmail(newContact.email)) {
                    break;
                }

                cout << "Invalid email address. Please enter a valid email address.\n";
            }

            cout << "Address: ";
            getline(cin, newContact.address);

            file << newContact.name << ',' << newContact.phone << ',' << newContact.email << ',' << newContact.address << '\n';
            cout << "Contact added successfully!\n";
            file.close();
        }

        bool isValidPhoneNumber(const string& phone) {
            return phone.length() == 10 && all_of(phone.begin(), phone.end(), ::isdigit);
        }

        bool isValidEmail(const string& email) {
            return email.find('@') != string::npos;
        }

        void listContacts() {
            ifstream file(filename);
            if (!file) {
                cout << "Error opening file.\n";
                return;
            }

            string line;
            cout << "Contact List:\n";
            while (getline(file, line)) {
                size_t pos = line.find(',');
                cout << "Name: " << line.substr(0, pos) << '\n';
                cout << "Phone: " << line.substr(pos + 1, line.find(',', pos + 1) - pos - 1) << '\n';
                cout << "Email: " << line.substr(line.find(',', pos + 1) + 1, line.find_last_of(',') - line.find(',', pos + 1) - 1) << '\n';
                cout << "Address: " << line.substr(line.find_last_of(',') + 1) << '\n';
                cout << '\n';
            }

            file.close();
        }

        void deleteContact() {
            ifstream inFile(filename);
            if (!inFile) {
                cout << "Error opening file.\n";
                return;
            }

            string tempFile = "temp.txt";
            ofstream outFile(tempFile);
            if (!outFile) {
                cout << "Error opening file.\n";
                inFile.close();
                return;
            }

            string name;
            cout << "Enter the name of the contact to delete: ";
            getline(cin, name);

            string line;
            bool found = false;
            while (getline(inFile, line)) {
                size_t pos = line.find(',');
                string contactName = line.substr(0, pos);
                if (contactName != name) {
                    outFile << line << '\n';
                } else {
                    found = true;
                }
            }

            inFile.close();
            outFile.close();

            if (found) {
                if (remove(filename.c_str()) != 0) {
                    cout << "Error deleting file.\n";
                    return;
                }
                if (rename(tempFile.c_str(), filename.c_str()) != 0) {
                    cout << "Error renaming file.\n";
                    return;
                }
                cout << "Contact deleted successfully!\n";
            } else {
                cout << "Contact not found.\n";
                remove(tempFile.c_str());
            }
        }

        void searchContact() {
            ifstream file(filename);
            if (!file) {
                cout << "Error opening file.\n";
                return;
            }

            string name;
            cout << "Enter the name of the contact to search: ";
            getline(cin, name);

            string line;
            bool found = false;
            while (getline(file, line)) {
                size_t pos = line.find(',');
                string contactName = line.substr(0, pos);
                if (contactName == name) {
                    cout << "Name: " << contactName << '\n';
                    cout << "Phone: " << line.substr(pos + 1, line.find(',', pos + 1) - pos - 1) << '\n';
                    cout << "Email: " << line.substr(line.find(',', pos + 1) + 1, line.find_last_of(',') - line.find(',', pos + 1) - 1) << '\n';
                    cout << "Address: " << line.substr(line.find_last_of(',') + 1) << '\n';
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Contact not found.\n";
            }

            file.close();
        }

        void modifyContact() {
            ifstream inFile(filename);
            if (!inFile) {
                cout << "Error opening file.\n";
                return;
            }

            string tempFile = "temp.txt";
            ofstream outFile(tempFile);
            if (!outFile) {
                cout << "Error opening file.\n";
                inFile.close();
                return;
            }

            string name;
            cout << "Enter the name of the contact to modify: ";
            getline(cin, name);

            string line;
            bool found = false;
            while (getline(inFile, line)) {
                size_t pos = line.find(',');
                string contactName = line.substr(0, pos);
                if (contactName != name) {
                    outFile << line << '\n';
                } else {
                    found = true;
                    Contact modifiedContact;
                    modifiedContact.name = contactName;

                    cout << "Enter modified contact details:\n";
                    cout << "Phone: ";
                    getline(cin, modifiedContact.phone);

                    while (!isValidPhoneNumber(modifiedContact.phone)) {
                        cout << "Invalid phone number. Please enter a 10-digit number.\n";
                        cout << "Phone: ";
                        getline(cin, modifiedContact.phone);
                    }

                    cout << "Email: ";
                    getline(cin, modifiedContact.email);

                    while (!isValidEmail(modifiedContact.email)) {
                        cout << "Invalid email address. Please enter a valid email address.\n";
                        cout << "Email: ";
                        getline(cin, modifiedContact.email);
                    }

                    cout << "Address: ";
                    getline(cin, modifiedContact.address);

                    outFile << modifiedContact.name << ',' << modifiedContact.phone << ',' << modifiedContact.email << ',' << modifiedContact.address << '\n';
                }
            }

            inFile.close();
            outFile.close();

            if (found) {
                if (remove(filename.c_str()) != 0) {
                    cout << "Error deleting file.\n";
                    return;
                }
                if (rename(tempFile.c_str(), filename.c_str()) != 0) {
                    cout << "Error renaming file.\n";
                    return;
                }
                cout << "Contact modified successfully!\n";
            } else {
                cout << "Contact not found.\n";
                remove(tempFile.c_str());
            }
        }

        
        void deleteAllContacts() {
            cout << "Are you sure you want to delete all contacts? This action cannot be undone. (y/n): ";
            string choice;
            getline(cin, choice);

            if (choice == "y" || choice == "Y") {
                ofstream file(filename, ios::trunc);
                if (!file) {
                    cout << "Error opening file.\n";
                    return;
                }

                cout << "All contacts deleted successfully!\n";
                file.close();
            } else {
                cout << "Operation canceled.\n";
            }
        }

       

        ~Book() {
            // Nothing specific to do here as the contacts are stored in the file.
        }
    };
}


int main() {
    using namespace AddressBook;

    Book book("add_book.txt");

    while (true) {
        cout << "Address Book - Options:\n";
        cout << "1. Add Contact\n";
        cout << "2. List Contacts\n";
        cout << "3. Delete Contact\n";
        cout << "4. Search Contact\n";
        cout << "5. Modify Contact\n";
        cout << "6. Delete All Contacts\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";

        string choice; // Change the choice variable to string type
       if (!(cin >> choice)) {
            // Error handling if input is not a string
            cout << "Invalid input. Please enter a valid choice.\n";
            cin.clear(); // Clear the error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore remaining characters in the input buffer
            continue; // Restart the loop
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == "7") {
            cout << "Exiting the program.\n";
            return 0;
        }

        // Convert the choice string to an integer
        int choiceNum;
        try {
            choiceNum = stoi(choice);
        } catch (const exception& e) {
            // Error handling if the choice string cannot be converted to an integer
            cout << "Invalid input. Please enter a valid choice.\n";
            continue; // Restart the loop
        }

        switch (choiceNum) {
            case 1:
                book.addContact();
                break;
            case 2:
                book.listContacts();
                break;
            case 3:
                book.deleteContact();
                break;
            case 4:
                book.searchContact();
                break;
            case 5:
                book.modifyContact();
                break;
            case 6:
                book.deleteAllContacts();
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }

        cout << '\n';
    }
}
