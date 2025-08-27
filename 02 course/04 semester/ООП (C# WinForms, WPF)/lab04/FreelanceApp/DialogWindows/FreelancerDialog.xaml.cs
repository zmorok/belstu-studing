using System.Collections.ObjectModel;
using System.ComponentModel.DataAnnotations;
using System.Windows;
using System.Windows.Input;
using FreelanceApp.Models;
using FreelanceApp.Services;

namespace FreelanceApp.DialogWindows
{
    // window represents part of admin panel to work with list of freelancers
    public partial class FreelancerDialog : Window
    {
        #region Fields
        private readonly User _currentUser;
        private readonly ObservableCollection<Freelancer> Freelancers;
        private readonly ObservableCollection<Freelancer> TempFreelancers;
        #endregion

        #region Properties
        public Freelancer? SelectedFreelancer { get; private set; } = null;
        #endregion

        #region ICommand
        public ICommand AddCommand => new RelayCommand(AddFreelancer);
        public ICommand SaveCommand => new RelayCommand(SaveFreelancer);
        public ICommand DeleteCommand => new RelayCommand(DeleteFreelancer);
        public ICommand ConfirmCommand => new RelayCommand(ConfirmChanges);
        public ICommand CancelCommand => new RelayCommand(CancelChanges);
        public ICommand ClearFieldsCommand => new RelayCommand(ClearFields);
        #endregion

        #region Constructor
        public FreelancerDialog(ObservableCollection<Freelancer> freelancers, User user)
        {
            InitializeComponent();
            _currentUser = user;
            Freelancers = freelancers;

            TempFreelancers = new ObservableCollection<Freelancer>(
                freelancers.Select(f => new Freelancer
                {
                    Freelancer_Id = f.Freelancer_Id,
                    Name = f.Name,
                    Skills = f.Skills,
                    Rating = f.Rating,
                    Description = f.Description
                }));

            FreelancersGrid.ItemsSource = TempFreelancers;
            DataContext = this;
        }
        #endregion

        #region Methods
        private void FreelancersGrid_SelectionChanged(object sender, RoutedEventArgs e)
        {
            if (FreelancersGrid.SelectedItem is Freelancer freelancer)
            {
                SelectedFreelancer = freelancer;
                NameBox.Text = freelancer.Name;
                SkillsBox.Text = freelancer.Skills;
                RatingBox.Text = freelancer.Rating.ToString("0.0");
                DescriptionBox.Text = freelancer.Description;
            }
        }
        private void ClearFields()
        {
            FreelancersGrid.SelectedItem = null;
            SelectedFreelancer = null;
            NameBox.Clear();
            SkillsBox.Clear();
            RatingBox.Clear();
            DescriptionBox.Clear();
        }
        private void AddFreelancer()
        {
            if (!double.TryParse(RatingBox.Text.Replace('.', ','), out double rating))
            {
                MessageBox.Show("Некорректный формат рейтинга!", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }
            Freelancer freelancer = new()
            {
                Name = NameBox.Text,
                Skills = SkillsBox.Text,
                Rating = rating,
                Description = DescriptionBox.Text
            };
            if (!Validate(freelancer)) return;
            TempFreelancers.Add(freelancer);
            FreelancersGrid.Items.Refresh();
            Logger.Log(_currentUser, $"Добавлен новый фрилансер: {freelancer.Name}.");
            ClearFields();
        }
        private void SaveFreelancer()
        {
            if (SelectedFreelancer == null) return;
            if (!double.TryParse(RatingBox.Text.Replace('.', ','), out double rating))
            {
                MessageBox.Show("Некорректный формат рейтинга!", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }
            Freelancer tempFreelancer = new()
            {
                Name = NameBox.Text,
                Skills = SkillsBox.Text,
                Rating = rating,
                Description = DescriptionBox.Text
            };
            if (!Validate(tempFreelancer)) return;
            SelectedFreelancer.Name = tempFreelancer.Name;
            SelectedFreelancer.Skills = tempFreelancer.Skills;
            SelectedFreelancer.Rating = tempFreelancer.Rating;
            SelectedFreelancer.Description = tempFreelancer.Description;
            FreelancersGrid.Items.Refresh();
            Logger.Log(_currentUser, $"Обновлён фрилансер: {SelectedFreelancer.Name}.");
            MessageBox.Show("Фрилансер обновлён!", "Успех", MessageBoxButton.OK, MessageBoxImage.Information);
        }
        private void DeleteFreelancer()
        {
            if (SelectedFreelancer != null)
            {
                var result = MessageBox.Show($"Удалить фрилансера '{SelectedFreelancer.Name}'?", "Удаление", MessageBoxButton.YesNo, MessageBoxImage.Warning);
                if (result == MessageBoxResult.Yes)
                {
                    TempFreelancers.Remove(SelectedFreelancer);
                    FreelancersGrid.Items.Refresh();
                    Logger.Log(_currentUser, $"Удалён фрилансер: {SelectedFreelancer.Name}.");
                    SelectedFreelancer = null;
                    ClearFields();
                }
            }
            else
                MessageBox.Show("Выберите фрилансера для удаления!", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Warning);
        }
        private void ConfirmChanges()
        {
            var confirm = MessageBox.Show("Сохранить изменения?", "Подтверждение", MessageBoxButton.YesNo, MessageBoxImage.Question);
            if (confirm != MessageBoxResult.Yes) return;
            MSSQL_DataManager.SyncFreelancers(Freelancers, TempFreelancers);
            Logger.Log(_currentUser, "Изменения сохранены.");
            DialogResult = true;
            Close();
        }
        private void CancelChanges()
        {
            Logger.Log(_currentUser, "Отменены изменения.");
            DialogResult = false;
            Close();
        }
        private static bool Validate(Freelancer freelancer)
        {
            var context = new ValidationContext(freelancer);
            var results = new List<ValidationResult>();
            if (!Validator.TryValidateObject(freelancer, context, results, true))
            {
                foreach (var error in results)
                    MessageBox.Show(error.ErrorMessage, "Ошибка валидации", MessageBoxButton.OK, MessageBoxImage.Warning);
                return false;
            }
            return true;
        }
        #endregion
    }
}
