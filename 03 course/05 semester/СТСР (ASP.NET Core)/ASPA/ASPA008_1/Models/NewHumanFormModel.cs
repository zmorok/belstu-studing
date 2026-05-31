namespace ASPA008_1.Models
{
    public enum FormMode { Create, Edit, Delete }

    public class NewHumanFormModel
    {
        public FormMode Mode { get; set; } = FormMode.Create;
        public int? Id { get; set; }
        public string? FullName { get; set; }
        public string? Nationality { get; set; }
        public string? TempFileName { get; set; }
        public bool Confirm { get; set; }
        public string PhotosRequestPath { get; set; } = "";
    }
}