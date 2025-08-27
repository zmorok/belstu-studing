using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace var05
{
    public partial class Abiturient
    {
        // уникальный ID, хэш
        private readonly int _id;

        // константы
        private const int MinGrade = 1;
        private const int MaxGrade = 10;

        // подсчет созданных объектов
        private static int _count;

        // закрытых поля
        private string _surname;
        private string _name;
        private string _lastName;
        private string _address;
        private string _phone;
        private int[] _grades;

        public int Id
        {
            get { return _id; }
        }

        public string Surname
        {
            get { return _surname; }
            set { _surname = value; }
        }

        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        public string LastName
        {
            get { return _lastName; }
            set { _lastName = value; }
        }

        public string Address
        {
            get { return _address; }
            set { _address = value; }
        }

        public string Phone
        {
            get { return _phone; }
            set { _phone = value; }
        }

        // свойство с ограниченным доступом по set
        public int[] Grades
        {
            get { return _grades; }
            set
            {
                if (value == null || value.Length == 0)
                    throw new ArgumentException("Оценка не может быть пустой.");
                foreach (var grade in value)
                {
                    if (grade < MinGrade || grade > MaxGrade)
                        throw new ArgumentException(
                            $"Оценка должна быть между {MinGrade} и {MaxGrade}."
                        );
                }
                _grades = value;
            }
        }

        // конструктор без параметров
        public Abiturient()
        {
            _id = GenerateUniqueId();
            _count++;
        }

        // конструктор с параметрами
        public Abiturient(
            string surname,
            string name,
            string lastName,
            string address,
            string phone,
            int[] grades
        )
        {
            Surname = surname;
            Name = name;
            LastName = lastName;
            Address = address;
            Phone = phone;
            Grades = grades;
        }

        // конструктор с параметрами по умолчанию
        public Abiturient(string surname, string name, string lastName)
            : this(surname, name, lastName, "Unknown", "Unknown", new int[0]) { }

        // закрытый конструктор
        private Abiturient(int id)
        {
            _id = id;
        }

        // статический конструктор
        static Abiturient()
        {
            _count = 0;
            Console.WriteLine("статический конструктор класса Abiturient");
        }

        // метод вывода информации о классе
        public static void PrintClassInfo()
        {
            Console.WriteLine($"Число созданных абитуриентов: {_count}");
        }

        // ref- и out-параметры
        public void ProcessGrades(ref int[] gradesArray, out double average)
        {
            gradesArray = _grades;
            average = CalculateAverageGrade();
        }

        // вычисление среднего балла
        public double CalculateAverageGrade()
        {
            if (_grades.Length == 0)
                return 0;
            double sum = 0;
            foreach (var grade in _grades)
            {
                sum += grade;
            }
            return sum / _grades.Length;
        }

        // переопределение методов Object:
        // Equals
        public override bool Equals(object obj)
        {
            if (obj is Abiturient other)
            {
                return _id == other._id;
            }
            return false;
        }

        // GetHashCode
        public override int GetHashCode()
        {
            return _id.GetHashCode();
        }

        // ToString
        public override string ToString()
        {
            return $"Абитуриент {Surname} {Name} {LastName}:\n\t Адресс: {Address}, Номер телефона: {Phone}, Оценки: {string.Join(", ", _grades)}\n";
        }

        // генерация уникального ID
        private static int GenerateUniqueId()
        {
            return _count + 1;
        }
    }
}
