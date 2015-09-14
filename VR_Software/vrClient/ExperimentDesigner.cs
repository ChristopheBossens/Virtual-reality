using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Xml;

namespace vrClient
{
    public struct TransitionParameters
    {
        public TransitionType transitionType;
        public int frameDuration;
        public bool lowerThan;
        public double velocityThreshold;
        public int velocityDuration;
    };

    public struct RewardParameters
    {
        public RewardType rewardType;
        public int rewardCount, rewardTotal;
        public bool lowerThan;
        public double velocityThreshold;
        public int velocityDuration;
    };

    public enum TransitionType
    {
        Frame,
        Velocity
    };

    public enum RewardType
    {
        None,
        Random,
        Velocity
    };

    class Stimulus
    {
        string filename;

        int width;
        int height;
        int top;
        int left;

        public void SetImage(string filename)
        {
            this.filename = filename;
        }

        public void SetPosition(int top, int left)
        {
            this.top = top;
            this.left = left;
        }

        public int GetWidth() { return this.width; }
        public int GetHeight(){ return this.height;}
        public int GetTop() { return top; }
        public int GetLeft(){ return left; }
        public string GetFilename(){ return filename; }

        public void SetWidth(int w) { this.width = w; }
        public void SetHeight(int h) { this.height = h; }
        public void SetTop(int t) { this.top = t; }
        public void SetLeft(int l) { this.left = l; }
        public void SetFilename(string s) { this.filename = s; }
    }

    class Trial
    {
        string name;

        TransitionParameters tp;
        RewardParameters rp;

        Stimulus leftStimulus;
        Stimulus rightStimulus;

        int blockPresentations;
        bool isiTrial;

        public Trial(string name)
        {   this.name = name; }
        public void SetTransitionParameters(TransitionParameters t)
        {   this.tp = t; }
        public TransitionParameters GetTransitionParameters()
        { return this.tp; }
        public void SetRewardParameters(RewardParameters r)
        {   this.rp = r;}
        public RewardParameters GetRewardParameters()
        { return this.rp; }
        public Stimulus GetLeftStimulus()
        { return this.leftStimulus; }
        public Stimulus GetRightStimulus()
        { return this.rightStimulus; }
        public void SetStimulus(Stimulus left, Stimulus right)
        {
            this.leftStimulus = left;
            this.rightStimulus = right;
        }

        public void SetBlockPresentations(int n)
        { this.blockPresentations = n; }
        public int GetBlockPresentations()
        { return this.blockPresentations; }
        public void SetName(string s)
        { this.name = s;}
        public string GetName()
        { return name; }
        public void SetISI(bool b)
        { this.isiTrial = b; }
        public bool GetISI()
        { return this.isiTrial; }
    }

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

