using System.Windows;

namespace FreelanceApp.Services
{
    // class for changing langs (ru/en)
    public static class LanguageManager
    {
        public static void SetLanguage(string langCode)
        {
            ResourceDictionary dict = new()
            {
                Source = langCode switch
                {
                    "ru" => new Uri("Strings/Strings.ru.xaml", UriKind.Relative),
                    _ => new Uri("Strings/Strings.en.xaml", UriKind.Relative),
                }
            };

            var langDict = Application.Current.Resources.MergedDictionaries
                           .FirstOrDefault(d => d.Source != null && d.Source.OriginalString.Contains("Strings"));

            if (langDict != null) Application.Current.Resources.MergedDictionaries.Remove(langDict);
            Application.Current.Resources.MergedDictionaries.Add(dict);
        }
    }
}
