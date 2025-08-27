using System.Windows;
using FreelanceApp.Services;
using System.Windows.Input;

namespace FreelanceApp.Authentication
{
    public partial class StartupWindow : Window
    {
        public StartupWindow()
        {
            InitializeComponent();
            DataContext = this;
        }

        // аутентификация
        public ICommand OpenLoginWindowCommand => new RelayCommand(() => {
            LoginWindow loginWindow = new();
            loginWindow.Show();
            Close();
        });

        // регистрация
        public ICommand OpenRegisterWindowCommand => new RelayCommand(() =>
        {
            RegisterWindow registerWindow = new();
            registerWindow.Show();
            Close();
        });
    }
}
