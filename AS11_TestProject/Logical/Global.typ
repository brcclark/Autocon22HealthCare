
TYPE
	gFBType : 	STRUCT  (*6D Interfaces*)
		In : Mc6DControllerFBIOType; (*Inputs*)
		Out : Mc6DControllerFBIOType; (*Outputs*)
	END_STRUCT;
	UserDataType : 	STRUCT 
		State : UserDataStateEnum;
		Pass : BOOL;
		Fail : BOOL;
	END_STRUCT;
	UserDataStateEnum : 
		(
		SHUTTLE_EMPTY,
		SHUTTLE_TRAY,
		SHUTTLE_BOTTOM_LOADED,
		SHUTTLE_ELECTRONICS_LOADED,
		SHUTTLE_ASSEMBLED,
		SHUTTLE_ASSEMBLED_IMAGING
		);
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
		StartupPars : StartupPars;
	END_STRUCT;
	StirDemoPars : 	STRUCT 
		Velocity : REAL;
		Accel : REAL;
		Decel : REAL;
		MoveCount : USINT;
		MoveList : ARRAY[0..maxMOVES_ARRAY]OF Acp6DMoveListTyp;
	END_STRUCT;
	StartupPars : 	STRUCT 
		MoveAsyncPars : McAcp6DMoveInPlaneAsyncParType;
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
	DestinationsEnum : 
		(
		DEST_DEFAULT,
		DEST_INCUBATOR,
		DEST_ANALYZER,
		DEST_PIPETTE_1,
		DEST_PIPETTE_2,
		DEST_TIPS,
		DEST_DYE
		);
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
END_TYPE
