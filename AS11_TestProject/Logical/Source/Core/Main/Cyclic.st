
PROGRAM _CYCLIC
	CASE state OF
		// ***************************** Getting Shuttles State
		MAIN_OFF:
		
			IF gSystemIf.Cmd.Power THEN
				gAcp6DIf.Cmd.Power := TRUE;
				
				state := MAIN_INIT;
			END_IF
		
		// ***************************** Initalization/Startup State
		MAIN_INIT:
			IF gAcp6DIf.Sts.Error THEN
				errorState := state;
				state := MAIN_ERROR;
			ELSIF gAcp6DIf.Sts.AssemblyReady THEN
				IF gStationsIf.Sts.RoutingInitalized THEN
					gStationsIf.Cmd.Enable := TRUE;
				
					IF gStationsIf.Sts.IncubatorActive THEN
						gStationsIf.Cmd.Recover := TRUE;
						state := MAIN_RECOVERING;
					END_IF
				END_IF
			END_IF
		// ***************************** Recovering State
		MAIN_RECOVERING:
			IF NOT gStationsIf.Cmd.Recover AND gStationsIf.Sts.PipetteActive[0] AND
				gStationsIf.Sts.PipetteActive[1] THEN
				
				//gSystemIf.Cmd.Run := TRUE;
				state := MAIN_READY;
			END_IF
		// ***************************** Ready State
		MAIN_READY:
			IF gSystemIf.Cmd.Run THEN
				gStationsIf.Cmd.Run := TRUE;
				
				state := MAIN_STARTUP;
			END_IF
		MAIN_STARTUP:
		//Startup, Send 1 shuttle to Analyzer, send 1 shuttle to Incubator
		
		
	END_CASE;
	 
END_PROGRAM
