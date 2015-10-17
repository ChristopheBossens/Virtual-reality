using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Xml;

namespace vrClient
{
    class ExperimentDesigner
    {
        int blockCount;
        string isiTrialName;
        string currentFileName = "";
        Dictionary<string, Trial> trialDictionary;

        public ExperimentDesigner()
        {
            trialDictionary = new Dictionary<string, Trial>();
        }

        // Interface with the trial dictionary
        public bool NameAvailable(string name) { return (!trialDictionary.ContainsKey(name));}
        public void AddTrial(Trial trial)
        {
            if (!trialDictionary.ContainsKey(trial.GetName()))
                trialDictionary.Add(trial.GetName(), trial);
        }
        public Trial GetTrial(string name)
        {
            if (trialDictionary.ContainsKey(name))
                return trialDictionary[name];
            else
                return null;
        }
        public void UpdateTrial(string originalName, Trial trial)
        {           
            Trial originalTrial = GetTrial(originalName);
            trialDictionary.Remove(originalName);
            AddTrial(trial);
        }
        public void RemoveTrial(string name)
        {
            if (trialDictionary.ContainsKey(name))
                trialDictionary.Remove(name);
        }

        // Functions for getting stimulus image data
        public string[] GetImageLocations()
        {
            List<string> imageLocationList = new List<string>(trialDictionary.Count*2);

            foreach (var pair in trialDictionary)
            {
                string leftStimulusLocation = pair.Value.GetLeftStimulus().GetFilename();
                string rightStimulusLocation = pair.Value.GetRightStimulus().GetFilename();

                if (leftStimulusLocation == rightStimulusLocation)
                {
                    if (!imageLocationList.Contains(leftStimulusLocation))
                        imageLocationList.Add(leftStimulusLocation);
                }
                else
                {
                    if (!imageLocationList.Contains(leftStimulusLocation))
                        imageLocationList.Add(leftStimulusLocation);
                    if (!imageLocationList.Contains(rightStimulusLocation))
                        imageLocationList.Add(rightStimulusLocation);
                }
            }
            return imageLocationList.ToArray();
        }

        // Functions for accessing class properties
        public string[] GetTrialNames()
        {
            string[] nameList = new string[trialDictionary.Count];
            int dictIndex = 0;

            foreach (var pair in trialDictionary)
            {
                nameList[dictIndex] = pair.Key;
                ++dictIndex;
            }

            return nameList;
        }
        public string GetCurrentFilename() { return currentFileName; }
        public string GetISITrial() { return this.isiTrialName;}
        public void SetISITrial(string s) {this.isiTrialName = s;}
        public void SetBlockCount(int n) { this.blockCount = n; }
        public int GetBlockCount() { return this.blockCount; }
        
