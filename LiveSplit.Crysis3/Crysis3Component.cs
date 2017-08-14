using LiveSplit.Model;
using LiveSplit.UI.Components;
using LiveSplit.UI;
using LiveSplit.TimeFormatters;

using System;
using System.Text;
using System.Xml;
using System.Windows.Forms;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Diagnostics;
using System.Linq;
using LiveSplit.ComponentUtil;

namespace LiveSplit.Crysis3 {
    public class Crysis3Component : LogicComponent {
        public override string ComponentName => "Crysis 3";

        bool stopped = false;
        public Crysis3Settings Settings { get; set; }
        public TcpListener Server;
        public List<TcpClient> Connections = new List<TcpClient>();
        public TimerModel Model { get; set; }
        public LiveSplitState State { get; set; }
        public ITimeFormatter SplitTimeFormatter;


        public Crysis3Component(LiveSplitState state) {
            this.Settings = new Crysis3.Crysis3Settings();

            SplitTimeFormatter = new RegularTimeFormatter(TimeAccuracy.Hundredths);
            Model = new TimerModel();
            State = state;

            Model.CurrentState = State;
            
            Start();
        }

        public void Start() {
            Server = new TcpListener(IPAddress.Any, 41573);
            Server.Start();
            Server.BeginAcceptTcpClient(AcceptTcpClient, null);


            Thread t = new Thread(new ParameterizedThreadStart(ProcessListener));
            t.Start();
        }

        public void ProcessListener(object _) {
            string dir = Directory.GetCurrentDirectory() + "\\Components";
            string exe = dir + "\\c3nl_loader.exe";
            Process game;
            
            while (!stopped) {
                game = Process.GetProcesses().FirstOrDefault(p => p.ProcessName.ToLower() == "crysis3" && !p.HasExited);
                if (game == null) {
                    continue;
                }
               
                var process = new Process();
                process.StartInfo.FileName = exe;
                process.StartInfo.WorkingDirectory = dir;
                process.StartInfo.UseShellExecute = false;
                process.Start();

                Thread.Sleep(2500);
            }
        }

        public void Stop() {
            stopped = true;
            CloseAllConnections();
        }
   
        public void AcceptTcpClient(IAsyncResult result) {
            try {
                var client = Server.EndAcceptTcpClient(result);
                Thread t = new Thread(new ParameterizedThreadStart(Connect));
                t.Start(client);
                Server.BeginAcceptTcpClient(AcceptTcpClient, null);
            } catch { }
        }

        public void Connect(object obj) {
            TcpClient client = (TcpClient)obj;
            Connections.Add(client);
            Stream stream = client.GetStream();

            int read = 0;
            byte[] message = new byte[4096];

            for (;;) {
                try {
                    read = stream.Read(message, 0, 4096);
                } catch {
                    break;
                }

                if (read == 0) {
                    break;
                }

                string ascii = Encoding.ASCII.GetString(message, 0, read);
                MessageHandler(ascii, stream);
            }

            stream.Close();
            client.Close();
        }
        
        public void Send(Stream stream, string message) {
            var buffer = Encoding.UTF8.GetBytes(message);
            stream.Write(buffer, 0, buffer.Length);
            stream.Flush();
        }

        public bool isStopped() {
            var t = State.CurrentTime[TimingMethod.RealTime];
            char[] time = SplitTimeFormatter.Format(t).ToCharArray();
            for (int i = 0; i < time.Length; i++) {
                if ((int)time[i] > 45 && (int)time[i] < 59 && time[i] != ':' && time[i] != '.' && time[i] != '0') {
                    return false;
                }
            }

            return true;
        }

        public void MessageHandler(string m, Stream stream) {
            var messages = m.Split('\n');
            for (int i = 0; i < messages.Length - 1; ++i) {
                string message = messages[i];
                if (this.Settings.AutoSplit && message.StartsWith("start")) {
                    Model.InitializeGameTime();
                    Model.Start();
                } else if (this.Settings.LoadRemoval && message.StartsWith("pause") && !isStopped()) {
                    Model.CurrentState.IsGameTimePaused = true;
                } else if (this.Settings.LoadRemoval && message.StartsWith("resume") && !isStopped()) {
                    Model.CurrentState.IsGameTimePaused = false;
                } else if (this.Settings.AutoSplit && message.StartsWith("split") && !isStopped()) {
                    Model.Split();
                } else if (this.Settings.AutoSplit && message.StartsWith("reset")) {
                    Model.Reset();
                }
            }
        }

        public void CloseAllConnections() {
            foreach (var client in Connections) {
                if (client != null) {
                    client.Close();
                }
            }

            Connections.Clear();

            if (Server != null) {
                Server.Stop();
            }
        }

        public override void Dispose() {
            Stop();
        }

        public override void Update(IInvalidator invalidator, LiveSplitState state, float width, float height, LayoutMode mode) {

        }

        public override XmlNode GetSettings(XmlDocument document) {
            return this.Settings.GetSettings(document);
        }

        public override Control GetSettingsControl(LayoutMode mode) {
            return this.Settings;
        }

        public override void SetSettings(XmlNode settings) {
            this.Settings.SetSettings(settings);
        }
    }
}
