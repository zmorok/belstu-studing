using FreelanceApp.Models;
using FreelanceApp.Services;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

// profile page
namespace FreelanceApp
{
    public partial class ProfileView : UserControl
    {
        #region Fields
        private readonly User _currentUser;
        private readonly ContentControl _mainContent;
        #endregion

        #region Properties
        public string IsDarkTheme => ThemeManager.IsDarkTheme ? "Тёмная" : "Светлая";
        public string Username => _currentUser.Username;
        #endregion

        #region Constructor
        public ProfileView(ContentControl mainContent, User user)
        {
            InitializeComponent();
            _currentUser = user;
            _mainContent = mainContent;
            DataContext = this;
        }
        #endregion

        #region ICommand
        public ICommand SetEnglishCommand => new RelayCommand(SetEnglish);
        public ICommand SetRussianCommand => new RelayCommand(SetRussian);
        public ICommand ToggleThemeCommand => new RelayCommand(ToggleTheme);
        public ICommand SaveCommand => new RelayCommand(SaveChanges);
        public ICommand GoBackCommand => new RelayCommand(GoBack);
        #endregion

        #region Methods
        private void SetEnglish()
        {
            Logger.Log(_currentUser, "Установил язык: EN");
            LanguageManager.SetLanguage("en");
        }

        private void SetRussian()
        {
            Logger.Log(_currentUser, "Установил язык: RU");
            LanguageManager.SetLanguage("ru");
        }

        private void ToggleTheme()
        {
            ThemeManager.ToggleTheme();
            Logger.Log(_currentUser, $"Сменил тему на: {IsDarkTheme}");
        }

        private void SaveChanges()
        {
            string newPassword = PasswordBox.Text.Trim();

            if (!string.IsNullOrWhiteSpace(newPassword))
            {
                _currentUser.Password = newPassword;

                bool success = UserDatabase.UpdateUser(_currentUser);
                if (success)
                {
                    Logger.Log(_currentUser, "Изменил пароль в профиле");
                    MessageBox.Show("Пароль успешно обновлён", "Успех", MessageBoxButton.OK, MessageBoxImage.Information);
                }
                else
                {
                    Logger.Log(_currentUser, "Ошибка при обновлении профиля (не найден)");
                    MessageBox.Show("Не удалось обновить данные пользователя", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
            else
            {
                MessageBox.Show("Поле пароля не должно быть пустым", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
        }

        private void GoBack()
        {
            _mainContent.Content = MainWindow.DefaultTextBlock;
            Logger.Log(_currentUser, "Вернулся на главное окно");
        }
        #endregion
    }
}
