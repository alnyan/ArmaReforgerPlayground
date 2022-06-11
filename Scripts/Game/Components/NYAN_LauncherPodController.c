class NYAN_LaunchPodControllerClass: ScriptComponentClass {};

class NYAN_LaunchPodController: ScriptComponent {
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
		
		float dx = Math.Clamp(m_directionHeading - m_rtHeading, -10, 10);
		float dy = Math.Clamp(m_directionElevation - m_rtElevation, -10, 10);
		
		m_rtHeading += timeSlice * dx;	
		m_rtElevation += timeSlice * dy;

		m_signalsComponent.SetSignalValue(m_signalAimTurnTurret, m_rtHeading);
		m_signalsComponent.SetSignalValue(m_signalAimElevationTurret, m_rtElevation);
	}
	
	void SetTargetHeadingElevation(float heading, float elevation) {
		m_directionHeading = heading;
		m_directionElevation = elevation;
	}
};