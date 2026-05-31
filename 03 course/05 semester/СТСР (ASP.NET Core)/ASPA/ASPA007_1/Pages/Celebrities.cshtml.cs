using DAL_Celebrity_MSSQL;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.Extensions.Options;

namespace ASPA007_1.Pages
{
    public class CelebritiesModel : PageModel
    {
        private readonly ILogger<CelebritiesModel> _logger;
        private readonly CelebritiesConfig _config;
        private readonly IRepository _repo;

        public string PhotosRequestPath { get; set; }
        public List<string?> Photos { get; set; } = [];
        public List<Celebrity> Celebrities { get; set; } = [];


        public CelebritiesModel(ILogger<CelebritiesModel> logger, IOptions<CelebritiesConfig> config, IRepository repo)
        {
            _logger = logger;
            _config = config.Value;
            _repo = repo;
            PhotosRequestPath = _config.PhotosRequestPath;
        }

        // получение списка фотографий по конфигурации
        public void OnGet()
        {
            var folder = _config.PhotosFolder;
            if (Directory.Exists(folder))
                Photos = Directory.GetFiles(folder, "*.jpg").Select(Path.GetFileName).ToList();

            Photos = FirstBrilliant(Photos);
            Celebrities = _repo.GetAllCelebrities().ToList();
        }

        // перемещает "brilliant.jpg" в начало списка, если такая фотография есть
        static private List<string?> FirstBrilliant(List<string?> photos)
        {
            int? idx = photos.FindIndex(p => p == "brilliant.jpg");

            if (idx.HasValue && idx.Value >= 0)
            {
                (photos[idx.Value], photos[0]) = (photos[0], photos[idx.Value]);
                return photos;
            }

            return photos;
        }
    }
}
