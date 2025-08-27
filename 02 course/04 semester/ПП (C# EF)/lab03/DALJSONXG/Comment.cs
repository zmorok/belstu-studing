namespace DALJSONXG
{
    public class Comment
    {
        public int Id { get; set; }
        public DateTime? Stamp { get; set; }
        public string Commtext { get; set; }
        public int WSrefId { get; set; }
        public WSRef WSref { get; set; }
    }
}
