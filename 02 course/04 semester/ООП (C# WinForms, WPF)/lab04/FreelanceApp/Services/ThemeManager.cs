using FreelanceApp.Models;
using System.Windows;

namespace FreelanceApp.Services
{
    // class for changing themes
    public static class ThemeManager
    {
        private static bool _isDarkTheme = IsCurrentThemeDark();
        public static bool IsDarkTheme => _isDarkTheme; // outer property

        public static void ToggleTheme(User currentUser = null)
        {
            ResourceDictionary dict = [];
            if (!_isDarkTheme)
            {
                dict.Source = new Uri("/Styles/Brushes/Brushes_BLACK.xaml", UriKind.Relative);
                _isDarkTheme = true;
            }
            else
            {
                dict.Source = new Uri("/Styles/Brushes/Brushes_WHITE.xaml", UriKind.Relative);
                _isDarkTheme = false;
            }

            string currentTheme = _isDarkTheme ? "BLACK" : "WHITE";  
            var existing = Application.Current.Resources.MergedDictionaries
                .FirstOrDefault(d => d.Source != null && d.Source.OriginalString.Contains("/Styles/Brushes/Brushes_"));

            if (existing != null) Application.Current.Resources.MergedDictionaries.Remove(existing);
            Application.Current.Resources.MergedDictionaries.Add(dict);
            if (currentUser != null) Logger.Log(currentUser, $"Текущая тема - {currentTheme}");
        }

        private static bool IsCurrentThemeDark()
        {
            var existing = Application.Current.Resources.MergedDictionaries
                .FirstOrDefault(d => d.Source != null && d.Source.OriginalString.Contains("/Styles/Brushes/Brushes_BLACK.xaml"));

            return existing == null;
        }
    }
}
