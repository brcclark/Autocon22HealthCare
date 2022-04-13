
{REDUND_ERROR} FUNCTION_BLOCK rdi6DCore (*TODO: Add your comment here*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		AsmRef : REFERENCE TO Mc6DAssemblyType;
		Enable : BOOL;
		ErrorReset : BOOL;
		Parameters : REFERENCE TO Pars6D_typ;
		Power : BOOL;
		AsmStop : BOOL;
		ShStop : BOOL;
		AsmHalt : BOOL;
		ShHalt : BOOL;
		AsmResume : BOOL;
		ShResume : BOOL;
		AsmLevitation : BOOL;
		ShLevitation : BOOL;
		AsmBuffer : BOOL;
		ShBuffer : BOOL;
		ShuttleID : UINT;
		Reboot : BOOL;
		MoveLinear : BOOL;
		MoveArc : BOOL;
		MoveShort : BOOL;
		MoveAsync : BOOL;
		Move6DOF : BOOL;
		ShRecover : BOOL;
	END_VAR
	VAR_OUTPUT
		Active : BOOL;
		Error : BOOL;
		ErrorID : DINT;
		PowerOn : BOOL;
		MovementActive : BOOL;
		AllShuttlesFound : BOOL;
		ValidShuttleSelected : BOOL;
		ShuttleArray : ARRAY[0..MAX_NUM_SHUTTLES_M1] OF ShuttleInfo_typ;
		AsmStatus : AsmStatus_typ;
		ShuttleBufferInfo : ShuttleBufferInfo_typ;
		CurrentState : STRING[80];
		StatusMessage : STRING[80];
	END_VAR
	VAR
		j : UINT;
		i : UINT;
		Internal : Internal6DCore_typ;
		zzEdge00000 : BOOL;
		zzEdge00001 : BOOL;
		zzEdge00002 : BOOL;
		zzEdge00003 : BOOL;
		zzEdge00004 : BOOL;
		zzEdge00005 : BOOL;
		zzEdge00006 : BOOL;
		zzEdge00007 : BOOL;
		zzEdge00008 : BOOL;
		zzEdge00009 : BOOL;
		zzEdge00010 : BOOL;
		zzEdge00011 : BOOL;
		zzEdge00012 : BOOL;
		zzEdge00013 : BOOL;
		zzEdge00014 : BOOL;
		zzEdge00015 : BOOL;
		zzEdge00016 : BOOL;
		zzEdge00017 : BOOL;
	END_VAR
END_FUNCTION_BLOCK
