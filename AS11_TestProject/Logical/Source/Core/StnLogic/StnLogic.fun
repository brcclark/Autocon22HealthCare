
{REDUND_ERROR} FUNCTION_BLOCK stnPipette (*TODO: Add your comment here*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Destination : REFERENCE TO rl6dDestinationType;
		InitialId : USINT;
		FirstDestination : REFERENCE TO rl6dDestinationType;
		Enable : BOOL;
		Operate : BOOL;
		Run : BOOL;
		Send : BOOL;
		NextDestination : REFERENCE TO rl6dDestinationType;
		Waypoint : REFERENCE TO rl6dWaypointType;
		WellPlateInfo : WellPlateInfoTyp;
		Velocity : REAL;
		Accel : REAL;
		DyePresent : BOOL;
		WaitTime : UDINT;
	END_VAR
	VAR_OUTPUT
		Active : BOOL;
		Error : BOOL;
		ErrorID : DINT;
		ShuttlePresent : BOOL;
		ShUserData : UserDataType;
		Shuttle : rl6dShuttleType;
		OperateDone : USINT;
		WaitComplete : BOOL;
	END_VAR
	VAR
		internal : stnPipetteIntTyp;
	END_VAR
END_FUNCTION_BLOCK

{REDUND_ERROR} FUNCTION_BLOCK stnGeneric (*TODO: Add your comment here*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Destination : REFERENCE TO rl6dDestinationType;
		Enable : BOOL;
		Run : BOOL;
		Send : BOOL;
		ResetData : BOOL;
		ShuttleType : ShuttleType;
		Recipe : FillRecipeTyp;
		ShColor : ColorEnum;
		ApplyRecipe : BOOL;
		NextDestination : REFERENCE TO rl6dDestinationType;
		Waypoint : REFERENCE TO rl6dWaypointType;
		Velocity : REAL;
		Accel : REAL;
		WaitTime : DINT;
	END_VAR
	VAR_OUTPUT
		Active : BOOL;
		Error : BOOL;
		ErrorID : UDINT;
		ShuttlePresent : BOOL;
		ShUserData : UserDataType;
		WaitComplete : BOOL;
	END_VAR
	VAR
		internal : stnGenericIntTyp;
	END_VAR
END_FUNCTION_BLOCK
