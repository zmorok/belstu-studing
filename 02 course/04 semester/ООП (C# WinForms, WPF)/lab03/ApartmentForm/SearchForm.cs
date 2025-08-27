using System.Text.Json;
using System.Text.RegularExpressions;

namespace ApartmentForm
{
    public partial class SearchForm : Form
    {
        private List<Apartment> dataCollection;
        private ApartmentFormLogic _logic = new();

        public SearchForm(List<Apartment> collection)
        {
            InitializeComponent();
            dataCollection = collection;
        }

        // обработка поиска
        private void BtnSearch_Click(object sender, EventArgs e)
        {
            string searchQuery = textBoxSearch.Text;

            Regex regex;

            List<Apartment> results = [];

            // поиск на полное соответствие
            if (radioExactMatch.Checked)
            {
                results = dataCollection.Where(apartment =>
                    apartment.GetType().GetProperties().Any(prop =>
                    {
                        var value = prop.GetValue(apartment);

                        if (value is null) return false;

                        if (value is string strValue)
                            return strValue.Equals(searchQuery, StringComparison.OrdinalIgnoreCase);

                        if (value is double or int or decimal)
                            return value.ToString() == searchQuery;

                        if (value is IEnumerable<string> list)
                            return list.Any(item => item.Equals(searchQuery, StringComparison.OrdinalIgnoreCase));

                        return value.GetType().GetProperties().Any(nestedProp =>
                        {
                            var nestedValue = nestedProp.GetValue(value);
                            return nestedValue is string nestedStr &&
                                   nestedStr.Equals(searchQuery, StringComparison.OrdinalIgnoreCase);
                        });
                    })
                ).ToList();
            }

            // поиск по регулярному выражению
            else if (radioRegex.Checked)
            {
                try
                {
                    regex = new Regex(searchQuery, RegexOptions.IgnoreCase);
                    results = dataCollection.Where(item => regex.IsMatch(item.CheckingString)).ToList();
                }
                catch (Exception ex)
                {
                    _logic.ErrorMessage($"Ошибка в регулярном выражении: {ex.Message}");
                    return;
                }
            }
            else
            {
                _logic.WarningMessage("Выберите режим поиска!");
                return;
            }

            listBoxResults.Items.Clear();
            if (results.Count != 0)
            {
                listBoxResults.Items.AddRange(results.ToArray());
            }
            else
            {
                listBoxResults.Items.Add("Ничего не найдено.");
            }
        }

        // открытие конструктора запросов
        private void ButtonOpenQueryBuilder_Click(object sender, EventArgs e)
        {
            QueryBuilderForm queryBuilder = new QueryBuilderForm();
            if (queryBuilder.ShowDialog() == DialogResult.OK)
            {
                textBoxSearch.Text = queryBuilder.GeneratedQuery;
            }
        }

        // сохранение результатов поиска
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
                    listBoxResults.Items.Clear();
                    textBoxSearch.Text = "";
                    listBoxResults.Items.Add("Результаты сохранены в файл.");
                }
                catch (Exception ex)
                {
                    _logic.ErrorMessage("Ошибка сохранения: " + ex.Message);
                }
            }
        }
    }
}
