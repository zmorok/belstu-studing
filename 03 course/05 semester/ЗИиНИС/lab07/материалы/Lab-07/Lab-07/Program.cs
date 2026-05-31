using System;
using System.Reflection;

namespace ЛР__7
{
    class Program
    {
        static void Main(string[] args)
        {
            int lenghtK = 15; //Должна быть равна 2^n
            int k = (int)(Math.Sqrt(lenghtK));
            int r = HemmingLength(k);
            int n = k + r;
            int lenghtN = lenghtK + (r * k);

            int[] masK = new int[lenghtK];
            int[] masDefault = new int[lenghtK];
            int[] masN = new int[lenghtK + (r * k)];
            int[,] checkMatrix = new int[n, r];

            int error;
            int errorLenght;

            GenerationRandMasMod2(masK);
            Console.WriteLine("Входная строка: ");
            OutMas(masK);
            masDefault = (int[])masK.Clone();

            //Console.WriteLine("\n\nПроверочная матрица: ");
            checkMatrix = CheckMatrix(k);
            //OutMatrixInv(checkMatrix, n, r);





            AddCheckBits(masK, masN, checkMatrix);
            Console.WriteLine("\n\nСтрока с проверочными битами: ");
            OutMas(masN);

            //var matrix0 = IterativeCode.MsgTo2DimMatrix(masN, 4, 6);
            //Printer.PrintMatrix("\nДвумерная матрица: ", matrix0, false);

            var matrix1 = IterativeCode.MsgTo2DimMatrix(masN, 6, 4);
            var checkBits = IterativeCode.CalculateCheckBits(matrix1);
            Printer.PrintBits("\n\nXr= ", checkBits);

            masN = Alternation(masN, 4);
            //masN = IterativeCode.Transposed(masN, k);

            Console.WriteLine("\nСтрока после перемежения: ");
            OutMas(masN);
            int[] masNY;
            masNY = masN;



            try
            {
                Console.WriteLine("\n\nВведите место ошибки");
                error = Convert.ToInt32(Console.ReadLine());
                Console.WriteLine("Введите длину ошибки");
                errorLenght = Convert.ToInt32(Console.ReadLine());
                for (int i = error; i < (error + errorLenght); i++)
                {
                    masN[i] = (masN[i] + 1) % 2;
                }
            }
            catch { }

            Console.WriteLine("\nСтрока с ошибками: ");
            OutMas(masN);

            masN = ReAlternation(masN, 4);
            Console.WriteLine("\nСтрока после деперемежения: ");
            OutMas(masN);
            //RemoveCheckBits(masK, masN, checkMatrix);


            Console.WriteLine();
            //var msg = new int[] { 0, 1, 1, 1, 0, 1, 1, 1, 1 };
            //Printer.PrintBits("Сообщение: ", masNY);

            var matrix = IterativeCode.MsgTo2DimMatrix(masN, 6, 4);





            //Printer.PrintBits("Xr= ", checkBits);


            var errors = IterativeCode.FindErrorPositions(matrix, checkBits);
            if (errors.Count > 0)
            {
                Console.Write("Найдены ошибки в битах: ");
                foreach (var pos in errors)
                {
                    Console.Write($"{pos} ");
                }
                masN = IterativeCode.FixError(masN, errors);

                Console.WriteLine("\n\nНачальное сообщение");
                OutMas(masDefault);
                Console.WriteLine("\nДоставленное сообщениe");
                masK = RemoveCheckBits(masK, masN, checkMatrix);
                OutMas(masK);

            }
            else
            {
                Console.Write("Ошибки не найдены или их нельзя исправить");
            }
            RemoveCheckBits(masK, masN, checkMatrix);
            //OutMas(masN);
            Console.ReadLine();

        }


        public static class IterativeCode
        {
            public static int[] FixError(int[] masN, List<int> errors)
            {
                foreach (var error in errors)
                {
                    masN[error] = (masN[error] + 1) % 2;
                }
                return masN;
            }
            public static int[,] MsgTo2DimMatrix(int[] msg)
            {
                int len = (int)Math.Sqrt(msg.Length);
                int height = msg.Length / len;
                return MsgTo2DimMatrix(msg, len, height);
            }

