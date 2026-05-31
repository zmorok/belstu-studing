using Microsoft.AspNetCore.HttpLogging;

internal class Program                                      // класс, в котором будет точка входа в приложение
{
    private static void Main(string[] args)                 // главная точка входа в приложение, args == параметры командной строки
    {
        var builder = WebApplication.CreateBuilder(args);   // билдер для конфигурации приложения 

        builder.Services.AddHttpLogging(o =>                // добавление сервиса логирования http
        { 
            o.LoggingFields = HttpLoggingFields.All;        // логировать всё
        });
        
        builder.Logging.AddFilter("Microsoft.AspNetCore.HttpLogging", LogLevel.Information); // фильтр логов для http

        var app = builder.Build();                          // инициализация приложения на основе конфигурации билдера

        app.UseHttpLogging();                               // middleware http-logger

        app.MapGet("/", () => "Моё первое ASPA");           // маршрут для запросов методом GET по корню сайта

        app.Run();                                          // запуск приложения и начало прослушки http-запросов
    }
}