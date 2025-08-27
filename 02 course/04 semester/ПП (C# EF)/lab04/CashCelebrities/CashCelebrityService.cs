using DAL_LES;

namespace CashCelebrities
{
    public interface ICashCelebrities
    {
        List<Celebrity> GetCelebrities();
        void PrintCashCelebs();
    }

    public class CashCelebrityService : ICashCelebrities
    {
        private readonly TimeSpan cacheDuration;
        private List<Celebrity> cachedCelebrities;
        private readonly IRepository repo;
        private DateTime prevDateTime;
        //private Timer timer;

        public CashCelebrityService(TimeSpan duration, IRepository repo)
        {
            this.cacheDuration = duration;
            this.repo = repo;
            this.cachedCelebrities = new List<Celebrity>();
            this.prevDateTime = DateTime.MinValue;

            //timer = new Timer(RefreshCachedCelebs, this, TimeSpan.FromSeconds(4), duration);
        }

        public List<Celebrity> GetCelebrities()
        {
            if (DateTime.Now - prevDateTime >= cacheDuration)
            {
                cachedCelebrities = repo.GetAllCelebrities();
                prevDateTime = DateTime.Now;
            }
            return cachedCelebrities ?? new List<Celebrity>();
        }

        public void PrintCashCelebs()
        {
            List<Celebrity> celebrities_list = cachedCelebrities.ToList();
            Console.WriteLine($"Время кеширования: {DateTime.Now:T}");
            Console.WriteLine($"Кешированные знаменитости: {celebrities_list.Count}");
            foreach (var c in celebrities_list)
            {
                Console.WriteLine($"Знаменитость {c.Id}: {c.FullName}, {c.Nationality}");
            }
        }

        // Метод, вызываемый таймером
        private static void RefreshCachedCelebs(object state)
        {
            if (state is CashCelebrityService service)
            {
                service.cachedCelebrities = service.repo.GetAllCelebrities();
                service.prevDateTime = DateTime.Now;
                Console.WriteLine($"[Таймер] Кэш обновлён: {DateTime.Now:T}");
            }
        }
    }
}