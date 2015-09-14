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

        public main_form()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            reward_velocity_comparison.SelectedIndex = 0;
            transition_velocity_comparison.SelectedIndex = 0;

            experimentDesigner = new ExperimentDesigner();
        }

        // Callback functions for designing experiments
        private void connect_button_Click(object sender, EventArgs e)
        {
            clientConnection = new ClientConnection(ip_text.Text, Convert.ToInt32(port_text.Text));
        }
        private void send_command_Click(object sender, EventArgs e)
        {
            string[] configFiles;
            int count;
            if (clientConnection != null)
            {
                clientConnection.RequestConfigFiles(out count, out configFiles);
                if (count > 0)
                {
                    for (int i = 0; i < count; ++i)
                        config_files.Items.Add(configFiles[i]);
                    config_files.Update();
                }
            }
        }
        private void load_left_stimulus_Click(object sender, EventArgs e)
        {
            ofDialog.Filter = "PNG Files (*.png)|*.png";

            DialogResult dr = ofDialog.ShowDialog();
            if (dr == DialogResult.OK)
            {
                left_stimulus.ImageLocation=ofDialog.FileName;
                Bitmap bmp = new Bitmap(ofDialog.FileName);
                left_stimulus_width.Text = bmp.Width.ToString();
                left_stimulus_height.Text = bmp.Height.ToString();
                left_stimulus_top.Text = "0";
                left_stimulus_left.Text = "0";
            }
        }
        private void test_sendstimulus_Click(object sender, EventArgs e)
        {
            clientConnection.SendImage(left_stimulus.Image, 0);
        }
        private void load_right_stimulus_Click(object sender, EventArgs e)
        {
            ofDialog.Filter = "PNG Files (*.png)|*.png";

            DialogResult dr = ofDialog.ShowDialog();
            if (dr == DialogResult.OK)
            {
                right_stimulus.ImageLocation = ofDialog.FileName;
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
        }
        private void update_trial_Click(object sender, EventArgs e)
        {
            if (experimentDesigner.NameAvailable(trial_name.Text))
            {
                try
                {
                    int selectedTrial = trial_list.SelectedIndex;
                    string selectedName = trial_list.SelectedItem.ToString();

                    if (selectedName != trial_name.Text)
                    { 
                        if (!(experimentDesigner.NameAvailable(trial_name.Text)))
                            return;
                    }

                    experimentDesigner.UpdateTrial(selectedName, GetGUITrial());

                    trial_list.Items[selectedTrial] = trial_name.Text;
                }
                catch (Exception exception)
                {
                    MessageBox.Show(exception.Message);
                }
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
        private void generate_xml_Click(object sender, EventArgs e)
        {

        }

        // Callback functions for saving, loading and uploading experiments
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
        private void upload_configuration_Click(object sender, EventArgs e)
        {
            string currentFilename = experimentDesigner.GetCurrentFilename();
            string configName = Path.GetFileNameWithoutExtension(currentFilename);

            if (currentFilename != "")
            {
                // Check if a configuration file with the same name is already available
                int count;
                string[] configFiles;
                clientConnection.RequestConfigFiles(out count, out configFiles);

                if (count > 0)
                {
                    for (int i = 0; i < count;++i)
                    {
                        if (configName == configFiles[i])
                        {
                            MessageBox.Show("A configuration file with name " + configName + " already exists!");
                            return;
                        }
                    }
                }

                // Start config creation by 
                clientConnection.UploadConfigFile(currentFilename);  
                clientConnection.SendConfigImage(configName,experimentDesigner.GetImageLocations());
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

                    string[] trialNames = experimentDesigner.GetTrialNames();
                    string isiTrialName = experimentDesigner.GetISITrial();

                    for (int trialIndex = 0; trialIndex < trialNames.Length; ++trialIndex)
                    {
                        if (trialNames[trialIndex] != isiTrialName)
                            trial_list.Items.Add(trialNames[trialIndex]);
                        else
                            isi_trial.Text = isiTrialName;
                    }

                    block_count.Value = experimentDesigner.GetBlockCount();
                    for (int trialIndex = 0; trialIndex < trialNames.Length; ++trialIndex)
                    {
                        if (trialNames[trialIndex] != isiTrialName)
                        {
                            SetGUITrial(experimentDesigner.GetTrial(trialNames[trialIndex]));
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

        // Helper functions for getting information from and into GUI elements
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
        }
        private void SetRightStimulus(Stimulus stim)
        {
            right_stimulus.ImageLocation = stim.GetFilename();
            right_stimulus_top.Text = stim.GetTop().ToString();
            right_stimulus_left.Text = stim.GetLeft().ToString();
            right_stimulus_width.Text = stim.GetWidth().ToString();
            right_stimulus_height.Text = stim.GetHeight().ToString();
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
                tp.lowerThan = (transition_velocity_comparison.SelectedIndex == 0);
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
                rp.lowerThan = (reward_velocity_comparison.SelectedIndex == 0);
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
            return stim;
        }
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
        private void SetGUITrial(Trial trial)
        {
            SetTransitionParameters(trial.GetTransitionParameters());
            SetRewardParameters(trial.GetRewardParameters());
            SetLeftStimulus(trial.GetLeftStimulus());
            SetRightStimulus(trial.GetRightStimulus());
            trial_name.Text = trial.GetName();
            block_presentations.Value = trial.GetBlockPresentations();
        }

        
        
    }
}
