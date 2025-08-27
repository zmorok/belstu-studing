// main.cpp
#include "Heap.h"
#include <iostream>
using namespace std;

// функция сравнения для элементов кучи
static heap::CMP cmpAAA(void* a1, void* a2) {
	AAA* A1 = static_cast<AAA*>(a1);
	AAA* A2 = static_cast<AAA*>(a2);

	if (A1->x > A2->x) {
		return heap::GREAT;
	}
	else if (A1->x < A2->x) {
		return heap::LESS;
	}
	else {
		return heap::EQUAL;
	}
}


int main() {
	setlocale(LC_ALL, "rus");
	int k, choice;
	heap::Heap h1 = heap::create(20, cmpAAA);
	heap::Heap h2 = heap::create(10, cmpAAA);
	for (;;) {
		system("cls");
		cout << "куча 1:\n";
		h1.scan(0);
		cout << "\nкуча 2:\n";
		h2.scan(0);
		cout << "\n\n\n-------------------------\n";
		cout << "1 - добавить элемент в кучу1" << endl;
		cout << "2 - удалить максимальный элемент кучи1" << endl;
		cout << "3 - удалить минимальный элемент кучи1" << endl;
		cout << "4 - удалить i-й элемент кучи1" << endl << "-------------------------" << endl;
		cout << "5 - добавить элемент в кучу2" << endl;
		cout << "6 - удалить максимальный элемент кучи2" << endl;
		cout << "7 - удалить минимальный элемент кучи2" << endl;
		cout << "8 - удалить i-й элемент кучи2" << endl << "-------------------------" << endl;;
		cout << "9 - объединить кучи" << endl;
		cout << "0 - выход" << endl;
		cout << "сделайте выбор: ";
		cin >> choice;
		switch (choice) {
			case 0:  
				exit(0);
			case 1: {
				AAA* a = new AAA;
				cout << "введите ключ" << endl;
				cin >> k;
				a->x = k;
				h1.insert(a);
			}
			break;
			case 2:   
				h1.extractMax();
				break;
			case 3:   
				h1.extractMin();
				break;
			case 4: {
				int i;
				cout << "введите индекс для удаления: ";
				cin >> i;
				h1.extractI(--i);
				break;
			}
			case 5: {
				AAA* a = new AAA;
				cout << "введите ключ" << endl;
				cin >> k;
				a->x = k;
				h2.insert(a);
			}
			break;
			case 6:   
				h2.extractMax();
				break;
			case 7:   
				h2.extractMin();
				break;
			case 8: {
				int i;
				cout << "введите индекс для удаления: ";
				cin >> i;
				h2.extractI(--i);
				break;
			}
			case 9: {
				h1.unionHeap(h2);
				break;
			}
			default:  
				cout << endl << "введена неверная команда!" << endl;
		}
		system("pause");
	} 
	return 0;
}
