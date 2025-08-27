using System.Security.Cryptography;
using System.Text;

class Program
{
    static void Main()
    {
        string surname = "Жук";
        string outputDir = "cipherData";
        Directory.CreateDirectory(outputDir);

        byte[] cipherText; // шифртекст
        byte[] desKey;     // ключ DES
        byte[] desIV;      // IV DES

        // === 1. DES шифрование ===
        using (DES des = DES.Create())
        {
            des.GenerateKey(); des.GenerateIV();
            desKey = des.Key; desIV = des.IV;

            using MemoryStream ms = new();
            using (ICryptoTransform enc = des.CreateEncryptor(desKey, desIV))
            using (CryptoStream cs = new(ms, enc, CryptoStreamMode.Write))
            using (StreamWriter sw = new(cs, Encoding.UTF8))
            {
                sw.Write(surname);       // фамилия внутрь крипто‑потока
            }
            cipherText = ms.ToArray();
        }

        File.WriteAllBytes(Path.Combine(outputDir, "des_key.bin"), desKey);
        File.WriteAllBytes(Path.Combine(outputDir, "des_iv.bin"), desIV);
        File.WriteAllBytes(Path.Combine(outputDir, "surname_des_cipher.bin"), cipherText);

        // === 2. DES проверка расшифровки ===
        string decrypted;
        using (DES des = DES.Create())
        using (ICryptoTransform dec = des.CreateDecryptor(desKey, desIV))
        using (MemoryStream ms = new(cipherText))
        using (CryptoStream cs = new(ms, dec, CryptoStreamMode.Read))
        using (StreamReader sr = new(cs, Encoding.UTF8))
        {
            decrypted = sr.ReadToEnd();
        }
        Console.WriteLine($"Расшифрованный текст: {decrypted}");

        // === 3. SHA‑384 hashing ===
        byte[] shaHash;
        shaHash = SHA384.HashData(Encoding.UTF8.GetBytes(surname));
        File.WriteAllBytes(Path.Combine(outputDir, "surname_sha384.bin"), shaHash);

        // === 4. Вывод в консоль в HEX‑формате ===
        Console.WriteLine("\nHEX REPRESENTATIONS:");
        Console.WriteLine($"DES Ключ    : {ToHex(desKey)}");
        Console.WriteLine($"DES IV      : {ToHex(desIV)}");
        Console.WriteLine($"Шифртекст   : {ToHex(cipherText)}");
        Console.WriteLine($"SHA‑384 хеш : {ToHex(shaHash)}");

        // === 5. RSA‑подпись хеша ===
        using RSA rsa = RSA.Create(2048);
        byte[] signature = rsa.SignHash(shaHash, HashAlgorithmName.SHA384, RSASignaturePadding.Pkcs1);
        File.WriteAllBytes(Path.Combine(outputDir, "signature.bin"), signature);

        bool ok = rsa.VerifyHash(shaHash, signature, HashAlgorithmName.SHA384, RSASignaturePadding.Pkcs1);
        Console.WriteLine($"\nВалидность подписи (оригинал): {(ok ? "Валидно" : "Не валидно")}");

        // меняем сообщение
        byte[] tamperedHash;
        tamperedHash = SHA384.HashData(Encoding.UTF8.GetBytes(surname + "X"));
        bool okTamperedMsg = rsa.VerifyHash(tamperedHash, signature, HashAlgorithmName.SHA384, RSASignaturePadding.Pkcs1);
        Console.WriteLine($"Валидность подписи после изменения сообщения: {(okTamperedMsg ? "Валидно" : "Не валидно")}");

        // меняем подпись
        byte[] badSignature = (byte[])signature.Clone();
        badSignature[0] ^= 0xFF;
        bool okTamperedSig = rsa.VerifyHash(shaHash, badSignature, HashAlgorithmName.SHA384, RSASignaturePadding.Pkcs1);
        Console.WriteLine($"Валидность подписи после изменения подписи: {(okTamperedSig ? "Валидно" : "Не валидно")}");
    }
    static string ToHex(byte[] data) => BitConverter.ToString(data);
}
