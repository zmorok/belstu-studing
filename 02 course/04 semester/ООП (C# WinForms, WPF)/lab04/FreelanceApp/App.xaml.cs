using FreelanceApp.UICommands;
using System.Windows;
using System.Windows.Input;

namespace FreelanceApp;

public partial class App : Application
{
    private AboutWindow? ab_window;

    protected override void OnStartup(StartupEventArgs e)
    {
        base.OnStartup(e);

        // setting the cursor for app
        var uri = new Uri("pack://application:,,,/Cursors/cursor.cur", UriKind.Absolute);
        var stream = GetResourceStream(uri).Stream;
        Mouse.OverrideCursor = new Cursor(stream);

        // setting the global binding for the "ShowAbout" command (F1)
        CommandManager.RegisterClassCommandBinding(
            typeof(Window),
            new(AppCommands.ShowHelp, (e, s) => {
                if (ab_window == null || !ab_window.IsVisible)
                {
                    ab_window = new AboutWindow();
                    ab_window.Title = AppCommands.ShowHelp.Text;
                    ab_window.Closed += (s, args) => ab_window = null;
                    ab_window.Show();
                }
                else ab_window.Activate();
            })
        );
    }
}