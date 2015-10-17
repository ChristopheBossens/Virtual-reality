namespace NetworkTest
{
    partial class Main
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
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.udp_ip = new System.Windows.Forms.TextBox();
            this.udp_port = new System.Windows.Forms.TextBox();
            this.udp_connect = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tcp_ip = new System.Windows.Forms.TextBox();
            this.tcp_connect = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.tcp_port = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.udp_info = new System.Windows.Forms.Label();
            this.tcp_info = new System.Windows.Forms.Label();
            this.tcp_run_test = new System.Windows.Forms.Button();
            this.udp_send_byte = new System.Windows.Forms.Button();
            this.tcp_send_byte = new System.Windows.Forms.Button();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(167, 26);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(26, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "Port";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 26);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(17, 13);
            this.label4.TabIndex = 2;
            this.label4.Text = "IP";
            // 
            // udp_ip
            // 
            this.udp_ip.Location = new System.Drawing.Point(35, 23);
            this.udp_ip.Name = "udp_ip";
            this.udp_ip.Size = new System.Drawing.Size(127, 20);
            this.udp_ip.TabIndex = 4;
            this.udp_ip.Text = "169.254.152.203";
            // 
            // udp_port
            // 
            this.udp_port.Location = new System.Drawing.Point(199, 23);
            this.udp_port.Name = "udp_port";
            this.udp_port.Size = new System.Drawing.Size(52, 20);
            this.udp_port.TabIndex = 5;
            this.udp_port.Text = "5678";
            // 
            // udp_connect
            // 
            this.udp_connect.Location = new System.Drawing.Point(262, 19);
            this.udp_connect.Name = "udp_connect";
            this.udp_connect.Size = new System.Drawing.Size(81, 27);
            this.udp_connect.TabIndex = 2;
            this.udp_connect.Text = "Connect";
            this.udp_connect.UseVisualStyleBackColor = true;
            this.udp_connect.Click += new System.EventHandler(this.udp_connect_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(170, 73);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(81, 27);
            this.button2.TabIndex = 6;
            this.button2.Text = "Run tests";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(1, 1);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(360, 256);
            this.tabControl1.TabIndex = 1;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage1.Controls.Add(this.tcp_send_byte);
            this.tabPage1.Controls.Add(this.tcp_info);
            this.tabPage1.Controls.Add(this.tcp_run_test);
            this.tabPage1.Controls.Add(this.tcp_ip);
            this.tabPage1.Controls.Add(this.tcp_connect);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Controls.Add(this.tcp_port);
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(352, 230);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "TCP/IP";
            // 
            // tabPage2
            // 
            this.tabPage2.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage2.Controls.Add(this.udp_send_byte);
            this.tabPage2.Controls.Add(this.udp_info);
            this.tabPage2.Controls.Add(this.button2);
            this.tabPage2.Controls.Add(this.udp_ip);
            this.tabPage2.Controls.Add(this.udp_connect);
            this.tabPage2.Controls.Add(this.label3);
            this.tabPage2.Controls.Add(this.udp_port);
            this.tabPage2.Controls.Add(this.label4);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(352, 230);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "UDP";
            // 
            // tcp_ip
            // 
            this.tcp_ip.Location = new System.Drawing.Point(35, 23);
            this.tcp_ip.Name = "tcp_ip";
            this.tcp_ip.Size = new System.Drawing.Size(127, 20);
            this.tcp_ip.TabIndex = 9;
            this.tcp_ip.Text = "169.254.152.203";
            // 
            // tcp_connect
            // 
            this.tcp_connect.Location = new System.Drawing.Point(262, 19);
            this.tcp_connect.Name = "tcp_connect";
            this.tcp_connect.Size = new System.Drawing.Size(81, 27);
            this.tcp_connect.TabIndex = 6;
            this.tcp_connect.Text = "Connect";
            this.tcp_connect.UseVisualStyleBackColor = true;
            this.tcp_connect.Click += new System.EventHandler(this.tcp_connect_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(167, 26);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(26, 13);
            this.label1.TabIndex = 8;
            this.label1.Text = "Port";
            // 
            // tcp_port
            // 
            this.tcp_port.Location = new System.Drawing.Point(199, 23);
            this.tcp_port.Name = "tcp_port";
            this.tcp_port.Size = new System.Drawing.Size(52, 20);
            this.tcp_port.TabIndex = 10;
            this.tcp_port.Text = "6789";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 26);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(17, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "IP";
            // 
            // udp_info
            // 
            this.udp_info.BackColor = System.Drawing.SystemColors.Info;
            this.udp_info.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.udp_info.Location = new System.Drawing.Point(11, 103);
            this.udp_info.Name = "udp_info";
            this.udp_info.Size = new System.Drawing.Size(334, 120);
            this.udp_info.TabIndex = 7;
            // 
            // tcp_info
            // 
            this.tcp_info.BackColor = System.Drawing.SystemColors.Info;
            this.tcp_info.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tcp_info.Location = new System.Drawing.Point(11, 103);
            this.tcp_info.Name = "tcp_info";
            this.tcp_info.Size = new System.Drawing.Size(334, 120);
            this.tcp_info.TabIndex = 12;
            // 
            // tcp_run_test
            // 
            this.tcp_run_test.Location = new System.Drawing.Point(170, 73);
            this.tcp_run_test.Name = "tcp_run_test";
            this.tcp_run_test.Size = new System.Drawing.Size(81, 27);
            this.tcp_run_test.TabIndex = 11;
            this.tcp_run_test.Text = "Run tests";
            this.tcp_run_test.UseVisualStyleBackColor = true;
            this.tcp_run_test.Click += new System.EventHandler(this.tcp_run_test_Click);
            // 
            // udp_send_byte
            // 
            this.udp_send_byte.Location = new System.Drawing.Point(262, 73);
            this.udp_send_byte.Name = "udp_send_byte";
            this.udp_send_byte.Size = new System.Drawing.Size(81, 27);
            this.udp_send_byte.TabIndex = 8;
            this.udp_send_byte.Text = "Send byte";
            this.udp_send_byte.UseVisualStyleBackColor = true;
            this.udp_send_byte.Click += new System.EventHandler(this.udp_send_byte_Click);
            // 
            // tcp_send_byte
            // 
            this.tcp_send_byte.Location = new System.Drawing.Point(262, 73);
            this.tcp_send_byte.Name = "tcp_send_byte";
            this.tcp_send_byte.Size = new System.Drawing.Size(81, 27);
            this.tcp_send_byte.TabIndex = 13;
            this.tcp_send_byte.Text = "Send byte";
            this.tcp_send_byte.UseVisualStyleBackColor = true;
            this.tcp_send_byte.Click += new System.EventHandler(this.tcp_send_byte_Click);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(360, 261);
            this.Controls.Add(this.tabControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Main";
            this.Text = "Network test";
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button udp_connect;
        private System.Windows.Forms.TextBox udp_port;
        private System.Windows.Forms.TextBox udp_ip;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TextBox tcp_ip;
        private System.Windows.Forms.Button tcp_connect;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tcp_port;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label tcp_info;
        private System.Windows.Forms.Button tcp_run_test;
        private System.Windows.Forms.Label udp_info;
        private System.Windows.Forms.Button udp_send_byte;
        private System.Windows.Forms.Button tcp_send_byte;
    }
}

