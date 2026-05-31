using DAL_Celebrity_MSSQL;
using Microsoft.AspNetCore.HttpLogging;
using Microsoft.AspNetCore.Diagnostics;
using Microsoft.Extensions.Options;

namespace ASPA007_1
{
    public static class CelebrityAPI
    {
        #region Configuaration
        public static IServiceCollection AddCelebrityConfiguration(this WebApplicationBuilder builder, string json = "Celebrities.config.json")
        {
            builder.Configuration.AddJsonFile(json, optional: false, reloadOnChange: true);
            return builder.Services.Configure<CelebritiesConfig>(builder.Configuration.GetSection("Celebrities"));
        }
        #endregion

        #region Services
        #region Repository
        public static IServiceCollection AddCelebrityServices(this WebApplicationBuilder builder)
        {
            builder.Services.AddScoped<IRepository, Repository>((IServiceProvider p) =>
            {
                CelebritiesConfig config = p.GetRequiredService<IOptions<CelebritiesConfig>>().Value;
                return new Repository(config.ConnectionString);
            });

            return builder.Services;
        }
        #endregion
        #region Logger
        public static IServiceCollection AddCelebrityLogger(this WebApplicationBuilder builder)
        {
            builder.Services.AddHttpLogging(o => { o.LoggingFields = HttpLoggingFields.All; });
            builder.Logging.AddFilter("Microsoft.AspNetCore.HttpLogging", LogLevel.Information);
            return builder.Services;
        }
        #endregion
        #endregion

        #region Maps

        #region Celebrities
        public static RouteHandlerBuilder MapCelebrities(this IEndpointRouteBuilder routeBuilder, string prefix = "/api/Celebrities")
        {
            var celebrities = routeBuilder.MapGroup(prefix);
            // все знаменитости
            celebrities.MapGet("/", (IRepository repo) => repo.GetAllCelebrities());
            // знаменитость по Id
            celebrities.MapGet("/{id:int:min(1)}", (int id, IRepository repo) =>
            {
                Celebrity? celebrity = repo.GetCelebrityById(id);
                if (celebrity == null) throw new FoundByIdException($"/Celebrities, Celebrity Id = {id}");
                return celebrity;
            });
            // знаменитость по Id события
            celebrities.MapGet("/Lifeevents/{id:int:min(1)}", (int id, IRepository repo) =>
            {
                Celebrity? celebrity = repo.GetCelebrityByLifeeventId(id);
                if (celebrity == null) throw new FoundByIdException($"/Celebrities, Lifeevent Id = {id}");
                return celebrity;
            });
            // удалить знаменитость по Id
            celebrities.MapDelete("/{id:int:min(1)}", (int id, IRepository repo) =>
            {
                Celebrity? fordelete = repo.GetCelebrityById(id) ?? throw new FoundByIdException($"/Celebrities, Celebrity Id = {id}");
                bool rc = repo.DelCelebrity(id);
                if (!rc) throw new DeleteByIdException($"/Celebrities, Celebrity Id = {id}");
                return fordelete;
            });
            // добавить новую знаменитость
            celebrities.MapPost("/", (IRepository repo, Celebrity celebrity) =>
            {
                int idExisting = repo.GetCelebrityIdByName(celebrity.FullName);

                if (idExisting == -1)
                {
                    bool rc = repo.AddCelebrity(celebrity);
                    if (!rc) throw new AddCelebrityException($"/Celebrities, Celebrity Name = {celebrity.FullName}");
                    return celebrity;
                }
                else throw new SurnameConflictException($"Celebrity with surname '{celebrity.FullName}' already exists.", celebrity.FullName);
            });
            // изменить знаменитость по Id
            celebrities.MapPut("/{id:int:min(1)}", (IRepository repo, int id, Celebrity celebrity) =>
            {
                bool rc = repo.UpdCelebrity(id, celebrity);
                if (!rc) throw new FoundByIdException($"/Celebrities, Celebrity Id = {id}");
                return repo.GetCelebrityById(id);
            });
            // получить файл фотографии по имени файла (fname)
            return celebrities.MapGet("/Photo/{fname}", async (IOptions<CelebritiesConfig> iconfig, HttpContext context, string fname) =>
            {
                CelebritiesConfig config = iconfig.Value;
                string filePath = Path.Combine(config.PhotosFolder, fname);
                if (!File.Exists(filePath))
                {
                    context.Response.StatusCode = 404;
                    await context.Response.WriteAsync($"Photo file '{fname}' not found.");
                    return;
                }
                await context.Response.SendFileAsync(filePath, 0, null);
            });

        }
        #endregion

