using DAL_LES;

class Program
{
    static void Main()
    {
        using (Repository repo = Repository.Create())
        {
            foreach (var le in repo.GetAllLifeevents()) repo.DelLifeevent(le.Id);
            foreach (var c in repo.GetAllCelebrities()) repo.DelCelebrity(c.Id);

            Console.WriteLine("== Добавление знаменитостей ==");
            var celeb1 = new Celebrity { FullName = "Альберт Эйнштейн", Nationality = "DE" };
            var celeb2 = new Celebrity { FullName = "Алан Тьюринг", Nationality = "UK" };
            repo.AddCelebrity(celeb1);
            repo.AddCelebrity(celeb2);

            var allCelebs = repo.GetAllCelebrities();
            Console.WriteLine("-- Получены знаменитости:");
            foreach (var c in allCelebs)
                Console.WriteLine($"  {c.Id}: {c.FullName} ({c.Nationality})");

            Pause("▶ Нажмите любую клавишу для обновления...");

            var einstein = allCelebs.First(c => c.FullName.Contains("Эйнштейн"));
            var turing = allCelebs.First(c => c.FullName.Contains("Тьюринг"));

            Console.WriteLine("\n== Обновление знаменитости ==");
            einstein.FullName = "Альберт Эйнштейн (обновлён)";
            repo.UpdateCelebrity(einstein.Id, einstein);
            Console.WriteLine($"  Обновлено: {repo.GetCelebrityById(einstein.Id)?.FullName}");

            Pause("▶ Нажмите любую клавишу для добавления событий...");

            Console.WriteLine("\n== Добавление событий ==");
            var life1 = new Lifeevent
            {
                CelebrityId = einstein.Id,
                Date = new DateTime(1905, 6, 30),
                Description = "Специальная теория относительности"
            };
            var life2 = new Lifeevent
            {
                CelebrityId = turing.Id,
                Date = new DateTime(1936, 5, 28),
                Description = "Работа о вычислимых числах"
            };
            var life3 = new Lifeevent
            {
                CelebrityId = turing.Id,
                Date = new DateTime(1945, 7, 1),
                Description = "Проект ACE"
            };

            repo.AddLifeevent(life1);
            repo.AddLifeevent(life2);
            repo.AddLifeevent(life3);

            Console.WriteLine("\n== Получение всех событий ==");
            foreach (var le in repo.GetAllLifeevents())
                Console.WriteLine($"  {le.Id}: {le.Description} ({le.Date.ToShortDateString()})");

            Pause("▶ Нажмите любую клавишу для обновления события...");

            Console.WriteLine("\n== Получение события по ID ==");
            var oneEvent = repo.GetAllLifeevents().First();
            var fetched = repo.GetLifeeventById(oneEvent.Id);
            Console.WriteLine($"  Получено: {fetched?.Description}");

            Console.WriteLine("\n== Обновление события ==");
            fetched!.Description += " (обновлено)";
            repo.UpdateLifeevent(fetched.Id, fetched);
            Console.WriteLine($"  После обновления: {repo.GetLifeeventById(fetched.Id)?.Description}");

            Pause("▶ Нажмите любую клавишу для просмотра событий по знаменитостям...");

            Console.WriteLine("\n== Получение событий по ID знаменитости ==");
            var turingEvents = repo.GetLifeeventsByCelebrityId(turing.Id);
            foreach (var le in turingEvents)
                Console.WriteLine($"  {le.Description}");

            Pause("▶ Нажмите любую клавишу для определения принадлежности событий...");

            Console.WriteLine("\n== Получение знаменитости по ID события ==");
            foreach (var ev in repo.GetAllLifeevents())
            {
                var c = repo.GetCelebrityByLifeeventId(ev.Id);
                Console.WriteLine($"  Событие \"{ev.Description}\" принадлежит: {c?.FullName}");
            }

            Pause("▶ Нажмите любую клавишу для удаления события...");

            Console.WriteLine("\n== Удаление одного события ==");
            if (turingEvents.Any())
            {
                var toDel = turingEvents.First();
                Console.WriteLine($"  Удаляю: {toDel.Description}");
                repo.DelLifeevent(toDel.Id);
            }

            Pause("▶ Нажмите любую клавишу для удаления знаменитости...");

            Console.WriteLine("\n== Удаление знаменитости ==");
            repo.DelCelebrity(einstein.Id);
            Console.WriteLine($"  Осталось: {repo.GetAllCelebrities().Count} знаменитостей");

            Pause("▶ Нажмите любую клавишу для финальной проверки...");

            Console.WriteLine("\n== Проверка оставшихся данных ==");
            foreach (var c in repo.GetAllCelebrities())
                Console.WriteLine($"  {c.FullName}");
            foreach (var e in repo.GetAllLifeevents())
                Console.WriteLine($"  {e.Description} ({e.Date.ToShortDateString()})");
        }

        Console.WriteLine("\n== Все методы протестированы ==");
    }

    static void Pause(string message)
    {
        Console.WriteLine();
        Console.WriteLine(message);
        Console.ReadKey(true);
    }
}
