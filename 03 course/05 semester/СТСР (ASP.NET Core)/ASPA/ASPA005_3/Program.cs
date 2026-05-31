using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Diagnostics;

var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();

app.UseExceptionHandler("/Error");

app.MapGet("/A/{x:int:max(100)}", (HttpContext ctx, [FromRoute] int x) => Results.Ok(new { path = ctx.Request.Path.Value, x }));
app.MapPost("/A/{x:int:range(0,100)}", (HttpContext ctx, [FromRoute] int x) => Results.Ok(new { path = ctx.Request.Path.Value, x }));
app.MapPut("/A/{x:int:min(1)}/{y:int:min(1)}", (HttpContext ctx, [FromRoute] int x, [FromRoute] int y) => Results.Ok(new { path = ctx.Request.Path.Value, x, y }));
app.MapDelete("/A/{x:int:min(1)}-{y:int:min(1):max(100)}", (HttpContext ctx, [FromRoute] int x, [FromRoute] int y) => Results.Ok(new { path = ctx.Request.Path.Value, x, y }));

app.MapGet("/B/{x:float}", (HttpContext ctx, [FromRoute] float x) => Results.Ok(new { path = ctx.Request.Path.Value, x }));
app.MapPost("/B/{x:float}/{y:float}", (HttpContext ctx, [FromRoute] float x, [FromRoute] float y) => Results.Ok(new { path = ctx.Request.Path.Value, x, y }));
app.MapDelete("/B/{x:float}-{y:float}", (HttpContext ctx, [FromRoute] float x, [FromRoute] float y) => Results.Ok(new { path = ctx.Request.Path.Value, x, y }));

app.MapGet("/C/{x:bool}", (HttpContext ctx, [FromRoute] bool x) => Results.Ok(new { path = ctx.Request.Path.Value, x }));
app.MapPost("/C/{x:bool},{y:bool}", (HttpContext ctx, [FromRoute] bool x, [FromRoute] bool y) => Results.Ok(new { path = ctx.Request.Path.Value, x, y }));

app.MapGet("/D/{x:datetime}", (HttpContext ctx, [FromRoute] DateTime x) => Results.Ok(new { path = ctx.Request.Path.Value, x }));
app.MapPost("/D/{x:datetime}|{y:datetime}", (HttpContext ctx, [FromRoute] DateTime x, [FromRoute] DateTime y) => Results.Ok(new { path = ctx.Request.Path.Value, x, y }));

app.MapGet("/E/12-{x:required}", (HttpContext ctx, [FromRoute] string x) => Results.Ok(new { path = ctx.Request.Path.Value, x }));
app.MapPut("/E/{x:alpha:length(2,12)}", (HttpContext ctx, [FromRoute] string x) => Results.Ok(new { path = ctx.Request.Path.Value, x }));

app.MapPut(@"/F/{x:regex(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.by$)}", (HttpContext ctx, [FromRoute] string x) => Results.Ok(new { path = ctx.Request.Path.Value, x }));

app.MapGet("/Error", (HttpContext ctx) =>
{
    var ex = ctx.Features.Get<IExceptionHandlerFeature>()?.Error;
    return Results.Ok(new { message = ex?.Message });
});

app.MapFallback((HttpContext ctx) =>
    Results.NotFound(new { message = $"path {ctx.Request.Path.Value} not supported" })
);

app.Run();
