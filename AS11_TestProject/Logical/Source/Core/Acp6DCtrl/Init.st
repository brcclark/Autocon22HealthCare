

PROGRAM _INIT
	gFB;
	gAcp6DIf.Par.Asm := g6dAssembly;
	 
	gSysRecipe.StartupPars.MoveAsyncPars.Velocity := 1.0;
	gSysRecipe.StartupPars.MoveAsyncPars.Acceleration := 10.0;
	gSysRecipe.StartupPars.MoveAsyncPars.AsyncPar[0].X := 0.120;
	gSysRecipe.StartupPars.MoveAsyncPars.AsyncPar[0].Y := 0.120;
	gSysRecipe.StartupPars.MoveAsyncPars.AsyncPar[1].X := 0.360;
	gSysRecipe.StartupPars.MoveAsyncPars.AsyncPar[1].Y := 0.120;
END_PROGRAM