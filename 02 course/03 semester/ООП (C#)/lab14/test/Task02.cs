using System;
using System.Linq;

namespace test
{
    internal partial class Program
    {
        private static void Task02()
        {
            var domain = AppDomain.CurrentDomain;
            Console.WriteLine("Информация о домене:");
            Console.WriteLine("\n\nТекущий домен:\t\t" + domain.FriendlyName);
            Console.WriteLine("Основная директория:\t" + domain.BaseDirectory);
            Console.WriteLine("Настройки конфигурации:\t" + domain.SetupInformation);
            Console.WriteLine("Все сборки текущего домена: {0}", string.Join(", ", domain.GetAssemblies().Select(d => d.GetName().Name)));
            Console.WriteLine();

            AppDomain newDomain = AppDomain.CreateDomain("НовыйДомен");
            Console.WriteLine($"Новый домен: {newDomain.FriendlyName}");
             
            try
            {
                newDomain.Load("TestAssembly");
                AppDomain.Unload(newDomain);
            }
            catch 
            {
                Console.WriteLine("Для задания загружал несуществующую сборку в домен {0}.", newDomain.FriendlyName);
            }
        }
    }
}
