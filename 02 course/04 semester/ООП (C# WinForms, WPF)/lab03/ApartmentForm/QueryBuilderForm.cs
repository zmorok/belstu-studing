using System;
using System.Windows.Forms;

namespace ApartmentForm
{
    public partial class QueryBuilderForm : Form
    {
        public string GeneratedQuery { get; private set; } = string.Empty;

        public QueryBuilderForm()
        {
            InitializeComponent();
        }

        private void btnGenerateQuery_Click(object sender, EventArgs e)
        {
            string pattern = "";

            if (checkBoxNumbers.Checked)
                pattern += @"\d+"; // Числа
            if (checkBoxLetters.Checked)
                pattern += @"[a-zA-Z]+"; // Буквы
            if (checkBoxSpecialChars.Checked)
                pattern += @"\W+"; // Спецсимволы

            if (!string.IsNullOrEmpty(textBoxCustomPattern.Text))
                pattern = textBoxCustomPattern.Text; // Если пользователь ввел собственный шаблон

            if (string.IsNullOrWhiteSpace(pattern))
            {
                MessageBox.Show("Выберите или введите шаблон!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            GeneratedQuery = pattern;
            this.DialogResult = DialogResult.OK;
            this.Close();
        }
    }
}
