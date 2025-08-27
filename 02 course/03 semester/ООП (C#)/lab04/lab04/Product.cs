namespace lab04
{
    internal abstract class Product
    {
        public string Name { get; set; }
        public double Price { get; set; }

        public Product(string name, double price)
        {
            Name = name;
            Price = price;
        }

        public abstract bool DoClone();

        public override string ToString()
        {
            return $"\tТип: {GetType().Name}, Название: {Name}\n\tЦена: {Price}";
        }

        public override bool Equals(object obj)
        {
            if (obj == null || this.GetType() != obj.GetType())
                return false;

            Product other = (Product)obj;
            return this.Name == other.Name && this.Price == other.Price;
        }

        public override int GetHashCode() => Name.GetHashCode() + Price.GetHashCode();
    }
}
