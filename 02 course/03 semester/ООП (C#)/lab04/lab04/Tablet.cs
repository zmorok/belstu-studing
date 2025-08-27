using System;

namespace lab04
{
    internal class Tablet : Technology, IProduct
    {
        public double ScreenSize { get; set; }

        public Tablet(string name, double price, string manufacturer, double screenSize)
            : base(name, price, manufacturer)
        {
            ScreenSize = screenSize;
        }

        public override bool DoClone()
        {
            Console.WriteLine("из базового Tablet");
            return true;
        }

        bool IProduct.DoClone()
        {
            Console.WriteLine("из интерфейса Tablet");
            return true;
        }

        public override string ToString()
        {
            return base.ToString() + $", Разрешение экрана: {ScreenSize} дюймов.";
        }
    }
}
