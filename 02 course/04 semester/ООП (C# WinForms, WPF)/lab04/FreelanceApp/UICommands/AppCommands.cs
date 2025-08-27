using System.Windows.Input;

namespace FreelanceApp.UICommands
{
    public static class AppCommands
    {
        public static readonly RoutedUICommand ShowHelp = new(
            "О программе",
            "ShowHelp",
            typeof(AppCommands),
            new() { new KeyGesture(Key.F1) }
        );
    }
}
