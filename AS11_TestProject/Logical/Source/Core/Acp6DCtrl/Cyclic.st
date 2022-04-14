
PROGRAM _CYCLIC
	gAcp6DIf.Sts.AssemblyInfo := fbs.AsmInfoFb.AssemblyInfo;
	gAcp6DIf.Sts.AssemblyDisabled := fbs.AsmStatusFb.AssemblyDisabled;
	gAcp6DIf.Sts.AssemblyReady := fbs.AsmStatusFb.AssemblyReady;
	gAcp6DIf.Sts.AssemblyHoming := fbs.AsmStatusFb.AssemblyHoming;
	gAcp6DIf.Sts.AssemblyStopping := fbs.AsmStatusFb.AssemblyStopping;
	gAcp6DIf.Sts.AssemblyStartup:= fbs.AsmStatusFb.AssemblyStartUp;
	gAcp6DIf.Sts.AssemblyErrorStop := fbs.AsmStatusFb.AssemblyErrorStop;
	fbs.AsmStatusFb.Assembly := ADR(gAcp6DIf.Par.Asm);
	fbs.AsmStatusFb.Enable := TRUE;
	
	fbs.AsmInfoFb.Assembly := ADR(gAcp6DIf.Par.Asm);
	fbs.AsmInfoFb.Execute := TRUE AND NOT gAcp6DIf.Cmd.ResetInfo;
	
	CASE state OF
		// ***************************** Idle State
		ACP6D_IDLE:
			IF gAcp6DIf.Sts.AssemblyReady THEN
				fbs.PowerOfFb.Assembly := ADR(gAcp6DIf.Par.Asm);
				fbs.PowerOfFb.Execute := TRUE;
				
				state := ACP6D_POWER_OFF;
			ELSIF gAcp6DIf.Cmd.Power AND gAcp6DIf.Sts.AssemblyDisabled THEN
				fbs.PowerOnFb.Assembly := ADR(gAcp6DIf.Par.Asm);
				fbs.PowerOnFb.Execute := TRUE;
				
				state := ACP6D_POWER_ON;
			END_IF
			
		// ***************************** Power On State
		ACP6D_POWER_ON:
			IF fbs.PowerOnFb.Error THEN
				gAcp6DIf.Sts.Error := TRUE;
				errorState := state;
				
				state := ACP6D_ERROR;
			ELSIF fbs.PowerOnFb.Done THEN
				fbs.PowerOnFb.Execute := FALSE;
				
				fbs.AsmGetShuttleFb.Assembly := ADR(gAcp6DIf.Par.Asm);
				fbs.AsmGetShuttleFb.Enable := TRUE;
				
				//Reset all the info in the interfaces
				
				brsmemset(ADR(gShIfs),0,NUM_SHUTTLES * SIZEOF(gShIfs[0]));
				
				state := ACP6D_GET_SHUTTLE;
			END_IF
				
		// ***************************** Getting Shuttles State
		ACP6D_GET_SHUTTLE:
			IF fbs.AsmGetShuttleFb.Error THEN
				gAcp6DIf.Sts.Error := TRUE;
				errorState := state;
				
				state := ACP6D_ERROR;
			ELSIF fbs.AsmGetShuttleFb.Valid THEN
				gShIfs[fbs.AsmGetShuttleFb.TotalCount - (fbs.AsmGetShuttleFb.RemainingCount + 1)].Cfg.Shuttle := fbs.AsmGetShuttleFb.Shuttle;
				gShIfs[fbs.AsmGetShuttleFb.TotalCount - (fbs.AsmGetShuttleFb.RemainingCount + 1)].Cfg.ID := fbs.AsmGetShuttleFb.AdditionalInfo.ShuttleID;
				gShIfs[fbs.AsmGetShuttleFb.TotalCount - (fbs.AsmGetShuttleFb.RemainingCount + 1)].Sts.Available := TRUE;
				IF fbs.AsmGetShuttleFb.RemainingCount > 0 THEN
					fbs.AsmGetShuttleFb.Next := TRUE;
					state := ACP6D_NEXT_SHUTTLE;
				ELSE
					fbs.AsmGetShuttleFb.Enable := FALSE;
					
					fbs.MoveAsyncFb.Assembly := ADR(gAcp6DIf.Par.Asm);
					FOR i := 0 TO gAcp6DIf.Sts.AssemblyInfo.NumberOfShuttles - 1 DO
	  					gSysRecipe.StartupPars.MoveAsyncPars.AsyncPar[i].Shuttle := ADR(gShIfs[i].Cfg.Shuttle);
					END_FOR;
					fbs.MoveAsyncFb.Parameters := gSysRecipe.StartupPars.MoveAsyncPars;
					
					//fbs.MoveAsyncFb.Execute := TRUE;
					state := ACP6D_OPERATIONAL; //No longer need this when using Routing Library
				END_IF
			END_IF
			
		// ***************************** Find Next Shuttle State	
		ACP6D_NEXT_SHUTTLE:
			IF fbs.AsmGetShuttleFb.Error THEN
				gAcp6DIf.Sts.Error := TRUE;
				errorState := state;
				
				state := ACP6D_ERROR;
			ELSIF fbs.AsmGetShuttleFb.Valid THEN
				fbs.AsmGetShuttleFb.Next := FALSE;
				
				state := ACP6D_GET_SHUTTLE;
			END_IF
		// ***************************** Move to Startup Positions State
		ACP6D_MOVE_STARTUP:
			IF fbs.MoveAsyncFb.Error THEN
				gAcp6DIf.Sts.Error := TRUE;
				errorState := state;
				
				state := ACP6D_ERROR;
			ELSIF fbs.MoveAsyncFb.Done THEN
				fbs.MoveAsyncFb.Execute := FALSE;

				state := ACP6D_OPERATIONAL;
			END_IF
		// ***************************** Operational State
		ACP6D_OPERATIONAL:
			IF NOT gAcp6DIf.Cmd.Power THEN
				fbs.PowerOfFb.Assembly := ADR(gAcp6DIf.Par.Asm);
				fbs.PowerOfFb.Execute := TRUE;
				
				state := ACP6D_POWER_OFF;
			END_IF
				
		// ***************************** Powering Off State
		ACP6D_POWER_OFF:
			IF fbs.PowerOfFb.Error THEN
				gAcp6DIf.Sts.Error := TRUE;
				errorState := state;
				
				state := ACP6D_ERROR;
			ELSIF fbs.PowerOfFb.Done THEN
				fbs.PowerOfFb.Execute := FALSE;
				brsmemset(ADR(gShIfs),0,NUM_SHUTTLES * SIZEOF(gShIfs[0]));
				
				state := ACP6D_IDLE;
			END_IF
		
		// ***************************** Error State
		ACP6D_ERROR:
			IF gAcp6DIf.Cmd.Reset THEN
				state := ACP6D_RESET;
			END_IF
			
		// ***************************** Restting State
		ACP6D_RESET:
			fbs.AsmGetShuttleFb.Enable := FALSE;
			fbs.AsmGetShuttleFb.Next := FALSE;
			fbs.MoveAsyncFb.Execute := FALSE;
			fbs.MoveAsyncFb.Execute := FALSE;
			fbs.PowerOnFb.Execute := FALSE;
			fbs.PowerOfFb.Execute := FALSE;
			
			gAcp6DIf.Sts.Error := FALSE;
			
			state := ACP6D_IDLE;
	END_CASE;
		
	fbs.AsmInfoFb();
	fbs.PowerOnFb();
	fbs.PowerOfFb();
	fbs.AsmGetShuttleFb();
	fbs.MoveAsyncFb();
	fbs.AsmStatusFb();
	
	testFb.Assembly := ADR(g6dAssembly);
	testFb.Parameters.AsyncPar[0].Shuttle := ADR(gShIfs[0].Cfg.Shuttle);
	testFb.Parameters.AsyncPar[1].Shuttle := ADR(gShIfs[1].Cfg.Shuttle);
	testFb.Parameters.AsyncPar[2].Shuttle := ADR(gShIfs[2].Cfg.Shuttle);
	testFb();

END_PROGRAM
