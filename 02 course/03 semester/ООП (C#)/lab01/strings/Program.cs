using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StringsLiterals
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("СТРОКОВЫЕ ЛИТЕРАЛЫ:");
            string str1 = "str";
            string str2 = "str";
            Console.WriteLine(String.Compare(str1, str2));

            Console.WriteLine("\n\nТРИ СТРОКИ:");
            String str21 = "str1";
            String str22 = "str";
            String str23 = "str12331";

            // сцепление
            Console.WriteLine(
                "Сцепление строк {0}, {1}, {2} в {3}.\n",
                str21,
                str22,
                str23,
                String.Join(str21, str22, str23)
            );

            // копированме
            string teststr = String.Copy(str22);
            Console.WriteLine("Копирование строки {0} в строку {1}.\n", str22, teststr);

            // выделение подстроки
            string substr = str23.Substring(1, 5);
            Console.WriteLine("Выделение подстроки: {0}\n", substr);

            // разделение строки на слова
            string str = "Hello, hi, this is str!";
            string[] strs = str.Split(' ');
            foreach (string s in strs)
            {
                Console.WriteLine(s);
            }
            Console.WriteLine('\n');

            // вставка подстроки в заданную позицию
            string mainstr = "Put your str before \"str\"";
            Console.WriteLine(mainstr);
            string insertedStr = mainstr.Insert(9, substr + " ");
            Console.WriteLine(insertedStr);

            // удаление заданной подстроки
            int indexStr = mainstr.IndexOf("\"");
            int lenStr = mainstr.LastIndexOf("\"") - indexStr + 1;
            string removedStr = mainstr.Remove(indexStr, lenStr);
            Console.WriteLine(removedStr + '\n');

            // интерполирование строк
            string firstname = "Dmitry";
            string lastname = "Sergeevich";
            string surname = "Zhuk";
            string msg = $"Hello, im {firstname}, my surname: {surname}, and lastname: {lastname}";
            Console.WriteLine(msg + '\n');

            // пустая строка и null строка
            string emptystr = "";
            string nullstr = null;
            Console.WriteLine(String.IsNullOrEmpty(emptystr) ? "Строка пуста или null" : "В строке что-то записано");
            Console.WriteLine(String.IsNullOrEmpty(nullstr) ? "Строка пуста или null" : "В строке что-то записано");

            // StringBuilder
            Console.WriteLine('\n');
            StringBuilder strBuild = new StringBuilder("Hello, this is StringBuilder!");
            strBuild.Remove(7, 8);
            Console.WriteLine(strBuild);

            strBuild.Append(" Appended str");
            Console.WriteLine(strBuild);

            strBuild.Insert(0, "Start str: ");
            Console.WriteLine(strBuild);
        }
    }
}
