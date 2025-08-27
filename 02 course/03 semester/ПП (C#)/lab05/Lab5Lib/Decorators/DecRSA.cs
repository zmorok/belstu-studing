using System;
using System.Security.Cryptography;
using Lab5Lib.Writers;

namespace Lab5Lib.Decorators
{
    public class DecRSA : Decorator
    {
        public DecRSA(IWriter writer) : base(writer) { }
        public override string Save(string message)
        {
            string publicKeyXml;
            byte[] encyptedData;
            message ??= string.Empty;
            int k1 = message.IndexOf(Constant.Delimiter);
            if (k1 == -1) throw new Exception("delimiter not found");
            string xs = message.Substring(0, k1);
            string xsbhcs = message.Substring(k1 + 1);
            byte[] xsbhc = Convert.FromBase64String(xsbhcs);
            using (RSACryptoServiceProvider rsa = new RSACryptoServiceProvider())
            {
                publicKeyXml = rsa.ToXmlString(true);
                rsa.ImportParameters(rsa.ExportParameters(false));
                encyptedData = rsa.Encrypt(xsbhc, false);
            }
            string result = $"{xs}{Constant.Delimiter}{Convert.ToBase64String(encyptedData)}{Constant.Delimiter}{publicKeyXml}";
            return writer?.Save(result);
        }
    }
}
