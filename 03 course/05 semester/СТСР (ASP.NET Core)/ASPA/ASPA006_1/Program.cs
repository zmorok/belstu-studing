using ASPA006_1;
using DAL_Celebrity_MSSQL;
using Microsoft.AspNetCore.Diagnostics;
using Microsoft.AspNetCore.HttpLogging;
using Microsoft.Extensions.Options;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddHttpLogging(o => { o.LoggingFields = HttpLoggingFields.All; });
builder.Logging.AddFilter("Microsoft.AspNetCore.HttpLogging", LogLevel.Information);

// собственная конфигурация
builder.Configuration.AddJsonFile("Celebrities.config.json", optional: false, reloadOnChange: true);
builder.Services.Configure<CelebritiesConfig>(builder.Configuration.GetSection("Celebrities"));

// scoped-сервис
builder.Services.AddScoped<IRepository, Repository>((IServiceProvider p) =>
{
    CelebritiesConfig config = p.GetRequiredService<IOptions<CelebritiesConfig>>().Value;
    return new Repository(config.ConnectionString);
});


var app = builder.Build();

app.MapGet("/", () => Results.Redirect("/index.html"));

app.UseDefaultFiles(new DefaultFilesOptions
{
    DefaultFileNames = { "index.html" }
});
app.UseStaticFiles();

app.UseExceptionHandler("/Error");
app.UseHttpLogging();

// ----------------------------------
var celebrities = app.MapGroup("/api/Celebrities");
// все знаменитости
celebrities.MapGet("/", (IRepository repo) =>  repo.GetAllCelebrities());
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
celebrities.MapGet("/Photo/{fname}", async (IOptions<CelebritiesConfig> iconfig, HttpContext context, string fname) =>
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


// ----------------------------------
var lifeevents = app.MapGroup("/api/Lifeevents");
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
lifeevents.MapPut("/{id:int:min(1)}", (IRepository repo, int id, Lifeevent lifeevent) =>
{
    bool rc = repo.UpdLifeevent(id, lifeevent);
    if (!rc) throw new FoundByIdException($"/Lifeevents, Lifeevent Id = {id}");
    return repo.GetLifeevetById(id);
});

app.MapFallback((HttpContext ctx) => Results.NotFound(new { error = $"path {ctx.Request.Path} not supported" }));

app.Map("/Error", (HttpContext ctx) =>
{
    Exception? ex = ctx.Features.Get<IExceptionHandlerFeature>()?.Error;

    IResult rc = Results.Problem(detail: "Panic", instance: app.Environment.EnvironmentName, title: "ASPA004", statusCode: 500);

    if (ex != null)
    {
        switch (ex)
        {
            /* 404 */
            case FoundByIdException: rc = Results.Problem(detail: ex.Message, title: "Not Found", statusCode: 404, instance: app.Environment.ApplicationName); break;

            /* 404 */
            case DeleteByIdException: rc = Results.NotFound(ex.Message); break;

            /* 400 */
            case BadHttpRequestException: rc = Results.BadRequest(ex.Message); break;

            case SaveException: rc = Results.Problem(title: "ASPA006/SaveChanges", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500); break;

            case AddCelebrityException: rc = Results.Problem(title: "ASPA006/addCelebrity", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500); break;

            case UnauthorizedAccessException: rc = Results.Problem(title: "ASPA006", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500); break;

            case NullCelebrityException: rc = Results.Problem(title: "ASPA006/NullCelebrity", detail: ex.Message, instance: app.Environment.EnvironmentName, statusCode: 500); break;

            case SurnameInvalidException: rc = Results.Problem(title: "Invalid surname", detail: ex.Message, instance: ctx.Request.Path, statusCode: 409); break;

            case SurnameConflictException: rc = Results.Problem(title: "Surname conflict", detail: ex.Message, instance: ctx.Request.Path, statusCode: 409); break;

            default: rc = Results.Problem(title: "ASPA006/Unknown Value:", detail: ex.Message); break;
        }
    }

    return rc;
});

app.Run();
