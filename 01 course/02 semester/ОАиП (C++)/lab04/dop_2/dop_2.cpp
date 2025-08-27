#include <iostream>
#include <string>
#include <algorithm>
#include <Windows.h>

using namespace std;

struct Train {
    string destination;    // Название пункта назначения
    int trainNumber;       // Номер поезда
    string departureTime;  // Время отправления
};

static bool compareByDestination(Train& train1, Train& train2) {
    return train1.destination < train2.destination;
}

static Train inputTrainFromKeyboard() {
    Train train;
    cout << "Введите название пункта назначения: ";
    getline(cin, train.destination);
    cout << "Введите номер поезда: ";
    cin >> train.trainNumber;
    cin.ignore();
    cout << "Введите время отправления: ";
    getline(cin, train.departureTime);
    cout << '\n';
    return train;
}

static void printTrain(Train& train) {
    cout << "Пункт назначения: " << train.destination << endl;
    cout << "Номер поезда: " << train.trainNumber << endl;
    cout << "Время отправления: " << train.departureTime << endl << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int numTrains;
    cout << "Введите количество поездов: ";
    cin >> numTrains;

    Train* trains = new Train[numTrains];
    cin.ignore();
    cout << "\nВведите информацию о " << numTrains << " поездах:" << endl;
    for (int i = 0; i < numTrains; i++) {
        cout << "Поезд #" << i + 1 << ":" << endl;
        trains[i] = inputTrainFromKeyboard();
    }

    sort(trains, trains + numTrains, compareByDestination);

    cout << "Информация о поездах:" << endl;
    for (int i = 0; i < numTrains; i++) {
        printTrain(trains[i]);
    }

    string timeToCheck;
    cout << "\nВведите время, после которого нужно вывести информацию о поездах: ";
    getline(cin, timeToCheck);

    bool found = false;
    cout << "\nПоезда, отправляющиеся после " << timeToCheck << ":" << endl;
    for (int i = 0; i < numTrains; i++) {
        if (trains[i].departureTime > timeToCheck) {
            printTrain(trains[i]);
            found = true;
        }
    }
    if (!found) {
        cout << "\nНет поездов, отправляющихся после " << timeToCheck << "." << endl;
    }

    delete[] trains;
}