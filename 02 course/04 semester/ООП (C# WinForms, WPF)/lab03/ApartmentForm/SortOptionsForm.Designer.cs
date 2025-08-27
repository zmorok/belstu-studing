namespace ApartmentForm
{
    partial class SortOptionsForm
    {
        private System.ComponentModel.IContainer components = null;
        private ComboBox comboBoxPrimary;
        private ComboBox comboBoxSecondary;
        private Button btnApplySort;
        private Label labelPrimary;
        private Label labelSecondary;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        private void InitializeComponent()
        {
            comboBoxPrimary = new ComboBox();
            comboBoxSecondary = new ComboBox();
            btnApplySort = new Button();
            labelPrimary = new Label();
            labelSecondary = new Label();
            SuspendLayout();
            // 
            // comboBoxPrimary
            // 
            comboBoxPrimary.DropDownStyle = ComboBoxStyle.DropDownList;
            comboBoxPrimary.Items.AddRange(new object[] { "Возрастание", "Убывание" });
            comboBoxPrimary.Location = new Point(160, 17);
            comboBoxPrimary.Name = "comboBoxPrimary";
            comboBoxPrimary.Size = new Size(160, 23);
            comboBoxPrimary.TabIndex = 1;
            // 
            // comboBoxSecondary
            // 
            comboBoxSecondary.DropDownStyle = ComboBoxStyle.DropDownList;
            comboBoxSecondary.Items.AddRange(new object[] { "Цена", "Метраж", "Количество комнат" });
            comboBoxSecondary.Location = new Point(160, 57);
            comboBoxSecondary.Name = "comboBoxSecondary";
            comboBoxSecondary.Size = new Size(160, 23);
            comboBoxSecondary.TabIndex = 3;
            // 
            // btnApplySort
            // 
            btnApplySort.Location = new Point(12, 100);
            btnApplySort.Name = "btnApplySort";
            btnApplySort.Size = new Size(308, 30);
            btnApplySort.TabIndex = 4;
            btnApplySort.Text = "Применить сортировку";
            btnApplySort.UseVisualStyleBackColor = true;
            btnApplySort.Click += BtnApplySort_Click;
            // 
            // labelPrimary
            // 
            labelPrimary.AutoSize = true;
            labelPrimary.Location = new Point(12, 20);
            labelPrimary.Name = "labelPrimary";
            labelPrimary.Size = new Size(122, 15);
            labelPrimary.TabIndex = 0;
            labelPrimary.Text = "Основной параметр:";
            // 
            // labelSecondary
            // 
            labelSecondary.AutoSize = true;
            labelSecondary.Location = new Point(12, 60);
            labelSecondary.Name = "labelSecondary";
            labelSecondary.Size = new Size(129, 15);
            labelSecondary.TabIndex = 2;
            labelSecondary.Text = "Вторичный параметр:";
            // 
            // SortOptionsForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(334, 150);
            Controls.Add(labelPrimary);
            Controls.Add(comboBoxPrimary);
            Controls.Add(labelSecondary);
            Controls.Add(comboBoxSecondary);
            Controls.Add(btnApplySort);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            Name = "SortOptionsForm";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Выбор сортировки";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion
    }
}
