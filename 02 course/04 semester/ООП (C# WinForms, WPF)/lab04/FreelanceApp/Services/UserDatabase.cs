using FreelanceApp.Models;
using System.IO;
using System.Text.Json;

namespace FreelanceApp.Services
{
    // class for handling actions with users
    public static class UserDatabase
    {
        #region UserStore
        private static readonly string JsonFilePath = "data/users.json";
        private static List<User> Users = LoadUsersFromJson();
        #endregion

        #region Methods
        private static List<User> LoadUsersFromJson()
        {
            if (!File.Exists(JsonFilePath)) return new List<User>();
            string json = File.ReadAllText(JsonFilePath);
            return JsonSerializer.Deserialize<List<User>>(json) ?? new List<User>();
        }
        private static void SaveUsersToJson()
        {
            string json = JsonSerializer.Serialize(Users, new JsonSerializerOptions { WriteIndented = true });
            File.WriteAllText(JsonFilePath, json);
        }
        public static User? Authenticate(string username, string password)
        {
            return Users.FirstOrDefault(u => u.Username == username && u.Password == password);
        }
        public static bool Register(string username, string password, UserRole role)
        {
            if (Users.Any(u => u.Username == username)) return false;
            Users.Add(new User { Username = username, Password = password, Role = role });
            SaveUsersToJson();
            return true;
        }
        public static bool UpdateUser(User updatedUser)
        {
            User? existingUser = Users.FirstOrDefault(u => u.Username == updatedUser.Username);
            if (existingUser == null) return false;
            existingUser.Password = updatedUser.Password;
            existingUser.Role = updatedUser.Role;
            SaveUsersToJson();
            return true;
        }
        #endregion
    }
}