using System.Collections.ObjectModel;
using System.ComponentModel.DataAnnotations;
using System.Windows;
using System.Windows.Input;
using FreelanceApp.Models;
using FreelanceApp.Services;

namespace FreelanceApp.DialogWindows
{
    // window represents part of admin panel to work with list of orders
    public partial class OrderDialog : Window
    {
        #region Fields
        private readonly User _currentUser;
        private readonly ObservableCollection<Order> Orders;
        private readonly ObservableCollection<Order> TempOrders;
        private readonly UndoRedoManager<Order> _undoManager;
        #endregion

        #region Properties
        public Order? SelectedOrder { get; private set; }
        #endregion

        #region ICommand
        public ICommand AddCommand => new RelayCommand(AddOrder);
        public ICommand SaveCommand => new RelayCommand(SaveOrder);
        public ICommand DeleteCommand => new RelayCommand(DeleteOrder);
        public ICommand ConfirmCommand => new RelayCommand(ConfirmChanges);
        public ICommand CancelCommand => new RelayCommand(CancelChanges);
        public ICommand ClearFieldsCommand => new RelayCommand(ClearFields);
        public ICommand UndoCommand => new RelayCommand(() => _undoManager.Undo(TempOrders));
        public ICommand RedoCommand => new RelayCommand(() => _undoManager.Redo(TempOrders));
        #endregion

        #region Constructor
        public OrderDialog(ObservableCollection<Order> orders, User user)
        {
            InitializeComponent();
            _currentUser = user;
            Orders = orders;
            TempOrders = new ObservableCollection<Order>(
                orders.Select(o => new Order
                {
                    Order_Id = o.Order_Id,
                    Title = o.Title,
                    Description = o.Description,
                    Price = o.Price,
                    Category = o.Category,
                    FullDescription = o.FullDescription
                }));
            _undoManager = new UndoRedoManager<Order>(
                cloneFunc: CloneOrder,
                applyFunc: ApplyOrderState
            );
            OrdersGrid.ItemsSource = TempOrders;
            DataContext = this;
        }
        #endregion

        #region Methods
        private void OrdersGrid_SelectionChanged(object sender, RoutedEventArgs e)
        {
            if (OrdersGrid.SelectedItem is Order order)
            {
                SelectedOrder = order;
                TitleBox.Text = order.Title;
                CategoryBox.Text = order.Category;
                DescriptionBox.Text = order.Description;
                PriceBox.Text = order.Price.ToString();
                FullDescriptionBox.Text = order.FullDescription;
            }
        }
        private void AddOrder()
        {
            if (!decimal.TryParse(PriceBox.Text.Replace('.', ','), out decimal price))
            {
                MessageBox.Show("Некорректный формат цены!", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }
            Order order = new()
            {
                Title = TitleBox.Text,
                Category = CategoryBox.Text,
                Description = DescriptionBox.Text,
                Price = price,
                FullDescription = FullDescriptionBox.Text
            };
            if (!Validate(order)) return;
            TempOrders.Add(order);
            _undoManager.Push("add", order);
            OrdersGrid.Items.Refresh();
            Logger.Log(_currentUser, $"Добавлен новый заказ: {order.Title}.");
            ClearFields();
        }
        private void SaveOrder()
        {
            if (SelectedOrder == null) return;
            if (!decimal.TryParse(PriceBox.Text.Replace('.', ','), out decimal price))
            {
                MessageBox.Show("Некорректный формат цены!", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }
            Order updated = new()
            {
                Order_Id = SelectedOrder.Order_Id,
                Title = TitleBox.Text,
                Category = CategoryBox.Text,
                Description = DescriptionBox.Text,
                Price = price,
                FullDescription = FullDescriptionBox.Text
            };
            if (!Validate(updated)) return;
            _undoManager.Push("edit", CloneOrder(SelectedOrder), CloneOrder(updated));
            ApplyOrderState(SelectedOrder, updated);
            OrdersGrid.Items.Refresh();
            Logger.Log(_currentUser, $"Заказ '{SelectedOrder.Title}' обновлён.");
            MessageBox.Show("Заказ обновлён!", "Успех", MessageBoxButton.OK, MessageBoxImage.Information);
        }
        private void DeleteOrder()
        {
            if (SelectedOrder == null)
            {
                MessageBox.Show("Выберите заказ для удаления!", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }
            var result = MessageBox.Show($"Удалить заказ '{SelectedOrder.Title}'?", "Подтверждение", MessageBoxButton.YesNo, MessageBoxImage.Warning);
            if (result != MessageBoxResult.Yes) return;
            _undoManager.Push("delete", CloneOrder(SelectedOrder));
            TempOrders.Remove(SelectedOrder);
            OrdersGrid.Items.Refresh();
            Logger.Log(_currentUser, $"Заказ '{SelectedOrder.Title}' удалён.");
            ClearFields();
        }
        private void ConfirmChanges()
        {
            var confirm = MessageBox.Show("Сохранить изменения?", "Подтверждение", MessageBoxButton.YesNo, MessageBoxImage.Question);
            if (confirm != MessageBoxResult.Yes) return;
            MSSQL_DataManager.SyncOrders(Orders, TempOrders);
            Logger.Log(_currentUser, "Изменения сохранены в базе данных.");
            DialogResult = true;
            Close();
        }
        private void CancelChanges()
        {
            Logger.Log(_currentUser, "Отменил изменения.");
            DialogResult = false;
            Close();
        }
        private void ClearFields()
        {
            OrdersGrid.SelectedItem = null;
            SelectedOrder = null;
            TitleBox.Clear();
            CategoryBox.SelectedIndex = -1;
            CategoryBox.SelectedItem = null;
            DescriptionBox.Clear();
            PriceBox.Clear();
            FullDescriptionBox.Clear();
        }
        private static bool Validate(Order order)
        {
            var context = new ValidationContext(order);
            var results = new List<ValidationResult>();
            if (!Validator.TryValidateObject(order, context, results, true))
            {
                foreach (var error in results)
                    MessageBox.Show(error.ErrorMessage, "Ошибка валидации", MessageBoxButton.OK, MessageBoxImage.Warning);
                return false;
            }
            return true;
        }
        private static Order CloneOrder(Order original)
        {
            return new Order
            {
                Order_Id = original.Order_Id,
                Title = original.Title,
                Category = original.Category,
                Description = original.Description,
                Price = original.Price,
                FullDescription = original.FullDescription
            };
        }
        private static void ApplyOrderState(Order target, Order source)
        {
            target.Title = source.Title;
            target.Category = source.Category;
            target.Description = source.Description;
            target.Price = source.Price;
            target.FullDescription = source.FullDescription;
        }
        #endregion
    }
}
