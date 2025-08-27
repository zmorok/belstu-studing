#include <iostream>
using namespace std;

static int countOccurrencesInText(const char* text, char& searchChar) {
    int count = 0;

    while (*text) {
        if (*text == searchChar) {
            count++;
        }
        text++;
    }

    return count;
}

int main() {
    setlocale(0, "");

    int n;
    const int maxInputLength = 1000;
    char inputText[maxInputLength];
    char searchChar;


    cout << "Введите количество строк (n): ";
    cin >> n;
    cin.ignore();


    cout << "Введите строки текста (по одной строке в каждой строке):" << endl;
    
    int sum = 0;

    for (int i = 0; i < n; ++i) {
        if (i == 0) {
            cout << "Введите символ для поиска: ";
            cin >> searchChar;
            cin.ignore();
        }
        
        cout << "Строка " << i + 1 << ": ";
        cin.getline(inputText, maxInputLength);

        int rem = countOccurrencesInText(inputText, searchChar);
        sum += rem;
    }
    cout << "Всего в " << n << " строках символ '" << searchChar << "' встречается " << sum << " раз.\n";
}