namespace FreelanceApp.Models
{
    public enum UserRole
    {
        Client,
        Administrator
    }

    public class User
    {
        public required string Username { get; set; }
        public required string Password { get; set; }
        public UserRole Role { get; set; }
    }
}
