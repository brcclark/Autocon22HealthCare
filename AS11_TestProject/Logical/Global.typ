
TYPE
	gFBType : 	STRUCT  (*6D Interfaces*)
		In : Mc6DControllerFBIOType; (*Inputs*)
		Out : Mc6DControllerFBIOType; (*Outputs*)
	END_STRUCT;
END_TYPE

(************************* Main If Typ*)

TYPE
	MainIfTyp : 	STRUCT 
		Cmd : MainIfCmdTyp;
		Par : USINT;
		Sts : USINT;
	END_STRUCT;
	MainIfCmdTyp : 	STRUCT 
		Power : BOOL;
		Run : BOOL;
		ErrorReset : BOOL;
	END_STRUCT;
	SystemRecipeIfTyp : 	STRUCT 
		DefaultVel : REAL;
		DefaultAccel : REAL;
		DefaultWaitTime : UDINT;
		StartupPars : StartupPars;
		WellRecipe : ARRAY[0..1]OF FillRecipeTyp;
		PipettePars : PipettePars;
	END_STRUCT;
	StartupPars : 	STRUCT 
		MoveAsyncPars : McAcp6DMoveInPlaneAsyncParType;
	END_STRUCT;
	PipettePars : 	STRUCT 
		HomePosition : REAL;
		Vel : REAL;
		SampleDistance : REAL;
		WellDistance : REAL;
	END_STRUCT;
END_TYPE

(************************* Acp6D Types*)

TYPE
	Acp6DIfTyp : 	STRUCT 
		Cmd : Acp6DCmdsTyp;
		Par : Acp6DParTyp;
		Sts : Acp6DStsTyp;
	END_STRUCT;
	Acp6DCmdsTyp : 	STRUCT 
		Power : BOOL;
		Reset : BOOL;
		ResetInfo : BOOL;
	END_STRUCT;
	Acp6DParTyp : 	STRUCT 
		ShuttleCount : USINT;
		Asm : Mc6DAssemblyType;
	END_STRUCT;
	Acp6DStsTyp : 	STRUCT 
		Error : BOOL;
		AssemblyInfo : McAcp6DAsmGetInfoType;
		AssemblyDisabled : BOOL;
		AssemblyReady : BOOL;
		AssemblyHoming : BOOL;
		AssemblyStopping : BOOL;
		AssemblyStartup : BOOL;
		AssemblyErrorStop : BOOL;
	END_STRUCT;
END_TYPE

(************************** Shuttle If Types*)

TYPE
	ShuttleIfTyp : 	STRUCT 
		Cmd : ShuttleIfCmdTyp;
		Par : ShuttleIfParTyp;
		Cfg : ShuttleIfCfgTyp;
		Sts : ShuttleIfStsTyp;
	END_STRUCT;
	ShuttleIfCmdTyp : 	STRUCT 
		Reset : BOOL;
	END_STRUCT;
	ShuttleIfParTyp : 	STRUCT 
		New_Member : USINT;
	END_STRUCT;
	ShuttleIfCfgTyp : 	STRUCT 
		ID : UINT;
		Shuttle : Mc6DShuttleType;
		RouterShuttle : rl6dShuttleType;
	END_STRUCT;
	ShuttleIfStsTyp : 	STRUCT 
		Available : BOOL;
		Error : BOOL;
		ErrorID : UDINT;
	END_STRUCT;
END_TYPE

(*Routing Helper Types*)

TYPE
	DestTyp : 	STRUCT 
		StationName : STRING[80];
		Position : rl6dPositionType;
		Entry : DestPathTyp;
		Exit : DestPathTyp;
		RecoveryPts : rl6dPositionType;
		Destination : rl6dDestinationType;
	END_STRUCT;
	DestPathTyp : 	STRUCT 
		Path : rl6dPathType;
		PathPoints : ARRAY[0..2]OF rl6dPositionType;
		Name : STRING[80];
		Length : USINT;
	END_STRUCT;
	WaypointTyp : 	STRUCT 
		Position : rl6dPositionType;
		Name : STRING[80];
		Waypoint : rl6dWaypointType;
		Destinations : ARRAY[0..5]OF DestinationsEnum;
		NextWaypoint : WaypointListEnum;
	END_STRUCT;
	WaypointListEnum : 
		(
		WP_NONE,
		WP_1,
		WP_2,
		WP_3,
		WP_4,
		WP_5
		);
	StationsIfTyp : 	STRUCT 
		Cmd : StationsCmdTyp;
		Par : StationsParTyp;
		Sts : StationsStsTyp;
	END_STRUCT;
	StationsCmdTyp : 	STRUCT 
		Enable : BOOL;
		Run : BOOL;
		Recover : BOOL;
		RequestIncubator : BOOL;
		RequestPipette : ARRAY[0..1]OF BOOL;
		RequestDye : BOOL;
		RequestTip : BOOL;
		RequestFreshShuttle : BOOL;
		RequestAnalyzer : BOOL;
	END_STRUCT;
	StationsParTyp : 	STRUCT 
		Incubator : IncubatorParTyp;
		Analyzer : IncubatorParTyp;
		Pipette : ARRAY[0..1]OF PipetteParTyp;
		Dye : DyeTipParTyp;
		Tip : DyeTipParTyp;
	END_STRUCT;
	PipetteParTyp : 	STRUCT 
		NextStation : DestinationsEnum;
		Waypoint : WaypointListEnum;
		ShuttleType : ShuttleType;
	END_STRUCT;
	DyeTipParTyp : 	STRUCT 
		RequestDestination : DestinationsEnum;
		Waypoint : WaypointListEnum;
	END_STRUCT;
	IncubatorParTyp : 	STRUCT 
		SampleType : ColorEnum;
		NextStation : DestinationsEnum;
		Waypoint : WaypointListEnum;
	END_STRUCT;
	StationsStsTyp : 	STRUCT 
		RoutingInitalized : BOOL;
		IncubatorShuttlePresent : BOOL;
		IncubatorActive : BOOL;
		PipetteActive : ARRAY[0..1]OF BOOL;
		PipetteNeedSample : ARRAY[0..1]OF BOOL;
		PipetteShuttlePresent : ARRAY[0..1]OF BOOL;
		PipetteReadyForShuttle : ARRAY[0..1]OF BOOL;
		TipsShuttlePresent : BOOL;
		DyeShuttlePresent : BOOL;
		AnalyzerShuttlePresent : BOOL;
	END_STRUCT;
	AnalyzerParTyp : 	STRUCT 
		NextStation : DestinationsEnum;
		Waypoint : WaypointListEnum;
		PipetteIndex : USINT;
	END_STRUCT;
END_TYPE
