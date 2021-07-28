using Sharpmake;
using Main;

namespace Projects
{
    class DefaultProject : Project
    {
        public DefaultProject()
            : base(typeof(CustomTarget), typeof(CustomConfiguration))
        {
            AddTargets(CustomTarget.DefaultTargets());
            SourceRootPath = Constants.CodePath + GetType().Name;
        }

        [Configure]
        public virtual void Configure(CustomConfiguration conf, CustomTarget target)
        {
            conf.ProjectPath = Constants.ProjectPath;
            conf.TargetPath = Constants.OutputPath + @"[target.Name]";
            conf.IntermediatePath = Constants.IntermediatePath + @"[target.Name]\[project.Name]";

            //
            conf.IncludePaths.Add(Constants.CodePath);

            //
            conf.DisableExceptions();
            conf.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP17);
            conf.Options.Add(Options.Vc.Compiler.RTTI.Disable);

            switch (target.Optimization)
            {
                case Optimization.Debug:
                    conf.Options.Add(Options.Vc.Compiler.Optimization.Disable);
                    break;
                case Optimization.Release:
                case Optimization.Retail:
                    conf.Options.Add(Options.Vc.Compiler.Optimization.MaximizeSpeed);
                    break;
            }
        }
    }

    //Global projects

    class GlobalProject : DefaultProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            conf.SolutionFolder = "1_Global";

            base.Configure(conf, target);
        }
    }

    [Generate]
    class Global : GlobalProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
            conf.SetLib();
        }
    }

    //Platform projects

    class PlatformProject : DefaultProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            conf.SolutionFolder = "2_Platform";

            base.Configure(conf, target);
        }
    }

    [Generate]
    class AssetManager : PlatformProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
            conf.SetLib();

            //Dependencies
            conf.AddPrivateDependency<ThirdParty.STBImage>(target);

            conf.AddPrivateDependency<Platform>(target);
        }
    }

    [Generate]
    class Platform : PlatformProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
            conf.SetLib();

            //Dependencies
            conf.AddPrivateDependency<ThirdParty.Glad>(target);
            conf.AddPrivateDependency<ThirdParty.Imgui>(target);

            ThirdParty.OpenGL.AddAsDependency(conf, target);
            ThirdParty.SFML.AddAsDependency(conf, target);
        }
    }

    //System projects

    class SystemProject : DefaultProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            conf.SolutionFolder = "3_System";

            base.Configure(conf, target);
        }
    }

    [Generate]
    class Graphics : SystemProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
            conf.SetLib();

            //Dependencies
            ThirdParty.SFML.AddAsDependency(conf, target);

            conf.AddPrivateDependency<ThirdParty.Glad>(target);
            conf.AddPrivateDependency<ThirdParty.GLM>(target);
            conf.AddPrivateDependency<ThirdParty.Imgui>(target);
            conf.AddPrivateDependency<AssetManager>(target);
            conf.AddPrivateDependency<Platform>(target);
        }
    }

    //Ungrouped

    class UngroupedProject : DefaultProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
        }
    }

    [Generate]
    class Editor : UngroupedProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
            conf.SetLib();

            //Dependencies
            conf.AddPrivateDependency<ThirdParty.Imgui>(target);

            conf.AddPrivateDependency<AssetManager>(target);
            conf.AddPrivateDependency<Graphics>(target);
            conf.AddPrivateDependency<Platform>(target);
        }
    }

    [Generate]
    class Launch : UngroupedProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
            conf.SetConsoleApp();

            //Dependencies
            conf.AddPrivateDependency<ThirdParty.Imgui>(target);

            conf.AddPrivateDependency<AssetManager>(target);
            conf.AddPrivateDependency<Editor>(target);
            conf.AddPrivateDependency<Graphics>(target);
            conf.AddPrivateDependency<Platform>(target);
        }
    }
}
