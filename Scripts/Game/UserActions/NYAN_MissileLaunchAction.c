class NYAN_MissileLaunchAction : ScriptedUserAction
{

	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		auto launcherPodController = NYAN_LauncherPodController.Cast(pOwnerEntity.FindComponent(NYAN_LauncherPodController));
		if (!launcherPodController) {
			return;
		}
		launcherPodController.Launch();
	}
	
	override bool GetActionNameScript(out string outName)
	{
		outName = "Launch missile (single)";
		return true;
	}
	
	override bool CanBePerformedScript(IEntity user)
	{
		return true;
	}

};