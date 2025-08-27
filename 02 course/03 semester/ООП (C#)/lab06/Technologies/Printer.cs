using System;

namespace Tecnologies
{
    public class Printer
    {
        public void IAmPrinting(Product obj)
        {
            Console.WriteLine(obj.ToString());
        }
    }
}
