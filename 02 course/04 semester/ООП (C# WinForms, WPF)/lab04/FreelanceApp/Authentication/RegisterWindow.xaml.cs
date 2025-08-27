using FreelanceApp.Models;
using FreelanceApp.Services;
using System.Windows;
using System.Windows.Input;

namespace FreelanceApp.Authentication
{
    public partial class RegisterWindow : Window
    {
        public RegisterWindow()
        {
            InitializeComponent();
            DataContext = this;
        }

        public ICommand RegisterCommand => new RelayCommand(() =>
        {
            string username = NewUsernameBox.Text;
            string password = NewPasswordBox.Password;
            UserRole role = RoleBox.SelectedIndex == 0 ? UserRole.Client : UserRole.Administrator;

            if (UserDatabase.Register(username, password, role))
            {
                string msg = Application.Current.TryFindResource("RegisterWindow_Greetings")?.ToString() ?? string.Empty;
                MessageBox.Show(msg, "", MessageBoxButton.OK, MessageBoxImage.Information);

                User user = new() { Username = username, Password = password, Role = role };
                Logger.Log(user, "Зарегистрировался");

                MainWindow mainWindow = new(user);
                mainWindow.Show();
                Close();
            }
            else
            {
                User user = new() { Username = username, Password = password };
                Logger.Log(user, "Попытка зарегистрироваться с уже существующим именем");

                string msg = Application.Current.TryFindResource("RegisterWindow_ErrorRegistration")?.ToString() ?? string.Empty;
                MessageBox.Show(msg, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        });

        public ICommand GoBackCommand => new RelayCommand(() =>
        {
            StartupWindow startupWindow = new StartupWindow();
            startupWindow.Show();
            Close();
        });
    }
}
