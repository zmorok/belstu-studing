using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace DALMSSQLXG
{
    public class Comment
    {
        [Key]
        public int Id { get; set; }
        public DateTime? Stamp { get; set; }
        public string Commtext { get; set; }
        [ForeignKey("WSRef")]
        public int WSrefId { get; set; }

        public WSRef WSref { get; set; }
    }
}
