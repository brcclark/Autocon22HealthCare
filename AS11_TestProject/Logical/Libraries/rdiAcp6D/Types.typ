(*Parameter structures*)

TYPE
	Pars6D_typ : 	STRUCT 
		Linear : McAcp6DInPlaneParType;
		Arc : McAcp6DArcParType;
		ShortAxis : McAcp6DShortAxParType;
		Async : MoveAsync_typ;
		SixDOF : McAcp6DMove6DParType;
		Levitation : McAcp6DLevitationParType;
		Buffer : UINT;
	END_STRUCT;
	MoveAsync_typ : 	STRUCT 
		Velocity : REAL;
		Acceleration : REAL;
		ShuttleTable : ARRAY[0..rdiMAX_ASYNC_SHUTTLES_M1]OF AsyncShuttle_typ;
	END_STRUCT;
	AsyncShuttle_typ : 	STRUCT 
		X : REAL;
		Y : REAL;
		ShuttleID : UINT;
	END_STRUCT;
END_TYPE

(*Output structures*)

TYPE
	AsmStatus_typ : 	STRUCT 
		AssemblyDisabled : BOOL;
		AssemblyReady : BOOL;
		AssemblyHoming : BOOL;
		AssemblyStopping : BOOL;
		AssemblyStartUp : BOOL;
		AssemblyErrorStop : BOOL;
	END_STRUCT;
	ShuttleInfo_typ : 	STRUCT 
		Axis : Mc6DShuttleType;
		ShuttleID : UINT;
		State : McAcp6DShStateEnum;
		Position : McAcp6DShPositionType;
	END_STRUCT;
	ShuttleBufferInfo_typ : 	STRUCT 
		ShuttleID : UINT;
		State : McAcp6DBufferStateEnum;
		NumberOfCommands : UINT;
	END_STRUCT;
END_TYPE

(*Internal 6D function block organization*)

TYPE
	Internal6DCore_typ : 	STRUCT 
		State : Core6DState_enum;
		Status : Internal6DCoreStatus_typ;
		FB : FB6D_typ;
	END_STRUCT;
	Core6DState_enum : 
		(
		rdi_VALIDATE_ASSEMBLY_REF,
		rdi_ENABLE_ASM_READ_STATUS,
		rdi_WAIT_FOR_POWER_ON,
		rdi_GET_ASM_INFO,
		rdi_VALIDATE_GET_SHUTTLES,
		rdi_GET_SHUTTLES,
		rdi_GET_SHUTTLES_NEXT,
		rdi_GET_SHUTTLE_INFO,
		rdi_CHECK_SHUTTLE_INFO,
		rdi_WAIT_FOR_COMMAND,
		rdi_MOVE_LINEAR,
		rdi_MOVE_ARC,
		rdi_MOVE_SHORT,
		rdi_MOVE_6DOF,
		rdi_MOVE_ASYNC,
		rdi_ASM_STOP,
		rdi_SHUTTLE_STOP,
		rdi_ASM_HALT,
		rdi_SHUTTLE_HALT,
		rdi_ASM_LEVITATION,
		rdi_SHUTTLE_LEVITATION,
		rdi_ASM_BUFFER,
		rdi_SHUTTLE_BUFFER,
		rd_SHUTTLE_BUFFER_STATUS,
		rdi_ASM_REBOOT,
		rdi_POWER_OFF,
		rdi_SHUTTLE_RECOVER,
		rdi_ERROR
		);
	Internal6DCoreStatus_typ : 	STRUCT 
		AsmGetInfoShuttleCount : UINT;
		GetShuttleCount : USINT;
		OldShuttleID : UINT;
		SelectedShuttleAxis : Mc6DShuttleType;
	END_STRUCT;
	FB6D_typ : 	STRUCT 
		Asm : AsmFB6D_typ;
		Stop : StopFB6D_typ;
		Halt : HaltFB6D_typ;
		Move : MoveFB6D_typ;
		Shuttle : ShuttleFB6D_typ;
		Levitation : LevitationFB6D_typ;
		Buffer : BufferFB6D_typ;
	END_STRUCT;
	AsmFB6D_typ : 	STRUCT 
		AsmPowerOn : MC_BR_AsmPowerOn_Acp6D;
		AsmPowerOff : MC_BR_AsmPowerOff_Acp6D;
		AsmReboot : MC_BR_AsmReboot_Acp6D;
		AsmGetInfo : MC_BR_AsmGetInfo_Acp6D;
		AsmReadStatus : MC_BR_AsmReadStatus_Acp6D;
		AsmGetShuttle : MC_BR_AsmGetShuttle_Acp6D;
	END_STRUCT;
	StopFB6D_typ : 	STRUCT 
		AsmStop : MC_BR_AsmStop_Acp6D;
		ShStop : MC_BR_ShStop_Acp6D;
	END_STRUCT;
	HaltFB6D_typ : 	STRUCT 
		AsmHalt : MC_BR_AsmHalt_Acp6D;
		ShHalt : MC_BR_ShHalt_Acp6D;
	END_STRUCT;
	MoveFB6D_typ : 	STRUCT 
		Linear : MC_BR_MoveInPlane_Acp6D;
		Arc : MC_BR_MoveArc_Acp6D;
		Short : MC_BR_MoveShortAxis_Acp6D;
		Async : MC_BR_MoveInPlaneAsync_Acp6D;
		SixDOF : MC_BR_Move6D_Acp6D;
	END_STRUCT;
	ShuttleFB6D_typ : 	STRUCT 
		ShReadInfo : ARRAY[0..MAX_NUM_SHUTTLES_M1]OF MC_BR_ShReadInfo_Acp6D;
		ShRecover : MC_BR_ShRecover_Acp6D;
	END_STRUCT;
	LevitationFB6D_typ : 	STRUCT 
		AsmLevitation : MC_BR_AsmLevitation_Acp6D;
		ShLevitation : MC_BR_ShLevitation_Acp6D;
	END_STRUCT;
	BufferFB6D_typ : 	STRUCT 
		AsmBuffer : MC_BR_AsmBuffer_Acp6D;
		ShBuffer : MC_BR_ShBuffer_Acp6D;
	END_STRUCT;
END_TYPE
