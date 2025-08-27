namespace Tecnologies
{
    public abstract partial class Technology : Product
    {
        public TechType Type { get; set; }
        public Warranty Warranty { get; set; }
        public string Manufacturer { get; set; }
    }
}
