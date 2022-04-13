
(* TODO: Add your comment here *)
FUNCTION_BLOCK Acp6DMoveBlockFb
	
	CASE internal.state OF
		// ***************************** Init State
		MV_INIT:
			IF Execute THEN
				IF Pars.NumMoves > maxMOVES THEN
					internal.errorState := internal.state;
					internal.state := MV_ERROR;
					Error := TRUE;
					ErrorID := -1;
				ELSIF Pars.NumMoves = 0 THEN
					Done := TRUE;
					internal.state := MV_DONE;
				ELSE
					internal.currentIndex := 0;	
					internal.numMoves := Pars.NumMoves;
					brsmemcpy(ADR(internal.movesList),Pars.MoveList,internal.numMoves * SIZEOF(internal.movesList[0]));
					internal.state := MV_CHECK_CMD;
				END_IF
			ELSE
				Error := FALSE;
				ErrorID := 0;
				WaitingForLast := FALSE;
				Done := FALSE;
			END_IF
			
		// ***************************** Checking Cmd of Current Parameter
		MV_CHECK_CMD:
			IF internal.currentIndex = internal.numMoves THEN
				IF WaitForLast THEN
					internal.state := MV_WAIT_FOR_LAST;
				ELSE
				Done := TRUE;
					internal.state := MV_DONE;
				END_IF
			ELSE
				CASE internal.movesList[internal.currentIndex].MoveType OF
					acp6d_MOVE_LINEAR:
						internal.fbs.LinearFb.Shuttle := ADR(Shuttle);
						internal.fbs.LinearFb.Parameters := internal.movesList[internal.currentIndex].Linear;
						internal.fbs.LinearFb.Execute := TRUE;
						
						internal.state := MV_MOVE_LINEAR;
					acp6d_MOVE_ARC:
						internal.fbs.ArcFb.Shuttle := ADR(Shuttle);
						internal.fbs.ArcFb.Parameters := internal.movesList[internal.currentIndex].Arc;
						internal.fbs.ArcFb.Execute := TRUE;
						
						internal.state := MV_MOVE_ARC;
					acp6d_MOVE_SHORT:
						internal.fbs.ShortAxFb.Shuttle := ADR(Shuttle);
						internal.fbs.ShortAxFb.Parameters := internal.movesList[internal.currentIndex].ShortAxis;
						internal.fbs.ShortAxFb.Execute := TRUE;
						
						internal.state := MV_MOVE_SHORT;
					acp6d_MOVE_PAUSE:
						internal.fbs.PauseFb.IN := TRUE;
						internal.fbs.PauseFb.PT := internal.movesList[internal.currentIndex].PauseTime;
						internal.state := MV_PAUSE;
				END_CASE;
				WaitingForLast := (internal.currentIndex + 1) = internal.numMoves AND WaitForLast;
			END_IF
		// ***************************** Linear Move State
		MV_MOVE_LINEAR:
			IF internal.fbs.LinearFb.Error THEN
				internal.errorState := internal.state;
				internal.state := MV_ERROR;
				Error := TRUE;
				ErrorID := internal.fbs.LinearFb.ErrorID;
			ELSIF internal.fbs.LinearFb.Acknowledge THEN
				internal.fbs.LinearFb.Execute := FALSE OR WaitingForLast;
				internal.currentIndex := internal.currentIndex + 1;
				
				internal.state := MV_CHECK_CMD;
			END_IF
		
		// ***************************** Arc Move State
		MV_MOVE_ARC:
			IF internal.fbs.ArcFb.Error THEN
				internal.errorState := internal.state;
				internal.state := MV_ERROR;
				Error := TRUE;
				ErrorID := internal.fbs.ArcFb.ErrorID;
			ELSIF internal.fbs.ArcFb.Acknowledge THEN
				internal.fbs.ArcFb.Execute := FALSE OR WaitingForLast;
				internal.currentIndex := internal.currentIndex + 1;
				
				internal.state := MV_CHECK_CMD;
			END_IF
		// ***************************** Short Axis Move State
		MV_MOVE_SHORT:
			IF internal.fbs.ShortAxFb.Error THEN
				internal.errorState := internal.state;
				internal.state := MV_ERROR;
				Error := TRUE;
				ErrorID := internal.fbs.ShortAxFb.ErrorID;
			ELSIF internal.fbs.ShortAxFb.Acknowledge THEN
				internal.fbs.ShortAxFb.Execute := FALSE OR WaitingForLast;
				internal.currentIndex := internal.currentIndex + 1;
				
				internal.state := MV_CHECK_CMD;
			END_IF
		// ***************************** Pause Move State
		MV_PAUSE:
			IF internal.fbs.PauseFb.Q THEN
				internal.fbs.PauseFb.IN := FALSE;
				internal.currentIndex := internal.currentIndex + 1;
			
				internal.state := MV_CHECK_CMD;
			END_IF
		// ***************************** Waiting for the last command State
		MV_WAIT_FOR_LAST:
			CASE internal.movesList[internal.currentIndex - 1].MoveType OF
				acp6d_MOVE_LINEAR:
					IF internal.fbs.LinearFb.Done THEN	
						internal.fbs.LinearFb.Execute := FALSE;
						WaitingForLast := FALSE;
						Done := TRUE;
						internal.state := MV_DONE;
					END_IF
				acp6d_MOVE_ARC:
					IF internal.fbs.ArcFb.Done THEN	
						internal.fbs.ArcFb.Execute := FALSE;
						WaitingForLast := FALSE;
						Done := TRUE;
						internal.state := MV_DONE;
					END_IF
				acp6d_MOVE_SHORT:
					IF internal.fbs.ShortAxFb.Done THEN	
						internal.fbs.ShortAxFb.Execute := FALSE;
						WaitingForLast := FALSE;
						Done := TRUE;
						internal.state := MV_DONE;
					END_IF
				acp6d_MOVE_PAUSE:
					WaitingForLast := FALSE;
					Done := TRUE;
					internal.state := MV_DONE;
				
			END_CASE;
		// ***************************** Done State
		MV_DONE:
			IF NOT Execute THEN
				internal.state := MV_RESET;
			END_IF
		// ***************************** Error State
		MV_ERROR:
			IF NOT Execute THEN
				internal.state := MV_RESET;
			END_IF
		// ***************************** Reset State
		MV_RESET:
			WaitingForLast := FALSE;
			Error := FALSE;
			ErrorID := 0;
			Done := FALSE;
			
			internal.currentIndex := 0;
			internal.numMoves := 0;
			brsmemset(ADR(internal.movesList),0,internal.numMoves * SIZEOF(internal.movesList[0]));
		
			internal.fbs.LinearFb.Execute := FALSE;
			internal.fbs.ArcFb.Execute := FALSE;
			internal.fbs.ShortAxFb.Execute := FALSE;
			internal.fbs.PauseFb.IN := FALSE;
		
			internal.state := MV_INIT;
		
	END_CASE
	
	//FB Calls
	internal.fbs.LinearFb();
	internal.fbs.ArcFb();
	internal.fbs.ShortAxFb();
	internal.fbs.PauseFb();

END_FUNCTION_BLOCK
