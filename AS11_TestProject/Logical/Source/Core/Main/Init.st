

PROGRAM _INIT
	gSysRecipe.DefaultVel := 0.5;
	gSysRecipe.DefaultAccel := 5;
	gSysRecipe.DefaultWaitTime := 1000;
	
	gStationsIf.Par.Incubator.Waypoint := WP_1;
	
	gSysRecipe.PipettePars.HomePosition := 70;
	gSysRecipe.PipettePars.SampleDistance := 50;
	gSysRecipe.PipettePars.WellDistance := 0;
	gSysRecipe.PipettePars.Vel := 50;
	gSysRecipe.PipettePars.TipPosition := -10;
	
END_PROGRAM