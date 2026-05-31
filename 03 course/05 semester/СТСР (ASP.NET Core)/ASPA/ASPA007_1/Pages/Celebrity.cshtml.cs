using ASPA007_1;
using DAL_Celebrity_MSSQL;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.Extensions.Options;

namespace ASPA007_1.Pages
{
    public class CelebrityModel : PageModel
    {
        
        public string? PhotosRequestPath { get; private set; }
        public Celebrity? celebrity { get; private set; }

        public IActionResult OnGet(Parms? ModelParms)
        {
            if (ModelParms == null || ModelParms.id == null || ModelParms.repo == null || ModelParms.config == null) 
                throw new ArgumentNullException(nameof(ModelParms));
            else
            {
                this.PhotosRequestPath = ModelParms.config.Value.PhotosRequestPath;
                return ((this.celebrity = ModelParms.repo.GetCelebrityById((int)ModelParms.id)) is null)
                        ? NotFound()
                        : ModelParms.AcceptMIMO == "json"
                                                ? RedirectToRoute("GetCelebrityById", new { Id = ModelParms.id })
                                                : Page();
            }
        }

        public class Parms
        {
            [FromRoute] public int? id { get; set; }
            [FromQuery(Name = "id")] public int? queryId { get; set; }
            [FromHeader(Name = "Accept")] public string? acceptheader { get; set; }
            [FromServices] public IRepository? repo { get; set; }
            [FromServices] public IOptions<CelebritiesConfig> config { get; set; }
        
            public string AcceptMIMO { get { return preffered(acceptheader, new string[] { "json", "html" }).Item1; } }
            private (string?, int) preffered(string? accept, string[] parms)
            {
                (string?, int) rc = (null, -1);
                if (accept != null)
                {
                    int k = -1, mink = accept.Length + 1, mini = -1;
                    for (int i = 0; i < parms.Length; i++) 
                    {
                        if ((k = accept.IndexOf(parms[i], StringComparison.OrdinalIgnoreCase)) >= 0)
                            if (k < mink) { mink = k; mini = i; }
                    }
                    rc = ((mini > 0) ? parms[mini] : null, mini);
                }
                return rc;
            }
        }

    }
}
