using Lab5Lib.Writers;

namespace Lab5Lib
{
    public class Decorator: IWriter
    {
        protected IWriter? writer;
        public Decorator(IWriter writer)
        {
            this.writer = writer;
        }
        public virtual string? Save(string? message)
        {
            return writer?.Save(message);
        }
    }
}
