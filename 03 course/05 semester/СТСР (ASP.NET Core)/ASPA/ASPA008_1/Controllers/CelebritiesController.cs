using ASPA008_1.Filters;
using ASPA008_1.Models;
using DAL_Celebrity_MSSQL;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Options;

namespace ASPA008_1.Controllers
{
    public class CelebritiesController(IRepository repo, IOptions<CelebritiesConfig> config) : Controller
    {
        private readonly IRepository repo = repo;
        private readonly IOptions<CelebritiesConfig> config = config;

        public record IndexModel(string PhotosRequestPath, List<Celebrity> Celebrities);

        public IActionResult Index()
        {
            return View(new IndexModel(config.Value.PhotosRequestPath, repo.GetAllCelebrities()));
        }

        public record HumanModel(
            string photosrequestpath,
            Celebrity celebrity,
            List<Lifeevent> lifeevents,
            Dictionary<string, string>? references);

        [InfoAsyncActionFilter(infotype: "WIKI, FACE")]
        public IActionResult Human(int id)
        {
            var celebrity = repo.GetCelebrityById(id);
            if (celebrity == null) return NotFound();

            var references =
                (Dictionary<string, string>?)HttpContext.Items[InfoAsyncActionFilter.Wikipedia];

            return View(new HumanModel(
                config.Value.PhotosRequestPath,
                celebrity,
                repo.GetLifeeventsByCelebrityId(id),
                references));
        }

        // ===========================
        //       CREATE / EDIT / DELETE FORM
        // ===========================

        [HttpGet]
        public IActionResult NewHumanForm(int? id, FormMode mode = FormMode.Create)
        {
            var pathReq = config.Value.PhotosRequestPath;
            NewHumanFormModel model;

            if ((mode == FormMode.Edit || mode == FormMode.Delete) && id.HasValue)
            {
                var c = repo.GetCelebrityById(id.Value);
                if (c == null) return RedirectToAction("Index");

                model = new NewHumanFormModel
                {
                    Mode = mode,
                    Id = c.Id,
                    FullName = c.FullName,
                    Nationality = c.Nationality,
                    TempFileName = c.ReqPhotoPath,
                    Confirm = true,
                    PhotosRequestPath = pathReq
                };
            }
            else
            {
                model = new NewHumanFormModel
                {
                    Mode = FormMode.Create,
                    PhotosRequestPath = pathReq
                };
            }

            return View(model);
        }

        [HttpPost]
        public async Task<IActionResult> NewHumanForm(
            int? id,
            string? fullname,
            string? nationality,
            IFormFile? upload,
            string? press,
            string? tempfilename,
            FormMode mode)
        {
            var folder = config.Value.PhotosFolder;
            var pathReq = config.Value.PhotosRequestPath;

            // ================= DELETE CONFIRM =================
            if (mode == FormMode.Delete && press == "Confirm")
            {
                repo.DelCelebrity(id!.Value);
                System.IO.File.Delete(Path.Combine(folder, tempfilename));
                return RedirectToAction("Index");
            }

            // ================= PREVIEW =================
            if (press == null && upload != null)
            {
                var name = upload.FileName;
                await using var fs = System.IO.File.Create(Path.Combine(folder, name));
                await upload.CopyToAsync(fs);

                return View(new NewHumanFormModel
                {
                    Mode = mode,
                    Id = id,
                    FullName = fullname,
                    Nationality = nationality,
                    TempFileName = name,
                    Confirm = true,
                    PhotosRequestPath = pathReq
                });
            }

            // ================= CONFIRM CREATE / EDIT =================
            if (press == "Confirm")
            {
                bool changed = false;
                string newphoto = upload?.FileName ?? "nophoto";
                if (newphoto != "nophoto")
                {
                    var newname = upload!.FileName;
                    await using var fs = new FileStream(Path.Combine(folder, tempfilename), FileMode.Create, FileAccess.Write, FileShare.None);
                    await upload.CopyToAsync(fs);
                    changed = true;
                }

                string? name = string.Empty;
                // если есть новое фото, берём его имя
                string[]? parts = changed ? newphoto.Split('.') : tempfilename?.Split('.');

                if (parts != null)
                {
                    if (parts.Length == 2) name = (parts[0] ?? "photo") + ('.' + parts[1]);
                    else if (parts.Length == 3) name = (parts[1] ?? "photo") + ('.' + parts[2]);
                    else name = "photo.jpg";
                }

                // новое имя
                var namePart = fullname!.Replace(" ", "_");
                var finalName = $"{namePart}.{name}";

                System.IO.File.Move(
                    Path.Combine(folder, tempfilename),
                    Path.Combine(folder, finalName),
                    overwrite: true);

                if (mode == FormMode.Create)
                {
                    repo.AddCelebrity(new Celebrity
                    {
                        FullName = fullname,
                        Nationality = nationality,
                        ReqPhotoPath = finalName
                    });
                }
                else
                {
                    var celeb = repo.GetCelebrityById(id!.Value)!;
                    celeb.FullName = fullname;
                    celeb.Nationality = nationality;
                    celeb.ReqPhotoPath = finalName;
                    repo.UpdCelebrity(celeb.Id, celeb);
                }

                return RedirectToAction("Index");
            }

            // ================= CANCEL =================
            if (press == "Cancel") return RedirectToAction("Index");

            return RedirectToAction("NewHumanForm");
        }

        // ================= BUTTON SHORTCUTS =================

        [HttpGet]
        public IActionResult Edit(int id) => RedirectToAction("NewHumanForm", new { id, mode = FormMode.Edit });

        [HttpGet]
        public IActionResult Delete(int id) => RedirectToAction("NewHumanForm", new { id, mode = FormMode.Delete });
    }
}
