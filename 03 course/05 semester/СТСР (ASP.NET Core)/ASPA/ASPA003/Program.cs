using DAL003;
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

        builder.Services.AddDirectoryBrowser();

        var app = builder.Build();

        app.UseHttpLogging();

        var picturePath = Path.Combine(app.Environment.ContentRootPath, "Celebrities");
        var photoProvider = new PhysicalFileProvider(picturePath);
        app.UseStaticFiles(new StaticFileOptions
        {
            FileProvider = photoProvider,
            RequestPath = "/Photo"
        });

        app.UseDirectoryBrowser(new DirectoryBrowserOptions
        {
            FileProvider = photoProvider,
            RequestPath = "/Celebrities/download"
        });

        app.UseStaticFiles(new StaticFileOptions
        {
            FileProvider = photoProvider,
            RequestPath = "/Celebrities/download",
            OnPrepareResponse = ctx =>
            {   
                if (ctx.Context.Request.Headers.ContainsKey("Referer"))
                {
                    ctx.Context.Response.Headers.Append("Content-Disposition", "attachment");
                }
                else { }
            }
        });

        Repository.JSONFileName = "Celebrities.json";
        using (IRepository repository = Repository.Create("Celebrities"))
        {
            app.MapGet("/Celebrities", () => repository.getAllCelebrities());
            app.MapGet("/Celebrities/{id:int}", (int id) => repository.getCelebrityById(id));
            app.MapGet("/Celebrities/BySurname/{surname}", (string surname) => repository.getCelebritiesBySurname(surname));
            app.MapGet("/Celebrities/PhotoPathById/{id:int}", (int id) => repository.getPhotoPathById(id));
            app.MapGet("/", () => "Hello, World!");
            app.Run();
        }
    }
}