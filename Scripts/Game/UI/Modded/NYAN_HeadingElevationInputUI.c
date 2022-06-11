modded enum ChimeraMenuPreset {
	NYAN_HeadingElevationInputMenu_ID
}

class NYAN_HeadingElevationInputUI: ChimeraMenuBase {
	private NYAN_LaunchPodController m_launcherPodController;
	private SCR_ButtonTextComponent mApplyButton;
	private EditBoxWidget mHeadingEditBox;
	private EditBoxWidget mElevationEditBox;
	private ref array<float> mTargetingLimits;
	
	override void OnMenuOpen() {
		Widget root = GetRootWidget();
		
		mTargetingLimits = new array<float>;
		
		mApplyButton = SCR_ButtonTextComponent.GetButtonText("ApplyButton", root);
		mHeadingEditBox = EditBoxWidget.Cast(root.FindWidget("MainPanel.MainLayout.HeadingEditBox"));
		mElevationEditBox = EditBoxWidget.Cast(root.FindWidget("MainPanel.MainLayout.ElevationEditBox"));
		
		if (mApplyButton) {
			mApplyButton.m_OnClicked.Insert(OnApplyButtonClicked);
		}
	}
	
	void SetLauncherEntity(IEntity launcher) {
		m_launcherPodController = NYAN_LaunchPodController.Cast(launcher.FindComponent(NYAN_LaunchPodController));
		
		if (!m_launcherPodController) {
			return;
		}
		
		m_launcherPodController.GetTargetingLimits(mTargetingLimits);
		
		if (!mHeadingEditBox || !mElevationEditBox) {
			return;
		}
		auto current = new array<float>;
		m_launcherPodController.GetTargetAngles(current);
		mHeadingEditBox.SetText(current[0].ToString());
		mElevationEditBox.SetText(current[1].ToString());
	}
	
	private void OnApplyButtonClicked() {
		if (!mHeadingEditBox || !mElevationEditBox || !m_launcherPodController) {
			return;
		}
		
		auto heading = mHeadingEditBox.GetText().ToFloat();
		auto elevation = mElevationEditBox.GetText().ToFloat();
		
		if (heading < mTargetingLimits[0] ||
			heading > mTargetingLimits[1] ||
			elevation < mTargetingLimits[2] ||
			elevation > mTargetingLimits[3]) {
			SCR_HintManagerComponent.ShowCustomHint("Heading/elevation exceed targeting limits", "", 2.5);
			return;
		}
				
		PrintFormat("Applying heading (%1) and elevation (%2)", heading, elevation);
		m_launcherPodController.SetTargetHeadingElevation(heading, elevation);
		
		Close();
	}
}