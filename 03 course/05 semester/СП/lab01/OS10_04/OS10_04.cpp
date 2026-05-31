#include <Windows.h>
#include <iostream>
#include <string>
#include "../OS10_HTAPI/HT.h"

using namespace std;

int main() {
    HT::HTHANDLE* ht1 = HT::Create(1280, 2, 64, 256, "D:\\HT\\ht_a_041.ht");
    HT::HTHANDLE* ht2 = HT::Create(1280, 2, 64, 256, "D:\\HT\\ht_b_041.ht");
    if (!ht1 || !ht2) {
        if (ht1) cout << HT::GetLastError(ht1) << "\n";
        if (ht2) cout << HT::GetLastError(ht2) << "\n";
        return 1;
    }

    // Insert
    HT::Insert(ht1, new HT::Element("ключ1", strlen("ключ1"), "значение", strlen("значение")));
    HT::Insert(ht1, new HT::Element("ключ2", strlen("ключ2"), "22значение", strlen("22значение")));
    HT::Insert(ht2, new HT::Element("key1", strlen("key1"), "data-1", strlen("data-1")));
    HT::Insert(ht2, new HT::Element("key2", strlen("key2"), "data-2", strlen("data-2")));

    // Get + print
    HT::Element* e;
    e = HT::Get(ht1, new HT::Element("ключ1", strlen("ключ1"))); if (e) { HT::print(e); delete e; }
    e = HT::Get(ht2, new HT::Element("key1", strlen("key1"))); if (e) { HT::print(e); delete e; }

    // Update
    {
        HT::Update(ht2, new HT::Element("key1", strlen("key1")), "data-NEW", strlen("data-NEW"));
        e = HT::Get(ht2, new HT::Element("key1", strlen("key1"))); if (e) { HT::print(e); delete e; }
    }

    // ht1[ключ1] -> ht2
    {
        HT::Element* src = HT::Get(ht1, new HT::Element("ключ1", strlen("ключ1")));
        if (src) {
            HT::Insert(ht2, new HT::Element(src->key, src->keylength, src->payload, src->payloadlength));
            HT::Delete(ht1, new HT::Element("ключ1", strlen("ключ1")));
            delete src;
        }
    }

    e = HT::Get(ht1, new HT::Element("ключ1", strlen("ключ1"))); if (e) { HT::print(e); delete e; }
    e = HT::Get(ht2, new HT::Element("ключ1", strlen("ключ1"))); if (e) { HT::print(e); delete e; }

    // Snapshot, Close
    HT::Snap(ht1); HT::Snap(ht2);
    HT::Close(ht1); HT::Close(ht2);
    ht1 = nullptr; ht2 = nullptr;

    // Open
    ht1 = HT::Open("D:\\HT\\ht_a.ht");
    ht2 = HT::Open("D:\\HT\\ht_b.ht");
    if (!ht1 || !ht2) {
        if (ht1) cout << HT::GetLastError(ht1) << "\n";
        if (ht2) cout << HT::GetLastError(ht2) << "\n";
        return 1;
    }
    e = HT::Get(ht2, new HT::Element("ключ1", strlen("ключ1"))); if (e) { HT::print(e); delete e; }

    // Delete
    HT::Delete(ht1, new HT::Element("ключ2", strlen("ключ2")));
    HT::Delete(ht2, new HT::Element("key2", strlen("key2")));

    // Snap + Close
    HT::Snap(ht1); HT::Snap(ht2);
    HT::Close(ht1); HT::Close(ht2);

    return 0;
}
