#include <iostream>
using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	int n, i;
	cout << "Введите кол-во дней, когда проводились измерения: ";
	cin >> n;

	int* mas = new int[n];

	for (i = 0; i < n; i++) {
		cout << "Введите данные за " << i + 1 << "-ый день: ";
		cin >> mas[i];
	}

	cout << endl;
	cout << "Измерительные данные за " << n << " дней: " << endl;
	for (i = 0; i < n; i++) {
		cout << i + 1 << " день. " << mas[i] << endl;
	}

	/*for (i = 0; i < n; i++) {
		if (i == 0 || i % 6 != 0) {
			continue;
		}
		else {
			cout << "Семёрка. " << i+1 << " " << mas[i] << endl;
		}
	}*/

	int maxWeekRain = 0;
	int maxWeek = 0;
	int weekRain = 0;

	for (int week = 0; week <= n - 7; week += 6) {
		weekRain = 0;
		for (i = week; i < week + 7; i++) {
			weekRain += mas[i];
		}
		if (maxWeekRain < weekRain) {
			maxWeekRain = weekRain;
			maxWeek = (week / 7) + 1;
			
		}
	}
	cout << "Макс. кол-во осадков выпало на " << maxWeek << "-ой неделе." << endl;
}