using System;

namespace matrix_mult
{
    public static class Matrix
    {
        static readonly Random random = new Random();
        public static int size = 0;
        public static double[,] matrixA = GenerateMatrix(size, random);
        public static double[,] matrixB = GenerateMatrix(size, random);

        static double[,] GenerateMatrix(int size, Random random)
        {
            double[,] matrix = new double[size, size];
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    matrix[i, j] = random.NextDouble() * 10;
                }
            }
            return matrix;
        }

        public static void SetSize(int _size)
        {
            size = _size;
            matrixA = GenerateMatrix(size, random);
            matrixB = GenerateMatrix(size, random);
        }
    }
}
