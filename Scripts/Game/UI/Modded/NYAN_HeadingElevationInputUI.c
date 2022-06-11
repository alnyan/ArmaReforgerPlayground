modded enum ChimeraMenuPreset {
	NYAN_HeadingElevationInputMenu_ID
}

class NYAN_HeadingElevationInputUI: ChimeraMenuBase {
	IEntity launcherPodEntity; 
	private SCR_ButtonTextComponent mApplyButton;
	private EditBoxWidget mHeadingEditBox;
	private EditBoxWidget mElevationEditBox;
	
	override void OnMenuOpen() {
		Widget root = GetRootWidget();
		
		mApplyButton = SCR_ButtonTextComponent.GetButtonText("ApplyButton", root);
		mHeadingEditBox = EditBoxWidget.Cast(root.FindWidget("MainPanel.MainLayout.HeadingEditBox"));
		mElevationEditBox = EditBoxWidget.Cast(root.FindWidget("MainPanel.MainLayout.ElevationEditBox"));
		
		if (mApplyButton) {
			mApplyButton.m_OnClicked.Insert(OnApplyButtonClicked);
		}
	}
	
	void OnApplyButtonClicked() {
		if (!launcherPodEntity) {
			return;
		}
		
		auto launcherPodController = NYAN_LaunchPodController.Cast(launcherPodEntity.FindComponent(NYAN_LaunchPodController));
		
		if (!mHeadingEditBox || !mElevationEditBox || !launcherPodController) {
			return;
		}
		
		auto heading = mHeadingEditBox.GetText().ToFloat();
		auto elevation = mElevationEditBox.GetText().ToFloat();
				
		PrintFormat("Applying heading (%1) and elevation (%2)", heading, elevation);
		launcherPodController.SetTargetHeadingElevation(heading, elevation);
		
		Close();
	}
}