#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

const int MAX_EXAMS = 10;
const int MAX_STUDENTS = 100;

struct ExamResult {
    string fullName;       // ФИО студента
    int numExams;          // Число экзаменов
    int grades[MAX_EXAMS]; // Полученные оценки
};

static bool passedAllWithHighMarks(ExamResult& result) {
    for (int i = 0; i < result.numExams; i++) {
        if (result.grades[i] < 4) {
            return false;
        }
    }
    return true;
}


static float calculateSuccessRate(ExamResult results[], int numStudents) {
    int passedCount = 0;
    for (int i = 0; i < numStudents; i++) {
        if (passedAllWithHighMarks(results[i])) {
            passedCount++;
        }
    }
    return (static_cast<float>(passedCount) / numStudents) * 100;
}

static ExamResult inputExamResultFromKeyboard() {
    ExamResult result;
    cout << "Введите ФИО студента: ";
    getline(cin, result.fullName);
    cout << "Введите количество экзаменов: ";
    cin >> result.numExams;
    cout << "Введите оценки по каждому экзамену (через пробел): ";
    for (int i = 0; i < result.numExams; i++) {
        cin >> result.grades[i];
    }
    cin.ignore();
    return result;
}

static void printExamResult(ExamResult& result) {
    cout << "ФИО студента: " << result.fullName << endl;
    cout << "Количество экзаменов: " << result.numExams << endl;
    cout << "Оценки: ";
    for (int i = 0; i < result.numExams; i++) {
        cout << result.grades[i] << " ";
    }
    cout << endl << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int numStudents;
    cout << "Введите количество студентов: ";
    cin >> numStudents;
    cin.ignore(); 
    ExamResult results[MAX_STUDENTS];
    for (int i = 0; i < numStudents; i++) {
        cout << "\nВведите информацию о студенте #" << i + 1 << ":" << endl;
        results[i] = inputExamResultFromKeyboard();
    }
    cout << "\nИнформация о студентах:" << endl;
    for (int i = 0; i < numStudents; i++) {
        printExamResult(results[i]);
    }
    float successRate = calculateSuccessRate(results, numStudents);
    cout << "Успеваемость студентов (сданные экзамены на 4 и 5): " << successRate << "%" << endl;
}