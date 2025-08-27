namespace ApartmentForm
{
    public partial class SortOptionsForm : Form
    {
        public string PrimarySort { get; private set; }
        public string SecondarySort { get; private set; }

        public SortOptionsForm()
        {
            InitializeComponent();
            PrimarySort = comboBoxPrimary.Text;
            SecondarySort = comboBoxSecondary.Text;
        }

        private void BtnApplySort_Click(object sender, EventArgs e)
        {
            PrimarySort = comboBoxPrimary.SelectedItem?.ToString() ?? "Цена";
            SecondarySort = comboBoxSecondary.SelectedItem?.ToString() ?? "Метраж";

            DialogResult = DialogResult.OK;
            Close();
        }
    }
}
