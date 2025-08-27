using System;
using System.IO;

namespace Tecnologies
{
    public class ExceptionDataInit : Exception
    {
        public ExceptionDataInit(string msg)
            : base(msg) { }
    }

    public class ExceptionDivision : DivideByZeroException
    {
        public ExceptionDivision(string msg)
            : base(msg) { }
    }

    public class ExceptionFile : IOException
    {
        public ExceptionFile(string msg)
            : base(msg) { }
    }
}
