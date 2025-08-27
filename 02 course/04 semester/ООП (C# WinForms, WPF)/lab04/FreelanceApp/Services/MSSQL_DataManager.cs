using System.Collections.ObjectModel;
using FreelanceApp.Models;
using Microsoft.EntityFrameworkCore;

namespace FreelanceApp.Services
{
    public class FreelanceAppContext : DbContext
    {
        public FreelanceAppContext()
        {
            Database.EnsureCreated();
        }

        public DbSet<Order> Orders { get; set; }
        public DbSet<Freelancer> Freelancers { get; set; }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlServer(
                "Server=localhost;Database=FreelanceApp_OOP;User Id=zmorok_pwd;Password=qwerty1234$;TrustServerCertificate=True;"
            );
        }
    }
    //
    //  TODO
    //  rewrite with transactions
    public class MSSQL_DataManager
    {
        #region LOAD_DATA
        public static void LoadData(
            out ObservableCollection<Order> orders,
            out ObservableCollection<Freelancer> freelancers
        )
        {
            using var context = new FreelanceAppContext();
            context.Database.EnsureCreated();

            orders = new ObservableCollection<Order>(context.Orders.ToList());
            freelancers = new ObservableCollection<Freelancer>(context.Freelancers.ToList());
        }
        #endregion

        #region ORDERS_SYNC
        public static void SyncOrders(
            ObservableCollection<Order> original,
            ObservableCollection<Order> updated
        )
        {
            using var context = new FreelanceAppContext();

            var originalById = original.ToDictionary(o => o.Order_Id);
            var updatedById = updated.ToDictionary(o => o.Order_Id);

            foreach (var orig in original.ToList())
            {
                if (updatedById.TryGetValue(orig.Order_Id, out var updatedItem))
                {
                    if (IsOrderChanged(orig, updatedItem))
                    {
                        orig.Title = updatedItem.Title;
                        orig.Description = updatedItem.Description;
                        orig.Price = updatedItem.Price;
                        orig.Category = updatedItem.Category;
                        orig.FullDescription = updatedItem.FullDescription;

                        context.Orders.Update(orig);
                    }
                }
                else
                {
                    original.Remove(orig);
                    context.Orders.Remove(orig);
                }
            }

            foreach (var newItem in updated)
            {
                if (newItem.Order_Id == 0 || !originalById.ContainsKey(newItem.Order_Id))
                {
                    original.Add(newItem);
                    context.Orders.Add(newItem);
                }
            }

            context.SaveChanges();
        }

        private static bool IsOrderChanged(Order a, Order b)
        {
            return a.Title != b.Title
                || a.Description != b.Description
                || a.Price != b.Price
                || a.Category != b.Category
                || a.FullDescription != b.FullDescription;
        }

        #endregion

        #region FREELANCERS_SYNC
        public static void SyncFreelancers(
            ObservableCollection<Freelancer> original,
            ObservableCollection<Freelancer> updated
        )
        {
            using var context = new FreelanceAppContext();

            var originalById = original.ToDictionary(f => f.Freelancer_Id);
            var updatedById = updated.ToDictionary(f => f.Freelancer_Id);

            foreach (var orig in original.ToList())
            {
                if (updatedById.TryGetValue(orig.Freelancer_Id, out var updatedItem))
                {
                    if (IsFreelancerChanged(orig, updatedItem))
                    {
                        orig.Name = updatedItem.Name;
                        orig.Skills = updatedItem.Skills;
                        orig.Rating = updatedItem.Rating;
                        orig.Description = updatedItem.Description;

                        context.Freelancers.Update(orig);
                    }
                }
                else
                {
                    original.Remove(orig);
                    context.Freelancers.Remove(orig);
                }
            }

            foreach (var newItem in updated)
            {
                if (newItem.Freelancer_Id == 0 || !originalById.ContainsKey(newItem.Freelancer_Id))
                {
                    original.Add(newItem);
                    context.Freelancers.Add(newItem);
                }
            }

            context.SaveChanges();
        }

        private static bool IsFreelancerChanged(Freelancer a, Freelancer b)
        {
            return a.Name != b.Name
                || a.Skills != b.Skills
                || a.Rating != b.Rating
                || a.Description != b.Description;
        }
        #endregion
    }
}
