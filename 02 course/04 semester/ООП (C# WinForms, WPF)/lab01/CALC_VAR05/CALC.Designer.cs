namespace CALC_VAR05
{
    partial class CALC
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            boxPrice = new TextBox();
            label1 = new Label();
            label2 = new Label();
            boxVolume = new TextBox();
            label3 = new Label();
            boxPlusPrice = new TextBox();
            label4 = new Label();
            boxDailyUsage = new TextBox();
            buttonCalculate = new Button();
            boxPricePerVolume = new TextBox();
            label5 = new Label();
            label6 = new Label();
            boxSelfPrice = new TextBox();
            label7 = new Label();
            boxPricePerMonth = new TextBox();
            SuspendLayout();
            // 
            // boxPrice
            // 
            boxPrice.Location = new Point(170, 21);
            boxPrice.Name = "boxPrice";
            boxPrice.Size = new Size(100, 23);
            boxPrice.TabIndex = 0;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(23, 24);
            label1.Name = "label1";
            label1.Size = new Size(70, 15);
            label1.TabIndex = 1;
            label1.Text = "Стоимость:";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(23, 57);
            label2.Name = "label2";
            label2.Size = new Size(88, 15);
            label2.TabIndex = 2;
            label2.Text = "Объём товара:";
            // 
            // boxVolume
            // 
            boxVolume.Location = new Point(170, 54);
            boxVolume.Name = "boxVolume";
            boxVolume.Size = new Size(100, 23);
            boxVolume.TabIndex = 3;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(23, 89);
            label3.Name = "label3";
            label3.Size = new Size(89, 15);
            label3.TabIndex = 4;
            label3.Text = "Фикс. наценка:";
            // 
            // boxPlusPrice
            // 
            boxPlusPrice.Location = new Point(170, 86);
            boxPlusPrice.Name = "boxPlusPrice";
            boxPlusPrice.Size = new Size(100, 23);
            boxPlusPrice.TabIndex = 5;
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(23, 126);
            label4.Name = "label4";
            label4.Size = new Size(134, 15);
            label4.TabIndex = 6;
            label4.Text = "Ежеднев. потребление:";
            // 
            // boxDailyUsage
            // 
            boxDailyUsage.Location = new Point(170, 123);
            boxDailyUsage.Name = "boxDailyUsage";
            boxDailyUsage.Size = new Size(100, 23);
            boxDailyUsage.TabIndex = 7;
            // 
            // buttonCalculate
            // 
            buttonCalculate.Location = new Point(23, 172);
            buttonCalculate.Name = "buttonCalculate";
            buttonCalculate.Size = new Size(512, 40);
            buttonCalculate.TabIndex = 8;
            buttonCalculate.Text = "Рассчитать данные";
            buttonCalculate.UseVisualStyleBackColor = true;
            buttonCalculate.Click += buttonCalculate_Click;
            // 
            // boxPricePerVolume
            // 
            boxPricePerVolume.Location = new Point(339, 42);
            boxPricePerVolume.Name = "boxPricePerVolume";
            boxPricePerVolume.ReadOnly = true;
            boxPricePerVolume.Size = new Size(196, 23);
            boxPricePerVolume.TabIndex = 9;
            boxPricePerVolume.TextAlign = HorizontalAlignment.Center;
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Location = new Point(339, 24);
            label5.Name = "label5";
            label5.Size = new Size(178, 15);
            label5.TabIndex = 10;
            label5.Text = "Стоимость за единицу объёма:";
            // 
            // label6
            // 
            label6.AutoSize = true;
            label6.Location = new Point(339, 71);
            label6.Name = "label6";
            label6.Size = new Size(95, 15);
            label6.TabIndex = 12;
            label6.Text = "Себестоимость:";
            // 
            // boxSelfPrice
            // 
            boxSelfPrice.Location = new Point(339, 89);
            boxSelfPrice.Name = "boxSelfPrice";
            boxSelfPrice.ReadOnly = true;
            boxSelfPrice.Size = new Size(196, 23);
            boxSelfPrice.TabIndex = 11;
            boxSelfPrice.TextAlign = HorizontalAlignment.Center;
            // 
            // label7
            // 
            label7.AutoSize = true;
            label7.Location = new Point(339, 121);
            label7.Name = "label7";
            label7.Size = new Size(196, 15);
            label7.TabIndex = 14;
            label7.Text = "Стоимость за месяц потребления:";
            // 
            // boxPricePerMonth
            // 
            boxPricePerMonth.Location = new Point(339, 139);
            boxPricePerMonth.Name = "boxPricePerMonth";
            boxPricePerMonth.ReadOnly = true;
            boxPricePerMonth.Size = new Size(196, 23);
            boxPricePerMonth.TabIndex = 13;
            boxPricePerMonth.TextAlign = HorizontalAlignment.Center;
            // 
            // CALC
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(563, 228);
            Controls.Add(label7);
            Controls.Add(boxPricePerMonth);
            Controls.Add(label6);
            Controls.Add(boxSelfPrice);
            Controls.Add(label5);
            Controls.Add(boxPricePerVolume);
            Controls.Add(buttonCalculate);
            Controls.Add(boxDailyUsage);
            Controls.Add(label4);
            Controls.Add(boxPlusPrice);
            Controls.Add(label3);
            Controls.Add(boxVolume);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(boxPrice);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            Name = "CALC";
            Text = "Калькулятор стоимости";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox boxPrice;
        private Label label1;
        private Label label2;
        private TextBox boxVolume;
        private Label label3;
        private TextBox boxPlusPrice;
        private Label label4;
        private TextBox boxDailyUsage;
        private Button buttonCalculate;
        private TextBox boxPricePerVolume;
        private Label label5;
        private Label label6;
        private TextBox boxSelfPrice;
        private Label label7;
        private TextBox boxPricePerMonth;
    }
}
