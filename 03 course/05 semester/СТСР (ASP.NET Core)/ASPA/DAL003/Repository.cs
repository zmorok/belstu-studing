using System.Text.Json;
using System.IO;

namespace DAL003
{
    public class Repository : IRepository
    {
        private Repository(string path)
        {
            DataDir = path;
            CelebPath = Path.GetFullPath(Path.Combine(BasePath, "..", "..", "..", DataDir + "\\"));
            Load();
        }

        public static string? JSONFileName { get; set; } = "Celebrities.json";

        public string BasePath => AppContext.BaseDirectory;
        public string DataDir = string.Empty;
        public string? CelebPath { get; set; }
        public Celebrity[] celebrities { get; set; } = [];
        public static IRepository Create(string path) => new Repository(path);
        public void Dispose() { }
        private void Load() => celebrities = getAllCelebrities();
        public Celebrity[] getAllCelebrities()
        {
            string? json = CelebPath + JSONFileName;
            using var fs = new FileStream(json, FileMode.Open, FileAccess.Read);
            return JsonSerializer.Deserialize<Celebrity[]>(fs) ?? [];
        }

        public Celebrity[] getCelebritiesBySurname(string surname)  => [.. celebrities.Select(c => c).Where(c => c.Surname == surname)];
        public Celebrity? getCelebrityById(int id) => celebrities.FirstOrDefault(c => c.Id == id);
        public string? getPhotoPathById(int id) => getCelebrityById(id)?.PhotoPath;
    }
}
