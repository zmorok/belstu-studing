using System.Collections.Generic;
using System.Text.Json;
using System.Windows.Forms;

namespace ApartmentForm
{
    public partial class ResultsForm : Form
    {
        private ApartmentFormLogic _logic = new();
        public ResultsForm(List<Apartment> results)
        {
            InitializeComponent();
            listBoxResults.DataSource = results;
        }

        private void ButtonSave_Click(object sender, EventArgs e)
        {

            SaveFileDialog sfd = new SaveFileDialog()
            {
                Filter = "JSON файлы (*.json)|*.json",
                Title = "Сохранить данные о результате"
            };
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                try
                {

                    string json = JsonSerializer.Serialize(listBoxResults.Items, ApartmentForm.serializerOptions);
                    File.WriteAllText(sfd.FileName, json);
                    _logic.InfoMessage("Результат сохранен");
                    Close();
                }
                catch (Exception ex)
                {
                    _logic.ErrorMessage("Ошибка сохранения: " + ex.Message);
                }
            }
        }
    }
}
