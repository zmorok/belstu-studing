#include <Windows.h>
#include <iostream>
#include "../OS10_HTAPI/HT.h"

using namespace std;

int main()
{
    HT::HTHANDLE* ht1 = nullptr;
    HT::HTHANDLE* ht2 = nullptr;

    try
    {
        ht1 = HT::Create(1000, 3, 10, 256, "D:\\HT\\HT0301.ht");
        ht2 = HT::Create(1000, 3, 10, 256, "D:\\HT\\HT0302.ht");
        if (!ht1 || !ht2) throw "Create failed";

        HT::Insert(ht1, new HT::Element("ключ1", strlen("ключ1"), "значение", strlen("значение")));
        HT::Insert(ht1, new HT::Element("ключ2", strlen("ключ2"), "22значение", strlen("22значение")));
        HT::Insert(ht2, new HT::Element("key1", strlen("key1"), "data-1", strlen("data-1")));
        HT::Insert(ht2, new HT::Element("key2", strlen("key2"), "data-2", strlen("data-2")));

        cout << "-- Insert done\n";

        // выводим исходные элементы
        HT::Element* e1 = HT::Get(ht1, new HT::Element("ключ1", strlen("ключ1")));
        if (e1) { HT::print(e1); delete e1; }
        HT::Element* e2 = HT::Get(ht2, new HT::Element("key2", strlen("key2")));
        if (e2) { HT::print(e2); delete e2; }

        // меняем местами (ключ1 <-> key1, ключ2 <-> key2)
        HT::Element* f1 = HT::Get(ht1, new HT::Element("ключ1", strlen("ключ1")));
        if (f1) {
            HT::Insert(ht2, new HT::Element(f1->key, f1->keylength, f1->payload, f1->payloadlength));
            HT::Delete(ht1, new HT::Element("ключ1", strlen("ключ1")));
            delete f1;
        }
        HT::Element* f2 = HT::Get(ht1, new HT::Element("ключ2", strlen("ключ2")));
        if (f2) {
            HT::Insert(ht2, new HT::Element(f2->key, f2->keylength, f2->payload, f2->payloadlength));
            HT::Delete(ht1, new HT::Element("ключ2", strlen("ключ2")));
            delete f2;
        }
        HT::Element* f3 = HT::Get(ht2, new HT::Element("key1", strlen("key1")));
        if (f3) {
            HT::Insert(ht1, new HT::Element(f3->key, f3->keylength, f3->payload, f3->payloadlength));
            HT::Delete(ht2, new HT::Element("key1", strlen("key1")));
            delete f3;
        }
        HT::Element* f4 = HT::Get(ht2, new HT::Element("key2", strlen("key2")));
        if (f4) {
            HT::Insert(ht1, new HT::Element(f4->key, f4->keylength, f4->payload, f4->payloadlength));
            HT::Delete(ht2, new HT::Element("key2", strlen("key2")));
            delete f4;
        }

        cout << "-- Swap done\n";

        // вывод после обмена
        HT::Element* g1 = HT::Get(ht1, new HT::Element("key1", strlen("key1")));
        if (g1) { HT::print(g1); delete g1; }
        HT::Element* g2 = HT::Get(ht2, new HT::Element("ключ2", strlen("ключ2")));
        if (g2) { HT::print(g2); delete g2; }

        // удаление всех
        HT::Delete(ht1, new HT::Element("key1", strlen("key1")));
        HT::Delete(ht1, new HT::Element("key2", strlen("key2")));
        HT::Delete(ht2, new HT::Element("ключ1", strlen("ключ1")));
        HT::Delete(ht2, new HT::Element("ключ2", strlen("ключ2")));
        cout << "-- Delete done\n";

        HT::Close(ht1); ht1 = nullptr;
        HT::Close(ht2); ht2 = nullptr;
        cout << "-- Close done\n";
    }
    catch (const char* msg)
    {
        cout << "Error: " << msg << "\n";
        if (ht1) cout << HT::GetLastError(ht1) << "\n";
        if (ht2) cout << HT::GetLastError(ht2) << "\n";
        if (ht1) HT::Close(ht1);
        if (ht2) HT::Close(ht2);
        return 1;
    }
    return 0;
}
