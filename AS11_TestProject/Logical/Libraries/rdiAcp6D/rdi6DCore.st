
(* TODO: Add your comment here *)
FUNCTION_BLOCK rdi6DCore
	
	IF Enable THEN
		
		
		CASE Internal.State OF
			
			rdi_VALIDATE_ASSEMBLY_REF:
				CurrentState := 'rdi_VALIDATE_ASSEMBLY_REF';
				
				IF AsmRef.controlif <> 0 THEN
					Internal.FB.Asm.AsmReadStatus.Assembly := ADR(AsmRef);
					Internal.FB.Asm.AsmGetInfo.Assembly := ADR(AsmRef);
					Internal.FB.Asm.AsmGetShuttle.Assembly := ADR(AsmRef);
					Internal.FB.Asm.AsmPowerOn.Assembly := ADR(AsmRef);
					Internal.FB.Asm.AsmPowerOff.Assembly := ADR(AsmRef);
					Internal.FB.Asm.AsmReboot.Assembly := ADR(AsmRef);
					Internal.FB.Move.Async.Assembly := ADR(AsmRef);
					Internal.FB.Stop.AsmStop.Assembly := ADR(AsmRef);
					Internal.FB.Halt.AsmHalt.Assembly := ADR(AsmRef);
					Internal.FB.Levitation.AsmLevitation.Assembly := ADR(AsmRef);
					Internal.FB.Buffer.AsmBuffer.Assembly := ADR(AsmRef);
					
					Internal.State := rdi_ENABLE_ASM_READ_STATUS;
						
				ELSE
					StatusMessage := 'Assembly reference not set';
				END_IF
					
				
			(* enable AsmReadStatus to get info about the system *)
			rdi_ENABLE_ASM_READ_STATUS:
				CurrentState := 'rdi_ENABLE_ASM_READ_STATUS';
				Internal.FB.Asm.AsmReadStatus.Enable := TRUE;
				
				IF Internal.FB.Asm.AsmReadStatus.Valid THEN
					Internal.State := rdi_WAIT_FOR_POWER_ON;
				END_IF
			
				
			(* wait for user to issue the Power command *)
			rdi_WAIT_FOR_POWER_ON:
				CurrentState := 'rdi_WAIT_FOR_POWER_ON';
				AllShuttlesFound := FALSE;
				Internal.FB.Asm.AsmPowerOn.Execute := Power;
				IF Internal.FB.Asm.AsmReadStatus.AssemblyReady THEN
					brsmemset(ADR(ShuttleArray), 0, SIZEOF(ShuttleArray));
					brsmemset(ADR(Internal.FB.Shuttle.ShReadInfo), 0, SIZEOF(Internal.FB.Shuttle.ShReadInfo));
					Internal.FB.Asm.AsmPowerOn.Execute := FALSE;
					Internal.State := rdi_GET_ASM_INFO;
				END_IF
				
			rdi_GET_ASM_INFO:
				CurrentState := 'rdi_GET_ASM_INFO';
				Internal.FB.Asm.AsmGetInfo.Execute := TRUE;
				IF Internal.FB.Asm.AsmGetInfo.Done THEN
					Internal.Status.AsmGetInfoShuttleCount := Internal.FB.Asm.AsmGetInfo.AssemblyInfo.NumberOfShuttles;
					Internal.FB.Asm.AsmGetInfo.Execute := FALSE;
					Internal.Status.GetShuttleCount := 0;
					
					Internal.State := rdi_VALIDATE_GET_SHUTTLES;
				END_IF
				
				
			(* Reattempt to get shuttles until successful; this state will not be needed when AsmGetShuttle is more reliable *)
			rdi_VALIDATE_GET_SHUTTLES:
				CurrentState := 'rdi_VALIDATE_GET_SHUTTLES';
				StatusMessage := 'Getting Shuttles';
				Internal.FB.Asm.AsmGetShuttle.Enable := TRUE;
				IF Internal.FB.Asm.AsmGetShuttle.Valid THEN
					IF Internal.FB.Asm.AsmGetShuttle.TotalCount > 0 THEN
						Internal.State := rdi_GET_SHUTTLES;
					ELSE
						Internal.FB.Asm.AsmGetShuttle.Enable := FALSE;
					END_IF
				END_IF
				
				
			
			(* get the shuttles on the system and store them into a shuttle array *)
			rdi_GET_SHUTTLES:
				CurrentState := 'rdi_GET_SHUTTLES';
				Internal.FB.Asm.AsmGetShuttle.Enable := TRUE;
				IF Internal.FB.Asm.AsmGetShuttle.Valid THEN
					IF Internal.FB.Asm.AsmGetShuttle.Shuttle.controlif <> 0 THEN
						ShuttleArray[Internal.Status.GetShuttleCount].Axis := Internal.FB.Asm.AsmGetShuttle.Shuttle;
						ShuttleArray[Internal.Status.GetShuttleCount].ShuttleID := Internal.FB.Asm.AsmGetShuttle.AdditionalInfo.ShuttleID;
					END_IF
					
					IF Internal.FB.Asm.AsmGetShuttle.RemainingCount > 0 THEN
						Internal.FB.Asm.AsmGetShuttle.Next := TRUE;
						Internal.State := rdi_GET_SHUTTLES_NEXT;
					ELSE
						AllShuttlesFound := TRUE;
						ShuttleID := ShuttleArray[Internal.Status.GetShuttleCount].ShuttleID;
						Internal.FB.Asm.AsmGetShuttle.Enable := FALSE;
						Internal.State := rdi_GET_SHUTTLE_INFO;
					END_IF
				END_IF
			
				
			rdi_GET_SHUTTLES_NEXT:
				CurrentState := 'rdi_GET_SHUTTLES_NEXT';
				Internal.FB.Asm.AsmGetShuttle.Next := FALSE;
				Internal.Status.GetShuttleCount := Internal.Status.GetShuttleCount + 1;
				Internal.State := rdi_GET_SHUTTLES;
			
			
				
			(* enable ShReadInfo for all shuttles *)
			rdi_GET_SHUTTLE_INFO:
				CurrentState := 'rdi_GET_SHUTTLE_INFO';
			
				FOR i := 0 TO MAX_NUM_SHUTTLES_M1 DO
					IF ShuttleArray[i].Axis.controlif <> 0 THEN
						Internal.FB.Shuttle.ShReadInfo[i].Shuttle := ADR(ShuttleArray[i].Axis);
						Internal.FB.Shuttle.ShReadInfo[i].Enable := TRUE;
					END_IF
				END_FOR
				
				Internal.State := rdi_CHECK_SHUTTLE_INFO;
				
			rdi_CHECK_SHUTTLE_INFO:
				(* I found that if you remove a shuttle, power off the controller, and power on again, the controller will
					still see the removed shuttle. So if the shuttle is in the RESERVED state right after power on, something
					is probably wrong and the shuttle should be deleted. If not, since the non-existing shuttle axis is still valid,
					things can get hung up *)
				
				FOR i := 0 TO MAX_NUM_SHUTTLES_M1 DO
					IF Internal.FB.Shuttle.ShReadInfo[i].ShuttleInfo.State = mcACP6D_SH_STATE_RESERVED THEN
						brsmemset(ADR(ShuttleArray[i]), 0, SIZEOF(ShuttleArray[i]));
						brsmemset(ADR(Internal.FB.Shuttle.ShReadInfo[i]), 0, SIZEOF(Internal.FB.Shuttle.ShReadInfo[i]));
					END_IF
				END_FOR
						
				Internal.State := rdi_WAIT_FOR_COMMAND;
			
			
			(* main waiting state *)
			rdi_WAIT_FOR_COMMAND:
				CurrentState := 'rdi_WAIT_FOR_COMMAND';
				
				IF NOT Power THEN
					StatusMessage := 'Powering Off';
					Internal.State := rdi_POWER_OFF;
				ELSIF EDGEPOS(AsmStop) THEN
					StatusMessage := 'Stopping Assembly';
					Internal.State := rdi_ASM_STOP;
				ELSIF EDGEPOS(ShStop) AND ValidShuttleSelected THEN
					StatusMessage := 'Stopping Shuttle';
					Internal.State := rdi_SHUTTLE_STOP;
				ELSIF EDGEPOS(AsmHalt) THEN
					StatusMessage := 'Halting Assembly';
					Internal.FB.Halt.AsmHalt.Option := mcACP6D_HALT;
					Internal.State := rdi_ASM_HALT;
				ELSIF EDGEPOS(ShHalt) AND ValidShuttleSelected THEN
					StatusMessage := 'Halting Shuttle';
					Internal.FB.Halt.ShHalt.Option := mcACP6D_HALT;
					Internal.State := rdi_SHUTTLE_HALT;
				ELSIF EDGEPOS(AsmResume) THEN
					StatusMessage := 'Resuming Assembly';
					Internal.FB.Halt.AsmHalt.Option := mcACP6D_CONTINUE;
					Internal.State := rdi_ASM_HALT;
				ELSIF EDGEPOS(ShResume) AND ValidShuttleSelected THEN
					StatusMessage := 'Resuming Shuttle';
					Internal.FB.Halt.ShHalt.Option := mcACP6D_CONTINUE;
					Internal.State := rdi_SHUTTLE_HALT;
				ELSIF EDGEPOS(MoveLinear) AND ValidShuttleSelected THEN
					StatusMessage := 'Started Movement: Linear';
					Internal.State := rdi_MOVE_LINEAR;
				ELSIF EDGEPOS(MoveArc) AND ValidShuttleSelected THEN
					StatusMessage := 'Started Movement: Arc';
					Internal.State := rdi_MOVE_ARC;
				ELSIF EDGEPOS(MoveShort) AND ValidShuttleSelected THEN
					StatusMessage := 'Started Movement: Short';
					Internal.State := rdi_MOVE_SHORT;
				ELSIF EDGEPOS(Move6DOF) AND ValidShuttleSelected THEN
					StatusMessage := 'Started Movement: 6DOF';
					Internal.State := rdi_MOVE_6DOF;
				ELSIF EDGEPOS(MoveAsync) THEN
					StatusMessage := 'Started Movement: Async';
					Internal.State := rdi_MOVE_ASYNC;
				ELSIF EDGEPOS(AsmLevitation) THEN
					StatusMessage := 'Started Asm Levitation Command';
					Internal.State := rdi_ASM_LEVITATION;
				ELSIF EDGEPOS(ShLevitation) THEN
					StatusMessage := 'Started Sh Levitation Command';
					Internal.State := rdi_SHUTTLE_LEVITATION;
				ELSIF EDGEPOS(AsmBuffer) AND ValidShuttleSelected THEN
					StatusMessage := 'Started Asm Buffer Command';
					Internal.State := rdi_ASM_BUFFER;
				ELSIF EDGEPOS(ShBuffer) AND ValidShuttleSelected THEN
					StatusMessage := 'Started Sh Buffer Command';
					Internal.State := rdi_SHUTTLE_BUFFER;
				ELSIF EDGEPOS(Reboot) THEN
					StatusMessage := 'Rebooting 6D Controller';
					Internal.State := rdi_ASM_REBOOT;
				ELSIF EDGEPOS(ShRecover) THEN
					StatusMessage := 'Recovering Shuttle';
					Internal.State := rdi_SHUTTLE_RECOVER;
				END_IF
				
				
				
				
				(* check if selected shuttle changed, then check if new ID exists *)
				IF ShuttleID <> Internal.Status.OldShuttleID THEN
					FOR i := 0 TO MAX_NUM_SHUTTLES_M1 DO
						IF ShuttleID = ShuttleArray[i].ShuttleID THEN
							Internal.Status.SelectedShuttleAxis := ShuttleArray[i].Axis;
							Internal.Status.OldShuttleID := ShuttleID;
							ValidShuttleSelected := TRUE;
							EXIT;
						END_IF
					END_FOR
					IF i = MAX_NUM_SHUTTLES THEN
						Internal.Status.OldShuttleID := ShuttleID;
						ValidShuttleSelected := FALSE;
					END_IF
				END_IF
			
			
				
			rdi_MOVE_LINEAR:
				CurrentState := 'rdi_MOVE_LINEAR';
				
				Internal.FB.Move.Linear.Shuttle := ADR(Internal.Status.SelectedShuttleAxis);
				brsmemcpy(ADR(Internal.FB.Move.Linear.Parameters), ADR(Parameters.Linear), SIZEOF(McAcp6DInPlaneParType));
				Internal.FB.Move.Linear.Execute := TRUE;
			
				
				IF Internal.FB.Move.Linear.Error THEN
					Internal.FB.Move.Linear.Execute := FALSE;
					IF Internal.FB.Move.Linear.ErrorID = rdiERROR_INVALID_PAR THEN
						StatusMessage := 'Movement Linear: Invalid parameters';
						Internal.State := rdi_WAIT_FOR_COMMAND;
					ELSE
						ErrorID := Internal.FB.Move.Linear.ErrorID;
						Internal.State := rdi_ERROR;
					END_IF
				ELSIF Internal.FB.Move.Linear.Acknowledge THEN
					Internal.FB.Move.Linear.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_COMMAND;
				END_IF
				
				
			rdi_MOVE_ARC:
				CurrentState := 'rdi_MOVE_ARC';
				
				Internal.FB.Move.Arc.Shuttle := ADR(Internal.Status.SelectedShuttleAxis);
				brsmemcpy(ADR(Internal.FB.Move.Arc.Parameters), ADR(Parameters.Arc), SIZEOF(McAcp6DArcParType));
				Internal.FB.Move.Arc.Execute := TRUE;
				
				IF Internal.FB.Move.Arc.Error THEN
					Internal.FB.Move.Arc.Execute := FALSE;
					IF Internal.FB.Move.Arc.ErrorID = rdiERROR_INVALID_PAR THEN
						StatusMessage := 'Movement Arc: Invalid parameters';
						Internal.State := rdi_WAIT_FOR_COMMAND;
					ELSE	
				
						ErrorID := Internal.FB.Move.Arc.ErrorID;
						Internal.State := rdi_ERROR;
					END_IF
				ELSIF Internal.FB.Move.Arc.Acknowledge THEN
					Internal.FB.Move.Arc.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_COMMAND;
				END_IF
				
				
			rdi_MOVE_SHORT:
				CurrentState := 'rdi_MOVE_SHORT';
				
				Internal.FB.Move.Short.Shuttle := ADR(Internal.Status.SelectedShuttleAxis);
				brsmemcpy(ADR(Internal.FB.Move.Short.Parameters), ADR(Parameters.ShortAxis), SIZEOF(McAcp6DShortAxParType));
				Internal.FB.Move.Short.Execute := TRUE;
				
				
				
				IF Internal.FB.Move.Short.Error THEN
					Internal.FB.Move.Short.Execute := FALSE;
					IF Internal.FB.Move.Short.ErrorID = rdiERROR_INVALID_PAR THEN
						StatusMessage := 'Movement Short: Invalid parameters';
						Internal.State := rdi_WAIT_FOR_COMMAND;
					ELSE
						ErrorID := Internal.FB.Move.Short.ErrorID;
						Internal.State := rdi_ERROR;
					END_IF
				ELSIF Internal.FB.Move.Short.Acknowledge THEN
					Internal.FB.Move.Short.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_COMMAND;
				END_IF
			
			rdi_MOVE_6DOF:
				CurrentState := 'rdi_MOVE_6DOF';
				
				Internal.FB.Move.SixDOF.Shuttle := ADR(Internal.Status.SelectedShuttleAxis);
				brsmemcpy(ADR(Internal.FB.Move.SixDOF.Parameters), ADR(Parameters.SixDOF), SIZEOF(McAcp6DMove6DParType));
				Internal.FB.Move.SixDOF.Execute := TRUE;
				
				
				IF Internal.FB.Move.SixDOF.Error THEN
					Internal.FB.Move.SixDOF.Execute := FALSE;
					IF Internal.FB.Move.SixDOF.ErrorID = rdiERROR_INVALID_PAR THEN
						StatusMessage := 'Movement 6DOF: Invalid parameters';
						Internal.State := rdi_WAIT_FOR_COMMAND;
					ELSE
						ErrorID := Internal.FB.Move.SixDOF.ErrorID;
						Internal.State := rdi_ERROR;
					END_IF
				ELSIF Internal.FB.Move.SixDOF.Acknowledge THEN
					Internal.FB.Move.SixDOF.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_COMMAND;
				END_IF
				
			rdi_MOVE_ASYNC:
				CurrentState := 'rdi_MOVE_ASYNC';
				Internal.FB.Move.Async.Execute := TRUE;
				
				Internal.FB.Move.Async.Parameters.Acceleration := Parameters.Async.Acceleration;
				Internal.FB.Move.Async.Parameters.Velocity := Parameters.Async.Velocity;
				
				FOR i := 0 TO rdiMAX_ASYNC_SHUTTLES_M1 DO
					FOR j := 0 TO MAX_NUM_SHUTTLES_M1 DO
						IF Parameters.Async.ShuttleTable[i].ShuttleID = 0 THEN
							EXIT;
						ELSIF Parameters.Async.ShuttleTable[i].ShuttleID = ShuttleArray[j].ShuttleID THEN
							Internal.FB.Move.Async.Parameters.AsyncPar[i].Shuttle := ADR(ShuttleArray[j].Axis);
							Internal.FB.Move.Async.Parameters.AsyncPar[i].X := Parameters.Async.ShuttleTable[i].X;
							Internal.FB.Move.Async.Parameters.AsyncPar[i].Y := Parameters.Async.ShuttleTable[i].Y;
							EXIT;
						END_IF
					END_FOR
					IF j = MAX_NUM_SHUTTLES THEN
						Internal.FB.Move.Async.Execute := FALSE;
						StatusMessage := 'Movement Async: Invalid Shuttle ID';
						Internal.State := rdi_WAIT_FOR_COMMAND;
						EXIT;
					END_IF
				END_FOR
				
				
				
				IF Internal.FB.Move.Async.Error THEN
					Internal.FB.Move.Async.Execute := FALSE;
					IF Internal.FB.Move.Async.ErrorID = rdiERROR_INVALID_PAR THEN
						StatusMessage := 'Movement Async: Invalid parameters';
						Internal.State := rdi_WAIT_FOR_COMMAND;
					ELSE
						ErrorID := Internal.FB.Move.Async.ErrorID;
						Internal.State := rdi_ERROR;
					END_IF
				ELSIF Internal.FB.Move.Async.Acknowledge THEN
					Internal.FB.Move.Async.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_COMMAND;
				END_IF
				
				
			rdi_ASM_STOP:
				CurrentState := 'rdi_ASM_STOP';
				Internal.FB.Stop.AsmStop.Execute := TRUE;
				
				IF Internal.FB.Stop.AsmStop.Error THEN
					Internal.FB.Stop.AsmStop.Execute := FALSE;
					ErrorID := Internal.FB.Stop.AsmStop.ErrorID;
					Internal.State := rdi_ERROR;
				ELSIF Internal.FB.Stop.AsmStop.Done THEN
					Internal.FB.Stop.AsmStop.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_COMMAND;
				END_IF
				
			rdi_SHUTTLE_STOP:
				CurrentState := 'rdi_SHUTTLE_STOP';
				Internal.FB.Stop.ShStop.Shuttle := ADR(Internal.Status.SelectedShuttleAxis);
				Internal.FB.Stop.ShStop.Execute := TRUE;
				
				IF Internal.FB.Stop.ShStop.Error THEN
					Internal.FB.Stop.ShStop.Execute := FALSE;
					ErrorID := Internal.FB.Stop.ShStop.ErrorID;
					Internal.State := rdi_ERROR;
				ELSIF Internal.FB.Stop.ShStop.Done THEN
					Internal.FB.Stop.ShStop.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_COMMAND;
				END_IF
				
			rdi_ASM_HALT:
				CurrentState := 'rdi_ASM_HALT';
				Internal.FB.Halt.AsmHalt.Execute := TRUE;
				//Internal.FB.Halt.AsmHalt.Option
				
				IF Internal.FB.Halt.AsmHalt.Error THEN
					Internal.FB.Halt.AsmHalt.Execute := FALSE;
					ErrorID := Internal.FB.Halt.AsmHalt.ErrorID;
					Internal.State := rdi_ERROR;
				ELSIF Internal.FB.Halt.AsmHalt.Done THEN
					Internal.FB.Halt.AsmHalt.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_COMMAND;
				END_IF
				
			rdi_SHUTTLE_HALT:
				CurrentState := 'rdi_SHUTTLE_HALT';
				Internal.FB.Halt.ShHalt.Shuttle := ADR(Internal.Status.SelectedShuttleAxis);
				Internal.FB.Halt.ShHalt.Execute := TRUE;
				
				IF Internal.FB.Halt.ShHalt.Error THEN
					Internal.FB.Halt.ShHalt.Execute := FALSE;
					ErrorID := Internal.FB.Halt.ShHalt.ErrorID;
					Internal.State := rdi_ERROR;
				ELSIF Internal.FB.Halt.ShHalt.Done THEN
					Internal.FB.Halt.ShHalt.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_COMMAND;
				END_IF
				
			rdi_ASM_LEVITATION:
				CurrentState := 'rdi_ASM_LEVITATION';
				Internal.FB.Levitation.AsmLevitation.Parameters := Parameters.Levitation;
				Internal.FB.Levitation.AsmLevitation.Execute := TRUE;
				
				IF Internal.FB.Levitation.AsmLevitation.Error THEN
					Internal.FB.Levitation.AsmLevitation.Execute := FALSE;
					ErrorID := Internal.FB.Levitation.AsmLevitation.ErrorID;
					Internal.State := rdi_ERROR;
				ELSIF Internal.FB.Levitation.AsmLevitation.Acknowledge THEN
					Internal.FB.Levitation.AsmLevitation.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_COMMAND;
				END_IF
				
			rdi_SHUTTLE_LEVITATION:
				CurrentState := 'rdi_SHUTTLE_LEVITATION';
				Internal.FB.Levitation.ShLevitation.Shuttle := ADR(Internal.Status.SelectedShuttleAxis);
				Internal.FB.Levitation.ShLevitation.Parameters := Parameters.Levitation;
				Internal.FB.Levitation.ShLevitation.Execute := TRUE;
				
				IF Internal.FB.Levitation.ShLevitation.Error THEN
					Internal.FB.Levitation.ShLevitation.Execute := FALSE;
					ErrorID := Internal.FB.Levitation.ShLevitation.ErrorID;
					Internal.State := rdi_ERROR;
				ELSIF Internal.FB.Levitation.ShLevitation.Acknowledge THEN
					Internal.FB.Levitation.ShLevitation.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_COMMAND;
				END_IF
				
			rdi_ASM_BUFFER:
				CurrentState := 'rdi_ASM_BUFFER';
				Internal.FB.Buffer.AsmBuffer.Option := Parameters.Buffer;
				Internal.FB.Buffer.AsmBuffer.Execute := TRUE;
				
				IF Internal.FB.Buffer.AsmBuffer.Error THEN
					Internal.FB.Buffer.AsmBuffer.Execute := FALSE;
					ErrorID := Internal.FB.Buffer.AsmBuffer.ErrorID;
					Internal.State := rdi_ERROR;
				ELSIF Internal.FB.Buffer.AsmBuffer.Done THEN
					Internal.FB.Buffer.AsmBuffer.Execute := FALSE;
					Internal.State := rd_SHUTTLE_BUFFER_STATUS;
				END_IF
				
				
			rdi_SHUTTLE_BUFFER:
				CurrentState := 'rdi_SHUTTLE_BUFFER';
				Internal.FB.Buffer.ShBuffer.Shuttle := ADR(Internal.Status.SelectedShuttleAxis);
				Internal.FB.Buffer.ShBuffer.Option := Parameters.Buffer;
				Internal.FB.Buffer.ShBuffer.Execute := TRUE;
				
				IF Internal.FB.Buffer.ShBuffer.Error THEN
					Internal.FB.Buffer.ShBuffer.Execute := FALSE;
					ErrorID := Internal.FB.Buffer.ShBuffer.ErrorID;
					Internal.State := rdi_ERROR;
				ELSIF Internal.FB.Buffer.ShBuffer.Done THEN
					Internal.FB.Buffer.ShBuffer.Execute := FALSE;
					Internal.State := rd_SHUTTLE_BUFFER_STATUS;
				END_IF
				
				
			rd_SHUTTLE_BUFFER_STATUS:
				CurrentState := 'rd_SHUTTLE_BUFFER_STATUS';
				Internal.FB.Buffer.ShBuffer.Shuttle := ADR(Internal.Status.SelectedShuttleAxis);
				Internal.FB.Buffer.ShBuffer.Option := mcACP6D_BUFFER_GET_STATUS;
				Internal.FB.Buffer.ShBuffer.Execute := TRUE;
				
				IF Internal.FB.Buffer.ShBuffer.Error THEN
					Internal.FB.Buffer.ShBuffer.Execute := FALSE;
					ErrorID := Internal.FB.Buffer.ShBuffer.ErrorID;
					Internal.State := rdi_ERROR;
				ELSIF Internal.FB.Buffer.ShBuffer.Done THEN
					ShuttleBufferInfo.ShuttleID := ShuttleID;
					ShuttleBufferInfo.NumberOfCommands := Internal.FB.Buffer.ShBuffer.BufferInfo.NumberOfCommands;
					ShuttleBufferInfo.State := Internal.FB.Buffer.ShBuffer.BufferInfo.State;
					Internal.FB.Buffer.ShBuffer.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_COMMAND;
				END_IF
				
			rdi_SHUTTLE_RECOVER:
				CurrentState := 'rdi_SHUTTLE_RECOVER';
				Internal.FB.Shuttle.ShRecover.Shuttle := ADR(Internal.Status.SelectedShuttleAxis);
				
				Internal.FB.Shuttle.ShRecover.Execute := TRUE;
				
				
				IF Internal.FB.Shuttle.ShRecover.Error THEN
					Internal.FB.Shuttle.ShRecover.Execute := FALSE;
					ErrorID := Internal.FB.Shuttle.ShRecover.ErrorID;
					Internal.State := rdi_ERROR;
				ELSIF Internal.FB.Shuttle.ShRecover.Done THEN
					Internal.FB.Shuttle.ShRecover.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_COMMAND;
				END_IF
				
				
				
				
			rdi_ASM_REBOOT:
				CurrentState := 'rdi_ASM_REBOOT';
				Internal.FB.Asm.AsmReboot.Execute := TRUE;
				
				IF Internal.FB.Asm.AsmReboot.Error THEN
					Internal.FB.Asm.AsmReboot.Execute := FALSE;
					ErrorID := Internal.FB.Asm.AsmReboot.ErrorID;
					Internal.State := rdi_ERROR;
				ELSIF Internal.FB.Asm.AsmReadStatus.AssemblyDisabled THEN
					Internal.FB.Asm.AsmReboot.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_POWER_ON;
				END_IF
				
				
				
			rdi_POWER_OFF:
				CurrentState := 'rdi_POWER_OFF';
				Internal.FB.Asm.AsmPowerOff.Execute := TRUE;
	
				IF Internal.FB.Asm.AsmPowerOff.Error THEN
					Internal.FB.Asm.AsmPowerOff.Execute := FALSE;
					ErrorID := Internal.FB.Asm.AsmPowerOff.ErrorID;
					Internal.State := rdi_ERROR;
				ELSIF Internal.FB.Asm.AsmReadStatus.AssemblyDisabled THEN
					
					Internal.FB.Asm.AsmPowerOff.Execute := FALSE;
					Internal.State := rdi_WAIT_FOR_POWER_ON;
				END_IF
				
				
				
			
			rdi_ERROR:
				CurrentState := 'rdi_ERROR';
				Error := TRUE;
				
				
				
				IF EDGEPOS(ErrorReset) THEN
					Error := FALSE;
					ErrorID := 0;
					Internal.State := rdi_WAIT_FOR_COMMAND;
				END_IF
			
		END_CASE
		
		
	
		(* FB calls *)
		Internal.FB.Asm.AsmReadStatus();
		Internal.FB.Asm.AsmGetInfo();
		Internal.FB.Asm.AsmGetShuttle();
		Internal.FB.Asm.AsmPowerOn();
		Internal.FB.Asm.AsmPowerOff();
		Internal.FB.Asm.AsmReboot();
	
		Internal.FB.Move.Linear();
		Internal.FB.Move.Arc();
		Internal.FB.Move.Short();
		Internal.FB.Move.SixDOF();
		Internal.FB.Move.Async();
	
		Internal.FB.Stop.AsmStop();
		Internal.FB.Stop.ShStop();
		Internal.FB.Halt.AsmHalt();
		Internal.FB.Halt.ShHalt();
	
		Internal.FB.Levitation.AsmLevitation();
		Internal.FB.Levitation.ShLevitation();
		Internal.FB.Buffer.AsmBuffer();
		Internal.FB.Buffer.ShBuffer();
		
		Internal.FB.Shuttle.ShRecover();
	
	
		(* set the output statuses *)
		Active := Enable;
		PowerOn := Internal.FB.Asm.AsmReadStatus.AssemblyReady;
	
		AsmStatus.AssemblyDisabled := Internal.FB.Asm.AsmReadStatus.AssemblyDisabled;
		AsmStatus.AssemblyErrorStop := Internal.FB.Asm.AsmReadStatus.AssemblyErrorStop;
		AsmStatus.AssemblyHoming := Internal.FB.Asm.AsmReadStatus.AssemblyHoming;
		AsmStatus.AssemblyReady := Internal.FB.Asm.AsmReadStatus.AssemblyReady;
		AsmStatus.AssemblyStartUp := Internal.FB.Asm.AsmReadStatus.AssemblyStartUp;
		AsmStatus.AssemblyStopping := Internal.FB.Asm.AsmReadStatus.AssemblyStopping;
	
		(* cyclically update shuttle info *)
		MovementActive := FALSE;
	
		FOR i := 0 TO MAX_NUM_SHUTTLES_M1 DO
			Internal.FB.Shuttle.ShReadInfo[i]();
		
		
			IF Internal.FB.Shuttle.ShReadInfo[i].Valid THEN
				ShuttleArray[i].ShuttleID := Internal.FB.Shuttle.ShReadInfo[i].ShuttleInfo.ShuttleID;
				ShuttleArray[i].Position := Internal.FB.Shuttle.ShReadInfo[i].ShuttleInfo.Position;
				ShuttleArray[i].State := Internal.FB.Shuttle.ShReadInfo[i].ShuttleInfo.State;
			
				MovementActive := MovementActive OR (ShuttleArray[i].State = mcACP6D_SH_STATE_MOTION) OR (ShuttleArray[i].State = mcACP6D_SH_STATE_STOPPING) OR (ShuttleArray[i].State = mcACP6D_SH_STATE_OBSTACLE);
			END_IF
		
		
		END_FOR
		
		
		(* if not enabled *)
	ELSE
		
		
	END_IF
	
	
	

END_FUNCTION_BLOCK
