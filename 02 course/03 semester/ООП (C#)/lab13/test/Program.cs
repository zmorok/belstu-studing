using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml;
using System.Xml.Linq;
using System.Xml.XPath;
using lab04;
using SerializeLib;

namespace test
{
    public class TabletCollection
    {
        public List<Tablet> Tablets { get; set; }

        public TabletCollection()
        {
            Tablets = new List<Tablet>();
        }
    }

    internal class Program
    {
        static void Main(string[] args)
        {
            string path = Path.Combine(Path.GetFullPath(Directory.GetCurrentDirectory()), @"TEST");
            try
            {
                if (!Directory.Exists(path)) { Directory.CreateDirectory(path); }
            }
            finally
            {
                Directory.SetCurrentDirectory(path);
            }
            bool exit = false;
            while (!exit)
            {
                Console.Clear();
                Console.WriteLine("Меню:");
                Console.WriteLine("1. Сериализация/Десериализация объекта в/из файла");
                Console.WriteLine("2. Сериализация/Десериализация коллекции объектов в/из файла");
                Console.WriteLine("3. XPath запросы");
                Console.WriteLine("4. LINQ запросы на XML/JSON");
                Console.WriteLine("0. Выход");
                Console.Write("Выберите действие: ");
                string choice = Console.ReadLine();
                switch (choice)
                {
                    case "1":
                        PerformSerializationDeserialization();
                        break;
                    case "2":
                        PerformCollectionSerializationDeserialization();
                        break;
                    case "3":
                        PerformXPathQueries();
                        break;
                    case "4":
                        PerformLinqQueries();
                        break;
                    case "0":
                        exit = true;
                        break;
                    default:
                        Console.WriteLine("Некорректный выбор. Попробуйте снова.");
                        break;
                }
                if (!exit)
                {
                    Console.WriteLine("Нажмите любую клавишу для продолжения...");
                    Console.ReadKey();
                }
            }
        }

        static void PerformSerializationDeserialization()
        {
            Tablet tablet = new Tablet("iPad Pro", 999.99, "Apple", 12.9);
            ISerializer serializerJson = new JsonDataSerializer();
            ISerializer serializerBinary = new BinaryDataSerializer();
            ISerializer serializerSoap = new SoapDataSerializer();
            ISerializer serializerXml = new XmlDataSerializer();

            string fileNameJson = "tablet.json";
            string fileNameBinary = "tablet.bin";
            string fileNameSoap = "tablet.soap";
            string fileNameXml = "tablet.xml";

            serializerJson.Serialize(tablet, fileNameJson);
            serializerBinary.Serialize(tablet, fileNameBinary);
            serializerSoap.Serialize(tablet, fileNameSoap);
            serializerXml.Serialize(tablet, fileNameXml);

            Console.WriteLine("Объект сериализован.");

            Tablet deserializedTabletJson = serializerJson.Deserialize<Tablet>(fileNameJson);
            Tablet deserializedTabletBinary = serializerBinary.Deserialize<Tablet>(fileNameBinary);
            Tablet deserializedTabletSoap = serializerSoap.Deserialize<Tablet>(fileNameSoap);
            Tablet deserializedTabletXml = serializerXml.Deserialize<Tablet>(fileNameXml);

            Console.WriteLine("Объект десериализован: ");
            Console.WriteLine("Json: " + deserializedTabletJson.ToString());
            Console.WriteLine("Binary: " + deserializedTabletBinary.ToString());
            Console.WriteLine("Soap: " + deserializedTabletSoap.ToString());
            Console.WriteLine("Xml: " + deserializedTabletXml.ToString());
        }

        static void PerformCollectionSerializationDeserialization()
        {
            var collection = new TabletCollection();
            collection.Tablets.Add(new Tablet("iPad Pro", 999.99, "Apple", 12.9));
            collection.Tablets.Add(new Tablet("Galaxy Tab S7", 649.99, "Samsung", 11));

            ISerializer serializerJson = new JsonDataSerializer();
            
            string fileNameJson = "tablets.json";
            

            serializerJson.Serialize(collection, fileNameJson);
            Console.WriteLine("Коллекция сериализована.");

            TabletCollection deserializedCollection = serializerJson.Deserialize<TabletCollection>(fileNameJson);
            Console.WriteLine("Коллекция десериализована:");
            foreach (var item in deserializedCollection.Tablets)
            {
                Console.WriteLine(item);
            }
        }

        static void PerformXPathQueries()
        {
            string xmlFile = "tablets.xml";
            XmlDocument doc = new XmlDocument();
            doc.Load(xmlFile);
            XPathNavigator navigator = doc.CreateNavigator();
            Console.WriteLine("XPath запросы:");

            // все элементы Tablet
            Console.Write(string.Join("", Enumerable.Repeat('=', 20)));
            Console.WriteLine("\nВсе элементы Tablet:");
            XPathNodeIterator nodes = navigator.Select("//Tablet");
            while (nodes.MoveNext())
            {
                Console.WriteLine(nodes.Current.OuterXml);
            }
            Console.WriteLine(string.Join("", Enumerable.Repeat('=', 20)));

            // элементы с производителем Apple
            Console.Write(string.Join("", Enumerable.Repeat('=', 20)));
            Console.WriteLine("\nВсе элементы Tablet 'Apple':");
            XPathNodeIterator nodesByManufacturer = navigator.Select(
                "//Tablet[Manufacturer = 'Apple']"
            );
            while (nodesByManufacturer.MoveNext())
            {
                Console.WriteLine(nodesByManufacturer.Current.OuterXml);
            }
            Console.WriteLine(string.Join("", Enumerable.Repeat('=', 20)));
        }

        static void PerformLinqQueries()
        {
            var tablets = new[]
            {
                new
                {
                    Name = "iPad Pro",
                    Price = 999.99,
                    Manufacturer = "Apple"
                },
                new
                {
                    Name = "Galaxy Tab S7",
                    Price = 649.99,
                    Manufacturer = "Samsung"
                }
                ,
                new
                {
                    Name = "Xiaomi Pad 5",
                    Price = 1200.32,
                    Manufacturer = "Xiaomi"
                }
                ,
                new
                {
                    Name = "HP S7",
                    Price = 1649.99,
                    Manufacturer = "HP"
                }
                ,
                new
                {
                    Name = "Asus Med 5g",
                    Price = 1200.5,
                    Manufacturer = "Asus"
                }
            };
            XElement root = new XElement(
                "Tablets",
                tablets.Select(t => new XElement(
                    "Tablet",
                    new XElement("Name", t.Name),
                    new XElement("Price", t.Price),
                    new XElement("Manufacturer", t.Manufacturer)
                ))
            );
            root.Save("tablets.xml");
            var query = root.Elements("Tablet").Where(t => (double)t.Element("Price") > 1000.0).Select(t => t);

            Console.WriteLine("Запрос LINQ:");
            foreach (var tablet in query)
            {
                Console.WriteLine(tablet);
            }
        }
    }
}
