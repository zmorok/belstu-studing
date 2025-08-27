namespace DALJSONXG
{
    public class WSRef
    {
        public int Id { get; set; }
        public string Url { get; set; }
        public string Description { get; set; }
        public int Plus { get; set; }
        public int Minus { get; set; }
        public List<Comment> Comments { get; set; } = new List<Comment>();
    }
}
