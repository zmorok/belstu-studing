using FreelanceApp.Models;
using FreelanceApp.DialogWindows;
using FreelanceApp.Services;
using System.Collections.ObjectModel;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows;

// freelancers page
namespace FreelanceApp
{
    public partial class FreelancersView : UserControl
    {
        #region Fields
        private readonly User _currentUser;
        private readonly ObservableCollection<Freelancer> Freelancers;
        private List<Freelancer> FilteredFreelancers;
        private readonly ContentControl MainContent;

        private bool isAscending = true;
        private string? lastSortField = null;
        #endregion

        #region Dependency Properties
        public static readonly DependencyProperty MaximumRatingThresholdProperty = DependencyProperty.Register(
            "MaximumRatingThreshold",
            typeof(double),
            typeof(FreelancersView),
            new FrameworkPropertyMetadata(
                5.0,
                FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
                OnMaxRatingChanged,
                CoerceRating
            ),
            ValidateRating
        );
        public double MaximumRatingThreshold
        {
            get => (double)GetValue(MaximumRatingThresholdProperty);
            set => SetValue(MaximumRatingThresholdProperty, value);
        }
        private static bool ValidateRating(object value) => value is double d && d >= 0 && d <= 5;
        private static object CoerceRating(DependencyObject d, object baseValue)
        {
            double val = (double)baseValue;
            return val < 0 ? 0 : val > 5 ? 5 : val;
        }
        private static void OnMaxRatingChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is FreelancersView view) Logger.Log(view._currentUser, $"MaximumRatingThreshold: {e.NewValue}");
        }
        #endregion

        #region ICommand
        public ICommand ApplyFilterCommand => new RelayCommand(ApplyFilter);
        public ICommand ResetFilterCommand => new RelayCommand(ResetFilter);
        public ICommand GoBackCommand => new RelayCommand(GoBack);
        public ICommand SortAscendingCommand => new RelayCommand(SortAscending);
        public ICommand SortDescendingCommand => new RelayCommand(SortDescending);
        #endregion

        #region Constructor
        public FreelancersView(ContentControl MainContent, ObservableCollection<Freelancer> freelancers, User user)
        {
            InitializeComponent();

            _currentUser = user;
            this.MainContent = MainContent;
            Freelancers = freelancers;
            FilteredFreelancers = freelancers.ToList();

            FreelancersList.ItemsSource = FilteredFreelancers;

            DataContext = this;
        }
        #endregion

        #region Methods
        private void ApplyFilter()
        {
            string nameFilter = NameBox.Text.Trim().ToLower();
            string skillFilter = SkillBox.Text.Trim().ToLower();
            double minRating = MaximumRatingThreshold;

            FilteredFreelancers = Freelancers
                .Where(f =>
                    f.Rating >= minRating &&
                    f.Name.ToLower().Contains(nameFilter) &&
                    f.Skills.ToLower().Contains(skillFilter))
                .ToList();

            FreelancersList.ItemsSource = FilteredFreelancers;
            FreelancersList.Items.Refresh();

            Logger.Log(_currentUser, $"Отфильтровал фрилансеров по критериям: {nameFilter}, {skillFilter}, {minRating}.");
        }

        private void ResetFilter()
        {
            NameBox.Clear();
            SkillBox.Clear();
            MinRatingBox.Clear();

            FilteredFreelancers = Freelancers.ToList();
            FreelancersList.ItemsSource = FilteredFreelancers;
            FreelancersList.Items.Refresh();

            Logger.Log(_currentUser, "Сбросил фильтры фрилансеров.");
        }

        private void GoBack()
        {
            MainContent.Content = MainWindow.DefaultTextBlock;
            Logger.Log(_currentUser, "Закрыл окно 'фрилансеры'.");
        }

        private void SortAscending()
        {
            string field = GetSelectedSortField();
            if (string.IsNullOrEmpty(field)) return;

            if (lastSortField == field && isAscending)
            {
                lastSortField = null;
                FreelancersList.ItemsSource = FilteredFreelancers;
                FreelancersList.Items.Refresh();
                Logger.Log(_currentUser, "Сбросил сортировку по возрастанию.");
                return;
            }

            isAscending = true;
            lastSortField = field;

            var sorted = FilteredFreelancers.OrderBy(GetKeySelector(field)).ToList();
            FreelancersList.ItemsSource = sorted;
            FreelancersList.Items.Refresh();

            Logger.Log(_currentUser, $"Отсортировал фрилансеров по полю {field} по возрастанию.");
        }

        private void SortDescending()
        {
            string field = GetSelectedSortField();
            if (string.IsNullOrEmpty(field)) return;

            if (lastSortField == field && !isAscending)
            {
                lastSortField = null;
                FreelancersList.ItemsSource = FilteredFreelancers;
                FreelancersList.Items.Refresh();
                Logger.Log(_currentUser, "Сбросил сортировку по убыванию.");
                return;
            }

            isAscending = false;
            lastSortField = field;

            var sorted = FilteredFreelancers.OrderByDescending(GetKeySelector(field)).ToList();
            FreelancersList.ItemsSource = sorted;
            FreelancersList.Items.Refresh();

            Logger.Log(_currentUser, $"Отсортировал фрилансеров по полю {field} по убыванию.");
        }

        private string GetSelectedSortField()
        {
            string selected = (SortBox.SelectedItem as ComboBoxItem)?.Content?.ToString() ?? string.Empty;

            string nameLocalized = TryFindResource("FreelancersView_Sorting_Type_Name") as string ?? string.Empty;
            string ratingLocalized = TryFindResource("FreelancersView_Sorting_Type_Rating") as string ?? string.Empty;

            return selected switch
            {
                var s when s == nameLocalized => nameof(Freelancer.Name),
                var s when s == ratingLocalized => nameof(Freelancer.Rating),
                _ => string.Empty
            };
        }

        private Func<Freelancer, object> GetKeySelector(string field)
        {
            return field switch
            {
                nameof(Freelancer.Name) => f => f.Name,
                nameof(Freelancer.Rating) => f => f.Rating,
                _ => f => f.Freelancer_Id
            };
        }

        public void RefreshFromOriginal()
        {
            FilteredFreelancers = Freelancers.ToList();
            FreelancersList.ItemsSource = FilteredFreelancers;
            FreelancersList.Items.Refresh();
        }

        private void Description(object sender, MouseButtonEventArgs e)
        {
            if (FreelancersList.SelectedItem is Freelancer freelancer)
            {
                var dialog = new FullDescriptionDialog(freelancer);
                dialog.ShowDialog();
                FreelancersList.SelectedItem = null;

                Logger.Log(_currentUser, $"Открыл окно 'фрилансер' с ID {freelancer.Freelancer_Id}.");
            }
        }
        #endregion
    }
}
