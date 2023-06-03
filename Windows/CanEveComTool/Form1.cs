using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Media;

//
using System.IO.Ports;
using System.Windows;
using System.Windows.Input;
using System.Text.RegularExpressions;


using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;

using System.Windows.Input;
using System.Windows.Markup;
using System.Runtime.CompilerServices;
using System.Threading;

namespace WindowsFormstest1
{
    public partial class Form1 : Form
    {
        public int status = 0;
        //串口实例
        SerialPort serialPort = new SerialPort();
        String send_str = "0";
        String display_dat="";
        int baud = 115200;
        bool isconnect;
        string[] serialProtArray;
        int form_h = 0,form_w=0,last_h;
        byte[] result; 
        public Form1()
        {

            string s = "hide";
            InitializeComponent();
            //获取当前计算机所有的串行端口名
            serialProtArray = SerialPort.GetPortNames();
            comboBox1.DataSource = serialProtArray;
            comboBox1.SelectedIndex = 0;
            comboBox2.SelectedIndex = 0;
             form_w=this.Width;
             form_h=this.Height;
            last_h=this.Height;
            dispaly(display_dat);
        }
        

        private const int WM_DEVICECHANGE = 0x219; //设备改变
        private const int DBT_DEVICEARRIVAL = 0x8000; //检测到新设备
        private const int DBT_DEVICEREMOVECOMPLETE = 0x8004; //移除设备

        

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);//调用父类方法，以确保其他功能正常
            switch (m.Msg)
            {
                case WM_DEVICECHANGE://设备改变事件
                    //MessageBox.Show("变动");
                    string s = "0";
                    comboBox1.DataSource = null;
                    serialProtArray = SerialPort.GetPortNames();
                    comboBox1.Items.Clear();
                    comboBox1.Items.AddRange(serialProtArray);
                    comboBox1.Refresh();
                    comboBox1.SelectedIndex = 0;
                    
                    //InitializeComponent();
                    switch ((int)m.WParam)
                    {
                        case DBT_DEVICEARRIVAL:
                            //MessageBox.Show("新设备");
                             s = "1";
                            //InitializeComponent();
                            break;

                        case DBT_DEVICEREMOVECOMPLETE:
                            //MessageBox.Show("移除设备");
                            s= "2";
                            //InitializeComponent();
                            break;
                        default:
                            //MessageBox.Show("其他变动");
                            break;
                    }
                    //刷新串口设备
                  
                    break;
                

            }
        }
        public void dispaly(String data)
        {
            display_box.Text = data;
            display_box.SelectionStart = this.display_box.Text.Length;
            //this.display_box.SelectionLength = ;
            display_box.ScrollToCaret();//设置一直显示底部
        }

        private void label1_Click(object sender, EventArgs e)
        {
            //MessageBox.Show("点击");
            send_serial();
            // System.Diagnostics.Process.GetCurrentProcess().Kill();
            //string s = "3";
            //InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //MessageBox.Show("你好！");
            //string time = DateTime.Now.ToString("yyyy-MM--dd HH:mm:ss");
            //this.label2.Text = time;
            if (!serialPort.IsOpen)
            {
                //设定参数
                serialPort.PortName = comboBox1.Text;//serialProtArray[comboBox1.GetItemText(comboBox1.SelectedIndex)];
                serialPort.BaudRate = baud;
                serialPort.Parity = Parity.None;
                serialPort.DataBits = 8;
                serialPort.StopBits = StopBits.One;
                try
                {
                    serialPort.DataReceived += GetReceiveMsg;
                    serialPort.Open();
                    open_uart.Text = "关闭串口";
                    open_uart.ForeColor = Color.Red;
                }
                catch
                {
                    MessageBox.Show("串口无法打开");
                }
                
            }
            else
            {
                serialPort.Close();
                open_uart.Text = "打开串口";
                open_uart.ForeColor = Color.Blue;
            } 
        }


        public void Music()
        {
            MessageBox.Show("music");
        }

        private void statusStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            MessageBox.Show("status1");
        }

        private void bindingSource1_CurrentChanged(object sender, EventArgs e)
        {
            MessageBox.Show("bingingsources1");
        }

        private void newplay(object sender, EventArgs e)
        {
            MessageBox.Show("newplay");
        }
        
        public void send_serial()
        {
            if (serialPort.IsOpen)
            {
                string time = DateTime.Now.ToString("HH:mm:ss:fff");
               
                display_dat +=time+"->: "+ send_str+"\r\n";
                dispaly(display_dat); 
                serialPort.Write(send_str);
            //MessageBox.Show("开启");
            }
            else
            {
                //serialPort.Close();
                MessageBox.Show("请打开串口");
            }
            
        }
        private void GetReceiveMsg(object sender, SerialDataReceivedEventArgs e)
        {
            //读取串口缓冲区的字节数据
            result = new byte[serialPort.BytesToRead];
            serialPort.Read(result, 0, serialPort.BytesToRead);
            string time = DateTime.Now.ToString("HH:mm:ss:fff");

            display_dat += time + "<-: " + System.Text.Encoding.UTF8.GetString(result) + "\r\n";


            //线程的依附性：只有创建某些对象的线程、才能访问它所创建的某些对象
            //解决跨线程调用的问题，可以采用封送(Marshal) .调用Invoke方法
            //使用封送
            new Thread(() =>
            {
                for (int i = 0; i < 2; i++)
                {
                    Action<int> action = (data) =>
                {
                    //this.label1.text = data.tostring();
                    dispaly(display_dat);
                    
                };
                    Thread.Sleep(100);
                    // 在拥有此控件的基础窗口句柄的线程上执行指定的委托
                    Invoke(action, i);
            }
            }).Start();
            //MessageBox 没有遵循依附性的原则，所以可以在工作线程中直接访问


            //string str = $"{DateTime.Now}\n";

            //判断读写模式
            //将接收到的字节数组转换为指定的消息格式
            //if (sendText)
            //{
            //    str += $"{Encoding.UTF8.GetString(result)}";
            //}
            //else
            //{
            //    for (int i = 0; i < result.Length; i++)
            //    {
            //        str += $"{result[i].ToString("X2")} ";
            //    }
            //}

            //在窗体中显示接收到的消息
            //SetRecMsgRbx(str.Trim());
        }



        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            //MessageBox.Show("text change");
            send_str = textBox1.Text;
        }

        private void change_com_port(object sender, EventArgs e)
        {
            //MessageBox.Show("port change");

        }

        private void show_text_change(object sender, EventArgs e)
        {
            //new Thread(() =>
            //{
            //    for (int i = 0; i < 2; i++)
            //    {
            //        Action<int> action = (data) =>
            //        {
            //            //this.label1.text = data.tostring();
            //            dispaly(display_dat);
            //        };
            //        // 在拥有此控件的基础窗口句柄的线程上执行指定的委托
            //        Invoke(action, i);
            //    }
            //}).Start();
            //Thread thread = new Thread(thread1);
            //thread.Start();
        }
        public void thread1()
        {
            Thread.Sleep(500);
            this.display_box.SelectionStart = this.display_box.Text.Length;
            //this.display_box.SelectionLength = ;
            this.display_box.ScrollToCaret();//设置一直显示底部
        }

        private void clear_show(object sender, EventArgs e)
        {
            display_dat = "";
            display_box.Text = "";
        }

        private void change_baud(object sender, EventArgs e)
        {
            //comboBox2.GetItemText(comboBox1.SelectedIndex);
            baud=Convert.ToInt32(comboBox2.Text);
            //switch (comboBox2.GetItemText(comboBox2.SelectedIndex))
            //{
            //    case "0":

            //        break;
            //    case "1":
            //        break;
            //    case "2":
            //        break;
            //    case "3":

            //        break;
            //    case "4":
            //        break;
            //    case "5":
            //        break;
            //}
        }

        private void form_change(object sender, EventArgs e)
        {
            //this.Height = ;
            //this.Width = last_h*(Convert.ToInt32((decimal)form_w/form_h));
            //last_h = this.Height;
        }
    }

}
