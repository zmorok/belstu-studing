namespace Tecnologies
{
    public abstract partial class Technology : Product
    {
        public Technology(string name, double price, string manufacturer, TechType type, Warranty warranty)
            : base(name, price)
        {
            Manufacturer = manufacturer;
            Type = type;
            Warranty = warranty;
        }
    }
}