        // Functions for exporting and importing xml files
        public void SaveConfiguration(string filename)
        {
            XmlWriter xmlWriter = XmlWriter.Create(filename);

            xmlWriter.WriteStartDocument();

            xmlWriter.WriteStartElement("experiment");

            xmlWriter.WriteStartElement("blocks");
            xmlWriter.WriteString(blockCount.ToString());
            xmlWriter.WriteEndElement();

            foreach ( var pair in trialDictionary)
            {
                xmlWriter.WriteStartElement("trial");

                xmlWriter.WriteStartElement("name");
                xmlWriter.WriteString(pair.Value.GetName());
                xmlWriter.WriteEndElement();

                xmlWriter.WriteStartElement("blockPresentations");
                xmlWriter.WriteString(pair.Value.GetBlockPresentations().ToString());
                xmlWriter.WriteEndElement();

                xmlWriter.WriteStartElement("isi");
                if (pair.Value.GetISI())
                    xmlWriter.WriteString("true");
                else
                    xmlWriter.WriteString("false");
                xmlWriter.WriteEndElement();

                XMLWriteRewardParameters(xmlWriter, pair.Value.GetRewardParameters());
                XMLWriteTransitionParameters(xmlWriter, pair.Value.GetTransitionParameters());
                XMLWriteStimulusParameters(xmlWriter, pair.Value.GetLeftStimulus(), "left");
                XMLWriteStimulusParameters(xmlWriter, pair.Value.GetRightStimulus(), "right");

                xmlWriter.WriteEndElement();
            }

            xmlWriter.WriteEndElement();
            xmlWriter.WriteEndDocument();
            xmlWriter.Close();
            currentFileName = filename;
        }
        public void LoadConfiguration(string filename)
        {
            Trial trial;
            string isiTrialName = "";
            Dictionary<string, Trial> newDictionary = new Dictionary<string, Trial>();

            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(filename);
            XmlNodeList trialList = xmlDoc.GetElementsByTagName("trial");
            foreach (XmlNode xmlNode in trialList)
            {
                bool isi;
                string trialName;
                int blockPresentations;
                RewardParameters rp = new RewardParameters();
                TransitionParameters tp = new TransitionParameters();
                Stimulus left = new Stimulus();
                Stimulus right = new Stimulus();


                // General trial info
                XmlElement trialElement = xmlNode as XmlElement;
                XmlElement xTrialName = (XmlElement)trialElement.GetElementsByTagName("name")[0];
                trialName = ((XmlElement)trialElement.GetElementsByTagName("name")[0]).InnerText;
                if (((XmlElement)trialElement.GetElementsByTagName("isi")[0]).InnerText == "true")
                {
                    isi = true;
                    isiTrialName = trialName;
                }
                else
                    isi = false;
                blockPresentations = Convert.ToInt32(((XmlElement)trialElement.GetElementsByTagName("blockPresentations")[0]).InnerText);

                // Reward parameters
                XmlElement xRewardParameters = (XmlElement)trialElement.GetElementsByTagName("reward")[0];
                rp = XMLReadRewardParameters(xRewardParameters);

                // Transition parameters
                XmlElement xTransitionParameters = (XmlElement)trialElement.GetElementsByTagName("transition")[0];
                tp = XMLReadTransitionParameters(xTransitionParameters);

                // Stimulus parameters
                XmlNodeList xNodes = trialElement.GetElementsByTagName("stimulus");
                XmlElement stimulus = (XmlElement)(trialElement.GetElementsByTagName("stimulus")[0]);
                if (stimulus.GetAttribute("screen") == "left")
                    left = XMLReadStimulusParameters(stimulus);
                else
                    right = XMLReadStimulusParameters(stimulus);

                stimulus = (XmlElement)(trialElement.GetElementsByTagName("stimulus")[1]);
                if (stimulus.GetAttribute("screen") == "right")
                    right = XMLReadStimulusParameters(stimulus);
                else
                    left = XMLReadStimulusParameters(stimulus);

                trial = new Trial(trialName);
                trial.SetISI(isi);
                trial.SetRewardParameters(rp);
                trial.SetTransitionParameters(tp);
                trial.SetStimulus(left, right);
                trial.SetBlockPresentations(blockPresentations);
                newDictionary.Add(trialName, trial);
            }
            int blockCount = Convert.ToInt32(xmlDoc.GetElementsByTagName("blocks")[0].FirstChild.InnerText);

            this.trialDictionary = new Dictionary<string, Trial>(newDictionary);
            this.blockCount = blockCount;
            this.isiTrialName = isiTrialName;
            currentFileName = filename;
        }
        public void GenerateExperiment(string filename)
        {
            // Prepare XML file
            XmlWriter xmlWriter = XmlWriter.Create(filename);
            xmlWriter.WriteStartDocument();
            xmlWriter.WriteStartElement("experiment");


            // Prepare a stimulus list dictionary with unique stimuli used in the experiment
            Dictionary<string, string> stimulusDictionary = GenerateStimulusDictionary();
            Dictionary<string, int> trialCount = GenerateTrialCount();
            XMLWriteStimulusList(xmlWriter, stimulusDictionary);

            // Generate trial order
            List<string> blockOrder = GenerateBlockList(trialCount);
            XMLWriteTrialList(xmlWriter, blockOrder);

            // Wrap up XML file
            xmlWriter.WriteEndElement();
            xmlWriter.WriteEndDocument();
            xmlWriter.Close();
        }

