using Microsoft.Extensions.DependencyInjection;
using ServiceLocator;
using DAL_LES;

class Program
{
    static void Main()
    {
        // Transient
        Console.WriteLine("TransientLocator:");
        TransientLocator.Register<IRepository>(() => Repository.Create());
        using (IRepository repo1 = TransientLocator.Resolve<IRepository>())
        {
            Console.WriteLine($"Transient: {repo1.GetHashCode()}");
        }
        using (IRepository repo2 = TransientLocator.Resolve<IRepository>())
        {
            Console.WriteLine($"Transient: {repo2.GetHashCode()}");
        }

        Console.WriteLine();

        // Singleton
        Console.WriteLine("SingletonLocator:");
        SingletonLocator.Register<IRepository>(Repository.Create());
        using (IRepository repo3 = SingletonLocator.Resolve<IRepository>())
        {
            Console.WriteLine($"Singleton: {repo3.GetHashCode()}");
        }
        using (IRepository repo4 = SingletonLocator.Resolve<IRepository>())
        {
            Console.WriteLine($"Singleton: {repo4.GetHashCode()}");
        }

        Console.WriteLine();

        // Scoped
        Console.WriteLine("ScopeLocator:");
        ScopeLocator.Register<IRepository>(() => Repository.Create());
        IServiceScopeFactory scopeFactory = ScopeLocator.CreateServiceScopeFactory();
        using (IServiceScope scope1 = scopeFactory.CreateScope())
        {
            IRepository? repo5 = scope1.ServiceProvider.GetService<IRepository>();
            IRepository? repo6 = scope1.ServiceProvider.GetService<IRepository>();
            if (repo5 != null && repo6 != null)
            {
                Console.WriteLine($"Scope 1 - Repo 1: {repo5.GetHashCode()}");
                Console.WriteLine($"Scope 1 - Repo 2: {repo6.GetHashCode()}");
            }
        }
        using (IServiceScope scope2 = scopeFactory.CreateScope())
        {
            IRepository? repo7 = scope2.ServiceProvider.GetService<IRepository>();
            IRepository? repo8 = scope2.ServiceProvider.GetService<IRepository>();
            IRepository? repo9 = scope2.ServiceProvider.GetService<IRepository>();

            if (repo7 != null && repo8 != null && repo9 != null)
            {
                Console.WriteLine($"Scope 2 - Repo 1: {repo7.GetHashCode()}");
                Console.WriteLine($"Scope 2 - Repo 2: {repo8.GetHashCode()}");
                Console.WriteLine($"Scope 2 - Repo 3: {repo9.GetHashCode()}");
            }
        }
    }
}
