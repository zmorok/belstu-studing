using System;

namespace lab04
{
    internal class Printer
    {
        public void IAmPrinting(Product obj)
        {
            Console.WriteLine(obj.ToString());
        }
    }
}
