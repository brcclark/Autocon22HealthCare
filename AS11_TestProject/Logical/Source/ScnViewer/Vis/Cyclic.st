
PROGRAM _CYCLIC
	FOR i := 0 TO MAX_SH_IDX DO
		IF gShIfs[i].Cfg.Shuttle.controlif <> 0 THEN
			Vis.Shuttles[i].Visible := TRUE;
			shInfoFB[i].Enable := TRUE;
			shInfoFB[i].Shuttle := ADR(gShIfs[i].Cfg.Shuttle);

			shInfoFB[i]();
			Vis.Shuttles[i].Position.X := shInfoFB[i].ShuttleInfo.Position.X * 1000;
			Vis.Shuttles[i].Position.Y := shInfoFB[i].ShuttleInfo.Position.Y * 1000;
			Vis.Shuttles[i].Position.Z := shInfoFB[i].ShuttleInfo.Position.Z * 1000;
		ELSE
			Vis.Shuttles[i].Visible := FALSE;
			shInfoFB[i].Enable := FALSE;
			shInfoFB[i]();
		END_IF
		
	END_FOR;	
  
END_PROGRAM
