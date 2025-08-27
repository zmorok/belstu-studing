using System;

namespace Tecnologies
{
    public class Computer : Technology, IProduct
    {
        public string CPU { get; set; }
        public int RAM { get; set; }

        public Computer(string name, double price, string manufacturer, string cpu, int ram, TechType type, Warranty warranty)
            : base(name, price, manufacturer, type, warranty)
        {
            CPU = cpu;
            RAM = ram;
        }

        public override bool DoClone()
        {
            Console.WriteLine("из базового Computer");
            return true;
        }

        bool IProduct.DoClone()
        {
            Console.WriteLine("из интерфейса Computer");
            return true;
        }

        public override string ToString()
        {
            return base.ToString() + $", ЦПУ: {CPU}, ОЗУ: {RAM} ГБ.";
        }
    }
}
