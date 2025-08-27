using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using FreelanceApp.Authentication;
using FreelanceApp.DialogWindows;
using FreelanceApp.Models;
using FreelanceApp.Services;

namespace FreelanceApp
{
    public partial class MainWindow : Window
    {
        private readonly User _currentUser;
        private ObservableCollection<Order> Orders = [];
        private ObservableCollection<Freelancer> Freelancers = [];

        public MainWindow(User user)
        {
            InitializeComponent();
            _currentUser = user;
            WelcomeText.Text = $"{WelcomeText.Text} {_currentUser.Username} ({_currentUser.Role})";
            MainContent.Content = DefaultTextBlock;

            LoadDbContext();

            if (_currentUser.Role == UserRole.Administrator)
                AdminPanel.Visibility = Visibility.Visible;
            
            DefaultTextBlock.SetResourceReference(TextBlock.ForegroundProperty, "Fg_Brush");
            DataContext = this;
            Logger.Log(_currentUser, "Открыл главное окно");
        }

        public ICommand ProfileCommand => new RelayCommand(OpenProfileWindow);
        public ICommand OrdersCommand => new RelayCommand(OpenOrdersWindow);
        public ICommand FreelancersCommand => new RelayCommand(OpenFreelancersWindow);
        public ICommand OrdersAdminCommand => new RelayCommand(OpenOrdersAdminDialog);
        public ICommand FreelancersAdminCommand => new RelayCommand(OpenFreelancersAdminDialog);
        public ICommand LogoutCommand => new RelayCommand(LogoutUser);
        public ICommand SetEnglishCommand => new RelayCommand(() =>
        {
            Logger.Log(_currentUser, "Текущий язык - EN");
            LanguageManager.SetLanguage("en");
            UpdateText();
        });
        public ICommand SetRussianCommand => new RelayCommand(() =>
        {
            Logger.Log(_currentUser, "Текущий язык - RU");
            LanguageManager.SetLanguage("ru");
            UpdateText();
        });
        public ICommand ToggleThemeCommand => new RelayCommand(() =>
        {
            ThemeManager.ToggleTheme();
        });

        private void LoadDbContext()
        {
            using var context = new FreelanceAppContext();
            Init.Execute(context);
            MSSQL_DataManager.LoadData(out Orders, out Freelancers);
        }

        private void OpenProfileWindow()
        {
            Logger.Log(_currentUser, "Выбрал пункт 'личный кабинет'");
            MainContent.Content = new ProfileView(MainContent, _currentUser);
        }

        private void OpenOrdersWindow()
        {
            if (MainContent.Content is FreelancersView)
                Logger.Log(_currentUser, "Закрыл окно 'фрилансеры'");
            else if (MainContent.Content is OrdersView)
            {
                MainContent.Content = new OrdersView(MainContent, Orders, _currentUser);
                return;
            }

            Logger.Log(_currentUser, "Выбрал пункт 'заказы'");
            MainContent.Content = new OrdersView(MainContent, Orders, _currentUser);
        }

        private void OpenFreelancersWindow()
        {
            if (MainContent.Content is OrdersView)
                Logger.Log(_currentUser, "Закрыл окно 'заказы'");
            else if (MainContent.Content is FreelancersView)
            {
                MainContent.Content = new FreelancersView(MainContent, Freelancers, _currentUser);
                return;
            }

            Logger.Log(_currentUser, "Выбрал пункт 'фрилансеры'");
            MainContent.Content = new FreelancersView(MainContent, Freelancers, _currentUser);
        }

        private void OpenOrdersAdminDialog()
        {
            Logger.Log(_currentUser, "Выбрал пункт 'управление заказами'");
            var dialog = new OrderDialog(Orders, _currentUser);
            if (dialog.ShowDialog() == true)
            {
                (MainContent.Content as OrdersView)?.RefreshFromOriginal();
            }
            Logger.Log(_currentUser, "Закрыл окно 'управление заказами'");
        }

        private void OpenFreelancersAdminDialog()
        {
            Logger.Log(_currentUser, "Выбрал пункт 'управление фрилансерами'");
            var dialog = new FreelancerDialog(Freelancers, _currentUser);
            if (dialog.ShowDialog() == true)
            {
                (MainContent.Content as FreelancersView)?.RefreshFromOriginal();
            }
            Logger.Log(_currentUser, "Закрыл окно 'управление фрилансерами'");
        }

        private void LogoutUser()
        {
            Logger.Log(_currentUser, "Вышел");
            new StartupWindow().Show();
            Close();
        }

        private void UpdateText()
        {
            var header = TryFindResource("MainWindow_Header") as string ?? "Welcome,";
            WelcomeText.Text = $"{header} {_currentUser.Username} ({_currentUser.Role})";
            DefaultTextBlock.Text = Application.Current.TryFindResource("DefaultTextBlock_Text")?.ToString() ?? string.Empty;
        }

        public static TextBlock DefaultTextBlock = new()
        {
            Text = Application.Current.TryFindResource("DefaultTextBlock_Text")?.ToString() ?? string.Empty,
            HorizontalAlignment = HorizontalAlignment.Center,
            VerticalAlignment = VerticalAlignment.Center
        };
    }
}
