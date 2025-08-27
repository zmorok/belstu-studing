#include "functions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <Windows.h>

using namespace std;

// ��������� �������
struct Client {
    int index;
    string name;
    string address;
    string phoneNumber;
    string company;
};


const int MAX_CLIENTS = 100;
Client clients[MAX_CLIENTS];
int numClients = 0;

// ������� ���������� ������� (redacted)
void addClient() {
    system("cls");
    cout << "\n\t\t���������� �������...\n\n";
    if (numClients < MAX_CLIENTS) {
        Client newClient;

        newClient.index = numClients;

        cout << "\t������� ��� �������: ";
        cin.ignore();
        getline(cin, newClient.name);

        cout << "\t������� ����� �������: ";
        getline(cin, newClient.address);

        cout << "\t������� ������� �������: ";
        getline(cin, newClient.phoneNumber);

        cout << "\t������� ������������ �������� �������: ";
        getline(cin, newClient.company);

        clients[numClients++] = newClient;
        cout << "\n\t������ �������� �������!\n\n";
    }
    else {
        cout << "��������� �������� �����������. ���������� �������� ������ �������.\n";
    }
}

// ������� ����������� ��������
 void displayClients() {
    system("cls");
    cout << "\n\t������ ��������:\n\n";
    cout << "������\t���\t\t\t�����\t\t\t�������\t\t\t��������\n";
    for (int i = 0; i < numClients; i++) {
        string tabs = clients[i].name.length() > 15 ? "\t" : "\t\t";
        cout << clients[i].index << "\t" <<
            clients[i].name << tabs <<
            clients[i].address << "\t\t" <<
            clients[i].phoneNumber << "\t\t" <<
            clients[i].company << "\n";
    }
    cout << '\n';
}

// ������� ������ ������� �� ������� (redacted)
 void displayClientAtIndex() {
     system("cls");
     int index;
     cout << "\n\t������� ������ ������� ��� �����������: ";
     cin >> index;
     for (const auto& client : clients) {
         if (client.index == index) {
             cout << "������: " << client.index <<
                 "\n\t���: " << client.name <<
                 "\n\t�����: " << client.address <<
                 "\n\t�������: " << client.phoneNumber <<
                 "\n\t��������: " << client.company << "\n\n";
             return;
         }
     }

     cout << "������ � ��������� �������� �� ������.\n";
 }

// ������� �������� ������� �� ������� 
 void deleteClient() {
     system("cls");
     displayClients();
     int index;
     cout << "������� ������ ������� ��� ��������: ";
     cin >> index;

     bool found = false;
     for (int i = 0; i < numClients; ++i) {
         if (clients[i].index == index) {
             found = true;
             numClients--;
             for (; i < numClients; i++) {
                 clients[i] = clients[i + 1];
             }
             cout << "\t������ ��� ������ �������.\n";
             break;
         }
     }

     if (!found) {
         cout << "\t������ � ��������� �������� �� ������.\n";
     }
 }

// ������� ���������� �������� �� ���������� ��������� � �������
 void sortClients() {
    system("cls");
    int sortChoice;
    cout << "\n\t�������� �������� ��� ����������:\n";
    cout << "\t1. �� �����\n";
    cout << "\t2. �� ������\n";
    cout << "\t3. �� ������ ��������\n";
    cout << "\t4. �� ��������\n";
    cout << "\n\t������� ����� ���������: ";
    cin >> sortChoice;

    int orderChoice;
    cout << "\n\t�������� ������� ����������:\n";
    cout << "\t1. �� �����������\n";
    cout << "\t2. �� ��������\n";
    cout << "\n\t������� ����� �������: ";
    cin >> orderChoice;

    bool ascending = (orderChoice == 1);
    // ����� ������� ��������� � ����������� �� ������ ������������
    bool (*compareFunction)(const Client&, const Client&);
    switch (sortChoice) {
    case 1:
        compareFunction = [](const Client& a, const Client& b) {
            return a.name < b.name;
            };
        break;
    case 2:
        compareFunction = [](const Client& a, const Client& b) {
            return a.address < b.address;
            };
        break;
    case 3:
        compareFunction = [](const Client& a, const Client& b) {
            return a.phoneNumber < b.phoneNumber;
            };
        break;
    case 4:
        compareFunction = [](const Client& a, const Client& b) {
            return a.company < b.company;
            };
        break;
    default:
        cout << "\n\t�������� �����. ������� � �������� ����.\n\n";
        return;
    }

    // ���������� �������� ���������
    for (int i = 0; i < numClients - 1; i++) {
        for (int j = 0; j < numClients - i - 1; j++) {
            if (compareFunction(clients[j], clients[j + 1]) == !ascending) {
                swap(clients[j], clients[j + 1]);
            }
        }
    }

    cout << "\n\t������ �������� ����� ����������:\n\n";
    cout << "������\t���\t\t�����\t\t�������\t\t��������\n";
    for (int i = 0; i < numClients; i++) {
        cout << clients[i].index << "\t" <<
            clients[i].name << "\t" <<
            clients[i].address << "\t" <<
            clients[i].phoneNumber << "\t" <<
            clients[i].company << "\n";
    }
    cout << '\n';
}

