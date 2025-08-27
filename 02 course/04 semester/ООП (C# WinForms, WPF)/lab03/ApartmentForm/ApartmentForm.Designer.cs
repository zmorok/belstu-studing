using System.Windows.Forms;
using System.Xml.Linq;

namespace ApartmentForm
{
    partial class ApartmentForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            labelMeters = new Label();
            labelRoomCount = new Label();
            labelYear = new Label();
            labelMaterial = new Label();
            textBoxMeters = new TextBox();
            monthCalendar = new MonthCalendar();
            comboBoxMaterials = new ComboBox();
            groupBoxOptions = new GroupBox();
            checkBox6 = new CheckBox();
            checkBox5 = new CheckBox();
            checkBox4 = new CheckBox();
            checkBox3 = new CheckBox();
            checkBox2 = new CheckBox();
            checkBox1 = new CheckBox();
            buttonSave = new Button();
            buttonLoad = new Button();
            listBoxResult = new ListBox();
            textBoxPrice = new TextBox();
            buttonPrint = new Button();
            trackBarMeters = new TrackBar();
            numericUpDownRoomCount = new NumericUpDown();
            statusStrip = new StatusStrip();
            statusLabelCount = new ToolStripStatusLabel();
            statusLabelAction = new ToolStripStatusLabel();
            statusLabelDateTime = new ToolStripStatusLabel();
            menuStripMain = new MenuStrip();
            tsbMenu = new ToolStripMenuItem();
            tsbSearch = new ToolStripMenuItem();
            tsbSort = new ToolStripMenuItem();
            tsbClear = new ToolStripMenuItem();
            удалитьToolStripMenuItem = new ToolStripMenuItem();
            tsbAbout = new ToolStripMenuItem();
            tsbForward = new ToolStripMenuItem();
            buttonShowMenu = new Button();
            buttonLoadApartments = new Button();
            labelFileLoaded = new Label();
            buttonLockMenu = new Button();
            groupBoxOptions.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)trackBarMeters).BeginInit();
            ((System.ComponentModel.ISupportInitialize)numericUpDownRoomCount).BeginInit();
            statusStrip.SuspendLayout();
            menuStripMain.SuspendLayout();
            SuspendLayout();
            // 
            // labelMeters
            // 
            labelMeters.AutoSize = true;
            labelMeters.Location = new Point(12, 32);
            labelMeters.Name = "labelMeters";
            labelMeters.Size = new Size(54, 15);
            labelMeters.TabIndex = 1;
            labelMeters.Text = "Метраж:";
            // 
            // labelRoomCount
            // 
            labelRoomCount.AutoSize = true;
            labelRoomCount.Location = new Point(15, 67);
            labelRoomCount.Name = "labelRoomCount";
            labelRoomCount.Size = new Size(118, 15);
            labelRoomCount.TabIndex = 3;
            labelRoomCount.Text = "Количество комнат:";
            // 
            // labelYear
            // 
            labelYear.AutoSize = true;
            labelYear.Location = new Point(540, 28);
            labelYear.Name = "labelYear";
            labelYear.Size = new Size(91, 15);
            labelYear.TabIndex = 12;
            labelYear.Text = "Год постройки:";
            // 
            // labelMaterial
            // 
            labelMaterial.AutoSize = true;
            labelMaterial.Location = new Point(12, 208);
            labelMaterial.Name = "labelMaterial";
            labelMaterial.Size = new Size(93, 15);
            labelMaterial.TabIndex = 15;
            labelMaterial.Text = "Тип материала:";
            // 
            // textBoxMeters
            // 
            textBoxMeters.Location = new Point(209, 29);
            textBoxMeters.Name = "textBoxMeters";
            textBoxMeters.ReadOnly = true;
            textBoxMeters.Size = new Size(47, 23);
            textBoxMeters.TabIndex = 0;
            textBoxMeters.TextChanged += CalculateAndDisplayPrice;
            // 
            // monthCalendar
            // 
            monthCalendar.Location = new Point(504, 52);
            monthCalendar.Name = "monthCalendar";
            monthCalendar.TabIndex = 11;
            // 
            // comboBoxMaterials
            // 
            comboBoxMaterials.DropDownStyle = ComboBoxStyle.DropDownList;
            comboBoxMaterials.FormattingEnabled = true;
            comboBoxMaterials.Items.AddRange(new object[] { "Кирпич", "Панель", "Монолит", "Дерево" });
            comboBoxMaterials.Location = new Point(118, 205);
            comboBoxMaterials.Name = "comboBoxMaterials";
            comboBoxMaterials.Size = new Size(121, 23);
            comboBoxMaterials.TabIndex = 14;
            comboBoxMaterials.SelectedIndexChanged += CalculateAndDisplayPrice;
            // 
            // groupBoxOptions
            // 
            groupBoxOptions.Controls.Add(checkBox6);
            groupBoxOptions.Controls.Add(checkBox5);
            groupBoxOptions.Controls.Add(checkBox4);
            groupBoxOptions.Controls.Add(checkBox3);
            groupBoxOptions.Controls.Add(checkBox2);
            groupBoxOptions.Controls.Add(checkBox1);
            groupBoxOptions.Location = new Point(12, 99);
            groupBoxOptions.Name = "groupBoxOptions";
            groupBoxOptions.Size = new Size(174, 100);
            groupBoxOptions.TabIndex = 41;
            groupBoxOptions.TabStop = false;
            groupBoxOptions.Text = "Опции";
            // 
            // checkBox6
            // 
            checkBox6.AutoSize = true;
            checkBox6.Location = new Point(100, 72);
            checkBox6.Name = "checkBox6";
            checkBox6.Size = new Size(66, 19);
            checkBox6.TabIndex = 55;
            checkBox6.Text = "Ширма";
            checkBox6.UseVisualStyleBackColor = true;
            checkBox6.CheckedChanged += CalculateAndDisplayPrice;
            // 
            // checkBox5
            // 
            checkBox5.AutoSize = true;
            checkBox5.Location = new Point(100, 47);
            checkBox5.Name = "checkBox5";
            checkBox5.Size = new Size(66, 19);
            checkBox5.TabIndex = 54;
            checkBox5.Text = "Балкон";
            checkBox5.UseVisualStyleBackColor = true;
            checkBox5.CheckedChanged += CalculateAndDisplayPrice;
            // 
            // checkBox4
            // 
            checkBox4.AutoSize = true;
            checkBox4.Location = new Point(100, 23);
            checkBox4.Name = "checkBox4";
            checkBox4.Size = new Size(67, 19);
            checkBox4.TabIndex = 53;
            checkBox4.Text = "Подвал";
            checkBox4.UseVisualStyleBackColor = true;
            checkBox4.CheckedChanged += CalculateAndDisplayPrice;
            // 
            // checkBox3
            // 
            checkBox3.AutoSize = true;
            checkBox3.Location = new Point(15, 72);
            checkBox3.Name = "checkBox3";
            checkBox3.Size = new Size(63, 19);
            checkBox3.TabIndex = 52;
            checkBox3.Text = "Туалет";
            checkBox3.UseVisualStyleBackColor = true;
            checkBox3.CheckedChanged += CalculateAndDisplayPrice;
            // 
            // checkBox2
            // 
            checkBox2.AutoSize = true;
            checkBox2.Location = new Point(15, 48);
            checkBox2.Name = "checkBox2";
            checkBox2.Size = new Size(59, 19);
            checkBox2.TabIndex = 51;
            checkBox2.Text = "Ванна";
            checkBox2.UseVisualStyleBackColor = true;
            checkBox2.CheckedChanged += CalculateAndDisplayPrice;
            // 
            // checkBox1
            // 
            checkBox1.AutoSize = true;
            checkBox1.Location = new Point(15, 23);
            checkBox1.Name = "checkBox1";
            checkBox1.Size = new Size(57, 19);
            checkBox1.TabIndex = 50;
            checkBox1.Text = "Кухня";
            checkBox1.UseVisualStyleBackColor = true;
            checkBox1.CheckedChanged += CalculateAndDisplayPrice;
            // 
            // buttonSave
            // 
            buttonSave.Location = new Point(232, 326);
            buttonSave.Name = "buttonSave";
            buttonSave.Size = new Size(216, 39);
            buttonSave.TabIndex = 42;
            buttonSave.Text = "Сохранить";
            buttonSave.UseVisualStyleBackColor = true;
            buttonSave.Click += ButtonSave_Click;
            // 
            // buttonLoad
            // 
            buttonLoad.Location = new Point(452, 326);
            buttonLoad.Name = "buttonLoad";
            buttonLoad.Size = new Size(216, 39);
            buttonLoad.TabIndex = 43;
            buttonLoad.Text = "Загрузить";
            buttonLoad.UseVisualStyleBackColor = true;
            buttonLoad.Click += ButtonLoad_Click;
            // 
            // listBoxResult
            // 
            listBoxResult.FormattingEnabled = true;
            listBoxResult.ItemHeight = 15;
            listBoxResult.Location = new Point(12, 371);
            listBoxResult.Name = "listBoxResult";
            listBoxResult.Size = new Size(656, 169);
            listBoxResult.TabIndex = 49;
            // 
            // textBoxPrice
            // 
            textBoxPrice.Font = new Font("Segoe UI", 40F, FontStyle.Bold);
            textBoxPrice.Location = new Point(12, 235);
            textBoxPrice.Name = "textBoxPrice";
            textBoxPrice.ReadOnly = true;
            textBoxPrice.Size = new Size(656, 78);
            textBoxPrice.TabIndex = 50;
            textBoxPrice.TextAlign = HorizontalAlignment.Center;
            // 
            // buttonPrint
            // 
            buttonPrint.Location = new Point(12, 326);
            buttonPrint.Name = "buttonPrint";
            buttonPrint.Size = new Size(216, 39);
            buttonPrint.TabIndex = 54;
            buttonPrint.Text = "Вывести";
            buttonPrint.UseVisualStyleBackColor = true;
            buttonPrint.Click += ButtonPrint_Click;
            // 
            // trackBarMeters
            // 
            trackBarMeters.LargeChange = 20;
            trackBarMeters.Location = new Point(72, 23);
            trackBarMeters.Maximum = 200;
            trackBarMeters.Name = "trackBarMeters";
            trackBarMeters.Size = new Size(131, 45);
            trackBarMeters.TabIndex = 55;
            trackBarMeters.Value = 10;
            trackBarMeters.Scroll += TrackBarMeters_Scroll;
            // 
            // numericUpDownRoomCount
            // 
            numericUpDownRoomCount.Location = new Point(135, 64);
            numericUpDownRoomCount.Name = "numericUpDownRoomCount";
            numericUpDownRoomCount.Size = new Size(121, 23);
            numericUpDownRoomCount.TabIndex = 56;
            // 
            // statusStrip
            // 
            statusStrip.Items.AddRange(new ToolStripItem[] { statusLabelCount, statusLabelAction, statusLabelDateTime });
            statusStrip.Location = new Point(0, 546);
            statusStrip.Name = "statusStrip";
            statusStrip.Size = new Size(680, 22);
            statusStrip.TabIndex = 0;
            statusStrip.Text = "statusStrip";
            // 
            // statusLabelCount
            // 
            statusLabelCount.Name = "statusLabelCount";
            statusLabelCount.Size = new Size(72, 17);
            statusLabelCount.Text = "Объектов: 0";
            // 
            // statusLabelAction
            // 
            statusLabelAction.Name = "statusLabelAction";
            statusLabelAction.Size = new Size(145, 17);
            statusLabelAction.Text = "Последнее действие: Нет";
            // 
            // statusLabelDateTime
            // 
            statusLabelDateTime.Name = "statusLabelDateTime";
            statusLabelDateTime.Size = new Size(106, 17);
            statusLabelDateTime.Text = "01.03.2025 19:29:05";
            // 
            // menuStripMain
            // 
            menuStripMain.Items.AddRange(new ToolStripItem[] { tsbMenu, tsbForward });
            menuStripMain.Location = new Point(0, 0);
            menuStripMain.Name = "menuStripMain";
            menuStripMain.Size = new Size(680, 24);
            menuStripMain.TabIndex = 0;
            menuStripMain.Text = "menuStripMain";
            // 
            // tsbMenu
            // 
            tsbMenu.DropDownItems.AddRange(new ToolStripItem[] { tsbSearch, tsbSort, tsbClear, удалитьToolStripMenuItem, tsbAbout });
            tsbMenu.Name = "tsbMenu";
            tsbMenu.Size = new Size(53, 20);
            tsbMenu.Text = "Меню";
            // 
            // tsbSearch
            // 
            tsbSearch.Name = "tsbSearch";
            tsbSearch.Size = new Size(149, 22);
            tsbSearch.Text = "Поиск";
            tsbSearch.Click += TsbSearch_Click;
            // 
            // tsbSort
            // 
            tsbSort.Name = "tsbSort";
            tsbSort.Size = new Size(149, 22);
            tsbSort.Text = "Сортировки";
            tsbSort.Click += TsbSort_Click;
            // 
            // tsbClear
            // 
            tsbClear.Name = "tsbClear";
            tsbClear.Size = new Size(149, 22);
            tsbClear.Text = "Очистка";
            tsbClear.Click += ButtonClear_Click;
            // 
            // удалитьToolStripMenuItem
            // 
            удалитьToolStripMenuItem.Name = "удалитьToolStripMenuItem";
            удалитьToolStripMenuItem.Size = new Size(149, 22);
            удалитьToolStripMenuItem.Text = "Удалить";
            удалитьToolStripMenuItem.Click += TsbDelete_Click;
            // 
            // tsbAbout
            // 
            tsbAbout.Name = "tsbAbout";
            tsbAbout.Size = new Size(149, 22);
            tsbAbout.Text = "О программе";
            tsbAbout.Click += TsbAbout_Click;
            // 
            // tsbForward
            // 
            tsbForward.Name = "tsbForward";
            tsbForward.Size = new Size(58, 20);
            tsbForward.Text = "Вперёд";
            tsbForward.Click += TsbForward_Click;
            // 
            // buttonShowMenu
            // 
            buttonShowMenu.Location = new Point(262, 29);
            buttonShowMenu.Name = "buttonShowMenu";
            buttonShowMenu.Size = new Size(106, 23);
            buttonShowMenu.TabIndex = 57;
            buttonShowMenu.Text = "Скрыть панель";
            buttonShowMenu.UseVisualStyleBackColor = true;
            buttonShowMenu.Click += ButtonShowMenu_Click;
            // 
            // buttonLoadApartments
            // 
            buttonLoadApartments.Location = new Point(262, 64);
            buttonLoadApartments.Name = "buttonLoadApartments";
            buttonLoadApartments.Size = new Size(230, 23);
            buttonLoadApartments.TabIndex = 58;
            buttonLoadApartments.Text = "Загрузить квартиры";
            buttonLoadApartments.UseVisualStyleBackColor = true;
            buttonLoadApartments.Click += ButtonLoadApartments_Click;
            // 
            // labelFileLoaded
            // 
            labelFileLoaded.AutoSize = true;
            labelFileLoaded.Location = new Point(305, 99);
            labelFileLoaded.Name = "labelFileLoaded";
            labelFileLoaded.Size = new Size(143, 15);
            labelFileLoaded.TabIndex = 59;
            labelFileLoaded.Text = "Загрузите файл квартир.";
            // 
            // buttonLockMenu
            // 
            buttonLockMenu.Location = new Point(374, 29);
            buttonLockMenu.Name = "buttonLockMenu";
            buttonLockMenu.Size = new Size(118, 23);
            buttonLockMenu.TabIndex = 60;
            buttonLockMenu.Text = "Закрепить панель";
            buttonLockMenu.UseVisualStyleBackColor = true;
            buttonLockMenu.Click += ButtonLockMenu_Click;
            // 
            // ApartmentForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(680, 568);
            Controls.Add(buttonLockMenu);
            Controls.Add(labelFileLoaded);
            Controls.Add(buttonLoadApartments);
            Controls.Add(buttonShowMenu);
            Controls.Add(menuStripMain);
            Controls.Add(statusStrip);
            Controls.Add(numericUpDownRoomCount);
            Controls.Add(trackBarMeters);
            Controls.Add(buttonPrint);
            Controls.Add(textBoxPrice);
            Controls.Add(listBoxResult);
            Controls.Add(buttonLoad);
            Controls.Add(buttonSave);
            Controls.Add(groupBoxOptions);
            Controls.Add(labelMaterial);
            Controls.Add(comboBoxMaterials);
            Controls.Add(labelYear);
            Controls.Add(monthCalendar);
            Controls.Add(labelRoomCount);
            Controls.Add(labelMeters);
            Controls.Add(textBoxMeters);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            Name = "ApartmentForm";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Форма для квартиры";
            groupBoxOptions.ResumeLayout(false);
            groupBoxOptions.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)trackBarMeters).EndInit();
            ((System.ComponentModel.ISupportInitialize)numericUpDownRoomCount).EndInit();
            statusStrip.ResumeLayout(false);
            statusStrip.PerformLayout();
            menuStripMain.ResumeLayout(false);
            menuStripMain.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox textBoxMeters;
        private Label labelMeters;
        private Label labelRoomCount;
        private MonthCalendar monthCalendar;
        private Label labelYear;
        private ComboBox comboBoxMaterials;
        private Label labelMaterial;
        private GroupBox groupBoxOptions;
        private Button buttonSave;
        private Button buttonLoad;
        private ListBox listBoxResult;
        private CheckBox checkBox6;
        private CheckBox checkBox5;
        private CheckBox checkBox4;
        private CheckBox checkBox3;
        private CheckBox checkBox2;
        private CheckBox checkBox1;
        private TextBox textBoxPrice;
        private Button buttonPrint;
        private TrackBar trackBarMeters;
        private NumericUpDown numericUpDownRoomCount; 
        private StatusStrip statusStrip;
        private ToolStripStatusLabel statusLabelCount;
        private ToolStripStatusLabel statusLabelAction;
        private ToolStripStatusLabel statusLabelDateTime;
        private MenuStrip menuStripMain;
        private ToolStripMenuItem tsbSort;
        private ToolStripMenuItem tsbClear;
        private ToolStripMenuItem tsbMenu;
        private ToolStripMenuItem tsbForward;
        private ToolStripMenuItem tsbSearch;
        private ToolStripMenuItem удалитьToolStripMenuItem;
        private ToolStripMenuItem tsbAbout;
        private Button buttonShowMenu;
        private Button buttonLoadApartments;
        private Label labelFileLoaded;
        private Button buttonLockMenu;
    }
}