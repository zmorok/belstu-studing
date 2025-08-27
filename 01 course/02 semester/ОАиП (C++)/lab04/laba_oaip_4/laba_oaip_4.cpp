#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

const int MAX_BOOKS = 100;

struct Book {
    string author;
    string title;
    string publisher;
    string section;
    string origin;
    bool available = true;
};

Book library[MAX_BOOKS];
int numBooks = 0;

static void addBook() {
    system("cls");
    cout << "\n\t\tДобавление книги...\n\n";
    if (numBooks < MAX_BOOKS) {
        Book newBook;
        cout << "\tВведите автора книги: ";
        cin.ignore();
        getline(cin, newBook.author);
        cout << "\tВведите название книги: ";
        getline(cin, newBook.title);
        cout << "\tВведите издателя: ";
        getline(cin, newBook.publisher);
        cout << "\tВведите жанр: ";
        getline(cin, newBook.section);
        cout << "\tВведите источник (покупка/кража/подарок): ";
        getline(cin, newBook.origin);
        cout << "\tВведите наличие (1 - Да, 0 - нет): ";
        cin >> newBook.available;
        library[numBooks++] = newBook;
        cout << "\n\tКнига добавлена успешно!\n\n";
    }
    else {
        cout << "Библиотека переполнена. Невозможно добавить книгу.\n";
    }
}

static void displayBooks() {
    system("cls");
    cout << "\n\tCписок книг библиотеки:\n\n";
    for (int i = 0; i < numBooks; i++) {
        cout << i + 1 << ")\tАвтор: " << library[i].author << "\n\tНазвание: " << library[i].title << "\n\tИздатель: " << library[i].publisher << "\n\tЖанр: " << library[i].section << "\n\tИсточник: " << library[i].origin << "\n\tНаличие: " << (library[i].available ? "Да" : "Нет") << "\n\n";
    }
    cout << '\n';
}

static void deleteBook() {
    system("cls");
    displayBooks();
    string titleToDelete;
    cout << "\tВведите название книги для удаления: ";
    cin.ignore();
    getline(cin, titleToDelete);
    int foundIndex = -1;
    for (int i = 0; i < numBooks; i++) {
        if (library[i].title == titleToDelete) {
            foundIndex = i;
            break;
        }
    }
    if (foundIndex != -1) {
        for (int i = foundIndex; i < numBooks - 1; i++) {
            library[i] = library[i + 1];
        }
        numBooks--;
        cout << "\n\tКнига удалена успешно. Перезапишите данные (№5).\n";
    }
    else {
        cout << "\n\tКнига не найдена.\n";
    }
}

static void searchByAuthor() {
    system("cls");
    cout << "\n\tВведите автора: ";
    string authorToSearch;
    cin.ignore();
    getline(cin, authorToSearch);
    cout << "\n\tКниги по автору [" << authorToSearch << "]:\n";
    bool found = false;
    for (int i = 0; i < numBooks; ++i) {
        if (library[i].author == authorToSearch) {
            cout << i + 1 << ")" << "\tНазвание: " << library[i].title << "\n\tЖанр: " << library[i].section << "\n\tНаличие: " << (library[i].available ? "Да" : "Нет") << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "\n\tНе найдено книги по автору [" << authorToSearch << "]\n\n";
    }
}

static void writeToFile() {
    system("cls");
    ofstream file("library.txt");
    if (file.is_open()) {
        for (int i = 0; i < numBooks; i++) {
            file << library[i].author << "," << library[i].title << "," << library[i].publisher << "," << library[i].section << "," << library[i].origin << "," << library[i].available << endl;
        }
        file.close();
        cout << "\n\tДанные библиотеки сохранены в файл успешно.\n\n";
    }
    else {
        cout << "\n\tНе удалось открыть файл.\n\n";
    }
}

static void readFromFile() {
    system("cls");
    ifstream file("library.txt");
    if (file.is_open()) {
        numBooks = 0;
        string line;
        while (getline(file, line)) {
            string sym = ",";
            size_t pos = 0;
            Book book;
            // Автор
            pos = line.find(sym);
            book.author = line.substr(0, pos);
            line.erase(0, pos + sym.length());
            // Название
            pos = line.find(sym);
            book.title = line.substr(0, pos);
            line.erase(0, pos + sym.length());
            // Издатель
            pos = line.find(sym);
            book.publisher = line.substr(0, pos);
            line.erase(0, pos + sym.length());
            // Раздел
            pos = line.find(sym);
            book.section = line.substr(0, pos);
            line.erase(0, pos + sym.length());
            // Источник
            pos = line.find(sym);
            book.origin = line.substr(0, pos);
            line.erase(0, pos + sym.length());
            // Наличие
            book.available = (line == "1");
            library[numBooks++] = book;
        }
        file.close();
        cout << "\n\tДанные библиотеки успешно прочитаны из файла.\n\n";
    }
    else {
        cout << "\n\tНе удалось открыть файл.\n\n";
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int choice;
    do {
        system("cls");
        cout << "\n\t\tСистема управления библиотекой\n\n";
        cout << "\t\t1. Добавить книгу\n";
        cout << "\t\t2. Отобразить книги\n";
        cout << "\t\t3. Удалить книгу\n";
        cout << "\t\t4. Поиск по автору\n";
        cout << "\t\t5. Записать в файл\n";
        cout << "\t\t6. Прочитать с файла\n";
        cout << "\t\t7. Выход\n";
        cout << "\n\t\tВведите номер операции: ";
        cin >> choice;
        switch (choice) {
        case 1:
            addBook();
            break;
        case 2:
            displayBooks();
            break;
        case 3:
            deleteBook();
            break;
        case 4:
            searchByAuthor();
            break;
        case 5:
            writeToFile();
            break;
        case 6:
            readFromFile();
            break;
        case 7:
            cout << "\n\t\tВыход из программы.\n\n";
            Sleep(500);
            return 0;
        default:
            cout << "\t\tНеверная операция. Попробуйте другую операцию.\n\n";
        }
        system("pause");
    } while (choice != 7); 
}