// ������� ������ ������ � ����� (���������� �� ������� � ����������)
 void writeClientsToFile() {
    system("cls");
    ofstream file("clients.txt");
    if (file.is_open()) {
        for (int i = 0; i < numClients; i++) {
            file << i << "," << clients[i].name <<
                "," << clients[i].address <<
                "," << clients[i].phoneNumber <<
                "," << clients[i].company << endl;
        }
        file.close();
        cout << "\n\t������ � �������� ��������� � ���� �������.\n\n";
        system("pause");
    }
    else {
        cout << "\n\t�� ������� ������� ���� ��� ������.\n\n";
        system("pause");
    }
    readClientsFromFile();
}

// ������� ������ ������ �� ����� (redacted)
 void readClientsFromFile() {
    system("cls");
    ifstream file("clients.txt");
    if (file.is_open()) {
        numClients = 0;
        string line;
        while (getline(file, line)) {
            string sym = ",";
            size_t pos = 0;
            Client client;
            // ������
            pos = line.find(sym);
            int index = stoi(line.substr(0, pos));
            client.index = index;
            line.erase(0, pos + sym.length());
            // ���
            pos = line.find(sym);
            client.name = line.substr(0, pos);
            line.erase(0, pos + sym.length());
            // �����
            pos = line.find(sym);
            client.address = line.substr(0, pos);
            line.erase(0, pos + sym.length());
            // �������
            pos = line.find(sym);
            client.phoneNumber = line.substr(0, pos);
            line.erase(0, pos + sym.length());
            // ��������
            pos = line.find(sym);
            client.company = line.substr(0, pos);
            line.erase(0, pos + sym.length());
            clients[index] = client;
            numClients++;
        }
        file.close();
    }
    else {
        cout << "\n\t�� ������� ������� ���� ��� ������.\n\n";
    }
}

// ������� ��������� ������ ������������
 void editClient() {
    system("cls");
    displayClients();
    int index;
    cout << "������� ������ ������� ��� ��������������: ";
    cin >> index;
    if (index >= 0 && index < numClients) {
        Client& client = clients[index];
        cout << "�������� ������ �������������� ������:\n";
        cout << "1. ������ ��������������\n";
        cout << "2. �������������� ��������� �����\n";
        cout << "������� ����� ��������: ";
        int choice;
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "������� ������ �������:\n";
            cout << "���: " << client.name << "\n";
            cout << "�����: " << client.address << "\n";
            cout << "�������: " << client.phoneNumber << "\n";
            cout << "��������: " << client.company << "\n\n";

            cout << "������� ����� ������:\n";
            cout << "���: ";
            cin.ignore();
            getline(cin, client.name);
            cout << "�����: ";
            getline(cin, client.address);
            cout << "�������: ";
            getline(cin, client.phoneNumber);
            cout << "��������: ";
            getline(cin, client.company);

            cout << "\n������ ������� ������� ��������.\n";
            break;
        case 2:
        {
            cout << "�������� ���� ��� ��������������:\n";
            cout << "1. ���\n";
            cout << "2. �����\n";
            cout << "3. �������\n";
            cout << "4. ��������\n";
            cout << "������� ����� ����: ";
            int fieldChoice;
            cin >> fieldChoice;
            cout << "������� ����� ��������: ";
            string newValue;
            cin.ignore();
            getline(cin, newValue);
            switch (fieldChoice) {
            case 1:
                client.name = newValue;
                break;
            case 2:
                client.address = newValue;
                break;
            case 3:
                client.phoneNumber = newValue;
                break;
            case 4:
                client.company = newValue;
                break;
            default:
                cout << "������������ ����� ����.\n";
                break;
            }
            cout << "\n������ ������� ������� ��������.\n";
            break;
        }
        default:
            cout << "������������ ����� ��������.\n";
            break;
        }
    }
    else {
        cout << "������������ ������ �������.\n";
    }
}

 // ������� ������ ���������� �� ������
void searchClientBySubstring() {
     system("cls");
     string substring;
     cout << "\n������� ��������� ��� ������: ";
     cin >> substring;

     cout << "\n���������� ������:\n";
     for (const auto& client : clients) {
         if (client.name.find(substring) != string::npos ||
             client.address.find(substring) != string::npos ||
             client.phoneNumber.find(substring) != string::npos ||
             client.company.find(substring) != string::npos) {

             string tabs = client.name.length() > 15 ? "\t" : "\t\t";
             cout << "\t" <<
                 client.index << "\t" << 
                 client.name << tabs <<
                 client.address << "\t\t" <<
                 client.phoneNumber << "\t\t" <<
                 client.company << "\n";;
         }
     }
     cout << '\n';
 }

// �������� ����� �������
static int binarySearchByIndex(Client clients[], int size, int key) {
    
    // ���������� ������� �� �������
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (clients[j].index > clients[j + 1].index) {
                swap(clients[j], clients[j + 1]);
            }
        }
    }

    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (clients[mid].index == key) {
            return mid;
        }

        if (key < clients[mid].index) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    return -1; 
}

// ������� ��������� ������ �� �������
void searchByBinaryWithIndex() {
    system("cls");
    int key;
    cout << "\n\t������� ������ ��� ��������� ������: ";
    cin >> key;
    int index = binarySearchByIndex(clients, numClients, key);
    if (index != -1) {
        int clientBinaryIndex = clients[index].index;
        for (const auto& client : clients) {
            if (client.index == clientBinaryIndex) {
                cout << "������: " << client.index <<
                    "\n\t���: " << client.name <<
                    "\n\t�����: " << client.address <<
                    "\n\t�������: " << client.phoneNumber <<
                    "\n\t��������: " << client.company << "\n\n";
                return;
            }
        }
    }
    else {
        cout << "������ � ��������� �������� �� ������.\n";
    }
}