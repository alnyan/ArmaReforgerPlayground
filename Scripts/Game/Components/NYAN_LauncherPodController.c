[ComponentEditorProps(description: "Component for remote control of launcher turrets", color: "0 0 255 255")]
class NYAN_LauncherPodControllerClass: ScriptComponentClass {};

class NYAN_LauncherPodController: ScriptComponent {
	[Attribute("0", UIWidgets.CheckBox, "Is remote (attached to the same parent)")]
	private bool mIsRemote;
	
	private IEntity mTurretEntity;
	private NYAN_LauncherPodComponent mLauncherPodComponent;
	
	override protected void EOnInit(IEntity owner) {
		if (!mIsRemote) {
			auto podController = NYAN_LauncherPodComponent.Cast(owner.FindComponent(NYAN_LauncherPodComponent));
			if (!podController) {
				Print("Invalid turret components: missing NYAN_LauncherPodComponent");
				return;
			}
			
			mTurretEntity = owner;
			mLauncherPodComponent = podController;
			return;
		}
		
		auto parent = owner.GetParent();
		if (!parent) {
			Print("Invalid remote/turret hierachy: remote is not attached to a parent");
			return;
		}
		auto slotManager = SlotManagerComponent.Cast(parent.FindComponent(SlotManagerComponent));
		if (!slotManager) {
			Print("Invalid remote/turret hierarchy: remote's parent doesn't have a slot manager");
			return;
		}
		
		array<EntitySlotInfo> res = {};
		int count = slotManager.GetSlotInfos(res);
		
		for (int i = 0; i < count; ++i) {
			if (!res[i]) {
				continue;
			}
			IEntity turret = res[i].GetAttachedEntity();
			if (!turret) {
				continue;
			}
			
			auto podController = NYAN_LauncherPodComponent.Cast(turret.FindComponent(NYAN_LauncherPodComponent));
			if (!podController) {
				continue;
			}
			
			mTurretEntity = turret;
			mLauncherPodComponent = podController;
			return;
		}
		
		Print("Invalid remote/turret hierarchy: no turret found in parent");
	}
	
	override protected void OnPostInit(IEntity owner) 
	{		
	    super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT);
	}
	
	NYAN_LauncherPodComponent GetLauncherComponent() {
		return mLauncherPodComponent;
	}
	
	void Launch() {
		if (mLauncherPodComponent && mTurretEntity) {
			mLauncherPodComponent.Launch(mTurretEntity);
		}
	}
}