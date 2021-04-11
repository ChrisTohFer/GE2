using Sharpmake;
using System.IO;
using System.Collections.Generic;

[module: Include("projects.sharpmake.cs")]
[module: Include("third_party.sharpmake.cs")]

namespace Main
{
    //-------------------------------------
    //CONSTANTS

    static class Constants
    {
        public static readonly string RootPath = Path.GetFullPath(Directory.GetCurrentDirectory() + @"\..\..") + @"\";
        public static readonly string CodePath = RootPath + @"Code\";
        public static readonly string ThirdPartyPath = RootPath + @"ThirdParty\";
        public static readonly string GeneratedPath = RootPath + @"Generated\";
        public static readonly string AssetsPath = RootPath + @"Assets\";
        public static readonly string ProjectPath = GeneratedPath + @"Projects\";
        public static readonly string OutputPath = GeneratedPath + @"Output\";
        public static readonly string IntermediatePath = GeneratedPath + @"Intermediate\";
    }

    //CONSTANTS
    //-------------------------------------

    //-------------------------------------
    //UTILITY

    static class Utility
    {
        //Logging
        static List<string> logList = new List<string>();
        public static void Log(string line)
        {
            logList.Add(line);
        }
        public static void PrintLog()
        {
            File.Delete(Constants.RootPath + "logfile.txt");
            if (logList.Count != 0)
                File.WriteAllLines(Constants.RootPath + "logfile.txt", logList);
        }

        //File copying
        public static void CopyFolderRecursive(DirectoryInfo source, DirectoryInfo target)
        {
            if (!target.Exists)
                target.Create();
            foreach (DirectoryInfo dir in source.GetDirectories())
            {
                CopyFolderRecursive(dir, new DirectoryInfo(target.FullName + @"\" + dir.Name));
            }
            foreach (FileInfo file in source.GetFiles())
            {
                file.CopyTo(target.FullName + @"\" + file.Name, true);
            }
        }
    }

    //UTILITY
    //-------------------------------------

    //-------------------------------------
    //CONFIGURATION

    class CustomConfiguration : Project.Configuration
    {
        public void SetConsoleApp()
        {
            Output = OutputType.Exe;
            Options.Add(Sharpmake.Options.Vc.Linker.SubSystem.Console);
        }
        public void SetWindowedApp()
        {
            Output = OutputType.Exe;
            Options.Add(Sharpmake.Options.Vc.Linker.SubSystem.Application);
        }
        public void SetDLL()
        {
            Output = OutputType.Dll;
        }
        public void SetLib()
        {
            Output = OutputType.Lib;
        }

        public void DisableExceptions()
        {
            Defines.Add("_HAS_EXCEPTIONS=0");
            Options.Add(Sharpmake.Options.Vc.Compiler.Exceptions.Disable);
        }
    }

    //CONFIGURATION
    //-------------------------------------


    //-------------------------------------
    //TARGET STUFF

    public class CustomTarget : ITarget
    {
        public Platform Platform;
        public DevEnv DevEnv;
        public Optimization Optimization;

        public CustomTarget() { }

        CustomTarget(Platform p, DevEnv de, Optimization o)
        {
            Optimization = o;
            Platform = p;
            DevEnv = de;
        }

        //This must be unique for all configurations that we intend to use
        public override string Name
        {
            get { return Optimization.ToString(); }
        }

        public static CustomTarget DefaultTargets()
        {
            Platform p = Platform.win64;
            DevEnv de = DevEnv.vs2019;
            Optimization o = (Optimization.Debug | Optimization.Release | Optimization.Retail);

            return new CustomTarget(p, de, o);
        }
    }

    //TARGET STUFF
    //-------------------------------------


    //-------------------------------------
    //SOLUTION STUFF

    [Generate]
    public class GE2 : Solution
    {
        public GE2()
            : base(typeof(CustomTarget))
        {
            AddTargets(CustomTarget.DefaultTargets());
        }

        [Configure]
        public virtual void Configure(Configuration conf, CustomTarget target)
        {
            conf.SolutionPath = Constants.GeneratedPath;

            conf.AddProject<Projects.Launch>(target);
            conf.AddProject<Projects.Global>(target);
        }
    }

    //SOLUTION STUFF
    //-------------------------------------


    //-------------------------------------
    //ARGUMENTS AND MAIN

    class CustomArguments
    {
        //Flags/Values - make these static so that they can be accessed from anywhere

        static CustomArguments()
        {
            //Defaults
        }

        //Options

    }

    public class Main
    {
        [Sharpmake.Main]
        public static void SharpmakeMain(Arguments args)
        {
            //We don't need to keep the reference to CustomArguments object since we save the flags in static variables
            CommandLine.ExecuteOnObject(new CustomArguments());
            args.Generate<GE2>();

            //Make sure assets are accessible - visual studio debugger uses project folder as local directory
            Utility.CopyFolderRecursive(new DirectoryInfo(Constants.AssetsPath), new DirectoryInfo(Constants.ProjectPath));

            //Logging
            Utility.PrintLog();
        }
    }

    //ARGUMENTS AND MAIN
    //-------------------------------------
}
