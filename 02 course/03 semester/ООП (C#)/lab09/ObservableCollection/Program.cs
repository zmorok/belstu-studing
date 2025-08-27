using System.Collections.ObjectModel;
using StudentLib;

namespace ObservableCollection
{
    internal partial class Program
    {
        static void Main(string[] args)
        {
            ObservableCollection<Student> students = new ObservableCollection<Student>();

            students.CollectionChanged += OnCollectionChanged;

            students.Add(new Student("Гриша", "наука 1"));
            students.Add(new Student("Боббби", "мисье бензопила"));

            students.RemoveAt(1); 

            students.Add(new Student("Кивик", "а фон Бернантович"));

            students[1] = new Student("замен", "бубу бебе");

            students.Clear();
        }
    }
}
