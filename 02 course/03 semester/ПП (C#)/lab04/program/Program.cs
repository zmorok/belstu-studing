using Lec04LibN;

using System;
using System.IO;

namespace program
{
    internal class Program
    {
        static void Main(string[] args)
        {
            string path = Directory.GetCurrentDirectory();
            path = Directory.GetParent(path).FullName;
            path = Directory.GetParent(path).FullName;
            try
            {
                if (Directory.Exists(path + "\\LOGS")) 
                { 
                    Console.WriteLine("Директория логов существует, проверьте её."); 
                    return;
                }

                Directory.CreateDirectory(path + "\\LOGS");
                Console.WriteLine("Директория логов создана.");
            }
            catch (Exception e)
            {
                Console.WriteLine("Процесс закончился с ошибкой: {0}", e.ToString());
            }
            finally 
            {
                Directory.SetCurrentDirectory(path + "\\LOGS");
                ILogger logger1 = Logger.Create();
                logger1.Log("000000");
                logger1.Log("010101");
                logger1.Start("A");
                logger1.Log("020202");
                logger1.Start("B");
                logger1.Log("030303");
                logger1.Stop();
                logger1.Log("040404");

                ILogger logger2 = Logger.Create();
                logger2.Log("050505");
                logger2.Start("C");
                logger2.Log("060606");
                logger2.Start("D");
                logger2.Log("070707");
                logger2.Log("080808");
                logger2.Stop();
                logger2.Log("090909");
                logger2.Log("101010");
                logger2.Log("111111");
                logger2.Stop();

                ILogger logger3 = Logger.Create();
                logger3.Log("121212");
                logger3.Start("E");
                logger3.Start("F");
                logger3.Stop();
                logger3.Stop();
            }
        }
    }
}
