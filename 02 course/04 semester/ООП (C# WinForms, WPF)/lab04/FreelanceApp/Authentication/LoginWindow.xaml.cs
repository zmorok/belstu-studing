using FreelanceApp.Models;
using FreelanceApp.Services;
using System.Windows;
using System.Windows.Input;

namespace FreelanceApp.Authentication
{
    // window for user's login
    public partial class LoginWindow : Window
    {
        public LoginWindow()
        {
            InitializeComponent();
            DataContext = this;
        }

        public ICommand LoginCommand => new RelayCommand(() =>
        {
            string username = UsernameBox.Text;
            string password = PasswordBox.Password;

            User? user = UserDatabase.Authenticate(username, password);
            if (user != null)
            {
                string msg = Application.Current.TryFindResource("LoginWindow_Greetings")?.ToString() ?? string.Empty;
                MessageBox.Show($"{msg}, {user.Username} ({user.Role})!");

                Logger.Log(user, "Выполнил вход");

                MainWindow mainWindow = new(user);
                mainWindow.Show();
                Close();
            }
            else
            {
                Logger.LogError(new User() { Username = username, Password = password }, "Попытка входа с неверными данными");

                string msg = Application.Current.TryFindResource("LoginWindow_ErrorPassword")?.ToString() ?? string.Empty;
                MessageBox.Show(msg, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        });

        public ICommand GoBackCommand => new RelayCommand(() =>
        {
            StartupWindow startupWindow = new();
            startupWindow.Show();
            Close();
        });
    }
}
