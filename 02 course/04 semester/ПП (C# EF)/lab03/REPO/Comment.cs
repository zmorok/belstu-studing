using System.ComponentModel.DataAnnotations.Schema;
using System.ComponentModel.DataAnnotations;

namespace REPO
{
    public class Comment
    {
        [Key]
        public int? Id { get; set; }

        [ForeignKey("WSRef")]
        public int? WSrefId { get; set; }
        public int? Session_Id { get; set; }
        public DateTime? Stamp { get; set; }
        public string? Commtext { get; set; }

        public WSRef WSRef { get; set; }
    }
}
