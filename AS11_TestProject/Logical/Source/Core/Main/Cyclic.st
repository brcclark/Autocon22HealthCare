
PROGRAM _CYCLIC
	CASE state OF
		// ***************************** Getting Shuttles State
		MAIN_OFF:
		
			IF gSystemIf.Cmd.Power THEN
				gAcp6DIf.Cmd.Power := TRUE;
				
				state := MAIN_INIT;
			END_IF
		
		// ***************************** Initalization/Startup State
		MAIN_OFF:
			IF gAcp6DIf.Sts.Error THEN
				errorState := state;
				state := MAIN_ERROR;
			ELSIF gAcp6DIf.Sts.AssemblyReady THEN
				state := MAIN_READY;
			END_IF
		
		// ***************************** Ready State
		MAIN_READY:
	END_CASE;
	 
END_PROGRAM
