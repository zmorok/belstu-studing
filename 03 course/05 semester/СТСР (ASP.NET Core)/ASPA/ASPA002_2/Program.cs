using Microsoft.AspNetCore.HttpLogging;
using Microsoft.Extensions.FileProviders;

internal class Program
{
    private static void Main(string[] args)
    {
        var builder = WebApplication.CreateBuilder(args);
        builder.Services.AddHttpLogging(o =>
        {
            o.LoggingFields = HttpLoggingFields.All;
        });

        builder.Logging.AddFilter("Microsoft.AspNetCore.HttpLogging", LogLevel.Information);

        var app = builder.Build();

        app.UseHttpLogging();
        app.UseWelcomePage("/aspnetcore");

        app.UseDefaultFiles(new DefaultFilesOptions
        {
            DefaultFileNames = { "Neumann.html" }
        });
        
        app.UseStaticFiles();
        app.UseStaticFiles(new StaticFileOptions { RequestPath = "/static" });

        var picturePath = Path.Combine(app.Environment.ContentRootPath, "Picture");
        app.UseStaticFiles(new StaticFileOptions
        {
            FileProvider = new PhysicalFileProvider(picturePath),
            RequestPath = "/pictures"
        });


        app.MapGet("/aspnetcore", () => "блаблабла");
        app.MapGet("/privet", () => "poka");

        app.Run();
    }
}