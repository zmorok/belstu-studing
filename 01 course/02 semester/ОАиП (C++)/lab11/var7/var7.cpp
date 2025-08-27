#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

// вершина бинарного дерева
struct Node {
    int key;
    string data;
    Node* left;
    Node* right;

    Node(int k, const string& d) : key(k), data(d), left(nullptr), right(nullptr) {}
};

// добавление вершины в бинарное дерево
static Node* insertNode(Node* root, int key, const string& data) {
    if (root == nullptr) {
        return new Node(key, data);
    }

    if (key < root->key) {
        root->left = insertNode(root->left, key, data);
    }
    else if (key > root->key) {
        root->right = insertNode(root->right, key, data);
    }
    return root;
}

// поиска вершины по ключу
static Node* searchNode(Node* root, int key) {
    if (root == nullptr || root->key == key) {
        return root;
    }

    if (key < root->key) {
        return searchNode(root->left, key);
    }

    return searchNode(root->right, key);
}

// удаления вершины по ключу
static Node* deleteNode(Node* root, int key) {
    if (root == nullptr) {
        return root;
    }

    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    }
    else {
        if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        Node* temp = root->right;
        while (temp->left != nullptr) {
            temp = temp->left;
        }

        root->key = temp->key;
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->key);
    }

    return root;
}

// вывод дерева
static void printTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    cout << "Ключ: " << root->key << ", Данные: " << root->data << endl;
    printTree(root->left);
    printTree(root->right);
}

// освобождение памяти
static void freeTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// чтение данных из файла и добавление их в дерево
static void readFromFile(Node*& root, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла." << endl;
        return;
    }

    int key;
    string data;
    while (file >> key >> data) {
        root = insertNode(root, key, data);
    }
    file.close();
}

// запись данных дерева в файл
static void writeToFileHelper(Node* root, ofstream& file) {
    if (root != nullptr) {
        file << root->key << " " << root->data << endl;
        writeToFileHelper(root->left, file);
        writeToFileHelper(root->right, file);
    }
}
static void writeToFile(Node* root, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла." << endl;
        return;
    }
    writeToFileHelper(root, file);
    file.close();
}

// подсчет количества листьев в дереве
static int countLeaves(Node* root) {
    if (root == nullptr) {
        return 0;
    }
    if (root->left == nullptr && root->right == nullptr) {
        return 1;
    }
    return countLeaves(root->left) + countLeaves(root->right);
}

static void displayMenu() {
    cout << "Меню:" << endl;
    cout << "1. Добавить узел" << endl;
    cout << "2. Вывести дерево" << endl;
    cout << "3. Удалить узел" << endl;
    cout << "4. Поиск узла" << endl;
    cout << "5. Загрузить данные из файла" << endl;
    cout << "6. Сохранить данные в файл" << endl;
    cout << "7. Посчитать количество листьев" << endl;
    cout << "0. Выход" << endl;
    cout << "Введите выбор: ";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Node* root = nullptr;
    int choice;

    do {
        system("cls");
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            system("cls");
            int key;
            string data;
            cout << "Введите новый узел: ";
            cout << "\nКлюч: ";
            cin >> key;
            cout << "Данные: ";
            cin >> data;
            root = insertNode(root, key, data);
            break;
        }
        case 2: {
            system("cls");
            cout << "Вывод дерева:" << endl;
            printTree(root);
            break;
        }
        case 3: {
            system("cls");
            int key;
            cout << "Введите ключ узла для удаления: ";
            cin >> key;
            root = deleteNode(root, key);
            break;
        }
        case 4: {
            system("cls");
            int key;
            cout << "Введите ключ узла для поиска: ";
            cin >> key;
            Node* node = searchNode(root, key);
            if (node) {
                cout << "Найден узел с данными: " << node->data << endl;
            }
            else {
                cout << "Узел не найден." << endl;
            }
            break;
        }
        case 5: {
            system("cls");
            readFromFile(root, "derevo.txt");
            break;
        }
        case 6: {
            system("cls");
            writeToFile(root, "derevo.txt");
            cout << "Данные сохранены в файл 'derevo.txt'." << endl;
            break;
        }
        case 7: {
            system("cls");
            int count = countLeaves(root);
            if (count) {
                cout << "В дереве " << count << " листов." << endl;
            }
            else {
                cout << "Дерево пусто." << endl;
            }
            break;
        }
        case 0:
            cout << "Выход из программы." << endl;
            break;
        default:
            cout << "Неверный ввод. Попробуйте ещё раз." << endl;
        }
        system("pause");
        cout << endl;
    } while (choice != 0);

    freeTree(root);

    return 0;
}