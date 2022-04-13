
FUNCTION_BLOCK Acp6DMoveBlockFb
	VAR_INPUT
		Execute : BOOL;
		Shuttle : {REDUND_UNREPLICABLE} Mc6DShuttleType;
		Pars : Acp6dMoveBlockParsTyp;
		WaitForLast : BOOL;
	END_VAR
	VAR_OUTPUT
		Busy : BOOL;
		Error : BOOL;
		ErrorID : UDINT;
		WaitingForLast : BOOL;
		Done : BOOL;
	END_VAR
	VAR
		internal : Acp6DMoveBlockInternalTyp;
	END_VAR
END_FUNCTION_BLOCK
