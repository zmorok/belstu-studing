using Microsoft.AspNetCore.HttpLogging;

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
        app.UseDefaultFiles();
        app.UseStaticFiles();    


        app.MapGet("/aspnetcore", () => "блаблабла");
        app.MapGet("/privet", () => "poka");

        app.Run();
    }
}