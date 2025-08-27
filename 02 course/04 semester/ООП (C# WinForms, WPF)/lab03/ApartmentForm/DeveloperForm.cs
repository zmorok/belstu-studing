
/*
    форма "Застройщика", которая создаётся при клике на соответствующую кнопку в главной форме 
 */

using System.ComponentModel.DataAnnotations;

namespace ApartmentForm
{
    public partial class DeveloperForm : Form
    {
        public AddressForm prevForm;
        private readonly ApartmentFormLogic _logic = new();

        public DeveloperForm()
        {
            InitializeComponent();
            this.prevForm = new();
        }

        public DeveloperForm(AddressForm prevForm)
        {
            InitializeComponent();
            this.prevForm = prevForm;
        }

        // инициализация полей формы данными застройщика
        public void InitDeveloperContext(Developer o)
        {
            textBoxDevName.Text = o.Name;
            comboBoxDevType.Text = o.Type;
            textBoxDevAddress.Text = o.Address;
            textBoxDevINN.Text = o.INN;
        }

        // подтвердить выбор застройщика
        private void ButtonConfirmDeveloper_Click(object sender, EventArgs e)
        {
            Developer currentDeveloper = new()
            {
                Name = textBoxDevName.Text,
                Type = comboBoxDevType.Text,
                Address = textBoxDevAddress.Text,
                INN = textBoxDevINN.Text
            };

            var validationResults = new List<ValidationResult>();
            var validationContext = new ValidationContext(currentDeveloper);

            if (!Validator.TryValidateObject(currentDeveloper, validationContext, validationResults, true))
            {
                string errors = string.Join("\n", validationResults.Select(vr => vr.ErrorMessage));
                _logic.ErrorMessage($"Ошибка валидации:\n{errors}");
                return;
            }

            ApartmentForm.developerField = currentDeveloper;
            prevForm.Close();
            Close();
        }

        // очистить поля
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

        private void tsbBack_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
