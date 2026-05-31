using ASPA007_1;
using DAL_Celebrity_MSSQL;
using Microsoft.AspNetCore.HttpLogging;
using Microsoft.Extensions.FileProviders;

using Microsoft.Extensions.Options;

var builder = WebApplication.CreateBuilder(args);

builder.AddCelebrityLogger();
builder.AddCelebrityConfiguration();
builder.AddCelebrityServices();

builder.Services.AddRazorPages();
builder.Services.AddRazorPages(o =>
{
    o.Conventions.AddPageRoute("/Celebrities", "/");
    o.Conventions.AddPageRoute("/NewCelebrity", "/0");
    o.Conventions.AddPageRoute("/Celebrity", "/Celebrities/{id:int:min(1)}");
    o.Conventions.AddPageRoute("/Celebrity", "/{id:int:min(1)}");
});

var app = builder.Build();

if (!app.Environment.IsDevelopment()) app.UseExceptionHandler("/Error");

app.UseHttpLogging();

app.UseHttpsRedirection();
app.UseStaticFiles();
app.UseStaticFiles(new StaticFileOptions
{
    FileProvider = new PhysicalFileProvider(builder.Configuration["Celebrities:PhotosFolder"]),
    RequestPath = builder.Configuration["Celebrities:PhotosRequestPath"]
});


app.UseRouting();

app.UseAuthorization();

app.MapRazorPages();

app.UseCelebritiesErrorHandler(app.Environment.EnvironmentName);
app.MapCelebrities();
app.MapLifeevents();
app.MapPhotoCelebrities();

app.Run();


