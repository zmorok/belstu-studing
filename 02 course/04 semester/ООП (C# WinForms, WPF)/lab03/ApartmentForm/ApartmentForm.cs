using System.Globalization;
using System.Net;
using System.Reflection;
using System.Text;
using System.Text.Encodings.Web;
using System.Text.Json;

/*
    главная форма с логикой для работы с квартирой
    включает в себя методы для обработки событий + методы для работы с данными
 */

namespace ApartmentForm
{
    public partial class ApartmentForm : Form
    {
        private ApartmentFormLogic _logic;

        // Для демонстрации – список квартир, с которым будем работать (при добавлении/сохранении можно расширять логику)
        private List<Apartment> apartments = new List<Apartment>();

        // поля для хранения адреса и застройщика
        public static Address addressField = new();
        public static Developer developerField = new();

        // формы для редактирования адреса и застройщика
        public DeveloperForm developerForm;
        public AddressForm addressForm;

        // закрепление панели
        private bool isToolStripLocked = false;

        public ApartmentForm()
        {
            InitializeComponent();
            _logic = new ApartmentFormLogic();
            developerForm = new DeveloperForm();
            addressForm = new AddressForm();
        }

        #region Новая логика
        // удалить первый элемент в списке квартир
        private void TsbDelete_Click(object sender, EventArgs e)
        {
            if (apartments.Any())
            {
                apartments.RemoveAt(0);
                _logic.InfoMessage("Удалён элемент.");
                UpdateStatus("Удалён элемент");
            }
        }

        // кнопка Вперёд
        private void TsbForward_Click(object sender, EventArgs e)
        {
            UpdateStatus("Переход вперед");
            if (addressForm.IsDisposed)
            {
                addressForm = new AddressForm();
            }
            addressForm.InitAddressContext(addressField);
            addressForm.ShowDialog();
        }

        // скрыть/показать меню
        private void ButtonShowMenu_Click(object sender, EventArgs e)
        {
            if (isToolStripLocked)
            {
                _logic.WarningMessage("Панель закреплена и не может быть скрыта.");
                return;
            }

            menuStripMain.Visible = !menuStripMain.Visible;
            buttonShowMenu.Text = menuStripMain.Visible ? "Скрыть панель" : "Показать панель";
        }

        // закрепление/открепление панели
        private void ButtonLockMenu_Click(object sender, EventArgs e)
        {
            isToolStripLocked = !isToolStripLocked;
            buttonLockMenu.Text = isToolStripLocked ? "Открепить панель" : "Закрепить панель";
            UpdateStatus(isToolStripLocked ? "Панель закреплена" : "Панель откреплена");

            foreach (ToolStripItem item in menuStripMain.Items)
            {
                item.Enabled = !isToolStripLocked;
            }
        }

        // кнопка Поиск
        private void TsbSearch_Click(object sender, EventArgs e)
        {
            SearchForm searchForm = new(apartments);
            searchForm.ShowDialog();
        }

        // кнопка Сортировка
        private void TsbSort_Click(object sender, EventArgs e)
        {
            SortOptionsForm sortOptionsForm = new SortOptionsForm();

            if (sortOptionsForm.ShowDialog() == DialogResult.OK)
            {
                var sorted = apartments;
                string primarySort = sortOptionsForm.PrimarySort;
                string secondarySort = sortOptionsForm.SecondarySort;


                switch (primarySort)
                {
                    case "Возрастание":
                        {
                            switch (secondarySort)
                            {
                                case "Цена":
                                    sorted = sorted.OrderBy(a => a.CalculatePrice()).ToList();
                                    break;
                                case "Метраж":
                                    sorted = sorted.OrderBy(a => a.Meters).ToList();
                                    break;
                                case "Количество комнат":
                                    sorted = sorted.OrderBy(a => a.RoomCount).ToList();
                                    break;
                            }
                            break;
                        }
                    case "Убывание":
                        {
                            switch (secondarySort)
                            {
                                case "Цена":
                                    sorted = sorted.OrderByDescending(a => a.CalculatePrice()).ToList();
                                    break;
                                case "Метраж":
                                    sorted = sorted.OrderByDescending(a => a.Meters).ToList();
                                    break;
                                case "Количество комнат":
                                    sorted = sorted.OrderByDescending(a => a.RoomCount).ToList();
                                    break;
                            }
                            break;
                        }
                }

                ResultsForm resultsForm = new ResultsForm(sorted);
                resultsForm.Show();
                UpdateStatus($"Сортировка выполнена ({primarySort}, {secondarySort})");
            }
        }

        // "О программе"
        private void TsbAbout_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Версия: 1.0.0\nРазработчик: Жук Дмитрий Сергеевич", "О программе");
        }

        // обновление строки состояния
        private void UpdateStatus(string action)
        {
            statusLabelCount.Text = $"Объектов: {apartments.Count}";
            statusLabelAction.Text = $"Последнее действие: {action}";
            statusLabelDateTime.Text = DateTime.Now.ToString("dd.MM.yyyy HH:mm:ss");
        }
        #endregion

        // опции для сериализации (форматирование + убирается \u...)
        public readonly static JsonSerializerOptions serializerOptions = new()
        {
            WriteIndented = true,
            Encoder = JavaScriptEncoder.UnsafeRelaxedJsonEscaping,
        };

        // сбор данных о квартире
        private Apartment? ComputeData()
        {
            if (!double.TryParse(textBoxMeters.Text, out double meters) || meters <= 0)
            {
                _logic.ErrorMessage("Введите корректное значение метража.");
                return null;
            }

            if (!int.TryParse(numericUpDownRoomCount.Text, out int roomCount) || roomCount <= 0)
            {
                _logic.ErrorMessage("Введите корректное количество комнат.");
                return null;
            }

            int yearBuilt = monthCalendar.SelectionStart.Year;
            if (yearBuilt < 1800 || yearBuilt > DateTime.Now.Year)
            {
                _logic.ErrorMessage("Введите корректный год постройки.");
                return null;
            }

            string material = comboBoxMaterials.SelectedItem?.ToString() ?? "";
            if (string.IsNullOrWhiteSpace(material))
            {
                _logic.ErrorMessage("Выберите материал стен.");
                return null;
            }

            List<string> options = [];
            foreach (CheckBox check in new CheckBox[] { checkBox1, checkBox2, checkBox3, checkBox4, checkBox5, checkBox6, })
            {
                if (check.Checked)
                    options.Add(check.Text);
            }

            Address address = new(addressField);

            return new()
            {
                Meters = meters,
                RoomCount = roomCount,
                YearBuilt = yearBuilt,
                Material = material,
                Options = options,
                Address = address,
                Developer = new(developerField),
            };
        }

        // вывод цены квартиры в настоящее время
        private void CalculateAndDisplayPrice(object sender, EventArgs e)
        {
            if (!double.TryParse(textBoxMeters.Text, out double meters) || meters <= 0)
            {
                textBoxPrice.Text = "0";
                return;
            }

            if (!int.TryParse(numericUpDownRoomCount.Text, out int roomCount) || roomCount <= 0)
            {
                textBoxPrice.Text = "0";
                return;
            }

            string material = comboBoxMaterials.SelectedItem?.ToString() ?? "";
            if (string.IsNullOrWhiteSpace(material))
            {
                textBoxPrice.Text = "0";
                return;
            }

            List<string> options = [];
            foreach (CheckBox check in new CheckBox[] { checkBox1, checkBox2, checkBox3, checkBox4, checkBox5, checkBox6, })
            {
                if (check.Checked)
                    options.Add(check.Text);
            }

            Apartment apartment = new Apartment
            {
                Meters = meters,
                RoomCount = roomCount,
                Material = material,
                Options = options,
                YearBuilt = monthCalendar.SelectionStart.Year,
            };

            textBoxPrice.Text = apartment.Price;
        }

        // вывод квартиры
        private void DisplayApartment(Apartment apartment)
        {
            foreach (var property in apartment.GetType().GetProperties(BindingFlags.Public | BindingFlags.Instance))
            {
                var value = property.GetValue(apartment)!;
                switch (property.Name)
                {
                    case "Address":
                        {
                            listBoxResult.Items.Add("Адрес:");
                            foreach (var addressProperty in value.GetType().GetProperties(BindingFlags.Public | BindingFlags.Instance))
                            {
                                var addressValue = addressProperty.GetValue(value);
                                listBoxResult.Items.Add($"\t{addressProperty.Name}: {addressValue}");
                            }
                        }
                        break;
                    case "Developer":
                        {
                            StringBuilder str = new();
                            foreach (var developerProperty in value.GetType().GetProperties(BindingFlags.Public | BindingFlags.Instance))
                            {
                                str.Append($"{developerProperty.Name}: {developerProperty.GetValue(value)}; ");
                            }
                            str.Length -= 2;
                            listBoxResult.Items.Add("Застройщик: " + str.ToString());
                        }
                        break;
                    case "Options":
                        {
                            StringBuilder str = new();
                            str.Append(property.Name + ": [");
                            foreach (var item in (System.Collections.IEnumerable)value)
                            {
                                str.Append(item + ", ");
                            }
                            str.Length -= 2;
                            str.Append(']');
                            listBoxResult.Items.Add(str.ToString());
                        }
                        break;
                    default:
                        listBoxResult.Items.Add($"{property.Name}: {value}");
                        break;
                }
            }
        }

        // очистка всех полей 
        private void ButtonClear_Click(object sender, EventArgs e)
        {
            if (_logic.QuestionMessage("Вы уверены, что хотите очистить все поля?") == DialogResult.Yes)
            {
                addressForm.Clear();
                _logic.AddressFieldClear();

                developerForm.Clear();
                _logic.DeveloperFieldClear();

                foreach (Control control in Controls)
                {
                    if (control is TextBox textBox)
                        textBox.Clear();
                    if (control is ListBox listBox)
                        listBox.Items.Clear();
                    if (control is ComboBox comboBox)
                        comboBox.SelectedIndex = -1;
                    if (control is CheckBox checkBox)
                        checkBox.Checked = false;
                    if (control is MonthCalendar calendar)
                        calendar.SetDate(DateTime.Now);
                    if (control is NumericUpDown numeric)
                        numeric.Value = 0;
                    if (control is TrackBar trackBar)
                        trackBar.Value = 0;
                    if (control is GroupBox groupBox)
                    {
                        foreach (Control groupControl in groupBox.Controls)
                        {
                            if (groupControl is TextBox groupTextBox)
                                groupTextBox.Clear();
                            if (groupControl is ComboBox groupComboBox)
                                groupComboBox.SelectedIndex = -1;
                            if (groupControl is CheckBox groupCheckBox)
                                groupCheckBox.Checked = false;
                        }
                    }
                }
            }
        }

        // печать данных о квартире
        private void ButtonPrint_Click(object sender, EventArgs e)
        {
            Apartment? apartment = ComputeData();
            if (apartment == null)
                return;
            else
            {
                listBoxResult.Items.Clear();
                listBoxResult.Items.Add("Текущая квартира:");
                DisplayApartment(apartment);
            }
        }

        // сохранение данных о квартире в файл
        private void ButtonSave_Click(object sender, EventArgs e)
        {
            Apartment? apartment = ComputeData();
            if (apartment == null) return;

            SaveFileDialog saveFileDialog = new SaveFileDialog
            {
                Filter = "JSON файлы (*.json)|*.json",
                Title = "Сохранить данные о квартире",
            };

            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    string json = JsonSerializer.Serialize(apartment, serializerOptions);
                    File.WriteAllText(saveFileDialog.FileName, json);
                    _logic.InfoMessage("Данные успешно сохранены.");
                    listBoxResult.Items.Clear();
                    listBoxResult.Items.Add("Сохранённая квартира:");
                    DisplayApartment(apartment);
                }
                catch (Exception ex)
                {
                    _logic.ErrorMessage("Ошибка при сохранении файла: " + ex.Message);
                }
            }
        }

        // загрузка данных о квартире из файла
        private void ButtonLoad_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog
            {
                Filter = "JSON файлы (*.json)|*.json",
                Title = "Выберите файл с данными о квартире",
            };

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    string json = File.ReadAllText(openFileDialog.FileName);
                    Apartment? loadedApartment = JsonSerializer.Deserialize<Apartment>(json);

                    if (loadedApartment != null)
                    {
                        textBoxMeters.Text = loadedApartment.Meters.ToString();
                        numericUpDownRoomCount.Text = loadedApartment.RoomCount.ToString();
                        monthCalendar.SetDate(new DateTime(loadedApartment.YearBuilt, 1, 1));
                        comboBoxMaterials.SelectedItem = loadedApartment.Material;

                        foreach (CheckBox check in new[] { checkBox1, checkBox2, checkBox3, checkBox4, checkBox5, checkBox6, })
                        {
                            check.Checked = loadedApartment.Options.Contains(check.Text);
                        }

                        if (loadedApartment.Address != null)
                        {
                            addressField.Country = loadedApartment.Address.Country;
                            addressField.City = loadedApartment.Address.City;
                            addressField.District = loadedApartment.Address.District;
                            addressField.Street = loadedApartment.Address.Street;
                            addressField.House = loadedApartment.Address.House;
                            addressField.Subhouse = loadedApartment.Address.Subhouse;
                            addressField.ApartmentNumber = loadedApartment.Address.ApartmentNumber;
                            addressForm.InitAddressContext(loadedApartment.Address);
                        }

                        if (loadedApartment.Developer != null)
                        {
                            developerField.Name = loadedApartment.Developer.Name;
                            developerField.Type = loadedApartment.Developer.Type;
                            developerField.Address = loadedApartment.Developer.Address;
                            developerField.INN = loadedApartment.Developer.INN;
                            developerForm.InitDeveloperContext(loadedApartment.Developer);
                        }

                        listBoxResult.Items.Clear();
                        listBoxResult.Items.Add("Загруженные данные:");
                        DisplayApartment(loadedApartment);
                    }


                }
                catch (Exception ex)
                {
                    _logic.ErrorMessage("Ошибка при загрузке файла: " + ex.Message);
                }
            }
        }

        private void TrackBarMeters_Scroll(object sender, EventArgs e)
        {
            textBoxMeters.Text = trackBarMeters.Value.ToString();
        }

        private void ButtonLoadApartments_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog
            {
                Filter = "JSON файлы (*.json)|*.json",
                Title = "Выберите файл с данными о квартире",
            };

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    string json = File.ReadAllText(openFileDialog.FileName);
                    string fileName = Path.GetFileName(openFileDialog.FileName);
                    apartments = JsonSerializer.Deserialize<List<Apartment>>(json)!;
                    labelFileLoaded.Text = "Файл " + fileName + " загружен.";
                    UpdateStatus("Файл " + fileName + " загружен.");
                }
                catch (Exception ex)
                {
                    _logic.ErrorMessage("Ошибка при загрузке файла: " + ex.Message);
                    labelFileLoaded.Text = "Ошибка.";
                }
            }
        }
    }
}
