using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Interleaving
{
    class Program
    {
        //1 2 4 8
        static void Main(string[] args)
        {
            int messageLenght, //длина сообщения
                blockLenght, //длина блока
                hemmingLenght, //Количество проверочных символов
                numberBlocks, //количество  блоков
                getBlockLenght, //длина блока с проверочными символами
                lenghtErrorThread,
                positionErrorThread;

            int[,] blockMatrix; //матрица для генерации стороквый последовтельностей
            int[,] checkMatrix; //матрица для кода Хемминга

            int[,] deinterleavingMatrix;

            int[] informationMesage;
            int[] interleavingInformationMesage;
            Console.Write("Введите длину сообщения: ");
            messageLenght = int.Parse(Console.ReadLine());

            showDivisors(messageLenght);
            Console.Write("Введите длину блока: ");

            blockLenght = int.Parse(Console.ReadLine());
            hemmingLenght = HemmingLenght(blockLenght);
            numberBlocks = messageLenght / blockLenght;
            getBlockLenght = blockLenght + hemmingLenght;
            blockMatrix = new int[numberBlocks, getBlockLenght];
            checkMatrix = new int[hemmingLenght, getBlockLenght];

            deinterleavingMatrix = new int[numberBlocks, getBlockLenght];
            Console.WriteLine("Количество проверочных символов: " + hemmingLenght);

            GenerateblockMatrix(blockLenght, blockMatrix, out checkMatrix);

            informationMesage = new int[blockMatrix.GetLength(0) * blockMatrix.GetLength(1)];
            interleavingInformationMesage = new int[
                blockMatrix.GetLength(0) * blockMatrix.GetLength(1)
            ];

            FillInformationMessageWithCheckSymbols(informationMesage, blockMatrix);
            InterleavingInfMessage(interleavingInformationMesage, blockMatrix);
            Console.Write("Инф сообщение: \t\t\t ");
            OutStandartMass(informationMesage);

            Console.Write("Перемноженное Инф сообщение:\t ");
            OutStandartMass(interleavingInformationMesage);

            Console.Write("Введите позицию начала ошибочного потока: ");
            positionErrorThread = int.Parse(Console.ReadLine());

            Console.Write("Введите длину ошибочного потока: ");
            lenghtErrorThread = int.Parse(Console.ReadLine());

            DoErrorThread(interleavingInformationMesage, positionErrorThread, lenghtErrorThread);

            Console.Write("Перемноженное Инф сообщение с ошибочным потоком:\t ");
            OutStandartMass(interleavingInformationMesage);

            deinterleavingInfMessage(interleavingInformationMesage, deinterleavingMatrix);
            Console.WriteLine("Деперемноженное сообщение в матрице:");
            OutStandartMass(deinterleavingMatrix, blockLenght);

            //Исправить ошибки
            FixError(deinterleavingMatrix, checkMatrix, blockLenght);

            Console.WriteLine("Исправленное Деперемноженное сообщение в матрице:");
            OutStandartMass(deinterleavingMatrix, blockLenght);

            Console.Write("Отправленная информационная часть сообщения:\t ");
            OutThread(blockMatrix, blockLenght);

            Console.Write("Полученная   информационная часть сообщения:\t ");
            OutThread(deinterleavingMatrix, blockLenght);

            Console.Write("Сравнение  информационной части сообщений:\t ");
            CompareMessages(blockMatrix, deinterleavingMatrix, blockLenght);

            Console.ReadKey();
        }

        static void showDivisors(int number)
        {
            int maxDivisor = number / 2;
            int startValue = 2;
            bool fisrtValue = true;
            Console.Write("Данное сообщение можно разделить на блоки по : ");
            while (startValue < maxDivisor + 1)
            {
                //Console.WriteLine("number % startValue = " + (number % startValue).ToString());
                if (number % startValue == 0)
                {
                    if (fisrtValue)
                    {
                        Console.Write(startValue.ToString());
                        fisrtValue = false;
                    }
                    else
                        Console.Write(", " + startValue.ToString());
                }
                startValue++;
            }
            Console.WriteLine();
        }

        static int HemmingLenght(int k)
        {
            int r = (int)(Math.Log(k, 2)) + 1;
            return r;
        }

        static void GenerateblockMatrix(int blockLenght, int[,] blockMatrix, out int[,] checkMatrix)
        {
            Random rand = new Random();
            for (int i = 0; i < blockMatrix.GetLength(0); i++)
            {
                for (int j = 0; j < blockLenght; j++)
                {
                    blockMatrix[i, j] = rand.Next(2);
                    ;
                }
            }
            OutStandartMass(blockMatrix, blockLenght);

            checkMatrix = CheckMatrix(blockLenght);

            Console.WriteLine("Проверочная матрица:");
            OutStandartMass(checkMatrix, blockLenght);

            FillCheckSymbols(blockMatrix, checkMatrix, blockLenght);

            Console.WriteLine("Блочная матрица:");
            OutStandartMass(blockMatrix, blockLenght);
        }

        static void OutStandartMass(int[,] mass, int k)
        {
            for (int i = 0; i < mass.GetLength(0); i++)
            {
                for (int j = 0; j < mass.GetLength(1); j++)
                {
                    Console.Write(mass[i, j]);
                    if (j + 1 == k)
                        Console.Write('|');
                }
                Console.WriteLine();
            }
        }

        static void OutStandartMass(int[] mass)
        {
            for (int i = 0; i < mass.GetLength(0); i++)
            {
                Console.Write(mass[i]);
            }
            Console.WriteLine();
        }

        static void OutThread(int[,] mass, int k)
        {
            for (int i = 0; i < mass.GetLength(0); i++)
            {
                for (int j = 0; j < k; j++)
                {
                    Console.Write(mass[i, j]);
                    if (j + 1 == k)
                        Console.Write('|');
                }
            }
            Console.WriteLine();
        }

        static void CompareMessages(int[,] inter, int[,] deintr, int k)
        {
            for (int i = 0; i < inter.GetLength(0); i++)
            {
                for (int j = 0; j < k; j++)
                {
                    if (inter[i, j] == deintr[i, j])
                        Console.Write('0');
                    else
                        Console.Write('1');
                    if (j + 1 == k)
                        Console.Write('|');
                }
            }
            Console.WriteLine();
        }

        //Создание пров. матрицы
        public static int[,] CheckMatrix(int k)
        {
            int r = HemmingLenght(k);
            int n = r + k;
            double rDouble = r - 1;
            int rPow = (int)(Math.Pow(2, rDouble));

            int[,] mas = new int[r, n];

            int[,] combinations = new int[rPow, r];

            for (int i = 0; i < rPow; i++)
            for (int j = 0; j < r; j++)
                combinations[i, j] = 0;

            //генератор бит.мн.
            for (int segmentLenght = 0; segmentLenght < r - 2; segmentLenght++)
            {
                if (segmentLenght * r > k)
                    break;

                for (int i = 0; i < segmentLenght + 2; i++)
                {
                    combinations[segmentLenght * r, i] = 1;
                }

                for (int segmentPositin = 1; segmentPositin < r; segmentPositin++)
                {
                    for (int i = 0; i < r - 1; i++)
                    {
                        combinations[segmentLenght * r + segmentPositin, i + 1] = combinations[
                            segmentLenght * r + segmentPositin - 1,
                            i
                        ];
                    }
                    combinations[segmentLenght * r + segmentPositin, 0] = combinations[
                        segmentLenght * r + segmentPositin - 1,
                        r - 1
                    ];
                }

                if (segmentLenght == r - 3)
                {
                    for (int i = 0; i < r; i++)
                    {
                        combinations[rPow - 1, i] = 1;
                    }
                }
            }

            //for (int i = 0; i < k; i++)
            //    for (int j = 0; j < r; j++)
            //        mas[i, j] = combinations[i, j];

            for (int i = 0; i < k; i++)
            for (int j = 0; j < r; j++)
                mas[j, i] = combinations[i, j];

            for (int i = 0; i < r; i++)
                mas[i, i + k] = 1;

            return mas;
        }

        static void FillCheckSymbols(int[,] blockMatrix, int[,] checkMatrix, int blockLenght)
        {
            int count = 0;
            for (int i = 0; i < blockMatrix.GetLength(0); i++)
            {
                for (int iC = 0; iC < checkMatrix.GetLength(0); iC++)
                {
                    for (int cK = 0; cK < blockLenght; cK++)
                    {
                        count += checkMatrix[iC, cK] * blockMatrix[i, cK];
                    }
                    blockMatrix[i, blockLenght + iC] = count % 2;
                    count = 0;
                }
            }
        }

        static void FillInformationMessageWithCheckSymbols(int[] infMessage, int[,] blockMatrix)
        {
            for (int i = 0; i < blockMatrix.GetLength(0); i++)
            {
                for (int j = 0; j < blockMatrix.GetLength(1); j++)
                    infMessage[i * blockMatrix.GetLength(1) + j] = blockMatrix[i, j];
            }
        }

        static void InterleavingInfMessage(int[] interleavingInformationMesage, int[,] blockMatrix)
        {
            for (int j = 0; j < blockMatrix.GetLength(1); j++)
            {
                for (int i = 0; i < blockMatrix.GetLength(0); i++)
                    interleavingInformationMesage[j * blockMatrix.GetLength(0) + i] = blockMatrix[
                        i,
                        j
                    ];
            }
        }

        static void DoErrorThread(
            int[] interleavingInformationMesage,
            int positionErrorThread,
            int lenghtErrorThread
        )
        {
            for (int i = 0; i < lenghtErrorThread; i++)
            {
                if (interleavingInformationMesage[positionErrorThread - 1 + i] == 0)
                    interleavingInformationMesage[positionErrorThread - 1 + i] = 1;
                else
                    interleavingInformationMesage[positionErrorThread - 1 + i] = 0;
            }
        }

        static void deinterleavingInfMessage(
            int[] interleavingInformationMesage,
            int[,] deinterleavingMatrix
        )
        {
            for (int j = 0; j < deinterleavingMatrix.GetLength(1); j++)
            {
                for (int i = 0; i < deinterleavingMatrix.GetLength(0); i++)
                    deinterleavingMatrix[i, j] = interleavingInformationMesage[
                        j * deinterleavingMatrix.GetLength(0) + i
                    ];
            }
        }

        static void FixError(int[,] deinterleavingMatrix, int[,] checkMatrix, int blockLenght)
        {
            //Запоминаем полученные проверочные биты блоков
            int[,] getCheckBits = new int[
                deinterleavingMatrix.GetLength(0),
                checkMatrix.GetLength(0)
            ];

            for (int i = 0; i < deinterleavingMatrix.GetLength(0); i++)
            for (int j = 0; j < checkMatrix.GetLength(0); j++)
                getCheckBits[i, j] = deinterleavingMatrix[i, blockLenght + j];

            //Пересчитываем проверочные биты
            FillCheckSymbols(deinterleavingMatrix, checkMatrix, blockLenght);
            Console.WriteLine("Блочная матрица:");
            OutStandartMass(deinterleavingMatrix, blockLenght);

            //Определяем и исправляем ошибки
            for (int i = 0; i < deinterleavingMatrix.GetLength(0); i++)
            {
                FixRowError(i);
            }

            void FixRowError(int rowIndex)
            {
                int[] sindom = new int[checkMatrix.GetLength(0)];
                int numberError = 0;
                //Складываем синдромы по модулю 2
                for (int cB = 0; cB < checkMatrix.GetLength(0); cB++)
                {
                    if (
                        deinterleavingMatrix[rowIndex, blockLenght + cB]
                        == getCheckBits[rowIndex, cB]
                    )
                    {
                        //sindom[cB] = 0;
                        if (cB + 1 == checkMatrix.GetLength(0) && numberError == 0)
                            return;
                    }
                    else
                    {
                        sindom[cB] = 1;
                        numberError++;
                    }
                }

                OutStandartMass(sindom);

                //Опрделяем и исправялем ошибку по синдрому
                int flag = 0;
                for (int j = 0; j < checkMatrix.GetLength(1); j++)
                {
                    flag = 0;
                    for (int i = 0; i < checkMatrix.GetLength(0); i++)
                    {
                        if (checkMatrix[i, j] == sindom[i])
                            flag++;
                    }
                    if (flag == checkMatrix.GetLength(0))
                    {
                        deinterleavingMatrix[rowIndex, j]++;
                        deinterleavingMatrix[rowIndex, j] %= 2;
                        return;
                    }
                }
            }
        }
    }
}
