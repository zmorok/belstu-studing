using System;
using System.Security.Cryptography;
using System.Text;
using Lab5Lib.Writers;

namespace Lab5Lib.Decorators
{
    public class DecMD5 : Decorator
    {
        public DecMD5(IWriter writer) : base(writer) { }
        public override string Save(string message)
        {
            using var md5 = MD5.Create();
            var dataBytes = Encoding.UTF8.GetBytes(message ?? string.Empty);
            var encryptedData = md5.ComputeHash(dataBytes);
            var hashedMessage = Convert.ToBase64String(encryptedData);
            var decoratedMessage = $"{message}{Constant.Delimiter}{hashedMessage}";
            return writer?.Save(decoratedMessage);
        }
    }
}