        // Get a dictionary with unique stimulus locations
        public Dictionary<string,string> GenerateStimulusDictionary()
        {
            Dictionary<string, string> stimulusDictionary = new Dictionary<string, string>();
            foreach (var trial in trialDictionary)
            {
                string leftStimulusName = trial.Value.GetLeftStimulusName();
                string rightStimulusName = trial.Value.GetRightStimulusName();

                if (!stimulusDictionary.ContainsKey(leftStimulusName))
                    stimulusDictionary.Add(leftStimulusName, trial.Value.GetLeftStimulusLocation());
                if (!stimulusDictionary.ContainsKey(rightStimulusName))
                    stimulusDictionary.Add(rightStimulusName, trial.Value.GetRightStimulusLocation());
            }

            return stimulusDictionary;
        }
        public Dictionary<string, int> GenerateTrialCount()
        {
            Dictionary<string, int> trialCount = new Dictionary<string,int>();
            foreach ( var trial in trialDictionary)
                trialCount[trial.Value.GetName()] = trial.Value.GetBlockPresentations();
            return trialCount;
        }
        public List<string> GenerateBlockList(Dictionary<string,int> trialCount)
        {
            List<string> blockOrder = new List<string>();
            foreach(var value in trialCount)
            {
                if (value.Key != isiTrialName)
                {
                    for (int i = 0; i < value.Value;++i)
                        blockOrder.Add(value.Key);
                }
            }
            return blockOrder;
        }
        public List<string> ShuffleBlockList(List<string> blockOrder)
        {
            Random rnd = new Random();
            for (int i = 0; i < blockOrder.Count; ++i)
            {
                string a = blockOrder[i];
                int swapIndex = rnd.Next(1, blockOrder.Count);
                blockOrder[i] = blockOrder[swapIndex];
                blockOrder[swapIndex] = a;
            }
            return blockOrder;
        }

        // Helper functions for writing to xml files
        public void XMLWriteStimulusList(XmlWriter xmlWriter, Dictionary<string, string> stimulusDictionary)
        {
            xmlWriter.WriteStartElement("stimulusList");
            int stimulusIndex = 1;
            foreach (var stimulus in stimulusDictionary)
            {
                xmlWriter.WriteStartElement("image");
                xmlWriter.WriteAttributeString("id", stimulusIndex.ToString());
                
                xmlWriter.WriteStartElement("name");
                xmlWriter.WriteString(stimulus.Key);
                xmlWriter.WriteEndElement();

                xmlWriter.WriteStartElement("location");
                xmlWriter.WriteString(stimulus.Value);
                xmlWriter.WriteEndElement();

                xmlWriter.WriteEndElement();
                stimulusIndex++;
            }
            xmlWriter.WriteEndElement();
        }
        public void XMLWriteTrialList(XmlWriter xmlWriter, List<string> blockOrder)
        {
            xmlWriter.WriteStartElement("trialList");

            int trialIndex = 1;
            for (int blockIndex = 0; blockIndex < blockCount; ++blockIndex)
            {
                blockOrder = ShuffleBlockList(blockOrder);
                foreach (var trial in blockOrder)
                {
                    if( trialDictionary.ContainsKey(isiTrialName))
                    {
                        XMLWriteTrial(xmlWriter, trialDictionary[isiTrialName], trialIndex);
                        ++trialIndex;
                    }
                    XMLWriteTrial(xmlWriter, trialDictionary[trial], trialIndex);
                    ++trialIndex;
                }
            }
            xmlWriter.WriteEndElement();
        }
        public void XMLWriteTrial(XmlWriter xmlWriter, Trial trial, int id)
        {
            xmlWriter.WriteStartElement("trial");
            xmlWriter.WriteAttributeString("id",id.ToString());
            xmlWriter.WriteAttributeString("name",trial.GetName());

            XMLWriteRewardParameters(xmlWriter, trial.GetRewardParameters());
            XMLWriteTransitionParameters(xmlWriter, trial.GetTransitionParameters());
            XMLWriteStimulusParameters(xmlWriter, trial.GetLeftStimulus(), "left");
            XMLWriteStimulusParameters(xmlWriter, trial.GetLeftStimulus(), "right");

            xmlWriter.WriteEndElement();
        }

