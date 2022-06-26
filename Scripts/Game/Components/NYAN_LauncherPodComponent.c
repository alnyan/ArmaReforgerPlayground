class NYAN_LauncherPodComponentClass: ScriptComponentClass {};

class NYAN_LauncherPodComponent: ScriptComponent {
	[Attribute("10", UIWidgets.Slider, "Heading traverse speed (deg/s)", "0 180 1")]
	private float m_headingTraverseSpeed;
	[Attribute("10", UIWidgets.Slider, "Elevation traverse speed (deg/s)", "0 180 1")]
	private float m_elevationTraverseSpeed;
	
	[Attribute("-180", UIWidgets.Slider, "Minimum heading", "-180 0 0.1")]
	private float m_minHeading;
	[Attribute("180", UIWidgets.Slider, "Maximum heading", "0 180 0.1")]
	private float m_maxHeading;
	[Attribute("-180", UIWidgets.Slider, "Minimum elevation", "-180 0 0.1")]
	private float m_minElevation;
	[Attribute("180", UIWidgets.Slider, "Maximum elevation", "0 180 0.1")]
	private float m_maxElevation;
	
	[Attribute("0", UIWidgets.Slider, "Forbidden zone left", "-180 0 0.1")]
	private float m_deadZoneLeft;
	[Attribute("0", UIWidgets.Slider, "Forbidden zone right", "0 180 0.1")]
	private float m_deadZoneRight;
	[Attribute("0", UIWidgets.Slider, "Forbidden zone down", "-180 0 0.1")]
	private float m_deadZoneDown;
	[Attribute("0", UIWidgets.Slider, "Forbidden zone up", "180 0 0.1")]
	private float m_deadZoneUp;
	
	private float m_directionHeading;
	private float m_directionElevation;
	
	private float m_rtHeading, m_rtElevation;
	
	private ProcAnimComponent m_animComponent;
	private SignalsManagerComponent m_signalsComponent;
	
	private int m_signalAimTurnTurret;
	private int m_signalAimElevationTurret;
	
	override protected void EOnInit(IEntity owner) {
		m_animComponent = ProcAnimComponent.Cast(owner.FindComponent(ProcAnimComponent));
		m_signalsComponent = SignalsManagerComponent.Cast(owner.FindComponent(SignalsManagerComponent));
												
		if (!m_animComponent || !m_signalsComponent) {
			return;
		}
		
		m_signalAimTurnTurret = m_animComponent.GetSignalIndex(0, "AimTurnTurret");
		m_signalAimElevationTurret = m_animComponent.GetSignalIndex(0, "AimElevationTurret");
	}
	
	override protected void OnPostInit(IEntity owner) 
	{		
	    super.OnPostInit(owner);
		owner.SetFlags(EntityFlags.ACTIVE, false);
		SetEventMask(owner, EntityEvent.INIT | EntityEvent.FRAME);
	}
	
	override protected void EOnFrame(IEntity owner, float timeSlice) {
		if (!m_signalsComponent) {
			return;
		}
		
		float dx = Math.Clamp(m_directionHeading - m_rtHeading, 
			-m_headingTraverseSpeed * timeSlice, m_headingTraverseSpeed * timeSlice);
		float dy = Math.Clamp(m_directionElevation - m_rtElevation, 
			-m_elevationTraverseSpeed * timeSlice, m_elevationTraverseSpeed * timeSlice);
		
		m_rtHeading += dx;	
		m_rtElevation += dy;

		m_signalsComponent.SetSignalValue(m_signalAimTurnTurret, m_rtHeading);
		m_signalsComponent.SetSignalValue(m_signalAimElevationTurret, m_rtElevation);
	}
	
	void GetTargetAngles(notnull out array<float> values) {
		values.Insert(m_directionHeading);
		values.Insert(m_directionElevation);
	}
	
	void GetTargetingLimits(notnull out array<float> values) {
		values.Insert(m_minHeading);
		values.Insert(m_maxHeading);
		values.Insert(m_minElevation);
		values.Insert(m_maxElevation);
	}
	
	void SetTargetHeadingElevation(float heading, float elevation) {
		m_directionHeading = Math.Clamp(heading, m_minHeading, m_maxHeading);
		m_directionElevation = Math.Clamp(elevation, m_minElevation, m_maxElevation);
	}
	
	bool CanLaunch() {
		if (m_deadZoneLeft != m_deadZoneRight &&
			m_deadZoneDown != m_deadZoneUp &&
		    m_rtHeading >= m_deadZoneLeft && 
		    m_rtHeading <= m_deadZoneRight &&
			m_rtElevation >= m_deadZoneDown &&
			m_rtElevation <= m_deadZoneUp) {
			return false;
		}

		return true;
	}
	
	void Launch(IEntity turret) {
		if (!CanLaunch()) {
			return;
		}
		
		array<EntitySlotInfo> res = {};
		vector rocketTransform[4];
		
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(turret.FindComponent(SlotManagerComponent));
		
		if (!slotManager) {
			return;
		}
		
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
			turret.RemoveChild(rocket);
			ApplyRecoil(turret, rocketTransform, 2500);
			LaunchRocket(rocket, rocketTransform, turret);
			return;
		}
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

		if (!moveComponent) {
			return;
		}
		
		rocket.SetWorldTransform(mat);
		CollisionTriggerComponent baseTriggerComponent = CollisionTriggerComponent.Cast(rocket.FindComponent(CollisionTriggerComponent));
		if (baseTriggerComponent) {
			baseTriggerComponent.SetLive(rocket);
		} else {
			NYAN_AltitudeTriggerComponent triggerComponent = NYAN_AltitudeTriggerComponent.Cast(rocket.FindComponent(NYAN_AltitudeTriggerComponent));
			if (!triggerComponent) {
				Print("Failed to find trigger component for munition", LogLevel.WARNING);
				return;
			}
			triggerComponent.SetLive(rocket);
		}
		moveComponent.Launch(vector.Forward, vector.Zero, 1.0, rocket, null, launcher, null, launcher);
	}
};