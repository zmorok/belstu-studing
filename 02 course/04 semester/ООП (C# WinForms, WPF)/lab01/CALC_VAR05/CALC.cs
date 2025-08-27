namespace CALC_VAR05
{
    public partial class CALC : Form
    {
        Calculator calculator;
        public CALC()
        {
            InitializeComponent();
            calculator = new Calculator();
        }

        private void buttonCalculate_Click(object sender, EventArgs e)
        {
            try
            {
                boxPrice.Text = boxPrice.Text.Replace(".", ",");
                boxVolume.Text = boxVolume.Text.Replace(".", ",");
                boxPlusPrice.Text = boxPlusPrice.Text.Replace(".", ",");
                boxDailyUsage.Text = boxDailyUsage.Text.Replace(".", ",");
                if (string.IsNullOrWhiteSpace(boxPrice.Text) || string.IsNullOrWhiteSpace(boxVolume.Text))
                {
                    MessageBox.Show("Пустые поля!\nНеобходимо заполнить Стоимость и Объём!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }
                if (!double.TryParse(boxPrice.Text, out double price) || !double.TryParse(boxVolume.Text, out double volume))
                {
                    MessageBox.Show("Поля - это числа, а не символы!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }
                if (price < 0 || volume < 0)
                {
                    MessageBox.Show("Стоимость и Объём не могут быть меньше 0!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

                double pricePerVolume = calculator.CalcPricePerVolume(price, volume);
                boxPricePerVolume.Text = pricePerVolume.ToString();

                if (!string.IsNullOrWhiteSpace(boxPlusPrice.Text) && double.TryParse(boxPlusPrice.Text, out double plusPrice))
                {
                    if (pricePerVolume > plusPrice)
                    {
                        double selfPrice = calculator.CalcSelfPrice(pricePerVolume, plusPrice);
                        boxSelfPrice.Text = selfPrice.ToString();
                    }
                    else
                    {
                        MessageBox.Show("Себестоимость не может быть меньше или равна 0!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        return;
                    }
                }

                if (!string.IsNullOrWhiteSpace(boxDailyUsage.Text) && double.TryParse(boxDailyUsage.Text, out double dailyUsage))
                {
                    double monthlyPrice = calculator.CalcMountlyPrice(pricePerVolume, dailyUsage);
                    boxPricePerMonth.Text = monthlyPrice.ToString();
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка: {ex.Message}", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }

    public interface ICalculator
    {
        public double CalcPricePerVolume(double price, double volume);
        public double CalcSelfPrice(double pricePerVolume, double plusPrice);
        public double CalcMountlyPrice(double pricePerVolume, double dailyUsage);
    }

    public class Calculator : ICalculator
    {
        public double CalcPricePerVolume(double price, double volume) => price / volume;
        public double CalcSelfPrice(double pricePerVolume, double plusPrice) => pricePerVolume - plusPrice;
        public double CalcMountlyPrice(double pricePerVolume, double dailyUsage) => pricePerVolume * dailyUsage * 30;
    }
}
