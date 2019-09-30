using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RainShaderCompiler
{
    public static class Extentions
    {
        public static T[] Concat<T>(this T[] x, T[] y)
        {
            if (x == null) throw new ArgumentNullException("x");
            if (y == null) throw new ArgumentNullException("y");
            int oldLen = x.Length;
            Array.Resize<T>(ref x, x.Length + y.Length);
            Array.Copy(y, 0, x, oldLen, y.Length);
            return x;
        }
    }
    


    class Program
    {
        public static void Compile(string glslCompiler, string shader)
        {
            Console.WriteLine("> Compiling : " + shader);
            string compiled = shader + ".spv";
            if (File.Exists(compiled) && File.GetLastWriteTime(compiled) > File.GetLastWriteTime(shader))
            {
                Console.WriteLine("> This file is already compiled.");
                return;
            }

            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.CreateNoWindow = false;
            startInfo.UseShellExecute = false;
            startInfo.FileName = glslCompiler;
            startInfo.WindowStyle = ProcessWindowStyle.Hidden;
            startInfo.Arguments = String.Format(" -V -r {0} -o {0}.spv", shader);
            //startInfo.Arguments = shader + " -o " + shader + ".spv";

            try
            {
                using (Process exeProcess = Process.Start(startInfo))
                {
                    exeProcess.WaitForExit();
                }
            }
            catch
            {
                Console.WriteLine("Error while compiling");
            }
        }


        static void Main(string[] args)
        {
            string vulkanSdkPath = Environment.GetEnvironmentVariable("VULKAN_SDK");
            if (vulkanSdkPath != null)
            {
                Console.WriteLine("> Found Vulkan SDK : " + vulkanSdkPath);
            }
            else
            {
                Console.WriteLine("> Unable to find vulkan : are you sure it is installed ?");
                Console.ReadLine();
                return;
            }
            string glslCompiler = vulkanSdkPath + "/Bin32/glslc.exe";
            string glslValidator = vulkanSdkPath + "/Bin32/glslangValidator.exe";

            string shadersPath = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location) + @"\..\..\data\shaders\glsl";
            if (args.Length == 0)
            {
                Console.WriteLine("> A path to shaders was not suplied, using the default path :");
            }
            else
            {
                Console.WriteLine("> Path to shaders :");
                shadersPath = args[0];
            }
            Console.WriteLine(shadersPath + "\n");

            //string[] vertexFiles = Directory.GetFiles(shadersPath, "*.vert", SearchOption.TopDirectoryOnly);
            //string[] fragmentFiles = Directory.GetFiles(shadersPath, "*.frag", SearchOption.TopDirectoryOnly);

            string[] shaderFiles = Directory.GetFiles(shadersPath, "*.vert", SearchOption.TopDirectoryOnly)
                .Concat(Directory.GetFiles(shadersPath, "*.frag", SearchOption.TopDirectoryOnly))
                .Concat(Directory.GetFiles(shadersPath, "*.geom", SearchOption.TopDirectoryOnly));
                

            Array.ForEach(shaderFiles, shader =>
            {
                Compile(glslValidator, shader);
                Console.WriteLine("");
            });

            //Array.ForEach(fragmentFiles, shader =>
            //{
            //    Compile(glslValidator, String.Format(" -V {0} -o {0}.spv", shader));
            //    Console.WriteLine("");
            //});
        }
    }
}
