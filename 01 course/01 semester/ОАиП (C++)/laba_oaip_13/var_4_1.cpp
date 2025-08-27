#include <iostream>
#include <Windows.h>
using namespace std;

int main() {
	setlocale(0, "");
	
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	const int size = 1000;

	char word[size];
	cout << "Введите вашу строку: ";
	cin.getline(word, size);
	char sym;
	cout << "Введите символ для удаления: "; cin >> sym;

	cout << "Ваша строка: " << word << endl;

	char* ro = word;
	char* wo = word;
	while (*ro) {
		if (*ro != sym) {
			*wo = *ro;
			++wo;
		}
		++ro;
	}
	*wo = '\0';
	cout << "Строка после удаления символа " << sym << " : " << word << endl;
}