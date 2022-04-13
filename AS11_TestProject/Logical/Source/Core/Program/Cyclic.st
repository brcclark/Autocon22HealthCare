
PROGRAM _CYCLIC
	//Move all shuttles to some start location
	//Leave one shuttle to just run through a handful of movements repeatedly
	//Route all the other shuttles to mix up their locations
	
	gSystemIf;
	CASE state OF
		0:
			IF test THEN
				fbs.MoveAsyncFb.Assembly := ADR(g6dAssembly);
				fbs.MoveAsyncFb.Parameters.AsyncPar[0].Shuttle := ADR(gShIfs[0].Cfg.Shuttle);
				fbs.MoveAsyncFb.Parameters.AsyncPar[0].X := 0.060;
				fbs.MoveAsyncFb.Parameters.AsyncPar[0].Y := 0.180;
				fbs.MoveAsyncFb.Parameters.AsyncPar[1].Shuttle := ADR(gShIfs[1].Cfg.Shuttle);
				fbs.MoveAsyncFb.Parameters.AsyncPar[1].X := 0.06;
				fbs.MoveAsyncFb.Parameters.AsyncPar[1].Y := 0.06;
				fbs.MoveAsyncFb.Parameters.AsyncPar[2].Shuttle := ADR(gShIfs[2].Cfg.Shuttle);
				fbs.MoveAsyncFb.Parameters.AsyncPar[2].X := 0.36;
				fbs.MoveAsyncFb.Parameters.AsyncPar[2].Y := 0.12;
				fbs.MoveAsyncFb.Parameters.Velocity := 0.25;
				fbs.MoveAsyncFb.Parameters.MoveOnlySelectedShuttles := FALSE;
				fbs.MoveAsyncFb.Parameters.Acceleration := 1.0;
				fbs.MoveAsyncFb.Execute := TRUE;
				
				state := 10;
			END_IF
		10:
			IF fbs.MoveAsyncFb.Done THEN
				fbs.MoveAsyncFb.Execute := FALSE;
				
				state := 11;
			END_IF	
		11:
			pars[0].MoveType := acp6d_MOVE_ARC;
			pars[0].Arc.Velocity := 0.05;
			pars[0].Arc.Acceleration := 1;
			pars[0].Arc.EndVelocity := 0;
			pars[0].Arc.Mode := mcACP6D_MOVE_RELATIVE;
			pars[0].Arc.Option := mcACP6D_ARC_ANGULAR;
			pars[0].Arc.Angular.Center.X := 0.005;
			pars[0].Arc.Angular.Center.Y := 0.0000;
			pars[0].Arc.Direction := mcACP6D_ARC_CLOCKWISE;
			pars[0].Arc.Angular.Angle := 3.1415 * 2 * 40; 
				
			fbs.HelpMoveFb.Shuttle := gShIfs[2].Cfg.Shuttle;
			fbs.HelpMoveFb.Pars.NumMoves := 1;
			fbs.HelpMoveFb.Pars.MoveList := ADR(pars);
			fbs.HelpMoveFb.Execute := TRUE;
			
			state := 20;
		20:
			fbs.MoveAsyncFb.Assembly := ADR(g6dAssembly);
			fbs.MoveAsyncFb.Parameters.AsyncPar[0].Shuttle := ADR(gShIfs[0].Cfg.Shuttle);
			fbs.MoveAsyncFb.Parameters.AsyncPar[0].X := 0.180;
			fbs.MoveAsyncFb.Parameters.AsyncPar[0].Y := 0.42;
			fbs.MoveAsyncFb.Parameters.AsyncPar[1].Shuttle := ADR(gShIfs[1].Cfg.Shuttle);
			fbs.MoveAsyncFb.Parameters.AsyncPar[1].X := 0.06;
			fbs.MoveAsyncFb.Parameters.AsyncPar[1].Y := 0.42;
			fbs.MoveAsyncFb.Parameters.AsyncPar[2].Shuttle := 0;
			fbs.MoveAsyncFb.Parameters.AsyncPar[2].X := 0;
			fbs.MoveAsyncFb.Parameters.AsyncPar[2].Y := 0;
			fbs.MoveAsyncFb.Parameters.Acceleration := 1.0;
			fbs.MoveAsyncFb.Parameters.Velocity := 0.25;
			fbs.MoveAsyncFb.Parameters.MoveOnlySelectedShuttles := TRUE;
			fbs.MoveAsyncFb.Execute := TRUE;
			state := 21;
		21:
			IF fbs.MoveAsyncFb.Done THEN
				fbs.MoveAsyncFb.Execute := FALSE;
				
				state := 22;
			END_IF
		22:
			fbs.MoveAsyncFb.Parameters.AsyncPar[0].Shuttle := ADR(gShIfs[0].Cfg.Shuttle);
			fbs.MoveAsyncFb.Parameters.AsyncPar[0].X := 0.060;
			fbs.MoveAsyncFb.Parameters.AsyncPar[0].Y := 0.06;
			fbs.MoveAsyncFb.Parameters.AsyncPar[1].Shuttle := ADR(gShIfs[1].Cfg.Shuttle);
			fbs.MoveAsyncFb.Parameters.AsyncPar[1].X := 0.36;
			fbs.MoveAsyncFb.Parameters.AsyncPar[1].Y := 0.36;
			fbs.MoveAsyncFb.Parameters.Acceleration := 1.0;
			fbs.MoveAsyncFb.Parameters.Velocity := 0.25;
			fbs.MoveAsyncFb.Parameters.MoveOnlySelectedShuttles := TRUE;
			fbs.MoveAsyncFb.Execute := TRUE;
			state := 30;
		

	END_CASE;
	
	fbs.HelpMoveFb();
	fbs.MoveAsyncFb();
	 
END_PROGRAM
