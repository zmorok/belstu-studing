using System.IO;
using System.Text.Json;

namespace SerializeLib
{
    public class JsonDataSerializer : ISerializer
    {
        public void Serialize<T>(T obj, string fileName)
        {
            string json = JsonSerializer.Serialize(obj);
            File.WriteAllText(fileName, json);
        }
        public T Deserialize<T>(string fileName)
        {
            string json = File.ReadAllText(fileName);
            return JsonSerializer.Deserialize<T>(json);
        }
    }
}
