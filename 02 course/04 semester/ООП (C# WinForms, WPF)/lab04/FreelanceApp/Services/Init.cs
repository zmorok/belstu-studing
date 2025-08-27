using System.IO;
using System.Text.Json;
using FreelanceApp.Models;

namespace FreelanceApp.Services
{
    // initialization of app's db
    class Init
    {
        public static void Execute(FreelanceAppContext context)
        {
            context.Database.EnsureCreated();
            if (!context.Freelancers.Any())
            {
                string freelancersJson = File.ReadAllText("data/freelancers.json");
                List<Freelancer> freelancers = JsonSerializer.Deserialize<List<Freelancer>>(freelancersJson)!;
                context.Freelancers.AddRange(freelancers);
            }

            if (!context.Orders.Any())
            {
                string ordersJson = File.ReadAllText("data/orders.json");
                List<Order> orders = JsonSerializer.Deserialize<List<Order>>(ordersJson)!;
                context.Orders.AddRange(orders);
            }
            context.SaveChanges();
        }
    }
}
