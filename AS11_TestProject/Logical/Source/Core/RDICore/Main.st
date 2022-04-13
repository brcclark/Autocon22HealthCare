
PROGRAM _INIT
	Core.Parameters := ADR(Pars);
	Core.AsmRef := ADR(g6dAssembly);
	Core.Enable := TRUE;
	ShState;
	TestStart := TRUE;
	
	
	Vis.Cmd.ShuttleID := 1;
	//
	
	Vis.Par.Linear.Position.X := 0.12;		// m
	Vis.Par.Linear.Position.Y := 0.12;		// m
	Vis.Par.Linear.Velocity := 1.0;			// m/s
	Vis.Par.Linear.EndVelocity := 0.0;		// m/s
	Vis.Par.Linear.Acceleration := 10.0;	// m/s^2
	
	Vis.Par.Arc.Radius.Position.X := 0.12;	// m
	Vis.Par.Arc.Radius.Position.Y := 0.12; // m
	Vis.Par.Arc.Radius.Radius := 0.0;		// m
	Vis.Par.Arc.Angular.Center.X := 0.12;	// m
	Vis.Par.Arc.Angular.Center.Y := 0.12;	// m
	Vis.Par.Arc.Angular.Angle := 0.0;		// rad
	Vis.Par.Arc.Velocity := 1.0;			// m/s
	Vis.Par.Arc.EndVelocity := 0.0;			// m/s
	Vis.Par.Arc.Acceleration := 10.0;		// m/s^2
	
	Vis.Par.ShortAxis.Rotation.Center.X := 0.18;	// m
	Vis.Par.ShortAxis.Rotation.Center.Y := 0.18;	// m
	Vis.Par.ShortAxis.Position.Z := 0.001;			// m
	Vis.Par.ShortAxis.Velocity.Z := 0.002;			// m/s
	Vis.Par.ShortAxis.Velocity.Rx := 0.0873;		// rad/s
	Vis.Par.ShortAxis.Velocity.Ry := 0.0873;		// rad/s
	Vis.Par.ShortAxis.Velocity.Rz := 0.0873;		// rad/s
	
	Vis.Par.SixDOF.InPlane.Position.X := 0.12;		// m
	Vis.Par.SixDOF.InPlane.Position.Y := 0.12;		// m
	Vis.Par.SixDOF.ShortAxis.Position.Z := 0.001;	// m
	Vis.Par.SixDOF.InPlane.Velocity := 1.0;			// m/s
	Vis.Par.SixDOF.InPlane.Acceleration := 10.0;	// m/s^2
	Vis.Par.SixDOF.ShortAxis.Velocity.Z := 0.002;	// m/s
	Vis.Par.SixDOF.ShortAxis.Velocity.Rx := 0.0873;	// rad/s
	Vis.Par.SixDOF.ShortAxis.Velocity.Ry := 0.0873;	// rad/s
	Vis.Par.SixDOF.ShortAxis.Velocity.Rz := 0.0873;	// rad/s
	
	Vis.Par.Async.Velocity := 0.5;		// m/s
	Vis.Par.Async.Acceleration := 5.0;	// m/s^2
	
	
END_PROGRAM

