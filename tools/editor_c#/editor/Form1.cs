using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace editor
{
    public partial class Form1 : Form
    {
        Form2 m_form;
        public Form1()
        {
            InitializeComponent();

            m_form = new Form2();
            // Set the Parent Form of the Child window.  
            m_form.MdiParent = this;
            // Display the new form.  
            m_form.Show();
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            Form2 newMDIChild = new Form2();
            // Set the Parent Form of the Child window.  
            newMDIChild.MdiParent = this;
            // Display the new form.  
            newMDIChild.Show();
        }

        private void launchEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            IntPtr handle = m_form.Handle;
            EngineDLL.OpenWindow(handle/*, "D:/_MARTIN/programming/_projects/rain_engine/resources"*/);
        }
    }
}
