using System;

namespace task03
{
    public class Postroyka
    {
        private readonly string material;
        public string Address { get; set; }

        public Postroyka(string address, string material)
        {
            Address = address;
            this.material = material;
        }

        public void ShowDetails()
        {
            Console.WriteLine($"\tМетод из Postroyka\nАдрес: {Address} | {ShowMaterial()}");
        }

        protected string ShowMaterial() => $"Материал: {material}";
    }

    public class Dom : Postroyka
    {   
        private int numberOfRooms;

        public Dom(string address, string material, int numberOfRooms)
            : base(address, material)
        {
            this.numberOfRooms = numberOfRooms;
        }

        public int NumberOfRooms
        {
            get { return numberOfRooms; }
            set { numberOfRooms = value; }
        }

        public void ShowHouseDetails()
        {
            ShowDetails();
            Console.WriteLine($"\tМетод из Dom\nКоличество комнат: {numberOfRooms}");
            ShowMaterial();
        }

        protected new void ShowMaterial()
        {
            Console.WriteLine("\tМетод из Dom\nOverridden material display in Dom.");
        }
    }

    class Program
    {
        static void Main()
        {
            Dom myHouse = new Dom("ул. Майская 123", "Кирпич", 4);
            myHouse.ShowHouseDetails();
        }
    }
}
