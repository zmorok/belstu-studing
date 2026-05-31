using DAL004;
using ASPA005_2;
using ASPA005_2.Validation;

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
            SurnameFilter.repository    =
            PhotoFilter.repository      =
            DeleteFilter.repository     =
            PutFilter.repository        = repository;

            RouteGroupBuilder api = app.MapGroup("/Celebrities");

            app.UseExceptionHandler("/Celebrities/Error");

            api.MapGet("/", () => repository.getAllCelebrities());
            api.MapGet("/{id:int}", (int id) =>
            {
                Celebrity? celebrity = repository.getCelebrityById(id);
                if (celebrity == null) throw new FoundByIdException($"/Celebrities, Celebrity Id = {id}");
                return celebrity;
            }
            );

            api.MapPost("/", (Celebrity celebrity) =>
            {
                int? id = repository.addCelebrity(celebrity);
                if (id == null) throw new AddCelebrityException("/Celebrities error, id == null");
                if (repository.SaveChanges() <= 0) throw new SaveException("/Celebrities error, SaveChanges() <= 0");
                return new Celebrity((int)id, celebrity.Firstname, celebrity.Surname, celebrity.PhotoPath);
            }
            )
            .AddEndpointFilter<SurnameFilter>()
            .AddEndpointFilter<PhotoFilter>();

            api.MapDelete("/{id:int}", (int id) =>
            {
                Celebrity? celebrity = repository.getCelebrityById(id);
                if (celebrity == null) throw new DeleteByIdException($"/Celebrities error, Id = {id}");
                repository.delCelebrityById(celebrity.Id);
                if (repository.SaveChanges() <= 0) throw new SaveException($"/Celebrities/{id} error, SaveChanges() <= 0");
                return $"Celebrity with Id = {id} deleted";
            }
            )
            .AddEndpointFilter<DeleteFilter>(); // нельзя удалять знаменитостей с айди [1:20]

            api.MapPut("/{id:int}", (int id, Celebrity celebrity) =>
            {
                _ = repository.getCelebrityById(id) ?? throw new FoundByIdException($"/Celebrities error, Id = {id}");

                int? newId = repository.updCelebrityById(id, celebrity);
                if (newId is null) throw new AddCelebrityException($"/Celebrities/{id} conflict while updating");

                if (repository.SaveChanges() <= 0) throw new SaveException($"/Celebrities/{id} error, SaveChanges() <= 0");

                return repository.getCelebrityById(newId.Value);
            }
            )
            .AddEndpointFilter<PutFilter>(); // длина фамилии не более 20 символов


            app.MapFallback((HttpContext ctx) => Results.NotFound(new { error = $"path {ctx.Request.Path} not supported" }));

            api.Map("/Error", (HttpContext ctx) =>
            {
                Exception? ex = ctx.Features.Get<IExceptionHandlerFeature>()?.Error;

                IResult rc = Results.Problem(detail: "Panic", instance: app.Environment.EnvironmentName, title: "ASPA004", statusCode: 500);

                if (ex != null)
                {
                    switch (ex)
                    {
                        /* 404 */
                        case FoundByIdException: rc = Results.NotFound(ex.Message); break;

                        /* 404 */
                        case DeleteByIdException: rc = Results.NotFound(ex.Message); break;
                        
                        /* 400 */
                        case BadHttpRequestException: rc = Results.BadRequest(ex.Message); break;

                        case SaveException: rc = Results.Problem(title: "ASPA005/SaveChanges", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500); break;

                        case AddCelebrityException: rc = Results.Problem(title: "ASPA005/addCelebrity", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500); break;

                        case UnauthorizedAccessException: rc = Results.Problem(title: "ASPA005", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500); break;

                        case NullCelebrityException: rc = Results.Problem(title: "ASPA005/NullCelebrity", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500); break;

                        case SurnameInvalidException: rc = Results.Problem(title: "Invalid surname", detail: ex.Message, instance: ctx.Request.Path, statusCode: 409); break;

                        case SurnameConflictException: rc = Results.Problem(title: "Surname conflict", detail: ex.Message, instance: ctx.Request.Path, statusCode: 409); break;

                        default: rc = Results.Problem(title: "ASPA005/Unknown Value:", detail: ex.Message); break;
                    }
                }

                return rc;
            }
            );

            app.Run();
        }
    }   
}