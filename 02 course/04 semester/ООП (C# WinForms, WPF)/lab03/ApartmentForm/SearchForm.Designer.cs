namespace ApartmentForm
{
    partial class SearchForm
    {
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.TextBox textBoxSearch;
        private System.Windows.Forms.Button btnSearch;
        private System.Windows.Forms.ListBox listBoxResults;
        private System.Windows.Forms.RadioButton radioExactMatch;
        private System.Windows.Forms.RadioButton radioRegex;
        private System.Windows.Forms.Button btnOpenQueryBuilder;

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
            textBoxSearch = new TextBox();
            btnSearch = new Button();
            listBoxResults = new ListBox();
            radioExactMatch = new RadioButton();
            radioRegex = new RadioButton();
            btnOpenQueryBuilder = new Button();
            buttonSave = new Button();
            SuspendLayout();
            // 
            // textBoxSearch
            // 
            textBoxSearch.Location = new Point(12, 12);
            textBoxSearch.Name = "textBoxSearch";
            textBoxSearch.Size = new Size(360, 23);
            textBoxSearch.TabIndex = 0;
            // 
            // btnSearch
            // 
            btnSearch.Location = new Point(12, 69);
            btnSearch.Name = "btnSearch";
            btnSearch.Size = new Size(120, 30);
            btnSearch.TabIndex = 1;
            btnSearch.Text = "Искать";
            btnSearch.Click += BtnSearch_Click;
            // 
            // listBoxResults
            // 
            listBoxResults.ItemHeight = 15;
            listBoxResults.Location = new Point(12, 109);
            listBoxResults.Name = "listBoxResults";
            listBoxResults.Size = new Size(360, 139);
            listBoxResults.TabIndex = 2;
            // 
            // radioExactMatch
            // 
            radioExactMatch.Checked = true;
            radioExactMatch.Location = new Point(12, 40);
            radioExactMatch.Name = "radioExactMatch";
            radioExactMatch.Size = new Size(104, 24);
            radioExactMatch.TabIndex = 3;
            radioExactMatch.TabStop = true;
            radioExactMatch.Text = "Полное соответствие";
            // 
            // radioRegex
            // 
            radioRegex.Location = new Point(160, 40);
            radioRegex.Name = "radioRegex";
            radioRegex.Size = new Size(104, 24);
            radioRegex.TabIndex = 4;
            radioRegex.Text = "Регулярное выражение";
            // 
            // btnOpenQueryBuilder
            // 
            btnOpenQueryBuilder.Location = new Point(160, 69);
            btnOpenQueryBuilder.Name = "btnOpenQueryBuilder";
            btnOpenQueryBuilder.Size = new Size(210, 30);
            btnOpenQueryBuilder.TabIndex = 5;
            btnOpenQueryBuilder.Text = "Конструктор запросов";
            btnOpenQueryBuilder.Click += ButtonOpenQueryBuilder_Click;
            // 
            // buttonSave
            // 
            buttonSave.Location = new Point(12, 256);
            buttonSave.Name = "buttonSave";
            buttonSave.Size = new Size(360, 23);
            buttonSave.TabIndex = 6;
            buttonSave.Text = "Сохранить результат?";
            buttonSave.UseVisualStyleBackColor = true;
            buttonSave.Click += ButtonSave_Click;
            // 
            // SearchForm
            // 
            ClientSize = new Size(384, 291);
            Controls.Add(buttonSave);
            Controls.Add(textBoxSearch);
            Controls.Add(btnSearch);
            Controls.Add(listBoxResults);
            Controls.Add(radioExactMatch);
            Controls.Add(radioRegex);
            Controls.Add(btnOpenQueryBuilder);
            Name = "SearchForm";
            Text = "Поиск";
            ResumeLayout(false);
            PerformLayout();
        }
        private Button buttonSave;
    }
}
