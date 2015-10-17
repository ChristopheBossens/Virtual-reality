using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace vrClient
{
    class ListboxMessenger
    {
        ListBox listboxReference;
        int maxMessages,currentMessageIndex;
        string[] messages;

        public delegate void DelegateAddMessage(string m);

        public ListboxMessenger(ListBox listbox, int ml)
        {
            listboxReference = listbox;
            maxMessages = ml;
            ClearList();
        }

        public void ClearList()
        {
            listboxReference.Items.Clear();
            messages = new string[maxMessages];
            currentMessageIndex = 0;
        }

        public void AddMessage(string m)
        {
            if (listboxReference.InvokeRequired)
            {
                listboxReference.Invoke(new DelegateAddMessage(AddMessage), m);
            }
            else
            {
                if (currentMessageIndex < maxMessages )
                {
                    messages[currentMessageIndex] = m;
                    listboxReference.Items.Add(m);
                    ++currentMessageIndex;
                }
                else
                {
                    for (int i = 0; i < (maxMessages - 1); ++i)
                        messages[i] = messages[i + 1];
                    messages[maxMessages - 1] = m;
                    listboxReference.Items.RemoveAt(0);
                    listboxReference.Items.Add(m);
                    listboxReference.Update();
                }
                listboxReference.SelectedIndex = listboxReference.Items.Count - 1;
                listboxReference.ClearSelected();
            }
        }
    }
}
