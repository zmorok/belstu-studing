using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace ApartmentForm
{
    public class Apartment
    {
        [Range(10, 1000, ErrorMessage = "Метраж должен быть от 10 до 1000 кв.м.")]
        public double Meters { get; set; }

        [Range(1, 10, ErrorMessage = "Количество комнат должно быть от 1 до 10.")]
        public int RoomCount { get; set; }

        [Required(ErrorMessage = "Список опций не может быть пустым.")]
        public List<string> Options { get; set; } = new List<string>();

        [Required(ErrorMessage = "Необходимо выбрать материал.")]
        public string Material { get; set; } = string.Empty;

        [Range(1800, 2100, ErrorMessage = "Год постройки должен быть в диапазоне от 1800 до 2100.")]
        public int YearBuilt { get; set; }

        public Address? Address { get; set; }
        public Developer? Developer { get; set; }

        public string Price => CalculatePrice().ToString("F2");

        public Apartment()
        {
            Meters = 0;
            RoomCount = 0;
            Options = new List<string>();
            Material = "";
            YearBuilt = 0;
            Address = new Address();
            Developer = new Developer();
        }

        public Apartment(string[]? values)
        {
            if (values != null)
            {
                Meters = double.Parse(values[0]);
                RoomCount = int.Parse(values[1]);
                Material = values[2];
            }
        }

        [JsonIgnore]
        public string CheckingString => $"{Meters}, {RoomCount}, {Material}";

        public Apartment(Apartment? o)
        {
            if (o != null)
            {
                Meters = o.Meters;
                RoomCount = o.RoomCount;    
                Options = new List<string>(o.Options);
                Material = o.Material;
                YearBuilt = o.YearBuilt;
                Address = new Address(o.Address);
                Developer = new Developer(o.Developer);
            }
        }

        public double CalculatePrice()
        {
            double basePricePerMeter = 1000.0;
            double price = Meters * basePricePerMeter;
            double roomBonus = RoomCount * 2000.0;
            price += roomBonus;
            double optionsBonus = Options?.Count * 500.0 ?? 0;
            price += optionsBonus;
            double materialMultiplier = 1.0;
            switch (Material)
            {
                case "Кирпич":
                    materialMultiplier = 1.1;
                    break;
                case "Панель":
                    materialMultiplier = 1.0;
                    break;
                case "Монолит":
                    materialMultiplier = 1.2;
                    break;
                case "Дерево":
                    materialMultiplier = 0.9;
                    break;
                default:
                    materialMultiplier = 1.0;
                    break;
            }
            return price * materialMultiplier;
        }

        public override string ToString()
        {
            return $"{Meters} кв.м, {RoomCount} комнаты, материал: {Material}, цена: {Price}";
        }
    }

    public class Address
    {
        [Required(ErrorMessage = "Страна обязательна.")]
        [RegularExpression(@"^[А-Яа-яA-Za-z\s\-]+$", ErrorMessage = "Страна должна содержать только буквы.")]
        public string? Country { get; set; }

        [Required(ErrorMessage = "Город обязателен.")]
        [RegularExpression(@"^[А-Яа-яA-Za-z\s\-]+$", ErrorMessage = "Город должен содержать только буквы.")]
        public string? City { get; set; }

        [RegularExpression(@"^[А-Яа-яA-Za-z\s\-]*$", ErrorMessage = "Район должен содержать только буквы.")]
        public string? District { get; set; }

        [Required(ErrorMessage = "Улица обязательна.")]
        [RegularExpression(@"^[А-Яа-яA-Za-z\s\-]+$", ErrorMessage = "Улица должна содержать только буквы.")]
        public string? Street { get; set; }

        [Required(ErrorMessage = "Дом обязателен.")]
        [RegularExpression(@"^\d*$", ErrorMessage = "Номер дома указан неверно.")]
        public string? House { get; set; }

        [RegularExpression(@"^\d+(\-|\s|)\d*$", ErrorMessage = "Корпус указан неверно.")]
        public string? Subhouse { get; set; }

        [Required(ErrorMessage = "Номер квартиры обязателен.")]
        [RegularExpression(@"^\d+$", ErrorMessage = "Номер квартиры должен быть числом.")]
        public string? ApartmentNumber { get; set; }

        public Address()
        {
            Country = "";
            City = "";
            District = "";
            Street = "";
            House = "";
            Subhouse = "";
            ApartmentNumber = "";
        }

        public Address(Address? o)
        {
            if (o != null)
            {
                Country = o.Country;
                City = o.City;
                District = o.District;
                Street = o.Street;
                House = o.House;
                Subhouse = o.Subhouse;
                ApartmentNumber = o.ApartmentNumber;
            }
        }
    }

    public class Developer
    {
        [Required(ErrorMessage = "Название застройщика обязательно.")]
        [RegularExpression(@"^[А-Яа-яA-Za-z0-9\s]+$", ErrorMessage = "Название должно содержать только буквы, цифры и дефисы.")]
        public string? Name { get; set; }

        [Required(ErrorMessage = "Тип компании обязателен.")]
        [RegularExpression(@"^(ООО|ОАО|ИП)$", ErrorMessage = "Тип компании должен быть: ООО, ОАО или ИП.")]
        public string? Type { get; set; }

        [Required(ErrorMessage = "Юридический адрес обязателен.")]
        [RegularExpression(@"^[А-Яа-яA-Za-z0-9\s\.,]+$", ErrorMessage = "Адрес должен содержать только буквы, цифры, пробелы, точки, запятые и дефисы.")]
        public string? Address { get; set; }

        [Required(ErrorMessage = "ИНН обязателен.")]
        [RegularExpression(@"^\d{10}$", ErrorMessage = "ИНН должен состоять из 10 цифр.")]
        [INNValidation]
        public string? INN { get; set; }

        public Developer()
        {
            Name = "";
            Type = "";
            Address = "";
            INN = "";
        }

        public Developer(Developer? o)
        {
            if (o != null)
            {
                Name = o.Name;
                Type = o.Type;
                Address = o.Address;
                INN = o.INN;
            }
        }
    }
}
