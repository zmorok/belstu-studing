using DAL_Celebrity_MSSQL;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.Extensions.Options;

namespace ASPA007_1.Pages
{
    public class NewCelebrityModel : PageModel
    {
        public IRepository repo;
        public string PhotosRequestPath { get; set; }
        public string PhotosFolder { get; set; }
        public Celebrity? Celebrity { get; set; }

        public NewCelebrityModel(IRepository repo, IOptions<CelebritiesConfig> config)
        {
            this.repo = repo;
            PhotosRequestPath = config.Value.PhotosRequestPath;
            PhotosFolder = config.Value.PhotosFolder;
        }

        public void OnGet()
        {
        }

        public IActionResult OnGetConfirm(string fullname, string nationality, string filename) 
        {
            ViewData["Confirm"] = true;
            Celebrity = new Celebrity() { FullName = fullname, Nationality = nationality, ReqPhotoPath = filename};
            return Page();
        }

        public IActionResult OnPost([FromForm] string? fullname, [FromForm] string? nationality, IFormFile upload, string? press, string? filename)
        {
            IActionResult rc = RedirectToPage("Celebrities");
            if (string.IsNullOrEmpty(press))
            {
                string ext = Path.GetExtension(upload.FileName)?.ToLower() ?? ".jpg";
                string safeName = fullname?.Replace(" ", "_") ?? "noname";
                string newFileName = $"{safeName}{ext}";
                string fp = Path.Combine(this.PhotosFolder, newFileName);
                int counter = 1;
                while (System.IO.File.Exists(fp))
                {
                    newFileName = $"{safeName}_{counter}{ext}";
                    fp = Path.Combine(this.PhotosFolder, newFileName);
                    counter++;
                }
                FileStream file = new FileStream(fp, FileMode.Create);
                upload.CopyTo(file);
                file.Close();
                rc = RedirectToPage("NewCelebrity", "Confirm", new { filename = newFileName, fullname = fullname, nationality = nationality });
            }
            else if (press.Equals("Confirm")) 
            {
                string finalFile = filename ?? throw new Exception("Filename missing!");
                this.repo.AddCelebrity(new Celebrity { FullName = fullname, Nationality = nationality, ReqPhotoPath = finalFile });
                rc = RedirectToPage("Celebrities");
            }
            else rc = RedirectToPage("NewCelebrity");

            return rc;
        }

        public IActionResult OnGetRepeat()
        {
            return Page();
        }
    }
}
