using System.Text;
namespace Lempel_Ziv_method;

class Program
{
    static void Main(string[] args)
    {
        Console.OutputEncoding = Encoding.UTF8;
        
        var compressor = new LZ77Compressor();
        compressor.Run();
    }
}