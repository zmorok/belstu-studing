using System.IO;
using System.Runtime.Serialization.Formatters.Soap;

namespace SerializeLib
{
    public class SoapDataSerializer : ISerializer
    {
        public void Serialize<T>(T obj, string fileName)
        {
            using (FileStream fs = new FileStream(fileName, FileMode.Create))
            {
                SoapFormatter formatter = new SoapFormatter();
                formatter.Serialize(fs, obj);
            }
        }

        public T Deserialize<T>(string fileName)
        {
            using (FileStream fs = new FileStream(fileName, FileMode.Open))
            {
                SoapFormatter formatter = new SoapFormatter();
                return (T)formatter.Deserialize(fs);
            }
        }
    }
}
