#include <iostream>
#include <cmath>

using namespace std;

const int TABLE_SIZE = 16;

// элемент таблицы
struct HashTableItem {
    int key;
    int value;
    bool isFilled;

    HashTableItem() : key(0), value(0), isFilled(false) {}
};

// модульное хеширование
static int moduloHash(int key) {
    return key % TABLE_SIZE;
}

// мультипликативное хеширование
static int multiplicativeHash(int key) {
    return floor(TABLE_SIZE * fmod((key * ((sqrt(5) - 1) / 2)), 1.0));
}

// добавление элемента в таблицу
static void insertItem(HashTableItem* table, int key, int value, int (*hashFunction)(int), int& collisionCount) {
    int index = hashFunction(key);
    int attempts = 0;

    while (table[index].isFilled && attempts < TABLE_SIZE) {
        attempts++;
        collisionCount++;
        index = (index + 1) % TABLE_SIZE; // метод решения коллизии (линейное пробирование)
    }

    if (attempts >= TABLE_SIZE) {
        cout << "Хеш-таблица полностью заполнена. Невозможно добавить новый элемент." << endl;
        return;
    }

    table[index].key = key;
    table[index].value = value;
    table[index].isFilled = true;

    cout << "Элемент добавлен." << endl;
}

// поиск элемента по ключу
static int searchItems(HashTableItem* table, int key, int (*hashFunction)(int), int* values) {
    int index = hashFunction(key);
    int count = 0;

    while (table[index].isFilled) {
        if (table[index].key == key) {
            values[count++] = table[index].value;
        }
        index = (index + 1) % TABLE_SIZE;
    }

    return count;
}

// удаление элемента по ключу (сдвиг элемента, который вызвал коллизию)
static void removeItem(HashTableItem* table, int key, int (*hashFunction)(int), int& collisionCount) {
    int index = hashFunction(key);
    bool found = false;

    while (table[index].isFilled && !found) {
        if (table[index].key == key) {
            found = true;
            table[index].isFilled = false;
            table[index].key = 0;
            table[index].value = 0;
            int currentIndex = index;
            int nextIndex = (index + 1) % TABLE_SIZE;
            while (table[nextIndex].isFilled) {
                if (hashFunction(table[nextIndex].key) == index) {
                    table[currentIndex] = table[nextIndex];
                    table[nextIndex].isFilled = false;
                    table[nextIndex].key = 0;
                    table[nextIndex].value = 0;
                    currentIndex = nextIndex;
                    collisionCount--;
                }
                nextIndex = (nextIndex + 1) % TABLE_SIZE;
            }
        }
        index = (index + 1) % TABLE_SIZE;
    }

    if (!found) {
        cout << "Элемент с таким ключом не найден." << endl;
    }
    else {
        cout << "Элемент удален." << endl;
    }
}

// вывод хеш-таблицы
static void printHashTable(HashTableItem* table) {
    cout << "Хеш-таблица:" << endl;
    cout << "Индекс   | Ключ  | Значение | Заполнено" << endl;
    cout << "--------------------------------------" << endl;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        cout << i << "\t | ";
        if (table[i].isFilled) {
            cout << table[i].key << "\t | " << table[i].value << "\t    | Да" << endl;
        }
        else {
            cout << "-\t | -\t    | Нет" << endl;
        }
    }
    cout << "--------------------------------------" << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");

    HashTableItem hashTable[TABLE_SIZE];

    int choice;
    int key, value;
    int moduloCollisions = 0, multiplicativeCollisions = 0;

    do {
        system("cls");
        cout << "Количество коллизий (модульное хеширование): " << moduloCollisions << endl;
        cout << "Количество коллизий (мультипликативное хеширование): " << multiplicativeCollisions << endl << endl;
        printHashTable(hashTable);
        cout << "\nМеню:\n";
        cout << "1. Добавить элемент  (модульное хеширование)\n";
        cout << "2. Найти элемент     (модульное хеширование)\n";
        cout << "3. Удалить элемент   (модульное хеширование)\n";
        cout << "----------------------------------------------\n";
        cout << "4. Добавить элемент  (мультипликативное хеширование)\n";
        cout << "5. Найти элемент     (мультипликативное хеширование)\n";
        cout << "6. Удалить элемент   (мультипликативное хеширование)\n";
        cout << "----------------------------------------------\n";
        cout << "0. Выйти\n\n";
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Введите ключ и значение для добавления (через пробел): ";
            cin >> key >> value;
            insertItem(hashTable, key, value, moduloHash, moduloCollisions);
            break;
        case 2:
            cout << "Введите ключ для поиска: ";
            cin >> key;
            {
                int values[TABLE_SIZE];
                int count = searchItems(hashTable, key, moduloHash, values);
                if (count > 0) {
                    cout << "Найденные значения: ";
                    for (int i = 0; i < count; ++i) {
                        cout << values[i] << " ";
                    }
                    cout << endl;
                }
                else {
                    cout << "Элемент с таким ключом не найден." << endl;
                }
            }
            break;
        case 3:
            cout << "Введите ключ для удаления: ";
            cin >> key;
            removeItem(hashTable, key, moduloHash, moduloCollisions);
            break;
        case 4:
            cout << "Введите ключ и значение для добавления (через пробел): ";
            cin >> key >> value;
            insertItem(hashTable, key, value, multiplicativeHash, multiplicativeCollisions);
            break;
        case 5:
            cout << "Введите ключ для поиска: ";
            cin >> key;
            {
                int values[TABLE_SIZE];
                int count = searchItems(hashTable, key, multiplicativeHash, values);
                if (count > 0) {
                    cout << "Найденные значения: ";
                    for (int i = 0; i < count; ++i) {
                        cout << values[i] << " ";
                    }
                    cout << endl;
                }
                else {
                    cout << "Элемент с таким ключом не найден." << endl;
                }
            }
            break;
        case 6:
            cout << "Введите ключ для удаления: ";
            cin >> key;
            removeItem(hashTable, key, multiplicativeHash, multiplicativeCollisions);
            break;
        case 0:
            cout << "Программа завершена." << endl;
            break;
        default:
            cout << "Некорректный выбор. Попробуйте еще раз." << endl;
        }
        system("pause");
    } while (choice != 0);

    return 0;
}