            public static int[,] MsgTo2DimMatrix(int[] msg, int height, int len)
            {
                if (len * height != msg.Length)
                    throw new ArgumentException("Размеры матрицы не соответствуют размерам сообщения");

                int[,] matrix = new int[len, height];
                for (int i = 0; i < len; i++)
                {
                    for (int j = 0; j < height; j++)
                    {
                        matrix[i, j] = msg[i * height + j];
                    }
                }
                return matrix;
            }
            public static int[] CalculateCheckBits(int[,] matrix)
            {
                int len = matrix.GetLength(1) + matrix.GetLength(0) + 1;
                int[] bits = new int[len];
                for (int i = 0; i < matrix.GetLength(1); i++)
                {
                    int colSum = 0;
                    for (int j = 0; j < matrix.GetLength(0); j++)
                    {
                        colSum += matrix[j, i];
                    }
                    bits[i] = colSum % 2;
                }
                int allSum = 0;
                for (int i = 0; i < matrix.GetLength(0); i++)
                {
                    int rowSum = 0;
                    for (int j = 0; j < matrix.GetLength(1); j++)
                    {
                        rowSum += matrix[i, j];
                    }
                    allSum += rowSum;
                    bits[i + matrix.GetLength(1)] = rowSum % 2;
                }
                for (int i = 0; i < len - 1; i++)
                {
                    allSum += bits[i];
                }
                bits[len - 1] = allSum % 2;
                return bits;
            }
            public static List<int> FindErrorPositions(int[,] matrix, int[] checkBits)
            {
                var checkBitsForMatrix = CalculateCheckBits(matrix);
                Printer.PrintBits("Xr= ", checkBitsForMatrix);

                var rowMismatch = new List<int>();
                var collMismatch = new List<int>();

                for (int i = 0; i < matrix.GetLength(1); i++)
                {
                    if (checkBits[i] != checkBitsForMatrix[i])
                    {
                        collMismatch.Add(i);
                    }
                }
                for (int i = 0; i < matrix.GetLength(0); i++)
                {
                    if (checkBits[i + matrix.GetLength(1)] != checkBitsForMatrix[i + matrix.GetLength(1)])
                    {
                        rowMismatch.Add(i);
                    }
                }
                List<int> result = new List<int>();
                foreach (var row in rowMismatch)
                {
                    foreach (var coll in collMismatch)
                    {
                        result.Add(row * matrix.GetLength(1) + coll);
                    }
                }

                return result;
            }


        }

        public static class Printer
        {
            public static void PrintMatrix(string msg, int[,] matrix, bool reverse = true)
            {
                Console.WriteLine($"{msg}");
                for (int i = 0; i < matrix.GetLength(reverse ? 1 : 0); i++)
                {
                    Console.Write("    ");
                    for (int j = 0; j < matrix.GetLength(reverse ? 0 : 1); j++)
                    {
                        Console.Write(reverse ? matrix[j, i] : matrix[i, j]);

                    }
                    Console.WriteLine();
                }

            }
            public static void PrintCheckMatrixH(string msg, int[,] matrix, bool reverse = true)
            {
                int r = matrix.GetLength(1) - 1;
                Console.WriteLine($"{msg}");
                for (int i = 0; i < matrix.GetLength(reverse ? 1 : 0); i++)
                {
                    Console.Write("    ");
                    var rowLen = matrix.GetLength(reverse ? 0 : 1);
                    for (int j = 0; j < rowLen; j++)
                    {
                        Console.Write(reverse ? matrix[j, i] : matrix[i, j]);
                        if (j == rowLen - 1 - r) Console.Write(" | ");
                        //if(j < rowLen - 1)
                        //    Console.Write(", ");
                    }
                    Console.WriteLine();
                }

            }
            public static void PrintCheckMatrix(string msg, int[,] matrix, bool reverse = true)
            {
                int r = matrix.GetLength(1);
                Console.WriteLine($"{msg}");
                for (int i = 0; i < matrix.GetLength(reverse ? 1 : 0); i++)
                {
                    Console.Write("    ");
                    var rowLen = matrix.GetLength(reverse ? 0 : 1);
                    for (int j = 0; j < rowLen; j++)
                    {
                        Console.Write(reverse ? matrix[j, i] : matrix[i, j]);
                        if (j == rowLen - 1 - r) Console.Write(" | ");
                        //if(j < rowLen - 1)
                        //    Console.Write(", ");
                    }
                    Console.WriteLine();
                }

            }
            public static void PrintBitsAndCheckBits(string msg, int[] bits, int k)
            {
                Console.Write(msg);
                int r = (int)Math.Log(k, 2) + 1;
                for (int i = 0; i < k; i++)
                {
                    Console.Write(bits[i]);
                }
                Console.Write('.');
                for (int i = 0; i < r; i++)
                {
                    Console.Write(bits[i + k]);
                }
                Console.WriteLine();
            }
            public static void PrintBits(string msg, int[] bits)
            {
                Console.Write(msg);
                for (int i = 0; i < bits.Length; i++)
                    Console.Write(bits[i]);
                Console.WriteLine();
            }
        }

