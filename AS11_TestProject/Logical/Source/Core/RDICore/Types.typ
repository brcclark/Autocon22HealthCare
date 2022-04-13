
TYPE
	Vis6D_typ : 	STRUCT 
		Cmd : Vis6DCmd_typ;
		Par : Pars6D_typ;
		Status : Vis6DStatus_typ;
		TableData : VisTableData_typ;
		Limits : Vis6DLimits_typ;
	END_STRUCT;
	Vis6DCmd_typ : 	STRUCT 
		ErrorReset : BOOL;
		Power : BOOL;
		AsmStop : BOOL;
		ShStop : BOOL;
		AsmHalt : BOOL;
		ShHalt : BOOL;
		AsmResume : BOOL;
		ShResume : BOOL;
		ShuttleID : UINT;
		RecoverShuttle : BOOL;
		MoveLinear : BOOL;
		MoveArc : BOOL;
		MoveShort : BOOL;
		MoveAsync : BOOL;
		Move6DOF : BOOL;
		AsmLevitation : BOOL;
		ShLevitation : BOOL;
		AsmBuffer : BOOL;
		ShBuffer : BOOL;
		Reboot : BOOL;
		ShRecover : BOOL;
	END_STRUCT;
	Vis6DStatus_typ : 	STRUCT 
		AsmStatus : AsmStatus_typ;
		AllShuttlesFound : BOOL;
		StatusMessage : STRING[80];
		CurrentState : STRING[80];
		ErrorID : DINT;
		SelectedShuttleIDString : STRING[80];
		MovementActive : BOOL;
		ShuttleBuffer : VisShuttleBufferStatus_typ;
		ErrorText : STRING[80];
	END_STRUCT;
	VisShuttleBufferStatus_typ : 	STRUCT 
		ShuttleID : UINT;
		State : STRING[80];
		NumberOfCommands : UINT;
	END_STRUCT;
	VisTableData_typ : 	STRUCT 
		Async : VisAsyncTable_typ;
		ShuttleStatus : VisShuttleStatusTable_typ;
	END_STRUCT;
	VisAsyncTable_typ : 	STRUCT 
		TargetXArray : ARRAY[0..rdiMAX_ASYNC_SHUTTLES_M1]OF REAL;
		TargetYArray : ARRAY[0..rdiMAX_ASYNC_SHUTTLES_M1]OF REAL;
		ShuttleIDArray : ARRAY[0..rdiMAX_ASYNC_SHUTTLES_M1]OF UINT;
	END_STRUCT;
	VisShuttleStatusTable_typ : 	STRUCT 
		ShuttleID : ARRAY[0..MAX_NUM_SHUTTLES_M1]OF UINT;
		ShuttleIDString : ARRAY[0..MAX_NUM_SHUTTLES_M1]OF STRING[80];
		ShuttleStateText : ARRAY[0..MAX_NUM_SHUTTLES_M1]OF STRING[80];
		X : ARRAY[0..MAX_NUM_SHUTTLES_M1]OF REAL;
		Y : ARRAY[0..MAX_NUM_SHUTTLES_M1]OF REAL;
		Z : ARRAY[0..MAX_NUM_SHUTTLES_M1]OF REAL;
		Rx : ARRAY[0..MAX_NUM_SHUTTLES_M1]OF REAL;
		Ry : ARRAY[0..MAX_NUM_SHUTTLES_M1]OF REAL;
		Rz : ARRAY[0..MAX_NUM_SHUTTLES_M1]OF REAL;
	END_STRUCT;
END_TYPE

(*Insert your comment here.*)

TYPE
	Vis6DRealMinMax_typ : 	STRUCT 
		Min : REAL;
		Max : REAL;
	END_STRUCT;
	Vis6DLimits_typ : 	STRUCT 
		Linear : Vis6DLimitsLinear_typ;
		Arc : Vis6DLimitsArc_typ;
		Short : Vis6DLimitsShort_typ;
		SixDOF : Vis6DLimits6DOF_typ;
		Async : Vis6DLimitsAsync_typ;
	END_STRUCT;
	Vis6DLimitsLinear_typ : 	STRUCT 
		X : Vis6DRealMinMax_typ;
		Y : Vis6DRealMinMax_typ;
		Velocity : Vis6DRealMinMax_typ;
		EndVelocity : Vis6DRealMinMax_typ;
		Acceleration : Vis6DRealMinMax_typ;
	END_STRUCT;
	Vis6DLimitsArc_typ : 	STRUCT 
		AngleX : Vis6DRealMinMax_typ;
		AngleY : Vis6DRealMinMax_typ;
		Angle : Vis6DRealMinMax_typ;
		RadiusX : Vis6DRealMinMax_typ;
		RadiusY : Vis6DRealMinMax_typ;
		Radius : Vis6DRealMinMax_typ;
		Velocity : Vis6DRealMinMax_typ;
		EndVelocity : Vis6DRealMinMax_typ;
		Acceleration : Vis6DRealMinMax_typ;
	END_STRUCT;
	Vis6DLimitsShort_typ : 	STRUCT 
		CenterX : Vis6DRealMinMax_typ;
		CenterY : Vis6DRealMinMax_typ;
		Z : Vis6DRealMinMax_typ;
		Rx : Vis6DRealMinMax_typ;
		Ry : Vis6DRealMinMax_typ;
		Rz : Vis6DRealMinMax_typ;
		VelZ : Vis6DRealMinMax_typ;
		VelRx : Vis6DRealMinMax_typ;
		VelRy : Vis6DRealMinMax_typ;
		VelRz : Vis6DRealMinMax_typ;
	END_STRUCT;
	Vis6DLimits6DOF_typ : 	STRUCT 
		X : Vis6DRealMinMax_typ;
		Y : Vis6DRealMinMax_typ;
		Velocity : Vis6DRealMinMax_typ;
		Acceleration : Vis6DRealMinMax_typ;
		Z : Vis6DRealMinMax_typ;
		Rx : Vis6DRealMinMax_typ;
		Ry : Vis6DRealMinMax_typ;
		Rz : Vis6DRealMinMax_typ;
		VelZ : Vis6DRealMinMax_typ;
		VelRx : Vis6DRealMinMax_typ;
		VelRy : Vis6DRealMinMax_typ;
		VelRz : Vis6DRealMinMax_typ;
	END_STRUCT;
	Vis6DLimitsAsync_typ : 	STRUCT 
		Velocity : Vis6DRealMinMax_typ;
		Acceleration : Vis6DRealMinMax_typ;
		X : Vis6DRealMinMax_typ;
		Y : Vis6DRealMinMax_typ;
	END_STRUCT;
END_TYPE
