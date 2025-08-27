using CashCelebrities;
using DAL_LES;
using Microsoft.Extensions.DependencyInjection;
using ServiceLocator;

internal class Program
{
    private static async Task Main()
    {
        TimeSpan cacheDuration = TimeSpan.FromSeconds(3);

        TransientLocator.Register<IRepository>(() => Repository.Create());
        TransientLocator.Register<ICelebrity>(() => Repository.Create());
        TransientLocator.Register<ILifeevent>(() => Repository.Create());
        TransientLocator.Register<ICommon>(() => Repository.Create());

        CashCelebrities.Scope.ScopeLocator.Register<ICashCelebrities>(duration =>
        {
            IRepository repo = TransientLocator.Resolve<IRepository>();
            return new CashCelebrityService(duration, repo);
        });

        IServiceScopeFactory scopeFactory = CashCelebrities.Scope.ScopeLocator.CreateServiceScopeFactory(cacheDuration);
        using (IServiceScope scope = scopeFactory.CreateScope())
        {
            ICashCelebrities? cashCeleb = scope.ServiceProvider.GetService<ICashCelebrities>();
            cashCeleb?.GetCelebrities();
            cashCeleb?.PrintCashCelebs();

            Console.WriteLine("\n=== Добавляем новых знаменитостей ===");

            using (IRepository repo = TransientLocator.Resolve<IRepository>())
            {
                foreach (var c in repo.GetAllCelebrities()) repo.DelCelebrity(c.Id);
                foreach (var le in repo.GetAllLifeevents()) repo.DelLifeevent(le.Id);

                Celebrity c1 = new Celebrity { FullName = "Celeb1", Nationality = "B" };
                Celebrity c2 = new Celebrity { FullName = "Celeb2", Nationality = "B" };
                repo.AddCelebrity(c1);
                repo.AddCelebrity(c2);
                Console.WriteLine("Знаменитости добавлены.");

                repo.GetAllCelebrities().ForEach(c => Console.WriteLine($"{c.Id}: {c.FullName} - {c.Nationality}"));
            }

            Console.WriteLine("\n\n[--- ICashCelebrities после добавления ---]");
            cashCeleb?.GetCelebrities();
            cashCeleb?.PrintCashCelebs();

            Console.WriteLine($"\nЖдём {cacheDuration.TotalSeconds} секунд");
            cashCeleb?.PrintCashCelebs();
            await Task.Delay(5000);

            Console.WriteLine("\n\n[--- ICashCelebrities после истечения кэша ---]");
            ICashCelebrities? refreshedCashCeleb = scope.ServiceProvider.GetService<ICashCelebrities>();
            refreshedCashCeleb?.GetCelebrities();
            refreshedCashCeleb?.PrintCashCelebs();

            await Task.Delay(5000);

            using (IRepository repo = TransientLocator.Resolve<IRepository>())
            {
                Celebrity c1 = new Celebrity { FullName = "Celeb3", Nationality = "B" };
                Celebrity c2 = new Celebrity { FullName = "Celeb4", Nationality = "B" };
                repo.AddCelebrity(c1);
                repo.AddCelebrity(c2);
            }

            Console.WriteLine("\n\n[--- ICashCelebrities после истечения кэша ---]");
            refreshedCashCeleb?.GetCelebrities();
            refreshedCashCeleb?.PrintCashCelebs();

        }

        Console.WriteLine("\n=== Тест завершён ===");
        Console.ReadLine();
    }
}