        static int[] SearchErrorLong(int[] masN, int[,] checkMatrix, int k)
        {
            int r = HemmingLength(k);
            int n = r + k;

            for (int i = 0; i < k; i++)
            {
                int[] temp = new int[n];
                for (int j = 0; j < n; j++)
                {
                    temp[j] = masN[(n * i) + j];
                }
                //Получение проверочных битов каждой строки
                //Console.WriteLine("\nTemp");
                SearchError(temp, checkMatrix, k);
                //OutMas(temp);

                //Запись строки в массив, для получения одной большой строки
                for (int j = 0; j < n; j++)
                {
                    masN[i * n + j] = temp[j];
                }

            }

            return masN;
        }

        static int[] RemoveCheckBits(int[] masK, int[] masN, int[,] checkMatrix)
        {
            int lenghtK = masK.Length; //Должна быть равна 2^n
            int lenghtN = masN.Length;
            int k = (int)(Math.Sqrt(lenghtK));
            int r = HemmingLength(k);
            int n = k + r;

            int[,] matrix = new int[k, n];

            //Разбиение массива на отдельные строки
            for (int i = 0; i < k; i++)
            {
                int[] temp = new int[n];
                for (int j = 0; j < n; j++)
                {
                    temp[j] = masN[(n * i) + j];
                }

                //Запись строки в массив, для получения одной большой строки
                for (int j = 0; j < k; j++)
                {
                    masK[i * k + j] = temp[j];
                }

            }
            return masK;
        }

        static int[] AddCheckBits(int[] masK, int[] masN, int[,] checkMatrix)
        {
            int lenghtK = masK.Length; //Должна быть равна 2^n
            int lenghtN = masN.Length;
            int k = (int)(Math.Sqrt(lenghtK));
            int r = HemmingLength(k);
            int n = k + r;

            int[,] matrix = new int[k, n];

            //Разбиение массива на отдельные строки
            for (int i = 0; i < k; i++)
            {
                int[] temp = new int[n];
                for (int j = 0; j < k; j++)
                {
                    temp[j] = masK[(k * i) + j];
                }
                Sindrom(checkMatrix, temp, k);
                for (int j = 0; j < n; j++)
                {
                    masN[i * n + j] = temp[j];
                }

            }
            return masN;
        }

        static int[] Alternation(int[] masN, int k)
        {
            //int r = HemmingLength(k);
            int n = k + 2;

            int[,] matrix = new int[k, n];
            //Получение матрицы
            for (int i = 0, m = 0; i < k; i++)
            {
                for (int j = 0; j < n; j++, m++)
                {
                    matrix[i, j] = masN[m];
                }
            }
            Console.WriteLine("\n\nПолученая матрица");
            OutMatrix(matrix, k, n);

            //Перемежение
            for (int i = 0, m = 0; i < n; i++)
            {
                for (int j = 0; j < k; j++, m++)
                {
                    masN[m] = matrix[j, i];
                }
            }

            return masN;
        }

        static int[] ReAlternation(int[] masN, int k)
        {
            //int r = HemmingLength(k);
            int n = k + 2;

            int[,] matrix = new int[k, n];
            //Получение матрицы
            for (int j = 0, m = 0; j < n; j++)
            {
                for (int i = 0; i < k; i++, m++)
                {
                    matrix[i, j] = masN[m];
                }
            }
            Console.WriteLine("\n\nПолученая матрица");
            OutMatrix(matrix, k, n);

            //RE:Перемежение
            for (int j = 0, m = 0; j < k; j++)
            {
                for (int i = 0; i < n; i++, m++)
                {
                    masN[m] = matrix[j, i];
                }
            }

            return masN;
        }

        static int[] GenerationRandMasMod2(int[] mas)
        {
            Random rnd = new Random();

            for (int i = 0; i < mas.Length; i++)
            {
                mas[i] = rnd.Next(2);
            }
            return mas;
        }


