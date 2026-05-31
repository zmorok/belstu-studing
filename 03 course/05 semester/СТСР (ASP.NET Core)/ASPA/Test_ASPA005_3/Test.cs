using System.Net.Http.Json;
using System.Text.Json;

namespace Test_ASPA005_3
{
    public class Test
    {
        class Answer<T>
        {
            public T? x { get; set; } = default;
            public T? y { get; set; } = default;
            public string? message { get; set; } = null;
        }

        public static string OK = "OK", NOK = "NOK";
        readonly HttpClient client = new HttpClient();

        public async Task ExecuteGET<T>(string path, Func<T?, T?, int, string> result) => await resultPRINT<T>("GET", path, await this.client.GetAsync(path), result); 

        public async Task ExecutePOST<T>(string path, Func<T?, T?, int, string> result) => await resultPRINT<T>("POST", path, await this.client.PostAsync(path, null), result);
        
        public async Task ExecutePUT<T>(string path, Func<T?, T?, int, string> result) => await resultPRINT<T>("PUT", path, await this.client.PutAsync(path, null), result);
        
        public async Task ExecuteDELETE<T>(string path, Func<T?, T?, int, string> result) => await resultPRINT<T>("DELETE", path, await this.client.DeleteAsync(path), result);

        async Task resultPRINT<T>(string method, string path, HttpResponseMessage rm, Func<T?, T?, int, string> result)
        {
            int status = (int)rm.StatusCode;
            try
            {
                Answer<T>? answer = await rm.Content.ReadFromJsonAsync<Answer<T>>() ?? default(Answer<T>);
                string r = result(default(T), default(T), status);
                T? x = default(T), y = default(T);

                if (answer != null) r = result(x = answer.x, y = answer.y, status);

                Console.WriteLine($" {r}: {method} {path}, status = {status}, x = {x}, y = {y}, m = {answer?.message}");
            }
            catch (JsonException ex)
            {
                string r = result(default(T), default(T), status);
                Console.WriteLine($" {r}: {method} {path}, status = {status}, x = null, y = null, m = {ex.Message}");
            }
        }
    }
}
