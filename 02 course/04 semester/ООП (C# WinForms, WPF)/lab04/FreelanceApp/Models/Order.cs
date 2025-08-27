using System.ComponentModel.DataAnnotations;

namespace FreelanceApp.Models
{
    public class Order
    {
        [Key]
        public int Order_Id { get; set; }

        [Required(ErrorMessage = "Заголовок необходим")]
        [StringLength(100, ErrorMessage = "Длина заголовка не должна превышать 100 символов.")]
        public required string Title { get; set; }

        [Required(ErrorMessage = "Описание необходимо")]
        [StringLength(500, ErrorMessage = "Описание не должно превышать 500 символов.")]
        public required string Description { get; set; }

        [Required(ErrorMessage = "Цена обязательна")]
        [Range(0.01, double.MaxValue, ErrorMessage = "Цена должна быть больше 0.")]
        [RegularExpression(@"^\d+([.,]\d{1,2})?$", ErrorMessage = "Цена должна быть числом с не более чем 2 знаками после запятой.")]
        public decimal Price { get; set; }

        [RegularExpression(@"^(Разработка|Дизайн|Маркетинг|Контент|Разное)$")]
        public required string Category { get; set; }

        public string FullDescription { get; set; }
    }
}