        //Создание пров. матрицы
        static int[,] CheckMatrix(int k)
        {
            int r = HemmingLength(k);
            int n = r + k;
            double rDouble = r - 1;
            int rPow = (int)(Math.Pow(2, rDouble));

            int[,] mas = new int[n, r];

            int[,] combinations = new int[rPow, r];

            for (int i = 0; i < rPow; i++)
                for (int j = 0; j < r; j++)
                    combinations[i, j] = 0;

            //генератор бит.мн.
            for (int segmentLenght = 0; segmentLenght < r - 2; segmentLenght++)
            {
                if (segmentLenght * r > k) break;

                for (int i = 0; i < segmentLenght + 2; i++)
                {
                    combinations[segmentLenght * r, i] = 1;
                }

                for (int segmentPositin = 1; segmentPositin < r; segmentPositin++)
                {
                    for (int i = 0; i < r - 1; i++)
                    {
                        combinations[segmentLenght * r + segmentPositin, i + 1] = combinations[segmentLenght * r + segmentPositin - 1, i];
                    }
                    combinations[segmentLenght * r + segmentPositin, 0] = combinations[segmentLenght * r + segmentPositin - 1, r - 1];
                }

                if (segmentLenght == r - 3)
                {
                    for (int i = 0; i < r; i++)
                    {
                        combinations[rPow - 1, i] = 1;
                    }
                }
            }



            for (int i = 0; i < k; i++)
                for (int j = 0; j < r; j++)
                    mas[i, j] = combinations[i, j];

            for (int i = 0; i < r; i++)
                mas[i + k, i] = 1;

            return mas;
        }

        //Поиск синдрома
        static int[] Sindrom(int[,] CheckMatrix, int[] mas, int k)
        {

            int r = HemmingLength(k);
            int n = r + k;
            int[] sindrom = new int[r];



            for (int i = 0, l = 0; i < r; i++, l = 0)
            {
                for (int j = 0; j < k; j++)
                {
                    if (CheckMatrix[j, i] == 1 && mas[j] == 1) l++;
                    else sindrom[i] = 0;
                }
                if (l % 2 == 1) sindrom[i] = 1;
                else sindrom[i] = 0;
            }

            for (int i = 0; i < r; i++)
            {
                mas[i + k] = sindrom[i];
            }

            return mas;
        }

        //Считаем r (кол-во пров. симв.)
        static int HemmingLength(int k)
        {
            int r = (int)(Math.Log(k, 2) + 1.99f);
            return r;
        }

        //Нахождение ошибок
        static int[] SearchError(int[] mas, int[,] checkMatrix, int k)
        {

            int r = HemmingLength(k);
            int n = r + k;

            int[] beforeSindrom = new int[r];

            //запоминаем проверочные биты
            for (int i = k; i < n; i++)
            {
                beforeSindrom[i - k] = mas[i];
            }

            mas = Sindrom(checkMatrix, mas, k);

            //Складываем синдром по модулю два
            for (int i = k, j = 0; i < n; i++)
            {
                if (beforeSindrom[i - k].Equals(mas[i]))
                {
                    mas[i] = 0;

                    j++;
                    //если сумма по модулю два все пров. бит равна нулю
                    if (j == r)
                    {
                        for (int l = k; l < n; l++)
                        {
                            mas[l] = beforeSindrom[l - k];
                        }
                        return mas;
                    }
                }
                else
                {
                    mas[i] = 1;
                }
            }

            for (int i = 0; i < n; i++)
            {
                int l = 0;
                for (int j = 0; j < r; j++)
                {
                    if (checkMatrix[i, j].Equals(mas[j + k])) l++;
                }
                if (l == r)
                {
                    mas[i] = (mas[i] + 1) % 2;
                }
            }
            //OutMas(mas);
            mas = Sindrom(checkMatrix, mas, k);

            return mas;
        }

        static void OutMas(int[] mas)
        {
            for (int i = 0; i < mas.Length; i++)
            {
                Console.Write(mas[i]);
            }
        }
        //вывод матрицы
        static void OutMatrix(int[,] matrix, int k, int n)
        {
            for (int i = 0; i < k; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    Console.Write(matrix[i, j]);
                    //if (j + 1 == k) Console.Write("|");
                }
                Console.WriteLine();
            }
        }

        static void OutMatrixInv(int[,] matrix, int k, int n)
        {
            for (int j = 0; j < n; j++)
            {
                for (int i = 0; i < k; i++)
                {
                    Console.Write(matrix[i, j]);
                }
                Console.WriteLine();
            }
        }
    }
}