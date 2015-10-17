using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.IO;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Net;
using System.Net.Sockets;

namespace vrClient
{
    public partial class main_form : Form
    {
        ClientConnection clientConnection;
        ExperimentDesigner experimentDesigner;
        ListboxMessenger matlabServerMessenger, cheetahServerMessages;


        public main_form()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            reward_velocity_comparison.SelectedIndex = 0;
            transition_velocity_comparison.SelectedIndex = 0;

            experimentDesigner = new ExperimentDesigner();

            matlabServerMessenger = new ListboxMessenger(matlab_server_messages, 20);
            cheetahServerMessages = new ListboxMessenger(cheetah_server_messages, 20);
        }

        // Callback functions for designing experiments
        private void load_left_stimulus_Click(object sender, EventArgs e)
        {
            ofDialog.Filter = "PNG Files (*.png)|*.png";

            DialogResult dr = ofDialog.ShowDialog();
            if (dr == DialogResult.OK)
            {
                left_stimulus.ImageLocation=ofDialog.FileName;
                Bitmap bmp = new Bitmap(ofDialog.FileName);
                left_stimulus_name.Text = Path.GetFileNameWithoutExtension(ofDialog.FileName);
                left_stimulus_width.Text = bmp.Width.ToString();
                left_stimulus_height.Text = bmp.Height.ToString();
                left_stimulus_top.Text = "0";
                left_stimulus_left.Text = "0";
            }
        }
        private void load_right_stimulus_Click(object sender, EventArgs e)
        {
            ofDialog.Filter = "PNG Files (*.png)|*.png";

            DialogResult dr = ofDialog.ShowDialog();
            if (dr == DialogResult.OK)
            {
                right_stimulus.ImageLocation = ofDialog.FileName;
                right_stimulus_name.Text = Path.GetFileNameWithoutExtension(ofDialog.FileName);
                Bitmap bmp = new Bitmap(ofDialog.FileName);
                right_stimulus_width.Text = bmp.Width.ToString();
                right_stimulus_height.Text = bmp.Height.ToString();
                right_stimulus_top.Text = "0";
                right_stimulus_left.Text = "0";
            }
        }
        private void add_trial_Click(object sender, EventArgs e)
        {
            if (experimentDesigner.NameAvailable(trial_name.Text))
            {
                try
                {
                    experimentDesigner.AddTrial(GetGUITrial());

                    trial_list.Items.Add(trial_name.Text);
                    trial_list.SelectedIndex = trial_list.Items.Count - 1;
                    trial_list.Update();
                }
                catch (Exception exception)
                {
                    MessageBox.Show(exception.Message);
                }
            }
            else
                MessageBox.Show("A trial called " + trial_name.Text + " already exists!");
        }
        private void update_trial_Click(object sender, EventArgs e)
        {
            try
            {
                int selectedTrial = trial_list.SelectedIndex;
                string originalName = trial_list.SelectedItem.ToString();
                string newName = trial_name.Text;
                if (originalName != newName)
                {
                    if (!(experimentDesigner.NameAvailable(newName)))
                    {
                        MessageBox.Show("A trial with name " + trial_name.Text + " is already available.");
                        return;
                    }
                }
                experimentDesigner.UpdateTrial(originalName, GetGUITrial());
                trial_list.Items[selectedTrial] = newName;
            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.Message);
            }
        } 
        private void trial_list_MouseDoubleClick(object sender, MouseEventArgs e)
                {
                    ListBox lb = (ListBox)sender;
                    string trialName = lb.SelectedItem.ToString();

                    SetGUITrial(experimentDesigner.GetTrial(trialName));
                }
        private void set_isi_trial_Click(object sender, EventArgs e)
        {
            Button b = (Button)sender;
            if (b.Text == ">>")
            {
                if (trial_list.SelectedIndex > -1)
                {
                    string trialName = trial_list.SelectedItem.ToString();
                    Trial trial = experimentDesigner.GetTrial(trialName);
                    trial.SetISI(true);
                    experimentDesigner.UpdateTrial(trialName, trial);
                    experimentDesigner.SetISITrial(trialName);
                    trial_list.Items.RemoveAt(trial_list.SelectedIndex);
                    trial_list.Update();
                    b.Text = "<<";
                }
            }
            else
            {
                string trialName = isi_trial.Text;
                Trial trial = experimentDesigner.GetTrial(trialName);
                trial.SetISI(false);
                experimentDesigner.UpdateTrial(trialName, trial);
                experimentDesigner.SetISITrial("");
                trial_list.Items.Add(trialName);
                trial_list.Update();
                b.Text = ">>";
            }
            isi_trial.Text = experimentDesigner.GetISITrial();
        }
        private void copy_left_stimulus_Click(object sender, EventArgs e)
        {
            Stimulus stim = GetLeftStimulus();
            SetRightStimulus(stim);
        }

        // Callback functions for saving and loading experimental files
        private void save_configuration_Click(object sender, EventArgs e)
        {
            SaveFileDialog sf = new SaveFileDialog();
            sf.Filter = "Experiment configuration files (*.econ)|*.econ";

            DialogResult dr = sf.ShowDialog();
            if (dr == DialogResult.OK)
            {
                try
                {
                    experimentDesigner.SetBlockCount(Convert.ToInt32(block_count.Value));
                    experimentDesigner.SaveConfiguration(sf.FileName);
                }
                catch (Exception E)
                {
                    MessageBox.Show(E.Message);
                }
            }
        }
        private void load_configuration_Click(object sender, EventArgs e)
        {
            OpenFileDialog of = new OpenFileDialog();
            of.Filter = "Experiment configuration files (*.econ)|*.econ";

            DialogResult dr = of.ShowDialog();
            if (dr == DialogResult.OK)
            {
                try
                {
                    experimentDesigner.LoadConfiguration(of.FileName);

                    trial_list.Items.Clear();
                    set_isi_trial.Text = ">>";
                    string[] trialNames = experimentDesigner.GetTrialNames();
                    string isiTrialName = experimentDesigner.GetISITrial();

                    for (int trialIndex = 0; trialIndex < trialNames.Length; ++trialIndex)
                    {
                        if (trialNames[trialIndex] != isiTrialName)
                            trial_list.Items.Add(trialNames[trialIndex]);
                        else
                        {
                            isi_trial.Text = isiTrialName;
                            set_isi_trial.Text = "<<";
                        }
                    }

                    block_count.Value = experimentDesigner.GetBlockCount();
                    for (int trialIndex = 0; trialIndex < trialNames.Length; ++trialIndex)
                    {
                        if (trialNames[trialIndex] != isiTrialName)
                        {
                            SetGUITrial(experimentDesigner.GetTrial(trialNames[trialIndex]));
                            trial_list.SelectedItem = trialNames[trialIndex];
                            break;
                        }
                    }
                }
                catch (Exception E)
                {
                    MessageBox.Show(E.Message);
                }
            }
        }
        private void xml_from_config_Click(object sender, EventArgs e)
        {
            SaveFileDialog sf = new SaveFileDialog();
            sf.Filter = "Experiment XML files (*.exml)|*.exml";

            DialogResult dr = sf.ShowDialog();
            if (dr == DialogResult.OK)
            {
                try
                {
                    experimentDesigner.GenerateExperiment(sf.FileName);
                }
                catch (Exception E)
                {
                    MessageBox.Show(E.Message);
                }
            }
        }

        // Callback functions interacting with matlab server
        private void connect_button_Click(object sender, EventArgs e)
        {
            if (clientConnection == null)
            {
                matlabServerMessenger.AddMessage("Trying to connect to Matlab server...");
                clientConnection = new ClientConnection(ip_text.Text, Convert.ToInt32(port_text.Text));
                clientConnection.SetMessenger(matlabServerMessenger);
                if (clientConnection.IsConnected())
                {
                    matlabServerMessenger.AddMessage("Connected to Matlab server");
                    disconnect_button.Enabled = true;
                    connect_button.Enabled = false;
                    load_config_xml.Enabled = true;
                    load_custom_xml.Enabled = true;
                }
            }
        }
        private void disconnect_button_Click(object sender, EventArgs e)
        {
            clientConnection.Disconnect();
            disconnect_button.Enabled = false;
            connect_button.Enabled = true;
            load_config_xml.Enabled = false;
            load_custom_xml.Enabled = false;
            run_experiment.Enabled = false;
        }
        private void load_config_xml_Click(object sender, EventArgs e)
        { }
        private void load_custom_xml_Click(object sender, EventArgs e)
        {
            ofDialog.Filter = "Experiment xml (*.exml)|*.exml";
            DialogResult dr = ofDialog.ShowDialog();
            if (dr == DialogResult.OK)
            {
                if (clientConnection == null)
                {
                    matlabServerMessenger.AddMessage("Error: No connection with Matlab server!");
                    return;
                }
                bool loadSuccesful = clientConnection.LoadExperimentConfig(ofDialog.FileName);

                if (loadSuccesful)
                {
                    run_experiment.Enabled = true;
                    matlabServerMessenger.AddMessage("Configuration file " + Path.GetFileName(ofDialog.FileName) + " succesfully loaded, ready to run experiment");
                }
                else
                {
                    run_experiment.Enabled = false;
                    matlabServerMessenger.AddMessage("Unable to load " + Path.GetFileName(ofDialog.FileName));
                }
            }
        }
        private void run_experiment_Click(object sender, EventArgs e)
        {
            SaveFileDialog sf = new SaveFileDialog();
            sf.Filter = "Exeriment log files (*.elog)|*.elog";
            DialogResult dr = sf.ShowDialog();
            if (dr == DialogResult.OK)
            {
                clientConnection.RunExperiment(sf.FileName);
            }
        }

        // Helper functions for setting information in GUI elements
        private void SetGUITrial(Trial trial)
        {
            SetTransitionParameters(trial.GetTransitionParameters());
            SetRewardParameters(trial.GetRewardParameters());
            SetLeftStimulus(trial.GetLeftStimulus());
            SetRightStimulus(trial.GetRightStimulus());
            trial_name.Text = trial.GetName();
            block_presentations.Value = trial.GetBlockPresentations();
        }
        private void SetTransitionParameters(TransitionParameters tp)
        {
            if (tp.transitionType == TransitionType.Frame)
            {
                transition_frame.Checked = true;
                transition_frame_duration.Value = tp.frameDuration;
            }
            else
            {
                transition_velocity.Checked = true;
                transition_velocity_comparison.SelectedIndex = Convert.ToInt32(tp.lowerThan);
                transition_velocity_duration.Value = tp.velocityDuration;
                transition_velocity_threshold.Value = Convert.ToDecimal(tp.velocityThreshold);
            }
        }
        private void SetRewardParameters(RewardParameters rp)
        {
            if (rp.rewardType == RewardType.None)
            {
                reward_none.Checked = true;
            }
            else if (rp.rewardType == RewardType.Random)
            {
                reward_random.Checked = true;
                reward_random_count.Value = rp.rewardCount;
                reward_random_total.Value = rp.rewardTotal;
            }
            else
            {
                reward_velocity.Checked = true;
                reward_velocity_comparison.SelectedIndex = Convert.ToInt32(rp.lowerThan);
                reward_velocity_duration.Value = rp.velocityDuration;
                reward_velocity_threshold.Value = Convert.ToDecimal(rp.velocityThreshold);

            }
        }
        private void SetLeftStimulus(Stimulus stim)
        {
            left_stimulus.ImageLocation = stim.GetFilename();
            left_stimulus_top.Text = stim.GetTop().ToString();
            left_stimulus_left.Text = stim.GetLeft().ToString();
            left_stimulus_width.Text = stim.GetWidth().ToString();
            left_stimulus_height.Text = stim.GetHeight().ToString();
            left_stimulus_name.Text = stim.GetName();
        }
        private void SetRightStimulus(Stimulus stim)
        {
            right_stimulus.ImageLocation = stim.GetFilename();
            right_stimulus_top.Text = stim.GetTop().ToString();
            right_stimulus_left.Text = stim.GetLeft().ToString();
            right_stimulus_width.Text = stim.GetWidth().ToString();
            right_stimulus_height.Text = stim.GetHeight().ToString();
            right_stimulus_name.Text = stim.GetName();
        }


        // Helper functions for getting information from GUI elements
        private Trial GetGUITrial()
        {
            Trial trial = new Trial(trial_name.Text);
            TransitionParameters tp = GetTransitionParameters();
            RewardParameters rp = GetRewardParameters();

            Stimulus leftStimulus = GetLeftStimulus();
            Stimulus rightStimulus = GetRightStimulus();

            trial.SetBlockPresentations(Convert.ToInt32(block_presentations.Value));
            trial.SetRewardParameters(rp);
            trial.SetTransitionParameters(tp);
            trial.SetStimulus(leftStimulus, rightStimulus);
            return trial;
        }
        private TransitionParameters GetTransitionParameters()
        {
            TransitionParameters tp = new TransitionParameters();
            if (transition_frame.Checked)
            {
                tp.transitionType = TransitionType.Frame;
                tp.frameDuration = Convert.ToInt32(transition_frame_duration.Value);
            }
            else
            {
                tp.transitionType = TransitionType.Velocity;
                tp.lowerThan = (transition_velocity_comparison.SelectedIndex == 1);
                tp.velocityThreshold = Convert.ToDouble(transition_velocity_threshold.Value);
                tp.velocityDuration = Convert.ToInt32(transition_velocity_duration.Value);
            }
            return tp;
        }
        private RewardParameters GetRewardParameters()
        {
            RewardParameters rp = new RewardParameters();
            if (reward_none.Checked)
            {
                rp.rewardType = RewardType.None;
            }
            else if (reward_random.Checked)
            {
                rp.rewardType = RewardType.Random;
                rp.rewardCount = System.Convert.ToInt32(reward_random_count.Value);
                rp.rewardTotal = System.Convert.ToInt32(reward_random_total.Value);
            }
            else
            {
                rp.rewardType = RewardType.Velocity;
                rp.lowerThan = (reward_velocity_comparison.SelectedIndex == 1);
                rp.velocityThreshold = Convert.ToDouble(reward_velocity_threshold.Value);
                rp.velocityDuration = Convert.ToInt32(reward_velocity_duration.Value);
            }
            return rp;
        }
        private Stimulus GetLeftStimulus()
        {
            Stimulus stim = new Stimulus();
            stim.SetImage(left_stimulus.ImageLocation);
            stim.SetWidth(Convert.ToInt32(left_stimulus_width.Text));
            stim.SetHeight(Convert.ToInt32(left_stimulus_height.Text));
            stim.SetTop(Convert.ToInt32(left_stimulus_top.Text));
            stim.SetLeft(Convert.ToInt32(left_stimulus_left.Text));
            stim.SetStimulusName(left_stimulus_name.Text);
            return stim;
        }
        private Stimulus GetRightStimulus()
        {
            Stimulus stim = new Stimulus();
            stim.SetImage(right_stimulus.ImageLocation);
            stim.SetWidth(Convert.ToInt32(right_stimulus_width.Text));
            stim.SetHeight(Convert.ToInt32(right_stimulus_height.Text));
            stim.SetTop(Convert.ToInt32(right_stimulus_top.Text));
            stim.SetLeft(Convert.ToInt32(right_stimulus_left.Text));
            stim.SetStimulusName(right_stimulus_name.Text);
            return stim;
        }

        // Drawing functions for receptive field mapper
        int rfX = 0;
        int rfY = 0;
        private void rf_grating_position_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            Brush b = new SolidBrush(Color.Red);
            Pen p = new Pen(b);
            g.DrawEllipse(p, rfX, rfY, 2, 2);
        }
        private void rf_grating_position_MouseMove(object sender, MouseEventArgs e)
        {
            Label rfLabel = (Label)sender;

            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                rfX = e.X;
                rfY = e.Y;

                if ((rfX > 0) && (rfX < rfLabel.Width) && (rfY > 0) && (rfY < rfLabel.Height))
                {
                    grating_center_x.Text = rfX.ToString();
                    grating_center_y.Text = rfY.ToString();
                    rf_grating_position.Invalidate();

                    clientConnection.UpdateRFData(rfX*  10/2, rfY*  10, 0, 0);
                }
            }
        }
        private void start_manual_mapping_Click(object sender, EventArgs e)
        {
            Button b = (Button)sender;
            if (Convert.ToInt16(b.Tag) == 0)
            {
                rfX = rf_grating_position.Width / 2;
                rfY = rf_grating_position.Height / 2;
                clientConnection.UpdateRFData(rfX * 10, rfY * 10 / 2, 0, 0);

                clientConnection.StartMapping();
                b.Text = "Stop manual mapping";
                b.Tag = 1;
            }
            else
            {
                clientConnection.StopMapping();
                b.Text = "Start manual mapping";
                b.Tag = 0;
            }

        }

        private void test_psth_Click(object sender, EventArgs e)
        {
            PSTHManager psthManager = new PSTHManager();
            psthManager.Initialize(32, 1000, new string[]{"isi","go","nogo","abb","bab"});
            psthManager.Show();
        }

        // Callbac functions for menu items
        private void showToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PSTHManager psthManager = new PSTHManager();
            psthManager.Initialize(32, 1000, new string[] { "isi", "go", "nogo", "abb", "bab" });
            psthManager.Show();
        }


    }
}
