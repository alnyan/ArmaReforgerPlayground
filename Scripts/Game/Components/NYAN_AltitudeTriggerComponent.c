// TODO: I think I may need to rename this component for clarity

class NYAN_AltitudeTriggerComponentClass: ScriptComponentClass {};

class NYAN_AltitudeTriggerComponent: ScriptComponent {
	[Attribute("", UIWidgets.EditBox, "Submunition to spawn on trigger")]
	protected ResourceName m_submunitionPrefab;
	[Attribute("10", UIWidgets.EditBox, "Submunition count to spawn")]
	protected int m_submunitionCount;
	[Attribute("10", UIWidgets.EditBox, "Submunition spread radius")]
	protected float m_submunitionSpread;
	[Attribute("10", UIWidgets.EditBox, "Trigger altitude at sea level")]
	protected float m_triggerAltitude;
	
	[Attribute("", UIWidgets.EditBox, "Explosion prefab")]
	protected ResourceName m_explosionPrefab;
	
	private RplComponent m_rplComponent;
	
	override protected void OnPostInit(IEntity owner) 
	{		
	    super.OnPostInit(owner);
		owner.SetFlags(EntityFlags.ACTIVE, false);
		
		m_rplComponent = RplComponent.Cast(owner.FindComponent(RplComponent));
		if (!m_rplComponent) {
			Print("NO RPL");
		}
	}
	
	override protected void EOnPhysicsMove(IEntity owner) {
		Physics physics = owner.GetPhysics();
		auto height = owner.GetOrigin()[1];
		auto yvel = physics.GetVelocity()[1];
		
		if (yvel <= 0 && height <= m_triggerAltitude) {
			ClearEventMask(owner, EntityEvent.PHYSICSMOVE);
			Trigger(owner);
		}
	}
	
	void SetLive(IEntity owner) {
		SetEventMask(owner, EntityEvent.PHYSICSMOVE);
	}
	
	void Trigger(IEntity owner) {
		if (!m_submunitionPrefab) {
			Print("No submunition prefab set", LogLevel.WARNING);
			return;
		}
		
		vector parentTransform[4];
		auto parentVelocity = owner.GetPhysics().GetVelocity();
		auto parentAngularVelocity = owner.GetPhysics().GetAngularVelocity();
		owner.GetWorldTransform(parentTransform);
		
		auto prefab = Resource.Load(m_submunitionPrefab);
		auto world = owner.GetWorld();
		auto spawnParams = new EntitySpawnParams();
		spawnParams.TransformMode = ETransformMode.WORLD;
		spawnParams.Transform = parentTransform;
		
		if (m_explosionPrefab) {
			// TODO: I'm not sure these replicate properly, I should try that once I figure out PeerTool
			auto explosionPrefab = Resource.Load(m_explosionPrefab);
			if (explosionPrefab) {
				GetGame().SpawnEntityPrefab(explosionPrefab, world, spawnParams);
			}
		}
		
		for (int i = 0; i < m_submunitionCount; ++i) {
			auto spawnedEntity = GetGame().SpawnEntityPrefab(prefab, world, spawnParams);
			auto entityPhysics = Physics.CreateDynamic(spawnedEntity, 1.0, EPhysicsLayerDefs.Projectile);
			if (!entityPhysics) {
				Print("Failed to create physics for submunition", LogLevel.WARNING);
				continue;
			}
			auto random = new RandomGenerator();
			vector randomVelocityFactor = "0 0 0";
			randomVelocityFactor[0] = random.RandFloatXY(-m_submunitionSpread, m_submunitionSpread);
			randomVelocityFactor[1] = random.RandFloatXY(-m_submunitionSpread, m_submunitionSpread);
			randomVelocityFactor[2] = random.RandFloatXY(-m_submunitionSpread, m_submunitionSpread);
	
			entityPhysics.SetVelocity(parentVelocity + randomVelocityFactor);
			entityPhysics.SetAngularVelocity(parentAngularVelocity);
			entityPhysics.SetActive(ActiveState.ACTIVE);
		}
		
		RplComponent.DeleteRplEntity(owner, true);
	}
}