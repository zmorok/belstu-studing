using DAL004;
using Microsoft.AspNetCore.Diagnostics;
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

        Repository.JSONFileName = "Celebrities.json";
        using (IRepository repository = Repository.Create("Celebrities"))
        {

            app.UseExceptionHandler("/Celebrities/Error");

            app.MapGet("/Celebrities", () => repository.getAllCelebrities());
            app.MapGet("/Celebrities/{id:int}", (int id) =>
            {
                Celebrity? celebrity = repository.getCelebrityById(id);
                if (celebrity == null) throw new FoundByIdException($"/Celebrities, Celebrity Id = {id}");
                return celebrity;
            }
            );

            app.MapPost("/Celebrities", (Celebrity celebrity) =>
            {
                int? id = repository.addCelebrity(celebrity);
                if (id == null) throw new AddCelebrityException("/Celebrities error, id == null");
                if (repository.SaveChanges() <= 0) throw new SaveException("/Celebrities error, SaveChanges() <= 0");
                return new Celebrity((int)id, celebrity.Firstname, celebrity.Surname, celebrity.PhotoPath);
            }
            );

            app.MapDelete("/Celebrities/{id:int}", (int id) =>
            {
                Celebrity? celebrity = repository.getCelebrityById(id);
                if (celebrity == null) throw new DeleteByIdException($"/Celebrities error, Id = {id}");
                repository.delCelebrityById(celebrity.Id);
                if (repository.SaveChanges() <= 0) throw new SaveException($"/Celebrities/{id} error, SaveChanges() <= 0");
                return $"Celebrity with Id = {id} deleted";
            }
            );

            app.MapPut("/Celebrities/{id:int}", (int id, Celebrity celebrity) =>
            {
                _ = repository.getCelebrityById(id) ?? throw new FoundByIdException($"/Celebrities error, Id = {id}");

                int? newId = repository.updCelebrityById(id, celebrity);
                if (newId is null) throw new AddCelebrityException($"/Celebrities/{id} conflict while updating");

                if (repository.SaveChanges() <= 0) throw new SaveException($"/Celebrities/{id} error, SaveChanges() <= 0");

                return repository.getCelebrityById(newId.Value);
            }
            );


            app.MapFallback((HttpContext ctx) => Results.NotFound(new { error = $"path {ctx.Request.Path} not supported" }));

            app.Map("/Celebrities/Error", (HttpContext ctx) =>
            {
                Exception? ex = ctx.Features.Get<IExceptionHandlerFeature>()?.Error;

                IResult rc = Results.Problem(detail: "Panic", instance: app.Environment.EnvironmentName, title: "ASPA004", statusCode: 500);

                if (ex != null)
                {
                    if (ex is FoundByIdException) rc = Results.NotFound(ex.Message); // 404
                    if (ex is BadHttpRequestException) rc = Results.BadRequest(ex.Message); // 400
                    if (ex is SaveException) rc = Results.Problem(title: "ASPA004/SaveChanges", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500);
                    if (ex is AddCelebrityException) rc = Results.Problem(title: "ASPA004/addCelebrity", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500);
                    if (ex is UnauthorizedAccessException) rc = Results.Problem(title: "ASPA00", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500);
                    if (ex is DeleteByIdException) rc = Results.NotFound(ex.Message); // 404
                }

                return rc;
            }
            );

            app.Run();
        }
    }

    public class FoundByIdException : Exception { public FoundByIdException(string message) : base($"Found by Id: {message}") { } };

    public class SaveException : Exception { public SaveException(string message) : base($"SaveChanges error: {message}") { } }

    public class AddCelebrityException : Exception { public AddCelebrityException(string message) : base($"AddCelebrityException error:{message}") { } }

    public class DeleteByIdException : Exception { public DeleteByIdException(string message) : base($"Delete by Id:Delete {message}") { } };

}
