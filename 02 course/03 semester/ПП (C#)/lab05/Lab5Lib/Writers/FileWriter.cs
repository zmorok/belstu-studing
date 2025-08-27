using System.IO;

namespace Lab5Lib.Writers
{
    public class FileWriter : IWriter
    {
        private readonly string _filename = Constant.FileName;
        public string Filename
        {
            get { return _filename; }
        }

        public FileWriter(string? filename = null)
        {
            _filename = filename ?? Constant.FileName;
        }

        public string? Save(string? message)
        {
            File.WriteAllText(_filename, message);

            return _filename;
        }
    }
}
