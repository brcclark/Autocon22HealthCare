
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#define TRUE 1
#define FALSE 0

/*

	General Station IF needs
		Cmd.Enable
		Cmd.Run
		
	
	//Incubator station
		When requested send the shuttle away to the requested destination

	//Analyzer		
		When requested, empty the completed well plate, reload the fresh one
		Make decision on if you should send to Pipette 1, 2 or a garage station

	//Dye and Tips stations
		Controlled together as a garage
			Garage publishes current status of the garages and their types of shuttles (if present)
		You make a request to the garage to request a shuttle of type to send to provided destination
		When Shuttle is present: Publish the type of shuttle present
		when requested, send the shuttle to the requested station


	Shuttle User Data
		Destination
		RecipeId
		ShuttleType
			WellPlate
			CellSample
			Tips
			Dye
		WellPlate Wells Status[bool:96]
		
		
	Recipe Definition
		FillTime
		NumSamples
		[Sample]
			Sample: Color
			SampleCount: 1-x
*/

void _CYCLIC ProgramCyclic(void)
{
	gStationsIf.Sts.IncubatorActive = incubatorStFb.Active;
	
	//Incubator Logic
	switch (incubatorState)
	{
		case INC_OFF:
			if(gStationsIf.Cmd.Enable){
				incubatorStFb.Destination = &gDests[DEST_INCUBATOR].Destination;
				incubatorStFb.Velocity = gSysRecipe.DefaultVel;
				incubatorStFb.Accel = gSysRecipe.DefaultAccel;
				incubatorStFb.WaitTime = gSysRecipe.DefaultWaitTime * 2;
				incubatorStFb.ShuttleType = SH_SAMPLE;
				incubatorStFb.Enable = TRUE;
				
				incubatorState = INC_STARTUP;
			}
			break;
		case INC_STARTUP:
			if(incubatorStFb.Active){
				incubatorState = INC_READY;
			}
			break;
		case INC_READY:
			if(gStationsIf.Cmd.Run){
				incubatorStFb.Run = TRUE;
				
				incubatorState = INC_WAIT_SHUTTLE;
			}
			break;
		case INC_WAIT_SHUTTLE:
			gStationsIf.Sts.IncubatorShuttlePresent = incubatorStFb.ShuttlePresent;
			if(incubatorStFb.ShuttlePresent){
				gShIfs[2].Cfg.RouterShuttle = incubatorStFb.internal.fbs.destMonFb.RouterShuttle; //Hack to set the RouterSh interface
				//This is all an ugly hack
				incubatorStFb.ShUserData.CurrentColor = COLOR_UNDEF;
				rl6dShCopyUserDataSync(&incubatorStFb.internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_WRITE,&incubatorStFb.ShUserData,sizeof(UserDataType));
				
				//end hack
				incubatorState = INC_WAIT_REQUEST;
			}			
			break;
		case INC_WAIT_REQUEST:
			gStationsIf.Sts.IncubatorShuttlePresent = incubatorStFb.ShuttlePresent;
			if(gStationsIf.Cmd.RequestIncubator){
				incubatorStFb.NextDestination = &gDests[gStationsIf.Par.Incubator.NextStation].Destination;
				incubatorStFb.Waypoint = &gWps[gStationsIf.Par.Incubator.Waypoint].Waypoint;
				incubatorStFb.ShColor = gStationsIf.Par.Incubator.SampleType;
				incubatorStFb.Send = TRUE;
				
				if(!incubatorStFb.ShuttlePresent){
					gStationsIf.Cmd.RequestIncubator = FALSE;
					incubatorStFb.Send = FALSE;
					incubatorState = INC_WAIT_SHUTTLE;
				}
			}
			break;
	}
	
	//Analyzer Logic
	switch (analyzerState)
	{
		case ANALYZER_OFF:
			if(gStationsIf.Cmd.Enable){
				analyzerStFb.Destination = &gDests[DEST_ANALYZER].Destination;
				gStationsIf.Par.Analyzer.Waypoint = WP_1;
				analyzerStFb.Velocity = gSysRecipe.DefaultVel;
				analyzerStFb.Accel = gSysRecipe.DefaultAccel;
				analyzerStFb.WaitTime = gSysRecipe.DefaultWaitTime;
				analyzerStFb.ShuttleType = SH_WELL;
				analyzerStFb.ResetData = TRUE;
				analyzerStFb.Enable = TRUE;
				
				analyzerState = ANALYZER_STARTUP;
			}
			break;
		case ANALYZER_STARTUP:
			if(analyzerStFb.Active){
				analyzerState = ANALYZER_READY;
			}
			break;
		case ANALYZER_READY:
			if(gStationsIf.Cmd.Run){
				analyzerStFb.Run = TRUE;
				
				analyzerState = ANALYZER_WAIT_SHUTTLE;
			}
			break;
		case ANALYZER_WAIT_SHUTTLE:
			gStationsIf.Sts.AnalyzerShuttlePresent = analyzerStFb.ShuttlePresent;
			if(analyzerStFb.ShuttlePresent){
				if(!initChecked){
					gShIfs[shCount].Cfg.RouterShuttle = analyzerStFb.internal.fbs.destMonFb.RouterShuttle;
					shCount++;
					if(shCount==2)
						initChecked = TRUE;
				}
				analyzerState = ANALYZER_WAIT_REQUEST;
			
			}
			break;
		case ANALYZER_WAIT_REQUEST:
			gStationsIf.Sts.AnalyzerShuttlePresent = analyzerStFb.ShuttlePresent;
			if(gStationsIf.Cmd.RequestAnalyzer){
				analyzerStFb.NextDestination = &gDests[gStationsIf.Par.Analyzer.NextStation].Destination;
				analyzerStFb.Waypoint = &gWps[gStationsIf.Par.Analyzer.Waypoint].Waypoint;
				analyzerState = ANALYZER_WAIT_PIPETTE_READY;
			}
			
			if(analyzerStFb.ShuttlePresent && (gStationsIf.Sts.PipetteReadyForShuttle[0] || gStationsIf.Sts.PipetteReadyForShuttle[1])){
				//if we have a shuttle and either pipette station is ready, we should send a sample to it	
				analyzerState = ANALYZER_REQUEST_SAMPLE;
			}
			
			break;
		case ANALYZER_REQUEST_SAMPLE:
			if(!gStationsIf.Cmd.RequestIncubator){
				gStationsIf.Cmd.RequestIncubator = TRUE;
				
				if(gStationsIf.Sts.PipetteReadyForShuttle[0]){
					//This is all an ugly hack
					analyzerStFb.ShUserData.Recipe = gSysRecipe.WellRecipe[0];
					rl6dShCopyUserDataSync(&analyzerStFb.internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_WRITE,&analyzerStFb.ShUserData,sizeof(UserDataType));
					gStationsIf.Par.Incubator.SampleType = analyzerStFb.ShUserData.Recipe.Samples[0].Sample;	
					//end hack
					gStationsIf.Par.Incubator.NextStation = DEST_PIPETTE_1;
					gStationsIf.Sts.PipetteReadyForShuttle[0] = FALSE;
				}
				else if (gStationsIf.Sts.PipetteReadyForShuttle[1]){
					//This is all an ugly hack
					analyzerStFb.ShUserData.Recipe = gSysRecipe.WellRecipe[1];
					rl6dShCopyUserDataSync(&analyzerStFb.internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_WRITE,&analyzerStFb.ShUserData,sizeof(UserDataType));
					gStationsIf.Par.Incubator.SampleType = analyzerStFb.ShUserData.Recipe.Samples[0].Sample;	
					//end hack
					
					gStationsIf.Par.Incubator.NextStation = DEST_PIPETTE_2;
					gStationsIf.Sts.PipetteReadyForShuttle[1] = FALSE;
				}
				analyzerState = ANALYZER_WAIT_REQUEST;
			}
			break;
		case ANALYZER_WAIT_PIPETTE_READY:
			gStationsIf.Sts.AnalyzerShuttlePresent = analyzerStFb.ShuttlePresent;
			
			analyzerStFb.Recipe = gSysRecipe.WellRecipe[gStationsIf.Par.Analyzer.NextStation - DEST_PIPETTE_1];
			analyzerStFb.ApplyRecipe = TRUE;
			analyzerStFb.Send = TRUE;
			if(!analyzerStFb.ShuttlePresent){
				gStationsIf.Cmd.RequestAnalyzer = FALSE;
				analyzerStFb.Send = FALSE;
				analyzerState = ANALYZER_WAIT_SHUTTLE;
			}
			
			break;
	}
	stnGeneric(&incubatorStFb);
	stnGeneric(&analyzerStFb);
}
