
TYPE
	Acp6dMoveBlockParsTyp : 	STRUCT 
		NumMoves : USINT;
		MoveList : REFERENCE TO ARRAY[0..1]OF Acp6DMoveListTyp;
	END_STRUCT;
	Acp6DMoveTypeEnum : 
		(
		acp6d_MOVE_LINEAR,
		acp6d_MOVE_ARC,
		acp6d_MOVE_SHORT,
		acp6d_MOVE_PAUSE
		);
	Acp6DMoveListTyp : 	STRUCT 
		MoveType : Acp6DMoveTypeEnum;
		Linear : McAcp6DInPlaneParType;
		Arc : McAcp6DArcParType;
		ShortAxis : McAcp6DShortAxParType;
		PauseTime : TIME;
	END_STRUCT;
	Acp6DMoveBlockInternalTyp : 	STRUCT 
		state : Acp6dMoveStatesEnum;
		errorState : Acp6dMoveStatesEnum;
		currentIndex : UINT;
		numMoves : USINT;
		movesList : ARRAY[0..maxMOVES_ARRAY]OF Acp6DMoveListTyp;
		fbs : Acp6DMoveInternalFbsTYp;
	END_STRUCT;
	Acp6DMoveInternalFbsTYp : 	STRUCT 
		LinearFb : MC_BR_MoveInPlane_Acp6D;
		ArcFb : MC_BR_MoveArc_Acp6D;
		ShortAxFb : MC_BR_MoveShortAxis_Acp6D;
		PauseFb : TON;
	END_STRUCT;
	Acp6dMoveStatesEnum : 
		(
		MV_INIT,
		MV_CHECK_CMD,
		MV_CHECK_FOR_LAST,
		MV_MOVE_LINEAR,
		MV_MOVE_ARC,
		MV_MOVE_SHORT,
		MV_PAUSE,
		MV_WAIT_FOR_LAST,
		MV_DONE,
		MV_ERROR,
		MV_RESET
		);
END_TYPE
