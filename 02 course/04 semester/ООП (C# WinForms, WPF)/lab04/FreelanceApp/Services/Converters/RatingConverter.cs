using System.Globalization;
using System.Windows.Data;

namespace FreelanceApp.Services.Converters
{
    internal class RatingConverter : IValueConverter
    {
        public double Min { get; set; } = 0;
        public double Max { get; set; } = 5;

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is double d) return d.ToString("F1", culture);
            return "0.0";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            string str = value?.ToString() ?? "0.0";
            if (double.TryParse(str, NumberStyles.Number, culture, out double result))
            {
                if (result < Min) return Min;
                if (result > Max) return Max;
                return result;
            }
            return Min;
        }
    }
}
