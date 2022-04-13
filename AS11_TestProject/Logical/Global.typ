
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
