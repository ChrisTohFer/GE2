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

    [Generate]
    class Global : DefaultProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
            conf.SetLib();
        }
    }
    
    [Generate]
    class Launch : DefaultProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
            conf.SetConsoleApp();

            conf.AddPrivateDependency<ThirdParty.Glad>(target);

            conf.AddPrivateDependency<Platform>(target);
        }
    }

    [Generate]
    class Platform : DefaultProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
            conf.SetLib();

            conf.AddPrivateDependency<ThirdParty.Glad>(target);
            conf.AddPrivateDependency<ThirdParty.Imgui>(target);

            ThirdParty.OpenGL.AddAsDependency(conf, target);
            ThirdParty.SFML.AddAsDependency(conf, target);
        }
    }
}
