using System;
using System.Reflection;
using System.Windows.Forms;
using System.Xml;

namespace LiveSplit.Crysis3 {
    public partial class Crysis3Settings : UserControl {
        public bool AutoSplit { get; set; }
        public bool LoadRemoval { get; set; }

        public Crysis3Settings() {
            InitializeComponent();

            this.AutoSplitToggled.DataBindings.Add("Checked", this, "AutoSplit", false, DataSourceUpdateMode.OnPropertyChanged);
            this.LoadRemovalToggled.DataBindings.Add("Checked", this, "LoadRemoval", false, DataSourceUpdateMode.OnPropertyChanged);

            AutoSplit = true;
            LoadRemoval = true;
        }

        public XmlNode GetSettings(XmlDocument doc) {
            XmlElement settingsNode = doc.CreateElement("Settings");

            settingsNode.AppendChild(ToElement(doc, "Version", Assembly.GetExecutingAssembly().GetName().Version.ToString(3)));

            settingsNode.AppendChild(ToElement(doc, "AutoSplitToggled", this.AutoSplitToggled));
            settingsNode.AppendChild(ToElement(doc, "LoadRemovalToggled", this.LoadRemovalToggled));

            return settingsNode;
        }

        public void SetSettings(XmlNode settings) {
            bool t;

            this.AutoSplit = settings["AutoSplit"] != null ? (Boolean.TryParse(settings["AutoSplit"].InnerText, out t) ? t : true) : true;
            this.LoadRemoval = settings["LoadRemoval"] != null ? (Boolean.TryParse(settings["LoadRemoval"].InnerText, out t) ? t : true) : true;
        }

        static bool ParseBool(XmlNode settings, string setting, bool default_ = false) {
            bool val;
            return settings[setting] != null ?
                (bool.TryParse(settings[setting].InnerText, out val) ? val : default_)
                : default_;
        }

        static XmlElement ToElement<T>(XmlDocument document, string name, T value) {
            XmlElement str = document.CreateElement(name);
            str.InnerText = value.ToString();
            return str;
        }

        private void AutoSplitToggled_CheckedChanged(object sender, EventArgs e) {

        }

        private void LoadRemovalToggled_CheckedChanged(object sender, EventArgs e) {

        }
    }
}
