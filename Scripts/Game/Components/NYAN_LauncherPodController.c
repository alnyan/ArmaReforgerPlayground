class NYAN_LaunchPodControllerClass: ScriptComponentClass {};

class NYAN_LaunchPodController: ScriptComponent {
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
};