        public void XMLWriteRewardParameters(XmlWriter xmlWriter, RewardParameters rp)
        {
            xmlWriter.WriteStartElement("reward");
            switch (rp.rewardType)
            {
                case RewardType.None:
                    xmlWriter.WriteAttributeString("type", "none");
                    break;
                case RewardType.Random:
                    xmlWriter.WriteAttributeString("type", "random");
                    xmlWriter.WriteStartElement("count");
                    xmlWriter.WriteString(rp.rewardCount.ToString());
                    xmlWriter.WriteEndElement();
                    xmlWriter.WriteStartElement("total");
                    xmlWriter.WriteString(rp.rewardTotal.ToString());
                    xmlWriter.WriteEndElement();
                    break;
                case RewardType.Velocity:
                    xmlWriter.WriteAttributeString("type", "velocity");
                    xmlWriter.WriteStartElement("threshold");
                    xmlWriter.WriteString(rp.velocityThreshold.ToString());
                    xmlWriter.WriteEndElement();
                    xmlWriter.WriteStartElement("duration");
                    xmlWriter.WriteString(rp.velocityDuration.ToString());
                    xmlWriter.WriteEndElement();
                    xmlWriter.WriteStartElement("thresholdComparison");
                    if (rp.lowerThan)
                        xmlWriter.WriteString("lower");
                    else
                        xmlWriter.WriteString("higher");
                    xmlWriter.WriteEndElement();
                    break;
            }
            xmlWriter.WriteEndElement();
        }
        public void XMLWriteTransitionParameters(XmlWriter xmlWriter, TransitionParameters tp)
        {
            xmlWriter.WriteStartElement("transition");
            switch (tp.transitionType)
            {
                case TransitionType.Frame:
                    xmlWriter.WriteAttributeString("type", "duration");
                    xmlWriter.WriteStartElement("duration");
                    xmlWriter.WriteString(tp.frameDuration.ToString());
                    xmlWriter.WriteEndElement();
                    break;
                case TransitionType.Velocity:
                    xmlWriter.WriteAttributeString("type", "velocity");
                    xmlWriter.WriteStartElement("threshold");
                    xmlWriter.WriteString(tp.velocityThreshold.ToString());
                    xmlWriter.WriteEndElement();

                    xmlWriter.WriteStartElement("duration");
                    xmlWriter.WriteString(tp.velocityDuration.ToString());
                    xmlWriter.WriteEndElement();

                    xmlWriter.WriteStartElement("thresholdComparison");
                    if (tp.lowerThan)
                        xmlWriter.WriteString("lower");
                    else
                        xmlWriter.WriteString("higher");
                    xmlWriter.WriteEndElement();
                    break;
            }
            xmlWriter.WriteEndElement();
        }
        public void XMLWriteStimulusParameters(XmlWriter xmlWriter, Stimulus stim, string screen)
        {
            xmlWriter.WriteStartElement("stimulus");
            xmlWriter.WriteAttributeString("screen", screen);
            xmlWriter.WriteStartElement("name");
            xmlWriter.WriteString(stim.GetName());
            xmlWriter.WriteEndElement();
            xmlWriter.WriteStartElement("location");
            xmlWriter.WriteString(stim.GetFilename());
            xmlWriter.WriteEndElement();
            xmlWriter.WriteStartElement("width");
            xmlWriter.WriteString(stim.GetWidth().ToString());
            xmlWriter.WriteEndElement();
            xmlWriter.WriteStartElement("height");
            xmlWriter.WriteString(stim.GetHeight().ToString());
            xmlWriter.WriteEndElement();
            xmlWriter.WriteStartElement("top");
            xmlWriter.WriteString(stim.GetTop().ToString());
            xmlWriter.WriteEndElement();
            xmlWriter.WriteStartElement("left");
            xmlWriter.WriteString(stim.GetLeft().ToString());
            xmlWriter.WriteEndElement();
            xmlWriter.WriteEndElement();
        }

