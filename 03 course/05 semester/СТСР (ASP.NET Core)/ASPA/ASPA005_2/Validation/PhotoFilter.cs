using DAL004;

namespace ASPA005_2.Validation
{
    public class PhotoFilter : IEndpointFilter
    {
        public static IRepository repository = default!;

        public async ValueTask<object?> InvokeAsync(EndpointFilterInvocationContext context, EndpointFilterDelegate next)
        {
            Celebrity? celebrity = context.GetArgument<Celebrity>(0) ?? throw new NullCelebrityException("/Celebrities error, celebrity is null");

            string basePath = AppContext.BaseDirectory;
            string fileName = Path.GetFileName(celebrity.PhotoPath);
            string fullPath = Path.Combine(basePath, fileName);

            if (!File.Exists(fullPath))
                context.HttpContext.Response.Headers.Append("X-Celebrity", $"NotFound = {fileName}");

            return await next(context);
        }
    }
}
