using System;

namespace Tecnologies
{
    public abstract partial class Technology : Product, IComparable<Technology>
    {
        public override string ToString()
        {
            return base.ToString()
                + $", Производитель: {Manufacturer}, Тип: {Type}, Дата: {Warranty.ExpirationDate}";
        }

        public int CompareTo(Technology other)
        {
            if (other == null)
                return 1;
            // return Price.CompareTo(other.Price); // asc
            return -Price.CompareTo(other.Price); // desc
        }
    }
}