        // Helper functions for reading from xml files
        private Stimulus XMLReadStimulusParameters(XmlElement xmlStimulus)
        {
            Stimulus stim = new Stimulus();

            int top = Convert.ToInt32(((XmlElement)xmlStimulus.GetElementsByTagName("top")[0]).InnerText);
            int left = Convert.ToInt32(((XmlElement)xmlStimulus.GetElementsByTagName("left")[0]).InnerText);
            int height = Convert.ToInt32(((XmlElement)xmlStimulus.GetElementsByTagName("height")[0]).InnerText);
            int width = Convert.ToInt32(((XmlElement)xmlStimulus.GetElementsByTagName("width")[0]).InnerText);
            string filename = ((XmlElement)xmlStimulus.GetElementsByTagName("location")[0]).InnerText;
            string name = ((XmlElement)xmlStimulus.GetElementsByTagName("name")[0]).InnerText;

            stim.SetStimulusName(name);
            stim.SetFilename(filename);
            stim.SetTop(top);
            stim.SetLeft(left);
            stim.SetHeight(height);
            stim.SetWidth(width);

            return stim;
        }
        private RewardParameters XMLReadRewardParameters(XmlElement xmlRewardParameters)
        {
            RewardParameters rp = new RewardParameters();
            string rewardType = xmlRewardParameters.GetAttribute("type");

            switch (rewardType)
            {
                case "velocity":
                    rp.rewardType = RewardType.Velocity;
                    rp.velocityDuration = Convert.ToInt32(((XmlElement)xmlRewardParameters.GetElementsByTagName("duration")[0]).InnerText);
                    rp.velocityThreshold = Convert.ToDouble(((XmlElement)xmlRewardParameters.GetElementsByTagName("threshold")[0]).InnerText);
                    if (((XmlElement)xmlRewardParameters.GetElementsByTagName("thresholdComparison")[0]).InnerText == "lower")
                        rp.lowerThan = true;
                    else
                        rp.lowerThan = false;
                    break;

                case "none":
                    rp.rewardType = RewardType.None;
                    break;

                case "random":
                    rp.rewardType = RewardType.Random;
                    rp.rewardCount = Convert.ToInt32(((XmlElement)xmlRewardParameters.GetElementsByTagName("count")[0]).InnerText);
                    rp.rewardTotal = Convert.ToInt32(((XmlElement)xmlRewardParameters.GetElementsByTagName("total")[0]).InnerText);
                    break;
            }

            return rp;
        }
        private TransitionParameters XMLReadTransitionParameters(XmlElement xmlTransitionParameters)
        {
            TransitionParameters tp = new TransitionParameters();
            string transitionType = xmlTransitionParameters.GetAttribute("type");

            switch (transitionType)
            {
                case "velocity":
                    tp.transitionType = TransitionType.Velocity;
                    tp.velocityDuration = Convert.ToInt32(((XmlElement)xmlTransitionParameters.GetElementsByTagName("duration")[0]).InnerText);
                    tp.velocityThreshold = Convert.ToDouble(((XmlElement)xmlTransitionParameters.GetElementsByTagName("threshold")[0]).InnerText);
                    if (((XmlElement)xmlTransitionParameters.GetElementsByTagName("thresholdComparison")[0]).InnerText == "lower")
                        tp.lowerThan = true;
                    else
                        tp.lowerThan = false;
                    break;
                case "duration":
                    tp.transitionType = TransitionType.Frame;
                    tp.frameDuration = Convert.ToInt32(((XmlElement)xmlTransitionParameters.GetElementsByTagName("duration")[0]).InnerText);
                    break;
            }

            return tp;
        }
    }
}
