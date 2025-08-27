namespace Lec04LibN
{
    public interface ILogger
    {
        void Start(string title);
        void Log(string message);
        void Stop();
    }
}
