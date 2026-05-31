using ASPA008_1.Helpers;
using Microsoft.AspNetCore.HttpLogging;
using Microsoft.Extensions.FileProviders;

internal class Program
{
    private static void Main(string[] args)
    {

        var builder = WebApplication.CreateBuilder(args);
        builder.AddCelebrityServices();
        builder.AddCelebritiesConfig();

        builder.Services.AddHttpLogging(o => { o.LoggingFields = HttpLoggingFields.All; });
        builder.Logging.AddFilter("Microsoft.AspNetCore.HttpLogging", LogLevel.Information);

        IConfiguration configuration = new ConfigurationBuilder().AddJsonFile("Celebrities.config.json").Build();
        builder.Services.AddControllersWithViews();

        var app = builder.Build();


        if (!app.Environment.IsDevelopment())
        {
            app.UseExceptionHandler("/Home/Error");

            app.UseHsts();
        }

        app.UseHttpsRedirection();
        app.UseHttpLogging();
        app.UseStaticFiles();
        app.UseStaticFiles(new StaticFileOptions
        {
            FileProvider = new PhysicalFileProvider(builder.Configuration["Celebrities:PhotosFolder"]),
            RequestPath = builder.Configuration["Celebrities:PhotosRequestPath"]
        });

        app.UseRouting();

        app.UseCelebErrorHandler("ASPA008_1");
        app.MapCelebrities(configuration);


        app.UseAuthorization();

        app.MapControllerRoute(
            name: "celebrity",
            pattern: "/0",
            defaults: new { Controller = "Celebrities", Action = "NewHumanForm" }
            );

        app.MapControllerRoute(
            name: "celebrity",
            pattern: "/{id:int:min(1)}",
            defaults: new { Controller = "Celebrities", Action = "Human" }
            );

        app.MapControllerRoute(
            name: "default",
            pattern: "{controller=Celebrities}/{action=Index}/{id?}");

        app.Run();
    }
}