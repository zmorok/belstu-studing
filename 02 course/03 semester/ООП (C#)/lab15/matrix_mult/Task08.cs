using System;
using System.Threading.Tasks;

namespace matrix_mult
{
    public class Task08
    {
        public static async Task ComputeTask08()
        {
            string input = "Строка, которая почему-то долго выводится.";
            await PrintMessageAsync(input, 100);
        }

        static async Task PrintMessageAsync(string input, int sleep)
        {
            foreach (char c in input)
            {
                Console.Write(c);
                await Task.Delay(sleep);
            }
            Console.WriteLine();
        }
    }
}
