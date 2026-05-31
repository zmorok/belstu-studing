using Microsoft.AspNetCore.Html;
using Microsoft.AspNetCore.Mvc.Rendering;

namespace ASPA008_1.Helpers
{
    public static class CelebrityHelpers
    {
        public static HtmlString CelebrityPhoto(this IHtmlHelper html, int id, string title, string src)
        {
            string onclick = "location.href=`/${this.id}`";
            string onload =
            "let ratio = this.naturalWidth / this.naturalHeight;" +
            "let h = 200, w = 0;" +
            "if (h != 0 && w == 0) {this.width = Math.round(ratio * h);};" +
            "if (h == 0 && w != 0) {e.height = Math.round(w / ratio);};";
            string result = $"<" +
                $"img id=\"{id}\"" +
                $"class=\"celebrity-photo\"" +
                $"title=\"{title}\"" +
                $"src=\"{src}\"" +
                $"onclick=\"{onclick}\"" +
                $"onload=\"{onload}\"" +
                $"/>";
            return new HtmlString(result);
        }
    }
}