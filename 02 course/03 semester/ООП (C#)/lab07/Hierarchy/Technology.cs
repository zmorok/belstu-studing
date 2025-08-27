using System;

namespace lab04
{
    public abstract class Technology : Product, IComparable<Technology>
    {
        public string Manufacturer { get; set; }

        public Technology(string name, double price, string manufacturer)
            : base(name, price)
        {
            Manufacturer = manufacturer;
        }

        public override string ToString()
        {
            return base.ToString() + $", Производитель: {Manufacturer}";
        }

        public int CompareTo(Technology other)
        {
            if (other == null) return 1;
            return this.Name.CompareTo(other.Name);
        }
    }
}
