using System.ComponentModel.DataAnnotations;

namespace FreelanceApp.Models
{
    public class Freelancer
    {
        [Key]
        public int Freelancer_Id { get; set; }
        public required string Name { get; set; }
        public required string Skills { get; set; }
        public double Rating { get; set; }
        public string Description { get; set; }
    }
}
