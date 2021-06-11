using Sharpmake;
using Main;

namespace ThirdParty
{
    class ThirdPartyProject : Projects.DefaultProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            conf.SolutionFolder = "0_Thirdparty";

            base.Configure(conf, target);
        }
    }


    static class SFML
    {
        public static void AddAsDependency(CustomConfiguration conf, CustomTarget target)
        {
            var rootPath = Constants.ThirdPartyPath + @"SFML-2.5.1\";

            //Include
            conf.IncludePrivatePaths.Add(rootPath + @"include");

            //Lib files
            conf.LibraryPaths.Add(rootPath + @"x64\lib");
            var suffix = target.Optimization == Optimization.Debug ? "-d" : "";
            conf.LibraryFiles.Add(
                @"sfml-audio" + suffix,
                @"sfml-graphics" + suffix,
                @"sfml-network" + suffix,
                @"sfml-system" + suffix,
                @"sfml-window" + suffix
            );

            //Dll files
            conf.TargetCopyFiles.Add(
                rootPath + @"x64\bin\sfml-audio" + suffix + @"-2.dll",
                rootPath + @"x64\bin\sfml-graphics" + suffix + @"-2.dll",
                rootPath + @"x64\bin\sfml-network" + suffix + @"-2.dll",
                rootPath + @"x64\bin\sfml-system" + suffix + @"-2.dll",
                rootPath + @"x64\bin\sfml-window" + suffix + @"-2.dll"
            );
        }
    }

    static class OpenGL
    {
        public static void AddAsDependency(CustomConfiguration conf, CustomTarget target)
        {
            conf.LibraryFiles.Add("opengl32");
        }
    }

    [Generate]
    class Glad : ThirdPartyProject
    {
        public Glad()
        {
            SourceRootPath = Constants.ThirdPartyPath + @"Glad\";
        }

        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
            conf.SetLib();

            conf.IncludePaths.Add("include");
        }
    }

    [Generate]
    class GLM : ThirdPartyProject
    {
        public GLM()
        {
            SourceRootPath = Constants.ThirdPartyPath + @"glm\";
        }

        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
            conf.SetLib();

            conf.Options.Add(Options.Vc.General.WarningLevel.Level0);
            conf.IncludePaths.Add(SourceRootPath);
        }
    }

    [Generate]
    class Imgui : ThirdPartyProject
    {
        public Imgui()
        {
            SourceRootPath = Constants.ThirdPartyPath + @"Imgui\";
        }

        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
            conf.SetLib();

            //Dependencies
            conf.AddPrivateDependency<Glad>(target);

            //Settings
            conf.IncludePaths.Add(SourceRootPath);
        }
    }

    [Generate]
    class STBImage : ThirdPartyProject
    {
        public STBImage()
        {
            SourceRootPath = Constants.ThirdPartyPath + @"stb_image\";
        }

        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
            conf.SetLib();

            //Settings
            conf.IncludePaths.Add(SourceRootPath);
        }
    }

}
