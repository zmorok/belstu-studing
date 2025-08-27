#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// структура слова
struct HashItem {
    string key;
    HashItem* next;

    HashItem() : key(""), next(nullptr) {}
    HashItem(string& key) : key(key), next(nullptr) {}
};

// хеширование
static int hashFunction(string& key, int tableSize) {
    int hash = 0;
    for (char c : key) {
        hash = (hash * 31 + c) % tableSize;
    }
    return hash;
}

// добавление слова
static void insert(HashItem** table, string& key, int tableSize) {
    int index = hashFunction(key, tableSize);

    HashItem* newItem = new HashItem(key);
    if (table[index] == nullptr) {
        table[index] = newItem;
    }
    else {
        newItem->next = table[index];
        table[index] = newItem;
    }
}

// поиск слова
static int search(HashItem** table, string& key, int tableSize) {
    int index = hashFunction(key, tableSize);
    HashItem* currentItem = table[index];
    while (currentItem != nullptr) {
        if (currentItem->key == key) {
            int count = 0;
            HashItem* temp = currentItem;
            while (temp != nullptr) {
                count++;
                temp = temp->next;
            }
            return count;
        }
        currentItem = currentItem->next;
    }
    return 0;
}

// удаление слов, начинающихся с указанной буквы
static void removeWordsStartingWith(HashItem** table, char letter, int tableSize) {
    for (int i = 0; i < tableSize; i++) {
        HashItem* prevNode = nullptr;
        HashItem* currentNode = table[i];
        while (currentNode != nullptr) {
            if (currentNode->key[0] == letter) {
                // Если это первый элемент списка
                if (prevNode == nullptr) {
                    table[i] = currentNode->next;
                    delete currentNode;
                    currentNode = table[i];
                }
                else {
                    prevNode->next = currentNode->next;
                    delete currentNode;
                    currentNode = prevNode->next;
                }
            }
            else {
                prevNode = currentNode;
                currentNode = currentNode->next;
            }
        }
    }
}

// вывод хеш-таблицы
static void printHashTable(HashItem** table, int tableSize) {
    cout << "Хеш-таблица:\n\n";
    cout << "Индекс   | Ключи\n";
    cout << "----------------\n";
    for (int i = 0; i < tableSize; ++i) {
        cout << i << "\t | ";
        HashItem* currentItem = table[i];
        while (currentItem != nullptr) {
            cout << currentItem->key << " ";
            currentItem = currentItem->next;
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int tableSize;
    cout << "Введите размерность таблицы: ";
    cin >> tableSize;

    // массив указателей на элементы таблицы
    HashItem** hashTable = new HashItem * [tableSize]();

    ifstream file("file.txt");
    if (file.is_open()) {
        string word;
        while (file >> word) {
            insert(hashTable, word, tableSize);
        }
        file.close();
    }
    else {
        cout << "Не удалось открыть файл.\n";
        return 1;
    }

    printHashTable(hashTable, tableSize);

    string searchWord;
    cout << "Введите слово для поиска: ";
    cin >> searchWord;

    int comparisons = search(hashTable, searchWord, tableSize);
    if (comparisons) {
        cout << "Слово найдено. Количество сравнений: " << comparisons << endl;
    }
    else {
        cout << "Слово не найдено." << endl;
    }

    char letterToDelete;
    cout << "Введите букву для удаления слов, начинающихся с этой буквы: ";
    cin >> letterToDelete;

    removeWordsStartingWith(hashTable, letterToDelete, tableSize);

    printHashTable(hashTable, tableSize);

    for (int i = 0; i < tableSize; ++i) {
        HashItem* currentItem = hashTable[i];
        while (currentItem != nullptr) {
            HashItem* temp = currentItem;
            currentItem = currentItem->next;
            delete temp;
        }
    }
    delete[] hashTable;
    return 0;
}