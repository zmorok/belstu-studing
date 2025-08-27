namespace Tecnologies
{
    public abstract partial class Technology : Product
    {
        public override string ToString()
        {
            return base.ToString() + $", Производитель: {Manufacturer}, Тип: {Type}, Дата: {Warranty.ExpirationDate}";
        }
    }
}
