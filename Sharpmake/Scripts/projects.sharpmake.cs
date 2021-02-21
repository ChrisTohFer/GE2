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

            conf.DisableExceptions();
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
    class Launch : DefaultProject
    {
        public override void Configure(CustomConfiguration conf, CustomTarget target)
        {
            base.Configure(conf, target);
            conf.SetConsoleApp();

            conf.AddPrivateDependency<ThirdParty.Glad>(target);

            ThirdParty.OpenGL.AddAsDependency(conf, target);
            ThirdParty.SFML.AddAsDependency(conf, target);
        }
    }
}
