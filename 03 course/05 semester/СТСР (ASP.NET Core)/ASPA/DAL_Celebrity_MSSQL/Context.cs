using DAL_Celebrity;
using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DAL_Celebrity_MSSQL;

public class Context : DbContext
{
    public string? ConnectionString { get; private set; } = null;
    public Context(string connectionString) : base()
    {
        ConnectionString = connectionString;
    }

    public Context() : base() { }
    public Context(DbContextOptions<Context> options) : base(options) { }

    public DbSet<Celebrity> Celebrities { get; set; }
    public DbSet<Lifeevent> Lifeevents { get; set; } 

    protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
    {
        if (!string.IsNullOrEmpty(ConnectionString)) { optionsBuilder.UseSqlServer(ConnectionString); }
        else
        {
            optionsBuilder.UseSqlServer("Server=localhost;Database=CelebrityDB_STSR_06;User Id=zmorok_pwd;Password=qwerty1234$;TrustServerCertificate=True;");
        }
    }

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<Celebrity>().ToTable("Celebrities").HasKey(p => p.Id);
        modelBuilder.Entity<Celebrity>().Property(p => p.Id).IsRequired();
        modelBuilder.Entity<Celebrity>().Property(p => p.FullName).IsRequired().HasMaxLength(50);
        modelBuilder.Entity<Celebrity>().Property(p => p.Nationality).IsRequired().HasMaxLength(2);
        modelBuilder.Entity<Celebrity>().Property(p => p.ReqPhotoPath).HasMaxLength(200);

        modelBuilder.Entity<Lifeevent>().ToTable("Lifeevents").HasKey(p => p.Id);
        modelBuilder.Entity<Lifeevent>().ToTable("Lifeevents");
        modelBuilder.Entity<Lifeevent>().Property(p => p.Id).IsRequired();
        modelBuilder.Entity<Lifeevent>().ToTable("Lifeevents").HasOne<Celebrity>().WithMany().HasForeignKey(p => p.CelebrityId);
        modelBuilder.Entity<Lifeevent>().Property(p => p.CelebrityId).IsRequired();
        modelBuilder.Entity<Lifeevent>().Property(p => p.Date);
        modelBuilder.Entity<Lifeevent>().Property(p => p.Description).HasMaxLength(256);
        modelBuilder.Entity<Lifeevent>().Property(p => p.ReqPhotoPath).HasMaxLength(256);
        base.OnModelCreating(modelBuilder);
    }
}
