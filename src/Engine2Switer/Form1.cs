using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace Engine2Switer
{
    public partial class Form1 : Form
    {
        IniFile inifile;
        List<string> resamp2 = new List<string>();
        List<string> wavtool2 = new List<string>();
        string rootPath = "";
        public Form1()
        {
            DirectoryInfo DI = new DirectoryInfo(AppDomain.CurrentDomain.BaseDirectory + "../../");
            InitializeComponent();
            rootPath = DI.FullName;
            label3.Visible = true;
            label3.Dock = DockStyle.Fill;
        }

        bool checkExists(string fp)
        {
            if (System.IO.File.Exists(fp))
            {
                return true;
            }
            else if (System.IO.File.Exists(rootPath + "\\" + fp))
            {
                return true;
            }
            return false;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Form.CheckForIllegalCrossThreadCalls = false;
            Thread th = new Thread(LoadINI);
            th.Start();
        }

        void LoadINI()
        {
            string DBS = Path.GetTempPath() + "\\UTAUDB";
            DirectoryInfo DBI = new DirectoryInfo(DBS);
            if (DBI.Exists && DBI.GetDirectories().Length > 0)
            {
                button3.Visible = true;
            }
            else
            {
                button3.Visible = false;
            }

            if (!System.IO.File.Exists(rootPath + "\\resampler.ini"))
            {
                MessageBox.Show("配置文件未找到！");
               // label3.Visible = false;
                this.Close();
                return;
            }
            else
            {
                inifile = new IniFile(rootPath + "\\resampler.ini");
            }

            string INIResampler = inifile.IniReadValue("Engine2", "Resampler");
            if (checkExists(INIResampler) && !resamp2.Contains(INIResampler)) { resamp2.Add(INIResampler); }
            string INIWavTool = inifile.IniReadValue("Engine2", "Wavtool2");
            if (checkExists(INIWavTool) && !wavtool2.Contains(INIWavTool)) { wavtool2.Add(INIWavTool); }
            string autotranslate = inifile.IniReadValue("Engine2", "AutoCodePage").Trim();
            if (autotranslate == "0") chkCodePage.Checked = false; else chkCodePage.Checked = true;
            string LangID = inifile.IniReadValue("Engine2", "SystemLangID").Trim();
            
            if (!System.IO.File.Exists(rootPath + "\\resampler2.list.txt"))
            {
                System.IO.File.WriteAllText(rootPath + "\\resampler2.list.txt", "");
            }
            else
            {
                string[] rempx = System.IO.File.ReadAllLines(rootPath + "\\resampler2.list.txt");
                foreach (string item in rempx)
                {
                    if (checkExists(item) && !resamp2.Contains(item))
                    {
                        resamp2.Add(item);
                    }
                }
            }
            if (!System.IO.File.Exists(rootPath + "\\wavtool2.list.txt"))
            {
                System.IO.File.WriteAllText(rootPath + "\\wavtool2.list.txt", "");
            }
            else
            {
                string[] rempx = System.IO.File.ReadAllLines(rootPath + "\\wavtool2.list.txt");
                foreach (string item in rempx)
                {
                    if (checkExists(item) && !wavtool2.Contains(item))
                    {
                        wavtool2.Add(item);
                    }
                }
            }

            cmb_Resampler.Items.Clear();
            cmb_Resampler.Items.AddRange(resamp2.ToArray());
            if (cmb_Resampler.Items.Count > 0) cmb_Resampler.SelectedIndex = 0;
            cmb_WaveTool.Items.Clear();
            cmb_WaveTool.Items.AddRange(wavtool2.ToArray());
            if (cmb_WaveTool.Items.Count > 0) cmb_WaveTool.SelectedIndex = 0;

            try
            {   
                int ccd = int.Parse(LangID);
                if (ccd <= 100)
                {
                    langCod.SelectedIndex = 0;
                }
                else
                {
                    switch (ccd)
                    {
                        case 936: langCod.SelectedIndex = 1; break;
                        case 950: langCod.SelectedIndex = 2; break;
                        case 949: langCod.SelectedIndex = 3; break;
                        default: langCod.Text=LangID; break;
                    }
                }
            }
            catch { langCod.SelectedIndex = 0;}

            label3.Visible = false;
        }

        private void btn_Resampler_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.InitialDirectory = rootPath;
            ofd.Filter = "*.dll|*.dll";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                cmb_Resampler.Text = ofd.FileName;
            }
        }

        private void btn_WavTool_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.InitialDirectory = rootPath;
            ofd.Filter = "*.dll|*.dll";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                cmb_WaveTool.Text = ofd.FileName;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string resp = cmb_Resampler.Text.Replace(rootPath, "");
            string wavp = cmb_WaveTool.Text.Replace(rootPath, "");

            bool chd = true;
            StringBuilder sb = new StringBuilder();
            sb.AppendLine("Engine file is not found:");
            chd = chd && checkExists(resp);
            if (!chd) sb.AppendLine("\tResampler2:" + resp);
            chd = chd && checkExists(wavp);
            if (!chd) sb.AppendLine("\tWavtool2:" + wavp);

            if (!chd)
            {
                sb.AppendLine("\r\nContinue Save?");
                if (MessageBox.Show(sb.ToString(), "Error", MessageBoxButtons.YesNo) != DialogResult.Yes)
                {
                    return;
                }
            }

            inifile.IniWriteValue("Engine2", "Resampler", resp);
            inifile.IniWriteValue("Engine2", "Wavtool2", wavp);
            if (chkCodePage.Checked)
            {
                inifile.IniWriteValue("Engine2", "AutoCodePage", "1");
            }
            else
            {
                inifile.IniWriteValue("Engine2", "AutoCodePage", "0");
            }
            try
            {
                switch (langCod.SelectedIndex)
                {
                    case 0: inifile.IniWriteValue("Engine2", "SystemLangID", "-1"); break;
                    case 1: inifile.IniWriteValue("Engine2", "SystemLangID", "936"); break;
                    case 2: inifile.IniWriteValue("Engine2", "SystemLangID", "950"); break;
                    case 3: inifile.IniWriteValue("Engine2", "SystemLangID", "949"); break;
                    default:
                        int pacm = int.Parse(langCod.Text);
                        if (pacm > 300)
                        {
                            inifile.IniWriteValue("Engine2", "SystemLangID", pacm.ToString());
                        }
                        else
                        {
                            inifile.IniWriteValue("Engine2", "SystemLangID", "-1");
                        }
                        break;
                }
            }
            catch { inifile.IniWriteValue("Engine2", "SystemLangID", "-1"); }

            List<string> rmpLST = new List<string>();
            foreach (string s in cmb_Resampler.Items)
            {
                rmpLST.Add(s);
            }
            if (!rmpLST.Contains(resp))
            {
                rmpLST.Add(resp);
            }
            System.IO.File.WriteAllLines(rootPath + "\\resampler2.list.txt", rmpLST.ToArray());

            List<string> wavLST = new List<string>();
            foreach (string s in cmb_WaveTool.Items)
            {
                wavLST.Add(s);
            }
            if (!wavLST.Contains(wavp))
            {
                wavLST.Add(wavp);
            }
            System.IO.File.WriteAllLines(rootPath + "\\wavtool2.list.txt", wavLST.ToArray());


            this.Close();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            button3.Visible = false;
            Thread th = new Thread(() => {
                string DBS = Path.GetTempPath() + "\\UTAUDB";
                DirectoryInfo DBI = new DirectoryInfo(DBS);
                DirectoryInfo[] DIL=DBI.GetDirectories();
                foreach (DirectoryInfo sdi in DIL)
                {
                    try
                    {
                        System.IO.Directory.Delete(sdi.FullName,true);
                    }
                    catch { ;}
                }
            });
            th.Start();
        }
    }
}
