using DAL_Celebrity_MSSQL;
using Microsoft.Extensions.Options;
using System.Net;

namespace ASPA008_1.Helpers
{
    public static class BuilderHelper
    {
        public class ASPA008_1Exception : Exception
        {

            public HttpStatusCode StatusCode { get; }


            public string ErrorCode { get; }

            public ASPA008_1Exception(int status, string code, string message, string detail)
            : base(message)
            {
                StatusCode = (HttpStatusCode)status;
                ErrorCode = code;
                Detail = detail;
            }

            public string? Detail { get; }

            public ASPA008_1Exception(int status, string code, string detail)
                : base(detail)
            {
                StatusCode = (HttpStatusCode)status;
                ErrorCode = code;
            }


            public ASPA008_1Exception(int status, string code, string format, params object[] args)
                : this(status, code, string.Format(format, args))
            {
            }

            public ASPA008_1Exception(string errorCode, string message)
                : this((int)HttpStatusCode.InternalServerError, errorCode, message)
            {
            }
        }

       
        public static IApplicationBuilder UseCelebErrorHandler(this IApplicationBuilder app, string prefix) =>
                app.Use(async (ctx, next) =>
                {
                    try { await next(); }
                    catch (Exception ex)
                    {
                        ctx.Response.StatusCode = ex is ASPA008_1Exception e ? (int)e.StatusCode : 500;
                        await ctx.Response.WriteAsJsonAsync(ex is ASPA008_1Exception e2
                            ? new
                            {
                                StatusCode = (int)e2.StatusCode,
                                ErrorCode = $"{prefix}-{e2.ErrorCode}",
                                e2.Message,
                                e2.Detail,
                                Timestamp = DateTime.UtcNow
                            }
                            : new
                            {
                                StatusCode = 500,
                                ErrorCode = $"{prefix}-UNKNOWN",
                                ex.Message,
                                Detail = "Internal server error",
                                Timestamp = DateTime.UtcNow
                            });
                    }
                });

        public static IServiceCollection AddCelebritiesConfig(this WebApplicationBuilder builder, string JsonFileName = "Celebrities.config.json")
        {
            builder.Configuration.AddJsonFile(JsonFileName);
            return builder.Services.Configure<CelebritiesConfig>(builder.Configuration.GetSection("Celebrities"));

        }
        public static IServiceCollection AddCelebrityServices(this WebApplicationBuilder builder, string JsonFileName = "Celebrities.config.json")
        {
            builder.Services.AddScoped<IRepository, Repository>((p) =>
            {
                return new Repository(builder.Configuration.GetSection("Celebrities").GetValue<string>("ConnectionString"));
            });
            builder.Services.AddSingleton<CelebrityTitles>((p) => new CelebrityTitles());
            builder.Services.AddSingleton<CountryCodes>((p) => new CountryCodes(p.GetRequiredService<IOptions<CelebritiesConfig>>().Value.CountryCodesPath));
            return builder.Services;
        }


        public static RouteHandlerBuilder MapCelebrities(this IEndpointRouteBuilder routeBuilder, IConfiguration config, string prefix = "/api/Celebrities")
        {
            var cel = routeBuilder.MapGroup("/api/Celebrities");

            var life = routeBuilder.MapGroup("/api/LifeEvents");
            life.MapGet("/Celebrities/{id:int:min(1)}", (IRepository repo, int id) =>
            {
                List<Lifeevent> lifeEventList = repo.GetLifeeventsByCelebrityId(id);
                if (lifeEventList.Count == 0)
                {
                    throw new FoundByIdException("Failed to get life events by celebritie's id. Something went wrong");
                }
                else
                {
                    return lifeEventList;
                }
            });

            cel.MapGet("/", (IRepository repo) => repo.GetAllCelebrities());

            cel.MapGet("/{id:int:min(1)}", (IRepository repo, int id) =>
            {
                if (repo.GetCelebrityById(id) != null)
                {
                    return repo.GetCelebrityById(id);
                }
                else
                {
                    throw new FoundByIdException($"Failed to get celebrity by {id} id. Such id does not exist");

                }
            });

            cel.MapGet("/LifeEvents/{id:int:min(1)}", (IRepository repo, int id) =>
            {
                Celebrity? celeb = repo.GetCelebrityByLifeeventId(id);
                if (celeb != null)
                {
                    return celeb;
                }
                else
                {
                    throw new FoundByIdException($"Could not get a celebrity by {id} id . Such id does not exist");
                }
            });

            cel.MapDelete("/{id:int:min(1)}", (IRepository repo, int id) =>
            {
                if (repo.DelCelebrity(id))
                {
                    return $"Celebrity with id {id} was successfully deleted!";
                }
                else
                {
                    throw new DeleteByIdException($"Failed to delete celebrity by {id} id. Celebrity with such id does not exist");
                }
            });

            cel.MapPost("/", (IRepository repo, Celebrity celeb) =>
            {
                if (repo.AddCelebrity(celeb))
                {
                    celeb.Id = repo.GetCelebrityIdByName(celeb.FullName);
                    return celeb;
                }
                else
                {
                    throw new AddCelebrityException("Failed to add celebrity");
                }
            });

            cel.MapPut("/{id:int:min(1)}", (IRepository repo, int id, Celebrity newCeleb) =>
            {
                if (repo.UpdCelebrity(id, newCeleb))
                {
                    newCeleb.Id = repo.GetCelebrityIdByName(newCeleb.FullName);
                    return newCeleb;
                }
                else
                {
                    throw new UpdCelebrityException($"Failed to update celebrity with id {id}. Something went wrong");
                }
            });

            return cel.MapGet("/photo/{fname}", async (IRepository repo, string fname) =>
            {
                var photoFoler = config.GetSection("Celebrities").GetSection("PhotosFolder").Value;
                var photoPath = Path.Combine(photoFoler, fname);
                if (!File.Exists(photoPath))
                {
                    throw new FileNotFoundException($"Photo file was not found by requirement({fname})");
                }
                else
                {
                    try
                    {
                        var bytes = await File.ReadAllBytesAsync(photoPath);
                        string contentType = GetContentTypeByExtension(Path.GetExtension(photoPath));
                        return Results.File(bytes, contentType);
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine(ex.Message);
                        return Results.Problem(ex.Message);
                    }
                }
            });



        }

        public static string GetContentTypeByExtension(string extension)
        {
            return extension.ToLower() switch
            {
                ".jpg" => "image/jpeg",
                ".jpeg" => "image/jpeg",
                ".png" => "image/png",
                ".gif" => "image/gif",
                ".bmp" => "image/bmp",
                _ => "application/octet-stream",
            };
        }
    }
}
