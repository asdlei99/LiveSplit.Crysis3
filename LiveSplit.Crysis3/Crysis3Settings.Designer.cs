namespace LiveSplit.Crysis3 {
    partial class Crysis3Settings {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.SettingsGroup = new System.Windows.Forms.GroupBox();
            this.AutoSplitToggled = new System.Windows.Forms.CheckBox();
            this.LoadRemovalToggled = new System.Windows.Forms.CheckBox();
            this.SettingsGroup.SuspendLayout();
            this.SuspendLayout();
            // 
            // SettingsGroup
            // 
            this.SettingsGroup.Controls.Add(this.AutoSplitToggled);
            this.SettingsGroup.Controls.Add(this.LoadRemovalToggled);
            this.SettingsGroup.Location = new System.Drawing.Point(15, 12);
            this.SettingsGroup.Name = "SettingsGroup";
            this.SettingsGroup.Size = new System.Drawing.Size(235, 100);
            this.SettingsGroup.TabIndex = 1;
            this.SettingsGroup.TabStop = false;
            this.SettingsGroup.Text = "Settings";
            // 
            // AutoSplitToggled
            // 
            this.AutoSplitToggled.AutoSize = true;
            this.AutoSplitToggled.Location = new System.Drawing.Point(25, 25);
            this.AutoSplitToggled.Name = "AutoSplitToggled";
            this.AutoSplitToggled.Size = new System.Drawing.Size(71, 17);
            this.AutoSplitToggled.TabIndex = 3;
            this.AutoSplitToggled.Text = "Auto-Split";
            this.AutoSplitToggled.UseVisualStyleBackColor = true;
            this.AutoSplitToggled.CheckedChanged += new System.EventHandler(this.AutoSplitToggled_CheckedChanged);
            // 
            // LoadRemovalToggled
            // 
            this.LoadRemovalToggled.AutoSize = true;
            this.LoadRemovalToggled.Location = new System.Drawing.Point(25, 50);
            this.LoadRemovalToggled.Name = "LoadRemovalToggled";
            this.LoadRemovalToggled.Size = new System.Drawing.Size(95, 17);
            this.LoadRemovalToggled.TabIndex = 2;
            this.LoadRemovalToggled.Text = "Load Removal";
            this.LoadRemovalToggled.UseVisualStyleBackColor = true;
            this.LoadRemovalToggled.Visible = false;
            this.LoadRemovalToggled.CheckedChanged += new System.EventHandler(this.LoadRemovalToggled_CheckedChanged);
            // 
            // Crysis3Settings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.SettingsGroup);
            this.Name = "Crysis3Settings";
            this.Size = new System.Drawing.Size(259, 125);
            this.SettingsGroup.ResumeLayout(false);
            this.SettingsGroup.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox SettingsGroup;
        private System.Windows.Forms.CheckBox AutoSplitToggled;
        private System.Windows.Forms.CheckBox LoadRemovalToggled;
    }
}
