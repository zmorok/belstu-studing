namespace SerializeLib
{
    public interface ISerializer
    {
        void Serialize<T>(T obj, string fileName);
        T Deserialize<T>(string fileName);
    }
}
