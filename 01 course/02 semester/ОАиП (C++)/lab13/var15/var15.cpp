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

// двойное хеширование
static int doubleHashingHash(int key, int i) {
    int h1 = (key % TABLE_SIZE);
    int h2 = 1 + (key % (TABLE_SIZE - 1));
    return (h1 + i * h2) % TABLE_SIZE;
}

// добавление элемента в таблицу
static void insertItem(HashTableItem* table, int key, int value, int& collisionCount) {
    int index = moduloHash(key);
    int i = 1;

    while (table[index].isFilled && i <= TABLE_SIZE) {
        index = doubleHashingHash(key, i);
        i++;
        collisionCount++;
    }

    if (i > TABLE_SIZE) {
        cout << "Хеш-таблица полностью заполнена. Невозможно добавить новый элемент." << endl;
        return;
    }

    table[index].key = key;
    table[index].value = value;
    table[index].isFilled = true;

    cout << "Элемент добавлен." << endl;
}

// поиск элемента по ключу с использованием двойного хеширования
static int searchItemsDoubleHashing(HashTableItem* table, int key, int (*hashFunction)(int, int), int* values) {
    int index = hashFunction(key, 0); // начинаем с i = 0
    int count = 0;
    int i = 0; // начальное значение i

    while (table[index].isFilled && i <= TABLE_SIZE) {
        if (table[index].key == key) {
            values[count++] = table[index].value;
        }
        i++;
        index = hashFunction(key, i);
    }

    return count;
}

// удаление элемента по ключу с использованием двойного хеширования
static void removeItemDoubleHashing(HashTableItem* table, int key, int (*hashFunction)(int, int), int& collisionCount) {
    int index = hashFunction(key, 0); // начинаем с i = 0
    bool found = false;
    int i = 0; // начальное значение i

    while (table[index].isFilled && !found && i <= TABLE_SIZE) {
        if (table[index].key == key) {
            found = true;
            table[index].isFilled = false;
            table[index].key = 0;
            table[index].value = 0;
            int currentIndex = index;
            int nextIndex = hashFunction(key, i + 1);
            i = 1; // начинаем со следующего значения i
            while (table[nextIndex].isFilled) {
                if (doubleHashingHash(table[nextIndex].key, i) == index) {
                    table[currentIndex] = table[nextIndex];
                    table[nextIndex].isFilled = false;
                    table[nextIndex].key = 0;
                    table[nextIndex].value = 0;
                    currentIndex = nextIndex;
                    collisionCount--;
                }
                i++;
                nextIndex = hashFunction(key, i);
            }
        }
        i++;
        index = hashFunction(key, i);
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
    int collisionCount = 0;

    do {
        system("cls");
        cout << "Количество коллизий: " << collisionCount << endl;
        printHashTable(hashTable);
        cout << "\nМеню:\n";
        cout << "1. Добавить элемент  (двойное хеширование)\n";
        cout << "2. Найти элемент     (двойное хеширование)\n";
        cout << "3. Удалить элемент   (двойное хеширование)\n";
        cout << "----------------------------------------------\n";
        cout << "0. Выйти\n\n";
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Введите ключ и значение для добавления (через пробел): ";
            cin >> key >> value;
            insertItem(hashTable, key, value, collisionCount);
            break;
        case 2:
            cout << "Введите ключ для поиска: ";
            cin >> key;
            {
                int values[TABLE_SIZE];
                int count = searchItemsDoubleHashing(hashTable, key, doubleHashingHash, values);
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
            removeItemDoubleHashing(hashTable, key, doubleHashingHash, collisionCount);
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