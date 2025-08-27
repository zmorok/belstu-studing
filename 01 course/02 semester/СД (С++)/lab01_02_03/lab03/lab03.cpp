#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

// Создание и запись данных в файл
static void createAndWriteToFile() {
    string filename;
    cout << "Введите название файла: ";
    cin >> filename;

    ofstream file(filename);
    if (file.is_open()) {
        string data;
        cout << "Введите данные для записи в файл (для завершения введите 'end'): ";
        while (true) {
            cin >> data;
            if (data == "end")
                break;
            file << data << '\n';
        }
        file.close();
        cout << "Данные успешно записаны в файл." << endl;
    }
    else {
        cout << "Ошибка открытия файла." << endl;
    }
}

// Чтение файла
static void readFile(string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string data;
        cout << "Содержимое файла '" << filename << "':" << endl;
        while (file >> data) {
            cout << data << " ";
        }
        cout << endl;
        file.close();
    }
    else {
        cout << "Ошибка открытия файла." << endl;
    }
}

// Удаление содержимого файла
static void clearFile(string& filename) {
    ofstream file(filename, ofstream::out | ofstream::trunc);
    if (file.is_open()) {
        file.close();
        cout << "Содержимое файла '" << filename << "' успешно удалено." << endl;
    }
    else {
        cout << "Ошибка открытия файла." << endl;
    }
}

// Удаление файла
static void deleteFile(string& filename) {
    if (remove(filename.c_str()) != 0) {
        cout << "Ошибка удаления файла." << endl;
    }
    else {
        cout << "Файл '" << filename << "' успешно удален." << endl;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int choice;
    string filename;

    do {
        system("cls");
        cout << "Выберите действие:" << endl;
        cout << "1. Создание и запись данных в файл" << endl;
        cout << "2. Чтение файла" << endl;
        cout << "3. Удаление содержимого в файле" << endl;
        cout << "4. Удаление файла" << endl;
        cout << "5. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
            system("cls");
            createAndWriteToFile();
            system("pause");
            break;
        case 2:
            system("cls");
            cout << "Введите название файла для чтения: ";
            cin >> filename;
            readFile(filename);
            system("pause");
            break;
        case 3:
            system("cls");
            cout << "Введите название файла для чистки содержимого: ";
            cin >> filename;
            clearFile(filename);
            system("pause");
            break;
        case 4:
            system("cls");
            cout << "Введите название файла для удаления: ";
            cin >> filename;
            deleteFile(filename);
            system("pause");
            break;
        case 5:
            cout << "Выход из программы." << endl;
            break;
        default:
            system("cls");
            cout << "Некорректный выбор. Попробуйте снова." << endl;
            system("pause");
            break;
        }
    } while (choice != 5);

    return 0;
}