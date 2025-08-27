using System;

namespace Tecnologies
{
    public sealed class PrintDevice : Technology, IProduct
    {
        public int PrintSpeed { get; set; }

        public PrintDevice(string name, double price, string manufacturer, int printSpeed, TechType type, Warranty warranty)
            : base(name, price, manufacturer, type, warranty)
        {
            PrintSpeed = printSpeed;
        }

        public override bool DoClone()
        {
            Console.WriteLine("из базового PrintDevice");
            return true;
        }

        bool IProduct.DoClone()
        {
            Console.WriteLine("из интерфейса PrintDevice");
            return true;
        }

        public override string ToString()
        {
            return base.ToString() + $", Скорость печать: {PrintSpeed} стр/м.";
        }
    }
}
