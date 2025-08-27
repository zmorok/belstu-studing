namespace lab04
{
    internal abstract class Technology : Product
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
    }
}
