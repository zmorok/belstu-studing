using DAL_Celebrity_MSSQL;
using Microsoft.AspNetCore.Mvc.Filters;
using System.Text.Json;
namespace ASPA008_1.Filters
{ 
public class InfoAsyncActionFilter(string infotype = "") : Attribute, IAsyncActionFilter
{
    public const string Wikipedia = "WIKI";
    public const string Facebook = "FACE";

    private readonly string infotype = infotype.ToUpper();

    public async Task OnActionExecutionAsync(ActionExecutingContext context, ActionExecutionDelegate next)
    {
        IRepository? repo = context.HttpContext.RequestServices.GetService<IRepository>();
        int id = context.ActionArguments.TryGetValue("id", out var val) && val is int i ? i : -1;
        Celebrity? celebrity = repo?.GetCelebrityById(id);
        if (celebrity != null)
        {
            if (infotype.Contains(Wikipedia)) context.HttpContext.Items[Wikipedia] = await WikiInfoCelebrity.GetReferences(celebrity.FullName);
            if (infotype.Contains(Facebook)) context.HttpContext.Items[Facebook] = getFromFace(celebrity.FullName);
        }

        await next();
    }

    string getFromFace(string fullname) => "Info from Face";

    public class WikiInfoCelebrity
    {
            public static async Task<Dictionary<string, string>> GetReferences(string fullName)
            {
                string uri = $"https://en.wikipedia.org/w/api.php?action=opensearch&search={Uri.EscapeDataString(fullName)}&prop=info&format=json";

                using HttpClient client = new HttpClient();
                client.DefaultRequestHeaders.Add("User-Agent", "MyCelebrityApp/1.0 (contact@example.com)");
                HttpResponseMessage message = await client.GetAsync(uri);
                var references = new Dictionary<string, string>();

                if (message.StatusCode != System.Net.HttpStatusCode.OK) return references;

                JsonElement result = await message.Content.ReadFromJsonAsync<JsonElement>();
                var titles = result[1].EnumerateArray().Select(e => e.GetString()).ToList();
                var urls = result[3].EnumerateArray().Select(e => e.GetString()).ToList();

                for (int i = 0; i < titles.Count && i < urls.Count; i++)
                    if (!string.IsNullOrEmpty(titles[i]) && !string.IsNullOrEmpty(urls[i])) references[titles[i]!] = urls[i]!;
                
                return references;
            }
        }
    }
}