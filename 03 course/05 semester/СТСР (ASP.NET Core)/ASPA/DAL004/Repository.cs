using System.Text.Json;
using System.IO;

namespace DAL004
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
        private int _changes = 0;
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

        public int? addCelebrity(Celebrity celebrity)
        {
            if (celebrity.Id > 0 && celebrities.Any(c => c.Id == celebrity.Id)) return null;
            int newId = celebrity.Id > 0 ? celebrity.Id : (celebrities.Length == 0 ? 1 : celebrities.Max(c => c.Id) + 1);

            var list = celebrities.ToList();
            list.Add(new Celebrity(newId, celebrity.Firstname, celebrity.Surname, celebrity.PhotoPath));
            celebrities = [.. list];

            _changes++;
            return newId;
        }

        public bool delCelebrityById(int id)
        {
            int idx = Array.FindIndex(celebrities, c => c.Id == id);
            if (idx < 0) return false;

            var list = celebrities.ToList();
            list.RemoveAt(idx);
            celebrities = [.. list];

            _changes++;
            return true;
        }


        public int? updCelebrityById(int id, Celebrity celebrity)
        {
            int idx = Array.FindIndex(celebrities, c => c.Id == id);
            if (idx < 0) return null;

            int targetId = celebrity.Id == 0 ? id : celebrity.Id;

            if (targetId != id && celebrities.Any(c => c.Id == targetId)) return null;

            var list = celebrities.ToList();
            list[idx] = new Celebrity(targetId, celebrity.Firstname, celebrity.Surname, celebrity.PhotoPath);
            celebrities = [.. list];

            _changes++;
            return targetId;
        }


        public int SaveChanges()
        {
            string? json = CelebPath + JSONFileName;

            using (var fs = new FileStream(json, FileMode.Create, FileAccess.Write, FileShare.None))
            {
                JsonSerializer.Serialize(fs, celebrities, new JsonSerializerOptions { WriteIndented = true });
            }

            int changed = _changes;
            _changes = 0;
            return changed;
        }
    }
}