PROGRAM _CYCLIC
	(* Insert code here *)
	gFB;
	

	IF TestStart THEN
		Vis.Cmd.ShuttleID := STRING_TO_UINT(Vis.Status.SelectedShuttleIDString);
	
		Core.Power := Vis.Cmd.Power;
		Core.ShuttleID := Vis.Cmd.ShuttleID;
		Core.ErrorReset := Vis.Cmd.ErrorReset;
		
		Core.MoveLinear := Vis.Cmd.MoveLinear;
		Core.MoveArc := Vis.Cmd.MoveArc;
		Core.MoveShort := Vis.Cmd.MoveShort;
		Core.Move6DOF := Vis.Cmd.Move6DOF;
		Core.MoveAsync := Vis.Cmd.MoveAsync;
		
		Core.AsmStop := Vis.Cmd.AsmStop;
		Core.AsmHalt := Vis.Cmd.AsmHalt;
		Core.AsmResume := Vis.Cmd.AsmResume;
		
		Core.ShStop := Vis.Cmd.ShStop;
		Core.ShHalt := Vis.Cmd.ShHalt;
		Core.ShResume := Vis.Cmd.ShResume;
		
		Core.AsmLevitation := Vis.Cmd.AsmLevitation;
		Core.ShLevitation := Vis.Cmd.ShLevitation;
		Core.AsmBuffer := Vis.Cmd.AsmBuffer;
		Core.ShBuffer := Vis.Cmd.ShBuffer;
		
		Core.Reboot := Vis.Cmd.Reboot;
		Core.ShRecover := Vis.Cmd.ShRecover;
		
		
		
		
		FOR i := 0 TO rdiMAX_ASYNC_SHUTTLES_M1 DO
			Vis.Par.Async.ShuttleTable[i].ShuttleID := Vis.TableData.Async.ShuttleIDArray[i];
			Vis.Par.Async.ShuttleTable[i].X := Vis.TableData.Async.TargetXArray[i];
			Vis.Par.Async.ShuttleTable[i].Y := Vis.TableData.Async.TargetYArray[i];
		END_FOR
		
		FOR i := 0 TO MAX_NUM_SHUTTLES_M1 DO
			Vis.TableData.ShuttleStatus.ShuttleID[i] := Core.ShuttleArray[i].ShuttleID;
			
			Vis.TableData.ShuttleStatus.ShuttleIDString[i] := '{"value":"';
			StringPlacehold := UINT_TO_STRING(Core.ShuttleArray[i].ShuttleID);
			Vis.TableData.ShuttleStatus.ShuttleIDString[i] := CONCAT(Vis.TableData.ShuttleStatus.ShuttleIDString[i], StringPlacehold);
			Vis.TableData.ShuttleStatus.ShuttleIDString[i] := CONCAT(Vis.TableData.ShuttleStatus.ShuttleIDString[i], '","text":"');
			StringPlacehold := UINT_TO_STRING(Core.ShuttleArray[i].ShuttleID);
			Vis.TableData.ShuttleStatus.ShuttleIDString[i] := CONCAT(Vis.TableData.ShuttleStatus.ShuttleIDString[i], StringPlacehold);
			Vis.TableData.ShuttleStatus.ShuttleIDString[i] := CONCAT(Vis.TableData.ShuttleStatus.ShuttleIDString[i], '"}');
			
			Vis.TableData.ShuttleStatus.ShuttleStateText[i] := ShStatusTexts[Core.ShuttleArray[i].State];
			
			Vis.TableData.ShuttleStatus.X[i] := Core.ShuttleArray[i].Position.X;
			Vis.TableData.ShuttleStatus.Y[i] := Core.ShuttleArray[i].Position.Y;
			Vis.TableData.ShuttleStatus.Z[i] := Core.ShuttleArray[i].Position.Z;
			Vis.TableData.ShuttleStatus.Rx[i] := Core.ShuttleArray[i].Position.Rx;
			Vis.TableData.ShuttleStatus.Ry[i] := Core.ShuttleArray[i].Position.Ry;
			Vis.TableData.ShuttleStatus.Rz[i] := Core.ShuttleArray[i].Position.Rz;
			
		END_FOR
		
		
		brsmemcpy(ADR(Pars), ADR(Vis.Par), SIZEOF(Pars6D_typ));
		

		
		(* Copy Statuses *)
		brsmemcpy(ADR(Vis.Status.AsmStatus), ADR(Core.AsmStatus), SIZEOF(AsmStatus_typ));
		brsmemcpy(ADR(Vis.Status.StatusMessage), ADR(Core.StatusMessage), SIZEOF(Vis.Status.StatusMessage));
		brsmemcpy(ADR(Vis.Status.CurrentState), ADR(Core.CurrentState), SIZEOF(Vis.Status.CurrentState));
		
		Vis.Status.AllShuttlesFound := Core.AllShuttlesFound;
		Vis.Status.MovementActive := Core.MovementActive;
		Vis.Status.ErrorID := Core.ErrorID;
		
		Vis.Status.ShuttleBuffer.ShuttleID := Core.ShuttleBufferInfo.ShuttleID;
		Vis.Status.ShuttleBuffer.NumberOfCommands := Core.ShuttleBufferInfo.NumberOfCommands;
		Vis.Status.ShuttleBuffer.State := ShBufferStateTexts[Core.ShuttleBufferInfo.State];
		
		
		IF Core.Error THEN
			IF Core.ErrorID = -1065799680 THEN
				Vis.Status.ErrorText := 'Wrong shuttle state';
			ELSIF Core.ErrorID = -1065801966 THEN
				Vis.Status.ErrorText := 'The ACOPOS6D object is invalid';
			ELSE
				Vis.Status.ErrorText := 'Lookup ErrorID in the Help';
			END_IF
		ELSE
			Vis.Status.ErrorText := '';
		END_IF
		

	END_IF
	
	Core();
	 
END_PROGRAM

PROGRAM _EXIT
	(* Insert code here *)
	 
END_PROGRAM

