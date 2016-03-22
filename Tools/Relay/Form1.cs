using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using MccDaq;

namespace Relay
{
    public partial class Form1 : Form
    {
        private MccDaq.MccBoard relayBoard;
        private MccDaq.ErrorInfo errorInfo;
        private bool[] relayStatus = new bool[8];

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            for (int i = 0; i < 8; ++i)
                relayStatus[i] = false;
        }


        private void initialize_device_Click(object sender, EventArgs e)
        {
            relayBoard = new MccDaq.MccBoard(0);
            errorInfo = MccDaq.MccService.ErrHandling(MccDaq.ErrorReporting.PrintAll, MccDaq.ErrorHandling.DontStop);

            board_name.Text = relayBoard.BoardName;
        }

        private void switch_relay(object sender, EventArgs e)
        {
            if (relayBoard == null)
            {
                MessageBox.Show("Initialize a device first","Error");
                return;
            }
            Button b = (Button)sender;
            int relayNumber = Convert.ToInt32(b.Tag);

            if (relayStatus[relayNumber-1] == false)
            {
                relayStatus[relayNumber - 1] = true;
                relayBoard.DBitOut(DigitalPortType.FirstPortA, relayNumber+15, DigitalLogicState.High);
            }
            else
            {
                relayStatus[relayNumber - 1] = false;
                relayBoard.DBitOut(DigitalPortType.FirstPortA, relayNumber+15, DigitalLogicState.Low);
            }

        }

        /* Switch of relay, then switch it on for a specified duration before
         * switching of again, can be used to test waterflow during a pulse given
         * in an experiment
         */ 
        private void pulse_relay(object sender, EventArgs e)
        {
            if (relayBoard == null)
            {
                MessageBox.Show("Initialize a device first", "Error");
                return;
            }
            Button b = (Button)sender;
            int relayNumber = Convert.ToInt32(b.Tag);
            int pulseDuration = Convert.ToInt32(pulse_duration.Value);

            if (relayStatus[relayNumber-1] == true)
            {
                relayStatus[relayNumber - 1] = false;
                relayBoard.DBitOut(DigitalPortType.FirstPortA, relayNumber + 15, DigitalLogicState.Low);
            }

            relayBoard.DBitOut(DigitalPortType.FirstPortA, relayNumber + 15, DigitalLogicState.High);
            relayStatus[relayNumber - 1] = true;
            Thread.Sleep(pulseDuration);
            relayBoard.DBitOut(DigitalPortType.FirstPortA, relayNumber + 15, DigitalLogicState.Low);
            relayStatus[relayNumber - 1] = false;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (relayBoard != null)
            {
                for (int relayNumber = 0; relayNumber < 8; ++relayNumber)
                {
                    if (relayStatus[relayNumber] == true)
                        relayBoard.DBitOut(DigitalPortType.FirstPortA, relayNumber + 15, DigitalLogicState.Low);
                }
            }
        }
    }
}
