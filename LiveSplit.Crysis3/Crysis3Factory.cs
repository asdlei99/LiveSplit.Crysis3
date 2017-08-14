using System.Reflection;
using LiveSplit.Crysis3;
using LiveSplit.UI.Components;
using System;
using LiveSplit.Model;
using System.Windows.Forms;

[assembly: ComponentFactory(typeof(Crysis3Factory))]

namespace LiveSplit.Crysis3 {
    public class Crysis3Factory : IComponentFactory {
        public string ComponentName => "Crysis 3";
        public string Description => "Automates load removal for Crysis 3.";
        public ComponentCategory Category => ComponentCategory.Control;

        public IComponent Create(LiveSplitState state) {
            return new Crysis3Component(state);
        }

        public string UpdateName => this.ComponentName;
        public string UpdateURL => "https://raw.githubusercontent.com/btbd/LiveSplit.Crysis3/master/";
        public Version Version => Assembly.GetExecutingAssembly().GetName().Version;
        public string XMLURL => this.UpdateURL + "Components/update.LiveSplit.Crysis3.xml";
    }
}
