class NYAN_InputHeadingElevationAction : ScriptedUserAction
{
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		auto launcherPodController = NYAN_LauncherPodController.Cast(pOwnerEntity.FindComponent(NYAN_LauncherPodController));
		if (!launcherPodController) {
			return;
		}
		
		auto menu = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.NYAN_HeadingElevationInputMenu_ID);
		auto headingElevationMenu = NYAN_HeadingElevationInputUI.Cast(menu);
		
		if (!headingElevationMenu) {
			return;
		}
		
		headingElevationMenu.SetLauncherComponent(launcherPodController.GetLauncherComponent());
	}
}