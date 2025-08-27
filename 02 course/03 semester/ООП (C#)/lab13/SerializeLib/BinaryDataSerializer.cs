using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace SerializeLib
{
    public class BinaryDataSerializer : ISerializer
    {
        public void Serialize<T>(T obj, string fileName)
        {
            using (FileStream fs = new FileStream(fileName, FileMode.Create))
            {
                BinaryFormatter formatter = new BinaryFormatter();
                formatter.Serialize(fs, obj);
            }
        }
        public T Deserialize<T>(string fileName)
        {
            using (FileStream fs = new FileStream(fileName, FileMode.Open))
            {
                BinaryFormatter formatter = new BinaryFormatter();
                return (T)formatter.Deserialize(fs);
            }
        }
    }
}
