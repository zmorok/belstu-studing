using System.IO;
using System.Text.Encodings.Web;
using System.Text.Json;
using FreelanceApp.Models;

namespace FreelanceApp.Services
{
    // class for loading/saving json data
    public static class JSON_DataManager
    {
        #region Filenames
        private static readonly string OrdersFilePath = "orders.json";
        private static readonly string FreelancersFilePath = "freelancers.json";
        #endregion

        #region Options
        static JsonSerializerOptions options = new JsonSerializerOptions
        {
            WriteIndented = true,
            Encoder = JavaScriptEncoder.UnsafeRelaxedJsonEscaping
        };
        #endregion

        #region Methods
        public static List<Order> LoadOrders()
        {
            if (!File.Exists(OrdersFilePath)) return new List<Order>();
            string json = File.ReadAllText(OrdersFilePath);
            return JsonSerializer.Deserialize<List<Order>>(json) ?? new List<Order>();
        }
        public static void SaveOrders(List<Order> orders)
        {
            string json = JsonSerializer.Serialize(orders, options);
            File.WriteAllText(OrdersFilePath, json);
        }
        public static List<Freelancer> LoadFreelancers()
        {
            if (!File.Exists(FreelancersFilePath)) return new List<Freelancer>();
            string json = File.ReadAllText(FreelancersFilePath);
            return JsonSerializer.Deserialize<List<Freelancer>>(json) ?? new List<Freelancer>();
        }
        public static void SaveFreelancers(List<Freelancer> freelancers)
        {
            string json = JsonSerializer.Serialize(freelancers, options);
            File.WriteAllText(FreelancersFilePath, json);
        }
        #endregion
    }
}
