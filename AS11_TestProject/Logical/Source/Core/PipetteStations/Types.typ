
TYPE
	PipetteStationStateEnum : 
		(
		PIP_OFF,
		PIP_STARTUP,
		PIP_READY,
		PIP_REQUEST_TIP,
		PIP_CHANGE_TIP,
		PIP_REQUEST_SAMPLE,
		PIP_WAIT_SAMPLE,
		PIP_OPERATE_SAMPLE,
		PIP_REQUEST_SHUTTLE,
		PIP_WAIT_WELL_TO_ARRIVE,
		PIP_PROCESS_SHUTTLE,
		PIP_REQUEST_DYE,
		PIP_PROCESS_DYE,
		PIP_CHECK_RECIPE,
		PIP_SEND_ANALYZER,
		PIP_ERROR
		);
	PipetteIfTyp : 	STRUCT 
		TipNeedReplace : BOOL;
		DyePresent : BOOL;
		SamplePresent : BOOL;
		shuttleIf : rl6dShuttleType;
		ShuttleOperating : BOOL;
		CurrentSample : ColorEnum;
		NextSample : ColorEnum;
		PipettePosition : REAL;
	END_STRUCT;
END_TYPE
