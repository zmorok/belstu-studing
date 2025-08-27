
/*
    форма "Адрес", которая создаётся при клике на соответствующую кнопку в главной форме 
 */

using System.ComponentModel.DataAnnotations;

namespace ApartmentForm
{
    public partial class AddressForm : Form
    {
        public DeveloperForm nextForm;
        private readonly ApartmentFormLogic _logic = new();

        public AddressForm()
        {
            InitializeComponent();
            nextForm = new DeveloperForm(this);
        }

        // инициализация полей формы данными адреса
        public void InitAddressContext(Address o)
        {
            textBoxCountry.Text = o.Country;
            textBoxCity.Text = o.City;
            textBoxDistrict.Text = o.District;
            textBoxStreet.Text = o.Street;
            textBoxHouse.Text = o.House;
            textBoxSubhouse.Text = o.Subhouse;
            textBoxApartmentNum.Text = o.ApartmentNumber;
        }

        // подтверждение адреса
        private void ButtonConfirmAddress_Click(object sender, EventArgs e)
        {
            Address currentAddress = new()
            {
                Country = textBoxCountry.Text,
                City = textBoxCity.Text,
                District = textBoxDistrict.Text,
                Street = textBoxStreet.Text,
                House = textBoxHouse.Text,
                Subhouse = textBoxSubhouse.Text,
                ApartmentNumber = textBoxApartmentNum.Text
            };

            var validationResults = new List<ValidationResult>();
            var validationContext = new ValidationContext(currentAddress);

            if (!Validator.TryValidateObject(currentAddress, validationContext, validationResults, true))
            {
                string errors = string.Join("\n", validationResults.Select(vr => vr.ErrorMessage));
                _logic.ErrorMessage($"Ошибка валидации:\n{errors}");
                return;
            }

            ApartmentForm.addressField = currentAddress;
            Close();
        }

        // очистка полей
        public void Clear()
        {
            foreach (Control control in Controls)
            {
                if (control is TextBox textBox) textBox.Clear();
                if (control is ListBox listBox) listBox.Items.Clear();
                if (control is ComboBox comboBox) comboBox.SelectedIndex = -1;
                if (control is CheckBox checkBox) checkBox.Checked = false;
                if (control is MonthCalendar calendar) calendar.SetDate(DateTime.Now);
                if (control is GroupBox groupBox)
                {
                    foreach (Control groupControl in groupBox.Controls)
                    {
                        if (groupControl is TextBox groupTextBox) groupTextBox.Clear();
                        if (groupControl is ComboBox groupComboBox) groupComboBox.SelectedIndex = -1;
                        if (groupControl is CheckBox groupCheckBox) groupCheckBox.Checked = false;
                    }
                }
            }
        }

        private void tsbForward_Click(object sender, EventArgs e)
        {
            Hide();
            nextForm.Owner = this;
            nextForm.InitDeveloperContext(ApartmentForm.developerField);
            nextForm.ShowDialog();
            Show();
        }

        private void tsbBack_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
