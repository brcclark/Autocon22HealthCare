

PROGRAM _INIT
	gSysRecipe.DefaultVel := 0.5;
	gSysRecipe.DefaultAccel := 5;
	gSysRecipe.DefaultWaitTime := 1000;
	
	gStationsIf.Par.Incubator.Waypoint := WP_1;
END_PROGRAM