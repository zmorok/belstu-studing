namespace ApartmentForm
{
    partial class QueryBuilderForm
    {
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.CheckBox checkBoxNumbers;
        private System.Windows.Forms.CheckBox checkBoxLetters;
        private System.Windows.Forms.CheckBox checkBoxSpecialChars;
        private System.Windows.Forms.TextBox textBoxCustomPattern;
        private System.Windows.Forms.Button btnGenerateQuery;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.checkBoxNumbers = new System.Windows.Forms.CheckBox();
            this.checkBoxLetters = new System.Windows.Forms.CheckBox();
            this.checkBoxSpecialChars = new System.Windows.Forms.CheckBox();
            this.textBoxCustomPattern = new System.Windows.Forms.TextBox();
            this.btnGenerateQuery = new System.Windows.Forms.Button();
            this.SuspendLayout();

            // checkBoxNumbers
            this.checkBoxNumbers.Location = new System.Drawing.Point(12, 12);
            this.checkBoxNumbers.Text = "Числа";

            // checkBoxLetters
            this.checkBoxLetters.Location = new System.Drawing.Point(12, 40);
            this.checkBoxLetters.Text = "Буквы";

            // checkBoxSpecialChars
            this.checkBoxSpecialChars.Location = new System.Drawing.Point(12, 68);
            this.checkBoxSpecialChars.Text = "Спецсимволы";

            // textBoxCustomPattern
            this.textBoxCustomPattern.Location = new System.Drawing.Point(12, 96);
            this.textBoxCustomPattern.Size = new System.Drawing.Size(240, 23);

            // btnGenerateQuery
            this.btnGenerateQuery.Location = new System.Drawing.Point(12, 130);
            this.btnGenerateQuery.Text = "Создать запрос";
            this.btnGenerateQuery.Click += new System.EventHandler(this.btnGenerateQuery_Click);

            // QueryBuilderForm
            this.ClientSize = new System.Drawing.Size(280, 170);
            this.Controls.Add(this.checkBoxNumbers);
            this.Controls.Add(this.checkBoxLetters);
            this.Controls.Add(this.checkBoxSpecialChars);
            this.Controls.Add(this.textBoxCustomPattern);
            this.Controls.Add(this.btnGenerateQuery);
            this.Text = "Конструктор запросов";
            this.ResumeLayout(false);
            this.PerformLayout();
        }
    }
}
