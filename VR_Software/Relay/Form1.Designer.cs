namespace Relay
{
    partial class Form1
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
            this.channel_1 = new System.Windows.Forms.Button();
            this.channel_2 = new System.Windows.Forms.Button();
            this.channel_3 = new System.Windows.Forms.Button();
            this.channel_4 = new System.Windows.Forms.Button();
            this.channel_5 = new System.Windows.Forms.Button();
            this.channel_6 = new System.Windows.Forms.Button();
            this.channel_7 = new System.Windows.Forms.Button();
            this.channel_8 = new System.Windows.Forms.Button();
            this.initialize_device = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.board_number = new System.Windows.Forms.NumericUpDown();
            this.board_name = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.board_number)).BeginInit();
            this.SuspendLayout();
            // 
            // channel_1
            // 
            this.channel_1.Location = new System.Drawing.Point(6, 19);
            this.channel_1.Name = "channel_1";
            this.channel_1.Size = new System.Drawing.Size(88, 34);
            this.channel_1.TabIndex = 0;
            this.channel_1.Tag = "1";
            this.channel_1.Text = "Channel 1";
            this.channel_1.UseVisualStyleBackColor = true;
            this.channel_1.Click += new System.EventHandler(this.switch_relay);
            // 
            // channel_2
            // 
            this.channel_2.Location = new System.Drawing.Point(100, 19);
            this.channel_2.Name = "channel_2";
            this.channel_2.Size = new System.Drawing.Size(88, 34);
            this.channel_2.TabIndex = 1;
            this.channel_2.Tag = "2";
            this.channel_2.Text = "Channel 2";
            this.channel_2.UseVisualStyleBackColor = true;
            this.channel_2.Click += new System.EventHandler(this.switch_relay);
            // 
            // channel_3
            // 
            this.channel_3.Location = new System.Drawing.Point(194, 19);
            this.channel_3.Name = "channel_3";
            this.channel_3.Size = new System.Drawing.Size(88, 34);
            this.channel_3.TabIndex = 2;
            this.channel_3.Tag = "3";
            this.channel_3.Text = "Channel 3";
            this.channel_3.UseVisualStyleBackColor = true;
            this.channel_3.Click += new System.EventHandler(this.switch_relay);
            // 
            // channel_4
            // 
            this.channel_4.Location = new System.Drawing.Point(288, 19);
            this.channel_4.Name = "channel_4";
            this.channel_4.Size = new System.Drawing.Size(88, 34);
            this.channel_4.TabIndex = 3;
            this.channel_4.Tag = "4";
            this.channel_4.Text = "Channel 4";
            this.channel_4.UseVisualStyleBackColor = true;
            this.channel_4.Click += new System.EventHandler(this.switch_relay);
            // 
            // channel_5
            // 
            this.channel_5.Location = new System.Drawing.Point(6, 59);
            this.channel_5.Name = "channel_5";
            this.channel_5.Size = new System.Drawing.Size(88, 34);
            this.channel_5.TabIndex = 4;
            this.channel_5.Tag = "5";
            this.channel_5.Text = "Channel 5";
            this.channel_5.UseVisualStyleBackColor = true;
            this.channel_5.Click += new System.EventHandler(this.switch_relay);
            // 
            // channel_6
            // 
            this.channel_6.Location = new System.Drawing.Point(100, 59);
            this.channel_6.Name = "channel_6";
            this.channel_6.Size = new System.Drawing.Size(88, 34);
            this.channel_6.TabIndex = 5;
            this.channel_6.Tag = "6";
            this.channel_6.Text = "Channel 6";
            this.channel_6.UseVisualStyleBackColor = true;
            this.channel_6.Click += new System.EventHandler(this.switch_relay);
            // 
            // channel_7
            // 
            this.channel_7.Location = new System.Drawing.Point(194, 59);
            this.channel_7.Name = "channel_7";
            this.channel_7.Size = new System.Drawing.Size(88, 34);
            this.channel_7.TabIndex = 6;
            this.channel_7.Tag = "7";
            this.channel_7.Text = "Channel 7";
            this.channel_7.UseVisualStyleBackColor = true;
            this.channel_7.Click += new System.EventHandler(this.switch_relay);
            // 
            // channel_8
            // 
            this.channel_8.Location = new System.Drawing.Point(288, 59);
            this.channel_8.Name = "channel_8";
            this.channel_8.Size = new System.Drawing.Size(88, 34);
            this.channel_8.TabIndex = 7;
            this.channel_8.Tag = "8";
            this.channel_8.Text = "Channel 8";
            this.channel_8.UseVisualStyleBackColor = true;
            this.channel_8.Click += new System.EventHandler(this.switch_relay);
            // 
            // initialize_device
            // 
            this.initialize_device.Location = new System.Drawing.Point(288, 19);
            this.initialize_device.Name = "initialize_device";
            this.initialize_device.Size = new System.Drawing.Size(88, 38);
            this.initialize_device.TabIndex = 8;
            this.initialize_device.Text = "Initialize device";
            this.initialize_device.UseVisualStyleBackColor = true;
            this.initialize_device.Click += new System.EventHandler(this.initialize_device_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.board_name);
            this.groupBox1.Controls.Add(this.board_number);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.initialize_device);
            this.groupBox1.Location = new System.Drawing.Point(2, 13);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(384, 81);
            this.groupBox1.TabIndex = 9;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Device info";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.channel_2);
            this.groupBox2.Controls.Add(this.channel_1);
            this.groupBox2.Controls.Add(this.channel_8);
            this.groupBox2.Controls.Add(this.channel_3);
            this.groupBox2.Controls.Add(this.channel_7);
            this.groupBox2.Controls.Add(this.channel_4);
            this.groupBox2.Controls.Add(this.channel_6);
            this.groupBox2.Controls.Add(this.channel_5);
            this.groupBox2.Location = new System.Drawing.Point(2, 100);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(384, 111);
            this.groupBox2.TabIndex = 10;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Test channels";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(15, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(73, 13);
            this.label1.TabIndex = 9;
            this.label1.Text = "Board number";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 50);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "Board name";
            // 
            // board_number
            // 
            this.board_number.Location = new System.Drawing.Point(100, 19);
            this.board_number.Name = "board_number";
            this.board_number.Size = new System.Drawing.Size(46, 20);
            this.board_number.TabIndex = 11;
            // 
            // board_name
            // 
            this.board_name.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.board_name.Location = new System.Drawing.Point(100, 50);
            this.board_name.Name = "board_name";
            this.board_name.Size = new System.Drawing.Size(87, 18);
            this.board_name.TabIndex = 12;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(398, 220);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "Relay Test";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.board_number)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button channel_1;
        private System.Windows.Forms.Button channel_2;
        private System.Windows.Forms.Button channel_3;
        private System.Windows.Forms.Button channel_4;
        private System.Windows.Forms.Button channel_5;
        private System.Windows.Forms.Button channel_6;
        private System.Windows.Forms.Button channel_7;
        private System.Windows.Forms.Button channel_8;
        private System.Windows.Forms.Button initialize_device;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label board_name;
        private System.Windows.Forms.NumericUpDown board_number;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
    }
}

