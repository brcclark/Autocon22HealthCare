
TYPE
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
	ShuttleType : 
		(
		SH_EMPTY,
		SH_WELL,
		SH_SAMPLE,
		SH_DYE,
		SH_TIPS
		);
	UserDataType : 	STRUCT 
		shId : USINT;
		ShType : ShuttleType;
		CurrentColor : ColorEnum;
		Recipe : FillRecipeTyp;
		Wells : ARRAY[0..17]OF WellTyp;
	END_STRUCT;
	WellTyp : 	STRUCT 
		Present : BOOL;
		Color : ColorEnum;
	END_STRUCT;
	ColorEnum : 
		(
		COLOR_UNDEF,
		COLOR_BLACK,
		COLOR_RED,
		COLOR_PINK,
		COLOR_DARK_BLUE,
		COLOR_LIGHT_BLUE,
		COLOR_DARK_GREEN,
		COLOR_LIGHT_GREEN,
		COLOR_DARK_YELLOW,
		COLOR_LIGHT_YELLOW,
		COLOR_GRAY,
		COLOR_WHITE,
		COLOR_DARK_PURPLE,
		COLOR_LIGHT_PURPLE,
		COLOR_DARK_ORANGE,
		COLOR_LIGHT_ORANGE
		);
END_TYPE

(*Fill Recipe Types*)

TYPE
	FillRecipeTyp : 	STRUCT 
		NumberSamples : USINT;
		CurrentSample : USINT;
		SampleWellCount : USINT;
		DyeWellCount : USINT;
		Samples : ARRAY[0..4]OF FillSampleTyp;
	END_STRUCT;
	FillSampleTyp : 	STRUCT 
		Sample : ColorEnum;
		SampleCount : USINT;
		CurrentCount : USINT;
	END_STRUCT;
	WellPlateInfoTyp : 	STRUCT 
		NumRows : USINT;
		NumColumns : USINT;
		RowGap : REAL;
		ColumnGap : REAL;
		OffsetX : REAL;
		OffsetY : REAL;
	END_STRUCT;
END_TYPE

(*Generic Station Types*)

TYPE
	stnGenericIntTyp : 	STRUCT 
		state : stnGenericEnumTyp;
		errorState : stnGenericEnumTyp;
		fbs : stnGenericFbTyp;
	END_STRUCT;
	stnGenericEnumTyp : 
		(
		STN_GENERIC_OFF,
		STN_GENERIC_INIT,
		STN_GENERIC_READY,
		STN_GENERIC_PROCESS,
		STN_GENERIC_WAIT_FOR_CMD,
		STN_GENERIC_SEND,
		STN_GENERIC_DONE,
		STN_GENERIC_ERROR,
		STN_GENERIC_RESET
		);
	stnGenericFbTyp : 	STRUCT 
		destMonFb : rl6dDestinationMonitor;
		moveDestFb : rl6dShuttleMoveDestination;
		waitFb : TON;
	END_STRUCT;
END_TYPE

(*Pipette Station Types*)

TYPE
	stnPipetteIntTyp : 	STRUCT 
		state : stnPipetteEnum;
		errorState : stnPipetteEnum;
		fillState : stnPipetteFillStatesEnum;
		fillPos : stnPipetteFillGridPosTyp;
		fbs : stnPipetteFbsTyp;
	END_STRUCT;
	stnPipetteEnum : 
		(
		STN_PIPETTE_OFF,
		STN_PIPETTE_INIT,
		STN_PIPETTE_WAIT_FIRST,
		STN_PIPETTE_SEND_FIRST,
		STN_PIPETTE_READY,
		STN_PIPETTE_DO_WAIT,
		STN_PIPETTE_PROCESS,
		STN_PIPETTE_WAIT_FOR_CMD,
		STN_PIPETTE_SEND,
		STN_PIPETTE_DONE,
		STN_PIPETTE_ERROR,
		STN_PIPETTE_RESET
		);
	stnPipetteFbsTyp : 	STRUCT 
		destMonFb : rl6dDestinationMonitor;
		moveDestFb : rl6dShuttleMoveDestination;
		waitFb : TON;
		moveInPlaneFb : MC_BR_MoveInPlane_Acp6D;
		moveArcFb : MC_BR_MoveArc_Acp6D;
	END_STRUCT;
	stnPipetteFillStatesEnum : 
		(
		FILL_MOVE,
		FILL_FILL,
		FILL_SWIRL,
		FILL_CHECK_RECIPE
		);
	stnPipetteFillGridPosTyp : 	STRUCT 
		X : USINT;
		Y : USINT;
	END_STRUCT;
END_TYPE
