using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
        string stimulusName;

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
        public int GetHeight() { return this.height; }
        public int GetTop() { return top; }
        public int GetLeft() { return left; }
        public string GetFilename() { return filename; }
        public string GetName() { return stimulusName; }

        public void SetWidth(int w) { this.width = w; }
        public void SetHeight(int h) { this.height = h; }
        public void SetTop(int t) { this.top = t; }
        public void SetLeft(int l) { this.left = l; }
        public void SetFilename(string s) { this.filename = s; }
        public void SetStimulusName(string s) { this.stimulusName = s; }
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
        { this.name = name; }
        public void SetTransitionParameters(TransitionParameters t)
        { this.tp = t; }
        public TransitionParameters GetTransitionParameters()
        { return this.tp; }
        public void SetRewardParameters(RewardParameters r)
        { this.rp = r; }
        public RewardParameters GetRewardParameters()
        { return this.rp; }
        public Stimulus GetLeftStimulus()
        { return this.leftStimulus; }
        public Stimulus GetRightStimulus()
        { return this.rightStimulus; }
        public string GetLeftStimulusName() { return leftStimulus.GetName(); }
        public string GetRightStimulusName() { return rightStimulus.GetName(); }
        public string GetLeftStimulusLocation() { return leftStimulus.GetFilename(); }
        public string GetRightStimulusLocation() { return rightStimulus.GetFilename(); }
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
        { this.name = s; }
        public string GetName()
        { return name; }
        public void SetISI(bool b)
        { this.isiTrial = b; }
        public bool GetISI()
        { return this.isiTrial; }
    }
}
