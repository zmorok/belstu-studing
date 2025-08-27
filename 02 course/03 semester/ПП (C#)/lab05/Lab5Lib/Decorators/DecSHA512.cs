using System;
using System.Security.Cryptography;
using System.Text;
using Lab5Lib.Writers;

namespace Lab5Lib.Decorators
{
    public class DecSHA512 : Decorator
    {
        public DecSHA512(IWriter writer) : base(writer) { }
        public override string Save(string message)
        {
            using var sha512 = SHA512.Create();
            var dataBytes = Encoding.UTF8.GetBytes(message ?? string.Empty);
            var hashedData = sha512.ComputeHash(dataBytes);
            var hashedMessage = Convert.ToBase64String(hashedData);
            var decoratedMessage = $"{message}{Constant.Delimiter}{hashedMessage}";
            return writer?.Save(decoratedMessage);
        }
    }
}
