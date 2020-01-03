using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.IO.Ports;
using System.IO;

using System.Net;
using System.Threading;
using System.Net.Sockets;
using System.DirectoryServices;
using System.Net.NetworkInformation;

using System.Text.RegularExpressions;  //提取IP时的正则
using NATUPNPLib;                      //Windows UPnP COM组件

using System.Diagnostics;
using System.Windows.Forms.DataVisualization.Charting;

namespace BalanceMeasure
{
    public partial class Form1 : Form
    {
        public int PortName;
       
       public StringBuilder Note_StringBuilder;
 
        //---定义  
        private delegate void ShowReceiveMessageCallBack(string text);
        //---声明一个委托  
     
       public static Socket socket;

        public Form1()
        {
            InitializeComponent();
            System.Windows.Forms.Control.CheckForIllegalCrossThreadCalls = false;
            InitChart();
        }
        
        private void Form1_Load(object sender, EventArgs e)
        {
            serialPort1.DataReceived += new SerialDataReceivedEventHandler(port1_DataReceived);
            serialPort2.DataReceived += new SerialDataReceivedEventHandler(port2_DataReceived);
        }
        private int port1DataCounter = 0;
        private int port2DataCounter = 0;
        //过滤数据
        private const int FILTER = 2;
        private void port1_DataReceived(object sender, SerialDataReceivedEventArgs e)//串口数据接收事件
        {
            if (!isStarted) return;
            //port 1
            try
                {
                    int ilen = serialPort1.BytesToRead;
                    byte[] bytes = new byte[ilen];
                    serialPort1.Read(bytes, 0, ilen);
                    string str = System.Text.Encoding.Default.GetString(bytes); //xx="中文";
                    textBox1.AppendText(str + '\r' + '\n');//添加内容

                    port1DataCounter++;
                    if (port1DataCounter % FILTER == 0)
                    {
                        float f1 = Math.Abs(Convert.ToSingle(str));
                        checkRemoveQueue();
                        //对数据B 做处理 方便显示
                        f1 += 1f;
                        dataQueueSerialA.Enqueue(f1);
                    }
                }catch{
                    textBox1.AppendText("串口数据接收出错，请检查!\r\n");
                }
        }
        private void port2_DataReceived(object sender, SerialDataReceivedEventArgs e)//串口数据接收事件
        {
            //port 2
            if (!isStarted)return;

            try
            {
                int ilen = serialPort2.BytesToRead;
                byte[] bytes = new byte[ilen];
                serialPort2.Read(bytes, 0, ilen);
                string str = System.Text.Encoding.Default.GetString(bytes); //xx="中文";
                textBox2.AppendText(str + '\r' + '\n');//添加内容

                port2DataCounter++;
                if (port2DataCounter % FILTER == 0)
                {
                    float f1 = Math.Abs(Convert.ToSingle(str));
                    checkRemoveQueue();
                    //对数据B 做处理 方便显示
                    f1 += 15.0f;
                    dataQueueSerialB.Enqueue(f1);
                }
            }catch{
                    textBox2.AppendText("串口数据接收出错，请检查!\r\n");
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (button1.Text == "连接")
            {
                try
                {
                    serialPort1.PortName = comboBox1.Text;
                    serialPort1.BaudRate = Convert.ToInt32(comboBox2.Text);
                    serialPort1.Open();
                    button1.Text = "断开连接";
                    comboBox1.Enabled = false;
                    comboBox2.Enabled = false;
                    textBox1.AppendText("串口已连接\r\n");
                }
                catch
                {
                    if (serialPort1.IsOpen)
                        serialPort1.Close();

                    button1.Text = "连接";
                    comboBox1.Enabled = true;
                    comboBox2.Enabled = true;
                    textBox1.AppendText("请检查串口连接\r\n");
                }
            }
            else if (button1.Text == "断开连接")
            {
                try
                {
                    serialPort1.Close();
                    button1.Text = "连接";
                    comboBox1.Enabled = true;
                    comboBox2.Enabled = true;
                    textBox1.AppendText("串口已断开\r\n");
                }
                catch { }
            }
        }
    

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Process.GetCurrentProcess().Kill();//彻底关闭软件
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (button2.Text == "连接")
            {
                try
                {
                    serialPort2.PortName = comboBox3.Text;
                    serialPort2.BaudRate = Convert.ToInt32(comboBox4.Text);
                    serialPort2.Open();
                    button2.Text = "断开连接";
                    comboBox4.Enabled = false;
                    comboBox3.Enabled = false;
                    textBox2.AppendText("串口已连接\r\n");
                }
                catch
                {
                    if (serialPort2.IsOpen)
                        serialPort2.Close();

                    button2.Text = "连接";
                    comboBox4.Enabled = true;
                    comboBox3.Enabled = true;
                    string text = "请检查串口连接 " +
                        " name " + comboBox3.Text + 
                        "rate " + Convert.ToInt32(comboBox4.Text);
                    textBox2.AppendText(text);
                }
            }
            else if (button2.Text == "断开连接")
            {
                try
                {
                    serialPort2.Close();
                    button2.Text = "连接";
                    comboBox4.Enabled = true;
                    comboBox3.Enabled = true;
                    textBox2.AppendText("串口已断开\r\n");
                }
                catch { }
            }
        }


