namespace DAL_LES
{
    public class Celebrity
    {
        public Celebrity() { FullName = ""; Nationality = "XX"; }
        public int Id { get; set; }
        public string FullName { get; set; }
        public string Nationality { get; set; }
        public string? ReqPhotoPath { get; set; }

        public ICollection<Lifeevent> Lifeevents { get; set; } = new List<Lifeevent>();  // связь событий с личностью
    }

    public class Lifeevent
    {
        public Lifeevent() { Description = ""; Date = DateTime.Now; }
        public int Id { get; set; }
        public int CelebrityId { get; set; }
        public DateTime Date { get; set; }
        public string Description { get; set; }
        public string? ReqPhotoPath { get; set; }

        public Celebrity Celebrity { get; set; } = null!; // связь личности с событиями
    }
}
