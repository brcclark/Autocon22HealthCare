
TYPE
	Acp6DFbsTyp : 	STRUCT 
		PowerOnFb : MC_BR_AsmPowerOn_Acp6D;
		PowerOfFb : MC_BR_AsmPowerOff_Acp6D;
		AsmGetShuttleFb : MC_BR_AsmGetShuttle_Acp6D;
		MoveAsyncFb : MC_BR_MoveInPlaneAsync_Acp6D;
		AsmStatusFb : MC_BR_AsmReadStatus_Acp6D;
		AsmInfoFb : MC_BR_AsmGetInfo_Acp6D;
	END_STRUCT;
	Acp6DStateEnum : 
		(
		ACP6D_IDLE,
		ACP6D_POWER_ON,
		ACP6D_GET_SHUTTLE,
		ACP6D_NEXT_SHUTTLE,
		ACP6D_MOVE_STARTUP,
		ACP6D_OPERATIONAL,
		ACP6D_POWER_OFF,
		ACP6D_ERROR,
		ACP6D_RESET
		);
END_TYPE
