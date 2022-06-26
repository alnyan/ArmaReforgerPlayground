modded enum ChimeraMenuPreset {
	NYAN_HeadingElevationInputMenu_ID
}

class NYAN_HeadingElevationInputUI: ChimeraMenuBase {
	private NYAN_LauncherPodComponent mLauncherPodComponent;
	
	private SCR_ButtonTextComponent mApplyButton;
	private EditBoxWidget mHeadingEditBox;
	private EditBoxWidget mElevationEditBox;
	private TextWidget mMinHeadingText;
	private TextWidget mMaxHeadingText;
	private TextWidget mMinElevationText;
	private TextWidget mMaxElevationText;
	private SliderWidget mHeadingSlider;
	private SliderWidget mElevationSlider;
	
	private ref array<float> mTargetingLimits;
	
	override void OnMenuOpen() {
		Widget root = GetRootWidget();
		
		mTargetingLimits = new array<float>;
		
		mApplyButton = SCR_ButtonTextComponent.GetButtonText("ApplyButton", root);
		mHeadingEditBox = EditBoxWidget.Cast(root.FindWidget("MainPanel.MainLayout.HeadingGroup.HeadingEditBox"));
		mElevationEditBox = EditBoxWidget.Cast(root.FindWidget("MainPanel.MainLayout.ElevationGroup.ElevationEditBox"));
		mMinHeadingText = TextWidget.Cast(root.FindWidget("MainPanel.MainLayout.HeadingGroup.MinHeadingText"));
		mMaxHeadingText = TextWidget.Cast(root.FindWidget("MainPanel.MainLayout.HeadingGroup.MaxHeadingText"));
		mMinElevationText = TextWidget.Cast(root.FindWidget("MainPanel.MainLayout.ElevationGroup.MinElevationText"));
		mMaxElevationText = TextWidget.Cast(root.FindWidget("MainPanel.MainLayout.ElevationGroup.MaxElevationText"));
		mHeadingSlider = SliderWidget.Cast(root.FindWidget("MainPanel.MainLayout.HeadingGroup.HeadingSlider"));
		mElevationSlider = SliderWidget.Cast(root.FindWidget("MainPanel.MainLayout.ElevationGroup.ElevationSlider"));

		if (mApplyButton) {
			mApplyButton.m_OnClicked.Insert(OnApplyButtonClicked);
		}
	}
	
	void SetLauncherComponent(notnull NYAN_LauncherPodComponent launcherComponent) {
		mLauncherPodComponent = launcherComponent;
		
		mLauncherPodComponent.GetTargetingLimits(mTargetingLimits);
		
		if (!mHeadingEditBox || 
			!mElevationEditBox || 
			!mMinHeadingText || 
			!mMaxHeadingText ||
			!mMinElevationText ||
			!mMaxElevationText ||
			!mHeadingSlider ||
			!mElevationSlider) {
			return;
		}
		
		mMinHeadingText.SetText(mTargetingLimits[0].ToString());
		mMaxHeadingText.SetText("+" + mTargetingLimits[1].ToString());
		mMinElevationText.SetText(mTargetingLimits[2].ToString());
		mMaxElevationText.SetText("+" + mTargetingLimits[3].ToString());
		mHeadingSlider.SetRange(mTargetingLimits[0], mTargetingLimits[1]);
		mElevationSlider.SetRange(mTargetingLimits[2], mTargetingLimits[3]);
		
		auto current = new array<float>;
		mLauncherPodComponent.GetTargetAngles(current);
		mHeadingEditBox.SetText(current[0].ToString());
		mElevationEditBox.SetText(current[1].ToString());
		mHeadingSlider.SetCurrent(current[0]);
		mElevationSlider.SetCurrent(current[1]);
	}
	
	private void OnApplyButtonClicked() {
		if (!mHeadingEditBox || !mElevationEditBox || !mLauncherPodComponent) {
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
		mLauncherPodComponent.SetTargetHeadingElevation(heading, elevation);
		
		Close();
	}
}