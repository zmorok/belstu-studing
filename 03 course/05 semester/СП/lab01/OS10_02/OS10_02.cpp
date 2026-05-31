#include <Windows.h>
#include <iostream>
#include "../OS10_HTAPI/HT.h"

using namespace std;

int main()
{
    HT::HTHANDLE* ht = nullptr;
    try
    {
        ht = HT::Create(1000, 3, 10, 256, "D:\\HT\\NITspace02.ht"); // создать HT-хранилище
        if (ht) cout << "-- Create: success";
        else throw "-- Create: error";

        if (HT::Insert(ht, new HT::Element("key222", 7, "payload", 8))) cout << "-- Insert: success";
        else throw "-- Insert: error";

        HT::Element* hte = HT::Get(ht, new HT::Element("key222", 7));
        if (hte) cout << "-- Get: success";
        else throw "-- Get: error";

        HT::print(hte);

        if (HT::Snap(ht)) cout << "-- Snap: success";
        else throw "-- Snap: error";

        if (HT::Update(ht, hte, "newpayload", 11)) cout << "-- Update: success";
        else throw "-- Update: error";

        HT::Element* htel = HT::Get(ht, new HT::Element("key222", 7));
        if (htel) cout << "-- Get: success";
        else throw "-- Get: error";

        HT::print(htel);
        if (HT::Delete(ht, htel)) cout << "Delete: success";
        else throw "-- Delete: error";

        if (HT::Close(ht)) cout << "Close: success";
        else throw "-- Close error";
    }
    catch (char* msg)
    {
        cout << msg << "\n";
        if (ht != nullptr) cout << HT::GetLastError(ht);
    }
}