        //图表
        private const int Y_MAX = 30;
        private const int QUEUE_LEN = 20;
        private Queue<double> dataQueueSerialA = new Queue<double>(QUEUE_LEN);
        private Queue<double> dataQueueSerialB = new Queue<double>(QUEUE_LEN);
        private bool isStarted = false;
   
        private void btnStart_Click(object sender, EventArgs e)
        {
            if (!isStarted)
            {
                this.timer1.Start();
                isStarted = true;
                btnStart.Text = "结束";
            }
            else
            {
                this.timer1.Stop();
                isStarted = false;
                btnStart.Text = "开始";
            }
         
        }
        private void btnStop_Click(object sender, EventArgs e)
        {
          
        }
        private void timer1_Tick(object sender, EventArgs e)
        {
         
            //定时更新数据
            this.chart1.Series[0].Points.Clear();
            this.chart1.Series[1].Points.Clear();


            int countA = dataQueueSerialA.Count;
            int countB = dataQueueSerialB.Count;
            int countMax = countA > countB ? countA : countB;
            Console.WriteLine("countA " + dataQueueSerialA.Count + "countB " + dataQueueSerialB.Count + "\r\n");
            for (int i = 0; i < countMax; i++){
                if (i < countA) {
                    this.chart1.Series[0].Points.AddY(dataQueueSerialA.ElementAt(i));
                }
                if (i < countB) {
                    this.chart1.Series[1].Points.AddY(dataQueueSerialB.ElementAt(i));
                }
            }
        }

        /// <summary>
        /// 初始化图表
        /// </summary>
        private void InitChart()
        {
            //定义图表区域
            this.chart1.ChartAreas.Clear();
            ChartArea chartArea1 = new ChartArea("C1");
            this.chart1.ChartAreas.Add(chartArea1);
            //定义存储和显示点的容器
            this.chart1.Series.Clear();
            Series series1 = new Series("Left-A");
            Series series2 = new Series("Right-B");
            series1.ChartArea = "C1";
            series2.ChartArea = "C1";
            this.chart1.Series.Add(series1);
            this.chart1.Series.Add(series2);
            //设置图表显示样式
            this.chart1.ChartAreas[0].AxisY.Minimum = 0;
            this.chart1.ChartAreas[0].AxisY.Maximum = Y_MAX;
            this.chart1.ChartAreas[0].AxisX.Minimum = 0;
            this.chart1.ChartAreas[0].AxisX.Maximum = QUEUE_LEN;
            this.chart1.ChartAreas[0].AxisX.Interval = 1;
            this.chart1.ChartAreas[0].AxisX.MajorGrid.LineColor = System.Drawing.Color.Silver;
            this.chart1.ChartAreas[0].AxisY.MajorGrid.LineColor = System.Drawing.Color.Silver;

            //this.chart1.ChartAreas[1].AxisY.Minimum = 0;
            //this.chart1.ChartAreas[1].AxisY.Maximum = Y_MAX;
            //this.chart1.ChartAreas[1].AxisX.Minimum = 0;
            //this.chart1.ChartAreas[1].AxisX.Maximum = QUEUE_LEN;
            //this.chart1.ChartAreas[1].AxisX.Interval = 1;
            //this.chart1.ChartAreas[1].AxisX.MajorGrid.LineColor = System.Drawing.Color.Silver;
            //this.chart1.ChartAreas[1].AxisY.MajorGrid.LineColor = System.Drawing.Color.Silver;

            //设置标题
            this.chart1.Titles.Clear();
            this.chart1.Titles.Add("S01");
            this.chart1.Titles[0].Text = "XXX显示";
            this.chart1.Titles[0].ForeColor = Color.RoyalBlue;
            this.chart1.Titles[0].Font = new System.Drawing.Font("Microsoft Sans Serif", 12F);

            //this.chart1.titles[1].text = "xxx显示";
            //this.chart1.titles[1].forecolor = color.royalblue;
            //this.chart1.titles[1].font = new system.drawing.font("microsoft sans serif", 12f);
            //设置图表显示样式
            this.chart1.Series[0].Color = Color.Red;
            this.chart1.Series[1].Color = Color.Blue;

            this.chart1.Titles[0].Text = "间隙间距 /mm";
            this.chart1.Series[0].ChartType = SeriesChartType.Spline;

           // this.chart1.Titles[1].Text = "XXX {1} 显示";
            this.chart1.Series[1].ChartType = SeriesChartType.Spline;

            this.chart1.Series[0].Points.Clear();
            this.chart1.Series[1].Points.Clear();
        }

        private void checkRemoveQueue()
        {
            if (dataQueueSerialA.Count >= QUEUE_LEN)
            {
                //先出列
                for (int i = 0; i < dataQueueSerialA.Count; i++)
                {
                    dataQueueSerialA.Dequeue();
                }
            }
            if (dataQueueSerialB.Count >= QUEUE_LEN)
            {
                //先出列
                for (int i = 0; i < dataQueueSerialB.Count; i++)
                {
                    dataQueueSerialB.Dequeue();
                }
            }
        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            textBox1.Clear();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            textBox2.Clear();
        }
    }

}
