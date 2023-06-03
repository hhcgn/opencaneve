
using System;
using System.Drawing;
using System.Windows.Forms;

namespace WindowsFormstest1
{
    
    partial class Form1
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
        bool isfirst=true;
        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.set1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.set2ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.set3ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.set4ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.set5ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.set6ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.set7ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.set11ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.set33ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.set44ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.set22ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.display_box = new System.Windows.Forms.TextBox();
            this.open_uart = new System.Windows.Forms.Button();
            this.send = new System.Windows.Forms.Button();
            this.clear = new System.Windows.Forms.Button();
            this.set1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // set1
            // 
            this.set1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.set1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.set2ToolStripMenuItem,
            this.set4ToolStripMenuItem});
            this.set1.Name = "set1";
            resources.ApplyResources(this.set1, "set1");
            // 
            // set2ToolStripMenuItem
            // 
            this.set2ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.set3ToolStripMenuItem});
            this.set2ToolStripMenuItem.Name = "set2ToolStripMenuItem";
            resources.ApplyResources(this.set2ToolStripMenuItem, "set2ToolStripMenuItem");
            // 
            // set3ToolStripMenuItem
            // 
            this.set3ToolStripMenuItem.Name = "set3ToolStripMenuItem";
            resources.ApplyResources(this.set3ToolStripMenuItem, "set3ToolStripMenuItem");
            // 
            // set4ToolStripMenuItem
            // 
            this.set4ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.set5ToolStripMenuItem,
            this.set7ToolStripMenuItem});
            this.set4ToolStripMenuItem.Name = "set4ToolStripMenuItem";
            resources.ApplyResources(this.set4ToolStripMenuItem, "set4ToolStripMenuItem");
            // 
            // set5ToolStripMenuItem
            // 
            this.set5ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.set6ToolStripMenuItem});
            this.set5ToolStripMenuItem.Name = "set5ToolStripMenuItem";
            resources.ApplyResources(this.set5ToolStripMenuItem, "set5ToolStripMenuItem");
            // 
            // set6ToolStripMenuItem
            // 
            this.set6ToolStripMenuItem.Name = "set6ToolStripMenuItem";
            resources.ApplyResources(this.set6ToolStripMenuItem, "set6ToolStripMenuItem");
            // 
            // set7ToolStripMenuItem
            // 
            this.set7ToolStripMenuItem.Name = "set7ToolStripMenuItem";
            resources.ApplyResources(this.set7ToolStripMenuItem, "set7ToolStripMenuItem");
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.set11ToolStripMenuItem,
            this.set22ToolStripMenuItem});
            resources.ApplyResources(this.menuStrip1, "menuStrip1");
            this.menuStrip1.Name = "menuStrip1";
            // 
            // set11ToolStripMenuItem
            // 
            this.set11ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.set33ToolStripMenuItem,
            this.set44ToolStripMenuItem});
            this.set11ToolStripMenuItem.Name = "set11ToolStripMenuItem";
            resources.ApplyResources(this.set11ToolStripMenuItem, "set11ToolStripMenuItem");
            // 
            // set33ToolStripMenuItem
            // 
            this.set33ToolStripMenuItem.Name = "set33ToolStripMenuItem";
            resources.ApplyResources(this.set33ToolStripMenuItem, "set33ToolStripMenuItem");
            // 
            // set44ToolStripMenuItem
            // 
            this.set44ToolStripMenuItem.Name = "set44ToolStripMenuItem";
            resources.ApplyResources(this.set44ToolStripMenuItem, "set44ToolStripMenuItem");
            this.set44ToolStripMenuItem.Click += new System.EventHandler(this.newplay);
            // 
            // set22ToolStripMenuItem
            // 
            this.set22ToolStripMenuItem.Name = "set22ToolStripMenuItem";
            resources.ApplyResources(this.set22ToolStripMenuItem, "set22ToolStripMenuItem");
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            resources.ApplyResources(this.comboBox1, "comboBox1");
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Sorted = true;
            this.comboBox1.TextUpdate += new System.EventHandler(this.change_com_port);
            // 
            // textBox1
            // 
            this.textBox1.BackColor = System.Drawing.SystemColors.ControlLight;
            resources.ApplyResources(this.textBox1, "textBox1");
            this.textBox1.Name = "textBox1";
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // comboBox2
            // 
            this.comboBox2.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
            this.comboBox2.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.comboBox2.FormattingEnabled = true;
            resources.ApplyResources(this.comboBox2, "comboBox2");
            this.comboBox2.Items.AddRange(new object[] {
            resources.GetString("comboBox2.Items"),
            resources.GetString("comboBox2.Items1"),
            resources.GetString("comboBox2.Items2"),
            resources.GetString("comboBox2.Items3"),
            resources.GetString("comboBox2.Items4"),
            resources.GetString("comboBox2.Items5"),
            resources.GetString("comboBox2.Items6"),
            resources.GetString("comboBox2.Items7"),
            resources.GetString("comboBox2.Items8")});
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Tag = "";
            this.comboBox2.TextChanged += new System.EventHandler(this.change_baud);
            // 
            // display_box
            // 
            this.display_box.BackColor = System.Drawing.SystemColors.ControlLight;
            resources.ApplyResources(this.display_box, "display_box");
            this.display_box.Name = "display_box";
            this.display_box.ReadOnly = true;
            this.display_box.TextChanged += new System.EventHandler(this.show_text_change);
            // 
            // open_uart
            // 
            resources.ApplyResources(this.open_uart, "open_uart");
            this.open_uart.Name = "open_uart";
            this.open_uart.UseVisualStyleBackColor = true;
            this.open_uart.Click += new System.EventHandler(this.Form1_Load);
            // 
            // send
            // 
            resources.ApplyResources(this.send, "send");
            this.send.Name = "send";
            this.send.UseVisualStyleBackColor = true;
            this.send.Click += new System.EventHandler(this.label1_Click);
            // 
            // clear
            // 
            resources.ApplyResources(this.clear, "clear");
            this.clear.Name = "clear";
            this.clear.UseVisualStyleBackColor = true;
            this.clear.Click += new System.EventHandler(this.clear_show);
            // 
            // Form1
            // 
            this.AcceptButton = this.send;
            resources.ApplyResources(this, "$this");
            this.AllowDrop = true;
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoValidate = System.Windows.Forms.AutoValidate.EnableAllowFocusChange;
            this.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.BackgroundImage = global::CanEveComTool.Properties.Resources.background;
            this.Controls.Add(this.clear);
            this.Controls.Add(this.send);
            this.Controls.Add(this.open_uart);
            this.Controls.Add(this.display_box);
            this.Controls.Add(this.comboBox2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.menuStrip1);
            this.DoubleBuffered = true;
            this.ForeColor = System.Drawing.SystemColors.MenuHighlight;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.KeyPreview = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Opacity = 0.95D;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
            this.SizeChanged += new System.EventHandler(this.form_change);
            this.set1.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        private void @new(object sender, EventArgs e)
        {
            throw new NotImplementedException();
        }

        #endregion
        private ContextMenuStrip set1;
        private ToolStripMenuItem set2ToolStripMenuItem;
        private ToolStripMenuItem set3ToolStripMenuItem;
        private ToolStripMenuItem set4ToolStripMenuItem;
        private ToolStripMenuItem set5ToolStripMenuItem;
        private ToolStripMenuItem set6ToolStripMenuItem;
        private ToolStripMenuItem set7ToolStripMenuItem;
        private MenuStrip menuStrip1;
        private ToolStripMenuItem set11ToolStripMenuItem;
        private ToolStripMenuItem set33ToolStripMenuItem;
        private ToolStripMenuItem set44ToolStripMenuItem;
        private ToolStripMenuItem set22ToolStripMenuItem;
        private ComboBox comboBox1;
        private TextBox textBox1;
        private ComboBox comboBox2;
        private TextBox display_box;
        private Button open_uart;
        private Button send;
        private Button clear;
    }
}

