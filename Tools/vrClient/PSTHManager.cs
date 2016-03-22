using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.IO;

using System.Drawing;
using System.Text;
using System.Threading.Tasks;

using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace vrClient
{
    class PSTHManager : Form
    {
        int binSize = 10;       // Bin size in ms
        int plotLength = 2000;  // Plot length in ms
        int[] binIntervals;
        int nBins;
        int nConditions;

        Chart[] charts;

        int nCols = 3;
        int nRows = 3;
        int plotWidth = 200;
        int plotHeight = 200;

        int formWidth = 610;
        int formHeight = 610;

        double[,] plotData;

        public PSTHManager()
        {
            this.Text = "PSTH Overview";
            this.Height = formHeight;
            this.Width = formWidth;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;

        }

        public void Initialize(int b, int l, string[] conditions)
        {
            binSize = b;
            plotLength = l;
            nConditions = conditions.Length;

            if (nConditions < 9)
            {
                InitializeData();
                InitializeCharts(conditions);
            }
        }
        private void InitializeData()
        {
            nBins = (plotLength + plotLength%binSize) / binSize;
            plotData = new double[nConditions,nBins];
            binIntervals = new int[nBins];

            for (int binIndex = 0; binIndex < nBins;++binIndex)
                binIntervals[binIndex] = (binIndex + 1) * binSize;

            for (int conditionIndex = 0; conditionIndex < nConditions; ++conditionIndex)
                ResetConditionData(conditionIndex);
        }
        private void InitializeCharts(string[] plotTitles)
        {
            this.Controls.Clear();
            charts = new Chart[nConditions];
            for (int conditionIndex = 0; conditionIndex < nConditions; ++conditionIndex)
            {
                int plotX = (conditionIndex % nCols) * plotWidth;
                int plotY = (conditionIndex / nRows) * plotHeight;

                charts[conditionIndex] = new Chart();

                // General plot properties
                charts[conditionIndex].Location = new System.Drawing.Point(plotX, plotY);
                charts[conditionIndex].Width = plotWidth;
                charts[conditionIndex].Height = plotHeight;
                charts[conditionIndex].Titles.Add(plotTitles[conditionIndex]);

                // Chart area
                charts[conditionIndex].ChartAreas.Add("psth");
                charts[conditionIndex].ChartAreas["psth"].Visible = true;
                charts[conditionIndex].ChartAreas["psth"].AxisX.Maximum = plotLength;
                charts[conditionIndex].ChartAreas["psth"].AxisX.Minimum = 0;
                charts[conditionIndex].ChartAreas["psth"].AxisY.Maximum = 1;
                charts[conditionIndex].ChartAreas["psth"].AxisY.Minimum = 0;
                charts[conditionIndex].ChartAreas["psth"].BackColor = Color.SeaGreen;

                // Data series definition
                charts[conditionIndex].Series.Add("data");
                charts[conditionIndex].Series["data"].SetDefault(true);
                charts[conditionIndex].Series["data"].Enabled = true;
                charts[conditionIndex].Series["data"].ChartType = SeriesChartType.FastLine;
                charts[conditionIndex].Series["data"].Color = System.Drawing.Color.Red;
                for (int binIndex = 0; binIndex < nBins; ++binIndex)
                    charts[conditionIndex].Series["data"].Points.AddXY(binIntervals[binIndex], plotData[conditionIndex, binIndex]);

                this.Controls.Add(charts[conditionIndex]);

            }
        }

        public void UpdateConditionData(int conditionID)
        { }
        public void ResetConditionData(int conditionID)
        {
            for (int binIndex = 0; binIndex < nBins; ++binIndex)
                plotData[conditionID, binIndex] = (double)binIndex*((double)binSize/plotLength);
        }
        public void UpdateConditionPlot(int conditionID)
        {
            for (int binIndex = 0; binIndex < nBins; ++binIndex)
                charts[conditionID].Series["data"].Points[binIndex].SetValueY(plotData[conditionID, binIndex]);
        }

        public void SetBinSize()
        {
        }

        public void SetPlotLength()
        {
        }

    }
}
