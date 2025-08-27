using System;

namespace lab04
{
    [Serializable]
    public abstract class Technology : Product
    {
        public string Manufacturer { get; set; }

        public Technology() 
        {
            Manufacturer = "DefManufacturer";
        }

        public Technology(string name, double price, string manufacturer)
            : base(name, price)
        {
            Manufacturer = manufacturer;
        }

        public override string ToString()
        {
            return base.ToString() + $", Производитель: {Manufacturer}";
        }
    }
}
