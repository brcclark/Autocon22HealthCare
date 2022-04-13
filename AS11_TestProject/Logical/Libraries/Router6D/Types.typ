
TYPE
	rl6dPositionType : 	STRUCT  (*Position on ACOPOS 6D Assembly*)
		X : REAL := 0; (*X Position*)
		Y : REAL := 0; (*Y Position*)
	END_STRUCT;
	rl6dRouterType : 	STRUCT  (*Router handle*)
		Ident : UDINT := 0; (*Router Ident*)
	END_STRUCT;
	rl6dPathType : 	STRUCT  (*Path handle*)
		Ident : UDINT := 0; (*Path Ident*)
	END_STRUCT;
	rl6dDestinationType : 	STRUCT  (*Destination handle*)
		Ident : UDINT := 0; (*Destination Ident*)
	END_STRUCT;
	rl6dWaypointType : 	STRUCT  (*Waypoint handle*)
		Ident : UDINT; (*Waypoint Ident*)
	END_STRUCT;
	rl6dShuttleType : 	STRUCT  (*Shuttle handle*)
		Ident : UDINT := 0; (*Shuttle Ident*)
	END_STRUCT;
	rl6dRouterManInternalType : 	STRUCT  (*Internal Data*)
		State : USINT := 0; (*Router state*)
	END_STRUCT;
	rl6dRouterAddPathInternalType : 	STRUCT  (*Internal Data*)
		State : USINT := 0; (*FB state*)
		Reference : UDINT := 0; (*Reference*)
	END_STRUCT;
	rl6dGetPathInternalType : 	STRUCT  (*Internal Data*)
		State : USINT := 0; (*FB state*)
		DataAddress : UDINT := 0; (*Recorded Data Address*)
		DataSize : UDINT := 0; (*Recorded Data Size*)
	END_STRUCT;
	rl6dRouterAddDestInternalType : 	STRUCT  (*Internal Data*)
		State : USINT := 0; (*FB state*)
		Reference : UDINT := 0; (*Reference*)
	END_STRUCT;
	rl6dDestAddRecPosInternalType : 	STRUCT  (*Internal Data*)
		State : USINT := 0; (*FB state*)
	END_STRUCT;
	rl6dDestSetWaypntInternalType : 	STRUCT  (*Internal Data*)
		State : USINT := 0; (*FB state*)
	END_STRUCT;
	rl6dRouterAddWaypntInternalType : 	STRUCT  (*Internal Data*)
		State : USINT := 0; (*FB state*)
		Reference : UDINT := 0; (*Reference*)
	END_STRUCT;
	rl6dWaypointAddDestInternalType : 	STRUCT  (*Internal Data*)
		State : USINT := 0; (*FB state*)
	END_STRUCT;
	rl6dWaypntSetWaypntInternalType : 	STRUCT  (*Internal Data*)
		State : USINT := 0; (*FB state*)
	END_STRUCT;
	rl6dRtrAddShFromAsmInternalType : 	STRUCT  (*Internal Data*)
		State : USINT := 0; (*State of execution*)
	END_STRUCT;
	rl6dRecoverModeEnum : 
		( (*Method to use for recovery*)
		rl6dRECOVER_MODE_BASIC (*Basic recover mode*)
		);
	rl6dDestinationMonInternalType : 	STRUCT  (*Internal Data*)
		State : USINT := 0; (*FB state*)
		Reference : UDINT := 0; (*Reference*)
	END_STRUCT;
	rl6dShMoveDestInternalType : 	STRUCT  (*Internal Data*)
		State : USINT := 0; (*FB state*)
		Reference : UDINT := 0; (*Reference*)
	END_STRUCT;
	rl6dShCopyUserDataInternalType : 	STRUCT  (*Internal Data*)
		State : USINT;
	END_STRUCT;
	rl6dShCopyUserDataModeEnum : 
		( (*Mode of shuttle copy user data*)
		rl6dSH_COPY_USER_DATA_READ, (*Read user data*)
		rl6dSH_COPY_USER_DATA_WRITE (*Write user data*)
		);
END_TYPE
