using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using FreelanceApp.Models;

namespace FreelanceApp.DialogWindows
{
    // window with full information about entity
    public partial class FullDescriptionDialog : Window
    {
        #region Constructor
        public FullDescriptionDialog(object obj)
        {
            InitializeComponent();
            GenerateFields(obj);

            if (obj is Order order) Title = $"Заказ #{order.Order_Id}";
            else if (obj is Freelancer freelancer) Title = $"Фрилансер {freelancer.Name}";
            else if (obj is User user) Title = $"Пользователь {user.Username}";
            FullDescDirect += FullDescriptionDialog_OrderSelectedDirect;
            Loaded += (s, e) => RaiseEvent(new RoutedEventArgs(FullDescDirectEvent, this));
        }
        #endregion

        #region Methods
        private void GenerateFields(object obj)
        {
            if (obj == null) return;

            Type type = obj.GetType();
            string typeName = type.Name;
            PropertyInfo[] properties = type.GetProperties();

            foreach (var prop in properties)
            {
                string propName = prop.Name;

                if (propName.Contains("Id", StringComparison.OrdinalIgnoreCase)) continue;

                object value = prop.GetValue(obj) ?? "";

                string resourceKey = $"{typeName}_{propName}";
                string localizedName = TryFindResource(resourceKey) as string ?? propName;

                var label = new TextBlock
                {
                    Text = localizedName + ":",
                    FontWeight = FontWeights.Bold,
                    Margin = new Thickness(0, 10, 0, 2)
                };

                var text = new TextBlock
                {
                    Text = value.ToString(),
                    TextWrapping = TextWrapping.Wrap
                };

                ContentPanel.Children.Add(label);
                ContentPanel.Children.Add(text);
            }
        }
        private void FullDescriptionDialog_OrderSelectedDirect(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Событие Direct обработано локально в OrdersView", "OrderSelectedDirect");
        }
        #endregion

        #region Events
        // Direct event
        public static readonly RoutedEvent FullDescDirectEvent = EventManager.RegisterRoutedEvent(
            "FullDescDirect",
            RoutingStrategy.Direct,
            typeof(RoutedEventHandler),
            typeof(OrdersView)
        );
        public event RoutedEventHandler FullDescDirect
        {
            add => AddHandler(FullDescDirectEvent, value);
            remove => RemoveHandler(FullDescDirectEvent, value);
        }
        #endregion
    }
}
