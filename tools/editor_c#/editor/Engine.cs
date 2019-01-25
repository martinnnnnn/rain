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
        [DllImport("rain_engine_export.dll")]
        public static extern void launch_rain_engine(IntPtr wndHandle);
    }
}
