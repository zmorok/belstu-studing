namespace ApartmentForm
{
    partial class ResultsForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.ListBox listBoxResults;

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

        private void InitializeComponent()
        {
            listBoxResults = new ListBox();
            buttonSave = new Button();
            SuspendLayout();
            // 
            // listBoxResults
            // 
            listBoxResults.Dock = DockStyle.Fill;
            listBoxResults.FormattingEnabled = true;
            listBoxResults.ItemHeight = 15;
            listBoxResults.Location = new Point(0, 0);
            listBoxResults.Name = "listBoxResults";
            listBoxResults.Size = new Size(484, 261);
            listBoxResults.TabIndex = 0;
            // 
            // buttonSave
            // 
            buttonSave.Location = new Point(356, 226);
            buttonSave.Name = "buttonSave";
            buttonSave.Size = new Size(116, 23);
            buttonSave.TabIndex = 1;
            buttonSave.Text = "Сохранить?";
            buttonSave.UseVisualStyleBackColor = true;
            buttonSave.Click += ButtonSave_Click;
            // 
            // ResultsForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(484, 261);
            Controls.Add(buttonSave);
            Controls.Add(listBoxResults);
            FormBorderStyle = FormBorderStyle.FixedDialog;
            Name = "ResultsForm";
            StartPosition = FormStartPosition.CenterParent;
            Text = "Результаты";
            ResumeLayout(false);
        }

        #endregion

        private Button buttonSave;
    }
}
