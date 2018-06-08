using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace editor
{
    public class EngineDLL
    {
        [DllImport("rain_lib.dll")]
        public static extern void LoadGame(string path);

        [DllImport("rain_lib.dll")]
        public static extern void OpenWindow(IntPtr wndHandle);

    }
}
