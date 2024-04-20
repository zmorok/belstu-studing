// main.cpp
#include "Heap.h"
#include <iostream>
using namespace std;

// ������� ��������� ��� ��������� ����
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
		cout << "���� 1:\n";
		h1.scan(0);
		cout << "\n���� 2:\n";
		h2.scan(0);
		cout << "\n\n\n-------------------------\n";
		cout << "1 - �������� ������� � ����1" << endl;
		cout << "2 - ������� ������������ ������� ����1" << endl;
		cout << "3 - ������� ����������� ������� ����1" << endl;
		cout << "4 - ������� i-� ������� ����1" << endl << "-------------------------" << endl;
		cout << "5 - �������� ������� � ����2" << endl;
		cout << "6 - ������� ������������ ������� ����2" << endl;
		cout << "7 - ������� ����������� ������� ����2" << endl;
		cout << "8 - ������� i-� ������� ����2" << endl << "-------------------------" << endl;;
		cout << "9 - ���������� ����" << endl;
		cout << "0 - �����" << endl;
		cout << "�������� �����: ";
		cin >> choice;
		switch (choice) {
			case 0:  
				exit(0);
			case 1: {
				AAA* a = new AAA;
				cout << "������� ����" << endl;
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
				cout << "������� ������ ��� ��������: ";
				cin >> i;
				h1.extractI(--i);
				break;
			}
			case 5: {
				AAA* a = new AAA;
				cout << "������� ����" << endl;
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
				cout << "������� ������ ��� ��������: ";
				cin >> i;
				h2.extractI(--i);
				break;
			}
			case 9: {
				h1.unionHeap(h2);
				break;
			}
			default:  
				cout << endl << "������� �������� �������!" << endl;
		}
		system("pause");
	} 
	return 0;
}