        // Functions for managing the trial dictionary
        public bool NameAvailable(string name)
        {
            if (trialDictionary.ContainsKey(name))
                return false;
            else 
                return true;
        }
        public void AddTrial(Trial trial)
        {
            if (!trialDictionary.ContainsKey(trial.GetName()))
                trialDictionary.Add(trial.GetName(), trial);
        }
        public void UpdateTrial(string name, Trial trial)
        {
            if (trialDictionary.ContainsKey(name))
                trialDictionary[name] = trial;
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
        public string GetISITrial()
        {
            return this.isiTrialName;
        }
        public void SetISITrial(string s)
        {
            this.isiTrialName = s;
        }
        public Trial GetTrial(string name)
        {
            if (trialDictionary.ContainsKey(name))
                return trialDictionary[name];
            else
                return null;
        }
        public void SetBlockCount(int n)
        { this.blockCount = n; }
        public int GetBlockCount()
        { return this.blockCount; }
        
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

                RewardParameters rp = pair.Value.GetRewardParameters();
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


                TransitionParameters tp = pair.Value.GetTransitionParameters();
                xmlWriter.WriteStartElement("transition");
                switch (tp.transitionType)
                {
                    case TransitionType.Frame:
                        xmlWriter.WriteAttributeString("type", "duration");
                        xmlWriter.WriteStartElement("duration");
                        xmlWriter.WriteString(tp.frameDuration.ToString());
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


                Stimulus stim = pair.Value.GetLeftStimulus();
                xmlWriter.WriteStartElement("stimulus");
                xmlWriter.WriteAttributeString("screen", "left");
                xmlWriter.WriteStartElement("name");
                xmlWriter.WriteString("test");
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

                stim = pair.Value.GetRightStimulus();
                xmlWriter.WriteStartElement("stimulus");
                xmlWriter.WriteAttributeString("screen", "right");
                xmlWriter.WriteStartElement("name");
                xmlWriter.WriteString("test");
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
                string rewardType;
                string transitionType;
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
                rewardType = xRewardParameters.GetAttribute("type");

                switch (rewardType)
                {
                    case "velocity":
                        rp.rewardType = RewardType.Velocity;
                        rp.velocityDuration = Convert.ToInt32(((XmlElement)xRewardParameters.GetElementsByTagName("duration")[0]).InnerText);
                        rp.velocityThreshold = Convert.ToDouble(((XmlElement)xRewardParameters.GetElementsByTagName("threshold")[0]).InnerText);
                        if (((XmlElement)xRewardParameters.GetElementsByTagName("thresholdComparison")[0]).InnerText == "lower")
                            rp.lowerThan = true;
                        else
                            rp.lowerThan = false;
                        break;

                    case "none":
                        rp.rewardType = RewardType.None;
                        break;
                    
                    case "random":
                        rp.rewardType = RewardType.Random;
                        rp.rewardCount = Convert.ToInt32(((XmlElement)xRewardParameters.GetElementsByTagName("count")[0]).InnerText);
                        rp.rewardTotal = Convert.ToInt32(((XmlElement)xRewardParameters.GetElementsByTagName("total")[0]).InnerText);
                        break;
                }

                // Transition parameters
                XmlElement xTransitionParameters = (XmlElement)trialElement.GetElementsByTagName("transition")[0];
                transitionType = xTransitionParameters.GetAttribute("type");

                switch (transitionType)
                {
                    case "velocity":
                        tp.transitionType = TransitionType.Velocity;
                        rp.velocityDuration = Convert.ToInt32(((XmlElement)xTransitionParameters.GetElementsByTagName("duration")[0]).InnerText);
                        rp.velocityThreshold = Convert.ToDouble(((XmlElement)xTransitionParameters.GetElementsByTagName("threshold")[0]).InnerText);
                        if (((XmlElement)xTransitionParameters.GetElementsByTagName("thresholdComparison")[0]).Value == "lower")
                            rp.lowerThan = true;
                        else
                            rp.lowerThan = false;
                        break;
                    case "duration":
                        tp.transitionType = TransitionType.Frame;
                        tp.frameDuration = Convert.ToInt32(((XmlElement)xTransitionParameters.GetElementsByTagName("duration")[0]).InnerText);
                        break;
                }

                // Stimulus parameters
                XmlNodeList xNodes = trialElement.GetElementsByTagName("stimulus");

                XmlElement stimulus = (XmlElement)(trialElement.GetElementsByTagName("stimulus")[0]);
                if (stimulus.GetAttribute("screen") == "left")
                    left = ParseStimulus(stimulus);
                else
                    right = ParseStimulus(stimulus);

                stimulus = (XmlElement)(trialElement.GetElementsByTagName("stimulus")[1]);
                if (stimulus.GetAttribute("screen") == "right")
                    right = ParseStimulus(stimulus);
                else
                    left = ParseStimulus(stimulus);

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
        private Stimulus ParseStimulus(XmlElement xmlStimulus)
        {
            Stimulus stim = new Stimulus();

            int top = Convert.ToInt32(((XmlElement)xmlStimulus.GetElementsByTagName("top")[0]).InnerText);
            int left = Convert.ToInt32(((XmlElement)xmlStimulus.GetElementsByTagName("left")[0]).InnerText);
            int height = Convert.ToInt32(((XmlElement)xmlStimulus.GetElementsByTagName("height")[0]).InnerText);
            int width = Convert.ToInt32(((XmlElement)xmlStimulus.GetElementsByTagName("width")[0]).InnerText);
            string filename = ((XmlElement)xmlStimulus.GetElementsByTagName("location")[0]).InnerText;

            stim.SetFilename(filename);
            stim.SetTop(top);
            stim.SetLeft(left);
            stim.SetHeight(height);
            stim.SetWidth(width);

            return stim;
        }

    }
}
