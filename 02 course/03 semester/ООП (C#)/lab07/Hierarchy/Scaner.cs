using System;

namespace lab04
{
    public class Scaner : Technology, IProduct
    {
        public int Resolution { get; set; }

        public Scaner(string name, double price, string manufacturer, int resolution)
            : base(name, price, manufacturer)
        {
            Resolution = resolution;
        }

        public override bool DoClone()
        {
            Console.WriteLine("из базового Scaner");
            return true;
        }

        bool IProduct.DoClone()
        {
            Console.WriteLine("из интерфейса Scaner");
            return true;
        }

        public override string ToString()
        {
            return base.ToString() + $", Расширение: {Resolution} дпи.";
        }
    }
}