        #region Lifeevents
        public static RouteHandlerBuilder MapLifeevents(this IEndpointRouteBuilder routeBuilder, string prefix = "/api/Lifeevents")
        {
            var lifeevents = routeBuilder.MapGroup(prefix);
            // все события
            lifeevents.MapGet("/", (IRepository repo) => repo.GetAllLifeevents());
            // событие по Id
            lifeevents.MapGet("/{id:int:min(1)}", (int id, IRepository repo) =>
            {
                Lifeevent? lifeevent = repo.GetLifeevetById(id);
                if (lifeevent == null) throw new FoundByIdException($"/Lifeevents, Lifeevent Id = {id}");
                return lifeevent;
            });
            // все события по Id знаменитости
            lifeevents.MapGet("/Celebrities/{id:int:min(1)}", (int id, IRepository repo) =>
            {
                var events = repo.GetLifeeventsByCelebrityId(id);
                return events;
            });
            // удалить событие по Id
            lifeevents.MapDelete("/{id:int:min(1)}", (int id, IRepository repo) =>
            {
                Lifeevent? fordelete = repo.GetLifeevetById(id) ?? throw new FoundByIdException($"/Lifeevents, Lifeevent Id = {id}");
                bool rc = repo.DelLifeevent(id);
                if (!rc) throw new DeleteByIdException($"/Lifeevents, Lifeevent Id = {id}");
                return fordelete;
            });
            // добавить новое событие
            lifeevents.MapPost("/", (IRepository repo, Lifeevent lifeevent) =>
            {
                bool rc = repo.AddLifeevent(lifeevent);
                if (!rc) throw new AddCelebrityException($"/Lifeevents, Lifeevent Description = {lifeevent.Description}");
                return lifeevent;
            });
            // изменить событие по Id
            return lifeevents.MapPut("/{id:int:min(1)}", (IRepository repo, int id, Lifeevent lifeevent) =>
            {
                bool rc = repo.UpdLifeevent(id, lifeevent);
                if (!rc) throw new FoundByIdException($"/Lifeevents, Lifeevent Id = {id}");
                return repo.GetLifeevetById(id);
            });
        }
        #endregion

        #region PhotoCelebrities
        public static RouteHandlerBuilder MapPhotoCelebrities(this IEndpointRouteBuilder routeBuilder, string? prefix = "/Photo")
        {
            if (string.IsNullOrEmpty(prefix)) prefix = routeBuilder.ServiceProvider.GetRequiredService<IOptions<CelebritiesConfig>>().Value.PhotosRequestPath;
            return routeBuilder.MapGet($"{prefix}/{{fname}}", async (IOptions<CelebritiesConfig> iconfig, HttpContext context, string fname) =>
            {
                CelebritiesConfig config = iconfig.Value;
                string filepath = Path.Combine(config.PhotosFolder, fname);
                FileStream file = File.OpenRead(filepath);
                BinaryReader sr = new (file);
                BinaryWriter sw = new (context.Response.BodyWriter.AsStream());
                int n = 0; byte[] buffer = new byte[2048];
                context.Response.ContentType = "image/jpeg";
                context.Response.StatusCode = StatusCodes.Status200OK;
                while ((n = await sr.BaseStream.ReadAsync(buffer, 0, 2048)) > 0) await sw.BaseStream.WriteAsync(buffer, 0, n);
                sr.Close(); sw.Close();
            });
        }
        #endregion

        #region Exceptions
        public static IApplicationBuilder UseCelebritiesErrorHandler(this WebApplication app, string instance = "ASPA007_1")
        {
            app.Map("/Error", (HttpContext ctx) =>
            {
                var env = ctx.RequestServices.GetRequiredService<IWebHostEnvironment>();

                Exception? ex = ctx.Features.Get<IExceptionHandlerFeature>()?.Error;

                IResult rc = Results.Problem(
                    detail: "Panic",
                    instance: env.EnvironmentName,
                    title: instance,
                    statusCode: 500);

                if (ex != null)
                {
                    rc = ex switch
                    {
                        FoundByIdException => Results.Problem(detail: ex.Message, title: "Not Found", statusCode: 404, instance: instance),

                        DeleteByIdException => Results.NotFound(ex.Message),

                        BadHttpRequestException => Results.BadRequest(ex.Message),

                        SaveException => Results.Problem(title: $"{instance}/SaveChanges", detail: ex.Message, instance: env.EnvironmentName, statusCode: 500),

                        AddCelebrityException => Results.Problem(title: $"{instance}/addCelebrity", detail: ex.Message, instance: env.EnvironmentName, statusCode: 500),

                        UnauthorizedAccessException => Results.Problem(title: instance, detail: ex.Message, instance: env.EnvironmentName, statusCode: 500),

                        NullCelebrityException => Results.Problem(title: $"{instance}/NullCelebrity", detail: ex.Message, instance: env.EnvironmentName, statusCode: 500),

                        SurnameInvalidException => Results.Problem(title: "Invalid surname", detail: ex.Message, instance: ctx.Request.Path, statusCode: 409),

                        SurnameConflictException => Results.Problem(title: "Surname conflict", detail: ex.Message, instance: ctx.Request.Path, statusCode: 409),

                        _ => Results.Problem(title: $"{instance}/Unknown Value:", detail: ex.Message)
                    };
                }

                return rc;
            });

            return app;
        }
        #endregion
        #endregion

        #region RAZOR: Titiles
        public static CelebrityTitles Titles { get; } = new CelebrityTitles();

        public class CelebrityTitles
        {
            public string Head { get; set; } = "Celebrities";
            public string Title { get; set; } = "Celebrities Title";
            public string Copyright { get; set; } = @" @ DSZ BSTU lab07 STSR";
        }
        #endregion
    }
}
