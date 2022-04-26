
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
			
			IF gShIfs[i].Cfg.RouterShuttle.Ident <> 0 AND i < 3 THEN
				rl6dShCopyUserDataSync(gShIfs[i].Cfg.RouterShuttle,rl6dSH_COPY_USER_DATA_READ,ADR(userData),SIZEOF(userData));
				Vis.Shuttles[i].Wells := userData.Wells;
				Vis.Shuttles[i].ShuttleColor := userData.CurrentColor;
				IF Vis.Shuttles[i].ShuttleColor = COLOR_UNDEF THEN
					Vis.Shuttles[i].ToolVisible := FALSE;
				ELSE
					Vis.Shuttles[i].ToolVisible := TRUE;
				END_IF
			END_IF
		ELSE
			Vis.Shuttles[i].Visible := FALSE;
			shInfoFB[i].Enable := FALSE;
			shInfoFB[i]();
		END_IF
	END_FOR;	
  
END_PROGRAM
