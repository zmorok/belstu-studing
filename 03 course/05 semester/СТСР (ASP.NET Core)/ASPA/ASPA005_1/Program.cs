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
            )
            .AddEndpointFilter(async (context, next) =>
            {
                Celebrity? celebrity = context.GetArgument<Celebrity>(0) ?? throw new NullCelebrityException("/Celebrities error, celebrity is null");

                if (celebrity.Surname is null) throw new SurnameInvalidException("Celebrity.Surname is null");
                if (celebrity.Surname.Length < 2) throw new SurnameInvalidException("Celebrity.Surname length < 2");

                return await next(context);
            }
            )
            .AddEndpointFilter(async (context, next) => 
            {
                Celebrity? celebrity = context.GetArgument<Celebrity>(0) ?? throw new NullCelebrityException("/Celebrities error, celebrity is null");

                if (repository.getAllCelebrities().Any(c => c.Surname.Equals(celebrity.Surname, StringComparison.OrdinalIgnoreCase)))
                    throw new SurnameConflictException( $"Celebrity with surname '{celebrity.Surname}' already exists", celebrity.Surname);

                return await next(context);
            }
            )
            .AddEndpointFilter(async (context, next) => 
            {
                Celebrity? celebrity = context.GetArgument<Celebrity>(0) ?? throw new NullCelebrityException("/Celebrities error, celebrity is null");

                string basePath = AppContext.BaseDirectory;
                //string celebrityPath = Path.GetFullPath(Path.Combine(basePath, "..", "..", "..", "\\Celebrities\\"));
                string celebrityPath = basePath + "Celebrities\\";
                string fileName = Path.GetFileName(celebrity.PhotoPath);
                string fullPath = Path.Combine(celebrityPath, fileName);

                if (!File.Exists(fullPath))
                    context.HttpContext.Response.Headers.Append("X-Celebrity", $"NotFound = {fileName}");
                
                return await next(context); 
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
                    switch (ex)
                    {
                        case FoundByIdException:
                            rc = Results.NotFound(ex.Message); // 404
                            break;

                        case BadHttpRequestException:
                            rc = Results.BadRequest(ex.Message); // 400
                            break;

                        case SaveException:
                            rc = Results.Problem(title: "ASPA005/SaveChanges", detail: ex.Message,
                                instance: app.Environment.EnvironmentName, statusCode: 500);
                            break;

                        case AddCelebrityException:
                            rc = Results.Problem(title: "ASPA005/addCelebrity", detail: ex.Message,
                                instance: app.Environment.EnvironmentName, statusCode: 500);
                            break;

                        case DeleteByIdException:
                            rc = Results.NotFound(ex.Message); // 404
                            break;

                        case UnauthorizedAccessException:
                            rc = Results.Problem(title: "ASPA005", detail: ex.Message,
                                instance: app.Environment.EnvironmentName, statusCode: 500);
                            break;

                        case NullCelebrityException:
                            rc = Results.Problem(title: "ASPA005/NullCelebrity", detail: ex.Message,
                                instance: app.Environment.EnvironmentName, statusCode: 500);
                            break;

                        // new exceptions
                        case SurnameInvalidException:
                            rc = Results.Problem(title: "Invalid surname", detail: ex.Message,
                                instance: ctx.Request.Path, statusCode: 409);
                            break;

                        case SurnameConflictException:
                            rc = Results.Problem(title: "Surname conflict", detail: ex.Message,
                                instance: ctx.Request.Path, statusCode: 409);
                            break;

                        default:
                            rc = Results.Problem(title: "ASPA005/Unknown Value:", detail: ex.Message);
                            break;
                    }
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

    // new exceptions
    public class NullCelebrityException : Exception { public NullCelebrityException(string message) : base($"Null Celebrity: {message}") { } };

    public class SurnameInvalidException : Exception { public SurnameInvalidException(string message) : base(message) { } };

    public class SurnameConflictException : Exception {
        public string Surname { get; }
        public SurnameConflictException(string message, string surname) : base(message) => Surname = surname;
    }
}
