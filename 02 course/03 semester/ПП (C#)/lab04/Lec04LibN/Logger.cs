using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Lec04LibN
{
    public class Logger : ILogger
    {
        private static Logger instance;
        private readonly string logFileName = $"LOG_{DateTime.Now:yyyyMMdd-HH-mm-ss}.txt";
        private readonly StreamWriter writer;
        private int logCounter = 0;
        private readonly Stack<string> namespaceStack = new Stack<string>();

        private Logger()
        {
            writer = new StreamWriter(logFileName);
            WriteLine("INIT");
        }

        public static Logger Create()
        {
            if (instance == null)
            {
                instance = new Logger();
            }
            return instance;
        }

        public void Start(string title)
        {
            namespaceStack.Push(title);
            WriteLine($"STRT {GetNamespacePath()}");
        }

        public void Log(string message)
        {
            WriteLine($"INFO {GetNamespacePath()} {message}");
        }

        public void Stop()
        {
            if (namespaceStack.Count > 0)
            {
                namespaceStack.Pop();
            }
            WriteLine($"STOP {GetNamespacePath()}");
        }

        private void WriteLine(string message)
        {
            writer.WriteLine($"{++logCounter:D6}-{DateTime.Now:dd.MM.yyyy HH:mm:ss}-{message}");
            writer.Flush();
        }

        private string GetNamespacePath()
        {
            if (namespaceStack.Count == 0)
            {
                return string.Join(":", namespaceStack.Reverse());
            }
            return string.Join(":", namespaceStack.Reverse()) + ":";
        }
    }
}
