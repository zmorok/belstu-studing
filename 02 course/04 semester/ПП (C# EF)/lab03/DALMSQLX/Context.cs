using Microsoft.EntityFrameworkCore;
using REPO;

namespace DALMSSQLX
{
    public class Context : DbContext
    {
        public Context() { Database.EnsureCreated(); }
        public DbSet<WSRef> WSRefs { get; set; }
        public DbSet<Comment> Comments { get; set; }
        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlServer("Server=localhost;Database=DAL11_X;User Id=zmorok_pwd;Password=qwerty1234$;TrustServerCertificate=True;");
        }
    }
}
