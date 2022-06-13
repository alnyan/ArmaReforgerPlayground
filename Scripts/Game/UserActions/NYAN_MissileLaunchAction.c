class NYAN_MissileLaunchAction : ScriptedUserAction
{

	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		Managed mSlotManager = pOwnerEntity.FindComponent(SlotManagerComponent);
		if (!mSlotManager) {
			return;
		}
		
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(mSlotManager);
		
		if (!slotManager) {
			return;
		}
		
		array<EntitySlotInfo> res = {};
		vector rocketTransform[4];
		
		int count = slotManager.GetSlotInfos(res);
		
		for (int i = 0; i < count; ++i) {
			if (!res[i]) {
				continue;
			}
			IEntity rocket = res[i].GetAttachedEntity();
			if (!rocket) {
				continue;
			}
			
			res[i].DetachEntity(false);
			rocket.GetWorldTransform(rocketTransform);
			pOwnerEntity.RemoveChild(rocket);
			ApplyRecoil(pOwnerEntity, rocketTransform, 2500);
			LaunchRocket(rocket, rocketTransform, pOwnerEntity);
			return;
		}
		
		Print("No rockets remain!!!");
	}
	
	override bool GetActionNameScript(out string outName)
	{
		outName = "Launch missile";
		return true;
	}
	
	override bool CanBePerformedScript(IEntity user)
	{
		return true;
	}
	
	private void ApplyRecoil(IEntity turret, vector rocketTransform[4], float amount) {
		IEntity target = turret.GetParent();
		if (!target) {
			target = turret;
		}
		vector rocketPosition = "0 0 0";
		rocketPosition = rocketPosition.Multiply4(rocketTransform);
		vector impulseVector = "0 0 -1";
		impulseVector = impulseVector.Multiply4(rocketTransform);
		impulseVector = (impulseVector - rocketPosition).Normalized() * amount;
		
		target.GetPhysics().ApplyImpulseAt(rocketPosition, impulseVector);
	}
	
	private void LaunchRocket(IEntity rocket, vector mat[4], IEntity launcher)
	{
		if (!rocket) {
			return;
		}

		MissileMoveComponent moveComponent = MissileMoveComponent.Cast(rocket.FindComponent(MissileMoveComponent));
		CollisionTriggerComponent triggerComponent = CollisionTriggerComponent.Cast(rocket.FindComponent(CollisionTriggerComponent));
		
		if (!moveComponent || !triggerComponent) {
			return;
		}
		
		rocket.SetWorldTransform(mat);
		triggerComponent.SetLive(rocket);
		moveComponent.Launch(vector.Forward, vector.Zero, 1.0, rocket, null, launcher, null, launcher);
	}
};