namespace ApartmentForm
{
    partial class DeveloperForm
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
            groupBoxDeveloper = new GroupBox();
            textBoxDevINN = new TextBox();
            labelDevINN = new Label();
            textBoxDevAddress = new TextBox();
            comboBoxDevType = new ComboBox();
            textBoxDevName = new TextBox();
            labelDevAddress = new Label();
            labelDevType = new Label();
            labelDevName = new Label();
            buttonConfirmDeveloper = new Button();
            menuStripMain = new MenuStrip();
            tsbBack = new ToolStripMenuItem();
            groupBoxDeveloper.SuspendLayout();
            menuStripMain.SuspendLayout();
            SuspendLayout();
            // 
            // groupBoxDeveloper
            // 
            groupBoxDeveloper.Controls.Add(textBoxDevINN);
            groupBoxDeveloper.Controls.Add(labelDevINN);
            groupBoxDeveloper.Controls.Add(textBoxDevAddress);
            groupBoxDeveloper.Controls.Add(comboBoxDevType);
            groupBoxDeveloper.Controls.Add(textBoxDevName);
            groupBoxDeveloper.Controls.Add(labelDevAddress);
            groupBoxDeveloper.Controls.Add(labelDevType);
            groupBoxDeveloper.Controls.Add(labelDevName);
            groupBoxDeveloper.Location = new Point(12, 27);
            groupBoxDeveloper.Name = "groupBoxDeveloper";
            groupBoxDeveloper.Size = new Size(311, 138);
            groupBoxDeveloper.TabIndex = 40;
            groupBoxDeveloper.TabStop = false;
            groupBoxDeveloper.Text = "Застройщик:";
            // 
            // textBoxDevINN
            // 
            textBoxDevINN.Location = new Point(136, 103);
            textBoxDevINN.Name = "textBoxDevINN";
            textBoxDevINN.Size = new Size(156, 23);
            textBoxDevINN.TabIndex = 52;
            // 
            // labelDevINN
            // 
            labelDevINN.AutoSize = true;
            labelDevINN.Location = new Point(18, 106);
            labelDevINN.Name = "labelDevINN";
            labelDevINN.Size = new Size(37, 15);
            labelDevINN.TabIndex = 51;
            labelDevINN.Text = "ИНН:";
            // 
            // textBoxDevAddress
            // 
            textBoxDevAddress.Location = new Point(136, 73);
            textBoxDevAddress.Name = "textBoxDevAddress";
            textBoxDevAddress.Size = new Size(156, 23);
            textBoxDevAddress.TabIndex = 50;
            // 
            // comboBoxDevType
            // 
            comboBoxDevType.DropDownStyle = ComboBoxStyle.DropDownList;
            comboBoxDevType.FormattingEnabled = true;
            comboBoxDevType.Items.AddRange(new object[] { "ООО", "ОАО", "ИП" });
            comboBoxDevType.Location = new Point(136, 45);
            comboBoxDevType.Name = "comboBoxDevType";
            comboBoxDevType.Size = new Size(156, 23);
            comboBoxDevType.TabIndex = 49;
            // 
            // textBoxDevName
            // 
            textBoxDevName.Location = new Point(136, 16);
            textBoxDevName.Name = "textBoxDevName";
            textBoxDevName.Size = new Size(156, 23);
            textBoxDevName.TabIndex = 47;
            // 
            // labelDevAddress
            // 
            labelDevAddress.AutoSize = true;
            labelDevAddress.Location = new Point(18, 76);
            labelDevAddress.Name = "labelDevAddress";
            labelDevAddress.Size = new Size(66, 15);
            labelDevAddress.TabIndex = 42;
            labelDevAddress.Text = "Юр. адрес:";
            // 
            // labelDevType
            // 
            labelDevType.AutoSize = true;
            labelDevType.Location = new Point(18, 48);
            labelDevType.Name = "labelDevType";
            labelDevType.Size = new Size(90, 15);
            labelDevType.TabIndex = 40;
            labelDevType.Text = "Тип компании:";
            // 
            // labelDevName
            // 
            labelDevName.AutoSize = true;
            labelDevName.Location = new Point(17, 20);
            labelDevName.Name = "labelDevName";
            labelDevName.Size = new Size(62, 15);
            labelDevName.TabIndex = 39;
            labelDevName.Text = "Название:";
            // 
            // buttonConfirmDeveloper
            // 
            buttonConfirmDeveloper.Location = new Point(12, 171);
            buttonConfirmDeveloper.Name = "buttonConfirmDeveloper";
            buttonConfirmDeveloper.Size = new Size(311, 26);
            buttonConfirmDeveloper.TabIndex = 41;
            buttonConfirmDeveloper.Text = "Подтвердить";
            buttonConfirmDeveloper.UseVisualStyleBackColor = true;
            buttonConfirmDeveloper.Click += ButtonConfirmDeveloper_Click;
            // 
            // menuStripMain
            // 
            menuStripMain.Items.AddRange(new ToolStripItem[] { tsbBack });
            menuStripMain.Location = new Point(0, 0);
            menuStripMain.Name = "menuStripMain";
            menuStripMain.Size = new Size(334, 24);
            menuStripMain.TabIndex = 44;
            menuStripMain.Text = "menuStripMain";
            // 
            // tsbBack
            // 
            tsbBack.Name = "tsbBack";
            tsbBack.Size = new Size(51, 20);
            tsbBack.Text = "Назад";
            tsbBack.Click += tsbBack_Click;
            // 
            // DeveloperForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(334, 209);
            Controls.Add(menuStripMain);
            Controls.Add(buttonConfirmDeveloper);
            Controls.Add(groupBoxDeveloper);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            Name = "DeveloperForm";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Застройщик";
            groupBoxDeveloper.ResumeLayout(false);
            groupBoxDeveloper.PerformLayout();
            menuStripMain.ResumeLayout(false);
            menuStripMain.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private GroupBox groupBoxDeveloper;
        private TextBox textBoxDevINN;
        private Label labelDevINN;
        private TextBox textBoxDevAddress;
        private ComboBox comboBoxDevType;
        private TextBox textBoxDevName;
        private Label labelDevAddress;
        private Label labelDevType;
        private Label labelDevName;
        private Button buttonConfirmDeveloper;
        private MenuStrip menuStripMain;
        private ToolStripMenuItem tsbBack;
    }
}