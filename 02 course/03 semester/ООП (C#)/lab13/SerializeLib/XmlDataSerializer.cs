using System.IO;
using System.Xml.Serialization;
using lab04;

namespace SerializeLib
{
    public class XmlDataSerializer : ISerializer
    {
        public void Serialize<T>(T obj, string fileName)
        {
            using (FileStream fs = new FileStream(fileName, FileMode.Create))
            {
                XmlSerializer formatter = new XmlSerializer(obj.GetType());
                formatter.Serialize(fs, obj);
            }
        }
        public T Deserialize<T>(string fileName)
        {
            using (FileStream fs = new FileStream(fileName, FileMode.Open))
            {
                XmlSerializer formatter = new XmlSerializer(typeof(Tablet));
                return (T)formatter.Deserialize(fs);
            }
        }
    }
}
