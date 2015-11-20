namespace Engine2Switer
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.cmb_Resampler = new System.Windows.Forms.ComboBox();
            this.cmb_WaveTool = new System.Windows.Forms.ComboBox();
            this.btn_Resampler = new System.Windows.Forms.Button();
            this.btn_WavTool = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.chkCodePage = new System.Windows.Forms.CheckBox();
            this.button3 = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.langCod = new System.Windows.Forms.ComboBox();
            this.LXCode = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(95, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "Resampler(DLL):";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 44);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(89, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "WavTool2(DLL):";
            // 
            // cmb_Resampler
            // 
            this.cmb_Resampler.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmb_Resampler.FormattingEnabled = true;
            this.cmb_Resampler.Location = new System.Drawing.Point(113, 16);
            this.cmb_Resampler.Name = "cmb_Resampler";
            this.cmb_Resampler.Size = new System.Drawing.Size(351, 20);
            this.cmb_Resampler.TabIndex = 2;
            // 
            // cmb_WaveTool
            // 
            this.cmb_WaveTool.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmb_WaveTool.FormattingEnabled = true;
            this.cmb_WaveTool.Location = new System.Drawing.Point(113, 41);
            this.cmb_WaveTool.Name = "cmb_WaveTool";
            this.cmb_WaveTool.Size = new System.Drawing.Size(351, 20);
            this.cmb_WaveTool.TabIndex = 3;
            // 
            // btn_Resampler
            // 
            this.btn_Resampler.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_Resampler.Location = new System.Drawing.Point(467, 16);
            this.btn_Resampler.Name = "btn_Resampler";
            this.btn_Resampler.Size = new System.Drawing.Size(42, 20);
            this.btn_Resampler.TabIndex = 4;
            this.btn_Resampler.Text = "...";
            this.btn_Resampler.UseVisualStyleBackColor = true;
            this.btn_Resampler.Click += new System.EventHandler(this.btn_Resampler_Click);
            // 
            // btn_WavTool
            // 
            this.btn_WavTool.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_WavTool.Location = new System.Drawing.Point(467, 41);
            this.btn_WavTool.Name = "btn_WavTool";
            this.btn_WavTool.Size = new System.Drawing.Size(42, 20);
            this.btn_WavTool.TabIndex = 5;
            this.btn_WavTool.Text = "...";
            this.btn_WavTool.UseVisualStyleBackColor = true;
            this.btn_WavTool.Click += new System.EventHandler(this.btn_WavTool_Click);
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.Location = new System.Drawing.Point(333, 71);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(85, 47);
            this.button1.TabIndex = 6;
            this.button1.Text = "Save";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.button2.Location = new System.Drawing.Point(424, 71);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(85, 47);
            this.button2.TabIndex = 7;
            this.button2.Text = "Cancel";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // chkCodePage
            // 
            this.chkCodePage.AutoSize = true;
            this.chkCodePage.Location = new System.Drawing.Point(12, 71);
            this.chkCodePage.Name = "chkCodePage";
            this.chkCodePage.Size = new System.Drawing.Size(174, 16);
            this.chkCodePage.TabIndex = 8;
            this.chkCodePage.Text = "Try to translate CodePage";
            this.chkCodePage.UseVisualStyleBackColor = true;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(222, 71);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(85, 47);
            this.button3.TabIndex = 10;
            this.button3.Text = "Clean Caches";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // label3
            // 
            this.label3.Font = new System.Drawing.Font("宋体", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.Location = new System.Drawing.Point(107, 64);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 11;
            this.label3.Text = "读取配置";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // langCod
            // 
            this.langCod.FormattingEnabled = true;
            this.langCod.Items.AddRange(new object[] {
            "==Auto==",
            "Chinese(ML)",
            "Chinese(TW)",
            "Korean"});
            this.langCod.Location = new System.Drawing.Point(113, 95);
            this.langCod.Name = "langCod";
            this.langCod.Size = new System.Drawing.Size(103, 20);
            this.langCod.TabIndex = 12;
            this.langCod.Text = "==Auto==";
            // 
            // LXCode
            // 
            this.LXCode.AutoSize = true;
            this.LXCode.Location = new System.Drawing.Point(12, 98);
            this.LXCode.Name = "LXCode";
            this.LXCode.Size = new System.Drawing.Size(95, 12);
            this.LXCode.TabIndex = 13;
            this.LXCode.Text = "Locate CodePage";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(521, 126);
            this.Controls.Add(this.LXCode);
            this.Controls.Add(this.langCod);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.chkCodePage);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btn_WavTool);
            this.Controls.Add(this.btn_Resampler);
            this.Controls.Add(this.cmb_WaveTool);
            this.Controls.Add(this.cmb_Resampler);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.MinimumSize = new System.Drawing.Size(404, 165);
            this.Name = "Form1";
            this.Text = "Resampler Switcher";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cmb_Resampler;
        private System.Windows.Forms.ComboBox cmb_WaveTool;
        private System.Windows.Forms.Button btn_Resampler;
        private System.Windows.Forms.Button btn_WavTool;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.CheckBox chkCodePage;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox langCod;
        private System.Windows.Forms.Label LXCode;
    }
}

