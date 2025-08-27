namespace StudentLib
{
    public class Student
    {
        private static int _id = 0; 
        public int Id { get; }
        public string Name { get; set; }
        public string Major { get; set; }

        public Student(string name, string major)
        {
            Id = ++_id;
            Name = name;
            Major = major;
        }

        public override string ToString()
        {
            return $"ID: {Id}, ФИО: {Name}, НАПРАВЛЕНИЕ: {Major}";
        }
    }
}
