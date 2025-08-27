using System.Globalization;
using System.Windows.Data;

namespace FreelanceApp.Services.Converters
{
    public class DecimalConverter : IValueConverter
    {
        public decimal Min { get; set; } = 0m;
        public decimal Max { get; set; } = 1000000m;

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is decimal d) return d.ToString(culture);
            return "0";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            string str = value?.ToString() ?? "0";
            if (decimal.TryParse(str, NumberStyles.Any, culture, out decimal result))
            {
                if (result < Min) return Min;
                if (result > Max) return Max;
                return result;
            }
            return Min;
        }
    }
}
