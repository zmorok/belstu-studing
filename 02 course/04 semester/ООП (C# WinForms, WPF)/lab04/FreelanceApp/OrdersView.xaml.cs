using FreelanceApp.Models;
using FreelanceApp.DialogWindows;
using FreelanceApp.Services;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

// orders page
namespace FreelanceApp
{
    public partial class OrdersView : UserControl
    {
        #region Fields
        private readonly User _currentUser;
        private readonly ObservableCollection<Order> Orders;
        private List<Order> FilteredOrders;
        private readonly ContentControl MainContent;

        private bool isAscending = true;
        private string? lastSortField = null;
        #endregion

        #region Dependecy Properties
        public static readonly DependencyProperty MinPriceProperty = DependencyProperty.Register(
            "MinPrice",
            typeof(decimal),
            typeof(OrdersView),
            new FrameworkPropertyMetadata(
                1000m,
                FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
                OnMinPriceChanged,
                CoercePrice
            ),
            ValidatePrice
        );
        public decimal MinPrice
        {
            get => (decimal)GetValue(MinPriceProperty);
            set => SetValue(MinPriceProperty, value);
        }

        public static readonly DependencyProperty MaxPriceProperty = DependencyProperty.Register(
            "MaxPrice",
            typeof(decimal),
            typeof(OrdersView),
            new FrameworkPropertyMetadata(
                1000000m,
                FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
                OnMaxPriceChanged,
                CoercePrice
            ),
            ValidatePrice
        );
        public decimal MaxPrice
        {
            get => (decimal)GetValue(MaxPriceProperty);
            set => SetValue(MaxPriceProperty, value);
        }

        private static bool ValidatePrice(object value) => value is decimal d && d >= 0;
        private static object CoercePrice(DependencyObject d, object baseValue)
        {
            var value = (decimal)baseValue;
            return value < 0m ? 0m : value > 1000000m ? 1000000m : value;
        }
        private static void OnMinPriceChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is OrdersView view) Logger.Log(view._currentUser, $"MinPrice установлен: {e.NewValue}"); 
            
        }
        private static void OnMaxPriceChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is OrdersView view) Logger.Log(view._currentUser, $"MaxPrice установлен: {e.NewValue}");
        }
        #endregion

        #region ICommand
        public ICommand ApplyFilterCommand => new RelayCommand(ApplyFilter);
        public ICommand ResetFilterCommand => new RelayCommand(ResetFilter);
        public ICommand SortAscendingCommand => new RelayCommand(SortAscending);
        public ICommand SortDescendingCommand => new RelayCommand(SortDescending);
        public ICommand GoBackCommand => new RelayCommand(GoBack);
        #endregion

        #region Constructor
        public OrdersView(ContentControl MainControl, ObservableCollection<Order> orders, User user)
        {
            InitializeComponent();

            _currentUser = user;
            Orders = orders;
            MainContent = MainControl;

            FilteredOrders = Orders.ToList();
            OrdersList.ItemsSource = FilteredOrders;

            DataContext = this;

            OrderSelected += OrdersView_OrderSelected;
            PreviewOrderSelected += OrdersView_PreviewOrderSelected;
        }
        #endregion

        #region Methods
        private void ApplyFilter()
        {
            string selectedCategory = (CategoryBox.SelectedItem as ComboBoxItem)?.Content?.ToString() ?? "Все";
            var langDict = Application.Current.Resources.MergedDictionaries
                            .FirstOrDefault(d => d.Source != null && d.Source.OriginalString.Contains("Strings"))!;

            bool isEnglish = langDict?.Source?.OriginalString.Contains("en") == true;
            if (isEnglish)
            {
                selectedCategory = selectedCategory switch
                {
                    "All" or "all" => "Все",
                    "Development" => "Разработка",
                    "Design" => "Дизайн",
                    "Marketing" => "Маркетинг",
                    "Content" => "Контент",
                    "Other" or "Others" => "Разное",
                    _ => string.Empty,
                };
            }

            decimal minPrice = MinPrice;
            decimal maxPrice = MaxPrice;

            string keyword = SearchBox.Text.Trim().ToLower();

            FilteredOrders = Orders
                .Where(order =>
                    order.Price >= minPrice &&
                    order.Price <= maxPrice &&
                    (string.IsNullOrEmpty(keyword) ||
                     order.Title.ToLower().Contains(keyword) ||
                     order.Description.ToLower().Contains(keyword)) &&
                    (selectedCategory == "Все" || order.Category == selectedCategory)
                )
                .ToList();

            OrdersList.ItemsSource = FilteredOrders;
            OrdersList.Items.Refresh();
            Logger.Log(_currentUser, $"Отфильтровал заказы по критериям: {minPrice} - {maxPrice}, {keyword}, {selectedCategory}.");
        }

        private void ResetFilter()
        {
            MinPriceBox.Clear();
            MaxPriceBox.Clear();
            SearchBox.Clear();
            CategoryBox.SelectedIndex = 0;
            SortBox.SelectedIndex = 0;

            FilteredOrders = Orders.ToList();
            OrdersList.ItemsSource = FilteredOrders;
            OrdersList.Items.Refresh();
            Logger.Log(_currentUser, "Сбросил фильтры.");
        }

        private void GoBack()
        {
            MainContent.Content = MainWindow.DefaultTextBlock;
            Logger.Log(_currentUser, "Закрыл окно 'заказы'.");
        }

        private void SortAscending()
        {
            string sortField = GetSelectedSortField();
            if (string.IsNullOrEmpty(sortField))
            {
                OrdersList.ItemsSource = Orders;
                OrdersList.Items.Refresh();
                return;
            }

            if (lastSortField == sortField && isAscending)
            {
                FilteredOrders = Orders.ToList();
                OrdersList.ItemsSource = FilteredOrders;
                OrdersList.Items.Refresh();
                lastSortField = null;
                Logger.Log(_currentUser, "Сбросил сортировку по возрастанию.");
                return;
            }

            FilteredOrders = FilteredOrders.OrderBy(GetKeySelector(sortField)).ToList();
            OrdersList.ItemsSource = FilteredOrders;
            OrdersList.Items.Refresh();
            Logger.Log(_currentUser, $"Отсортировал заказы по полю {sortField} по возрастанию.");

            isAscending = true;
            lastSortField = sortField;
        }

        private void SortDescending()
        {
            string sortField = GetSelectedSortField();
            if (string.IsNullOrEmpty(sortField))
            {
                OrdersList.ItemsSource = Orders;
                OrdersList.Items.Refresh();
                return;
            }

            if (lastSortField == sortField && !isAscending)
            {
                FilteredOrders = Orders.ToList();
                OrdersList.ItemsSource = FilteredOrders;
                OrdersList.Items.Refresh();
                lastSortField = null;
                Logger.Log(_currentUser, "Сбросил сортировку по убыванию.");
                return;
            }

            FilteredOrders = FilteredOrders.OrderByDescending(GetKeySelector(sortField)).ToList();
            OrdersList.ItemsSource = FilteredOrders;
            OrdersList.Items.Refresh();
            Logger.Log(_currentUser, $"Отсортировал заказы по полю {sortField} по убыванию.");

            isAscending = false;
            lastSortField = sortField;
        }

        private string GetSelectedSortField()
        {
            string selected = (SortBox.SelectedItem as ComboBoxItem)?.Content?.ToString() ?? string.Empty;

            string priceLocalized = Application.Current.TryFindResource("OrdersView_Sorting_Type_Price")?.ToString() ?? string.Empty;
            string titleLocalized = Application.Current.TryFindResource("OrdersView_Sorting_Type_Name")?.ToString() ?? string.Empty;
            string categoryLocalized = Application.Current.TryFindResource("OrdersView_Sorting_Type_Category")?.ToString() ?? string.Empty;

            return selected switch
            {
                var s when s == priceLocalized => nameof(Order.Price),
                var s when s == titleLocalized => nameof(Order.Title),
                var s when s == categoryLocalized => nameof(Order.Category),
                _ => string.Empty
            };
        }

        private Func<Order, object> GetKeySelector(string field)
        {
            return field switch
            {
                nameof(Order.Price) => o => o.Price,
                nameof(Order.Title) => o => o.Title,
                nameof(Order.Category) => o => o.Category,
                _ => o => o.Order_Id
            };
        }

        public void RefreshFromOriginal()
        {
            FilteredOrders = Orders.ToList();
            OrdersList.ItemsSource = FilteredOrders;
            OrdersList.Items.Refresh();
        }

        private void Description(object sender, RoutedEventArgs e)
        {
            if (OrdersList.SelectedItem is Order order)
            {
                RoutedEventArgs previewArgs = new(PreviewOrderSelectedEvent);
                RaiseEvent(previewArgs);

                if (previewArgs.Handled)
                {
                    Logger.Log(_currentUser, "Отмена открытия описания заказа родителем.");
                    OrdersList.SelectedItem = null;
                    return;
                }

                FullDescriptionDialog dialog = new(order);
                dialog.ShowDialog();
                OrdersList.SelectedItem = null;

                Logger.Log(_currentUser, $"Открыл окно описания заказа {order.Order_Id}.");
                RaiseEvent(new RoutedEventArgs(OrderSelectedEvent));
            }
        }

        private void OrdersView_OrderSelected(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Пользователь выбрал заказ", "OrderSelected (RoutedEvent)");
        }
        private void OrdersView_PreviewOrderSelected(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Перехват до открытия описания заказа", "PreviewOrderSelected");
            //e.Handled = true;
        }

        #endregion

        #region Events
        // (for Description)
        // Bubble event
        public static readonly RoutedEvent OrderSelectedEvent = EventManager.RegisterRoutedEvent(
            "OrderSelected",
            RoutingStrategy.Bubble,
            typeof(RoutedEventHandler),
            typeof(OrdersView)
        );
        public event RoutedEventHandler OrderSelected
        {
            add => AddHandler(OrderSelectedEvent, value);
            remove => RemoveHandler(OrderSelectedEvent, value);
        }

        // Tunnel event
        public static readonly RoutedEvent PreviewOrderSelectedEvent = EventManager.RegisterRoutedEvent(
            "PreviewOrderSelected",
            RoutingStrategy.Tunnel,
            typeof(RoutedEventHandler),
            typeof(OrdersView)
        );
        public event RoutedEventHandler PreviewOrderSelected
        {
            add => AddHandler(PreviewOrderSelectedEvent, value);
            remove => RemoveHandler(PreviewOrderSelectedEvent, value);
        }
        #endregion
    }
}
