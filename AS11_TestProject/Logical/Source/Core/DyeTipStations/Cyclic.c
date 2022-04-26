
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
	gStationsIf.Sts.IncubatorActive = dyeStFb.Active;
	
	//Tips Station Logic
	switch (tipsState)
	{
		case GEN_OFF:
			if(gStationsIf.Cmd.Enable){
				tipsStFb.Destination = &gDests[DEST_TIPS].Destination;
				tipsStFb.Velocity = gSysRecipe.DefaultVel;
				tipsStFb.Accel = gSysRecipe.DefaultAccel;
				tipsStFb.WaitTime = 0;//gSysRecipe.DefaultWaitTime;
				tipsStFb.ShuttleType = SH_TIPS;
				tipsStFb.Enable = TRUE;
				
				tipsState = GEN_STARTUP;
			}
			break;
		case GEN_STARTUP:
			if(tipsStFb.Active){
				tipsState = GEN_READY;
			}
			break;
		case GEN_READY:
			if(gStationsIf.Cmd.Run){
				tipsStFb.Run = TRUE;
				
				tipsState = GEN_WAIT_REQUEST;
			}
			break;
		case GEN_WAIT_SHUTTLE:
			gStationsIf.Sts.TipsShuttlePresent = tipsStFb.ShuttlePresent;
			if(gStationsIf.Sts.TipsShuttlePresent){
				//gStationsIf.Cmd.RequestTip = FALSE;
				tipsState = GEN_WAIT_REQUEST;
			}
			
			break;
		case GEN_WAIT_REQUEST:
			gStationsIf.Sts.TipsShuttlePresent= tipsStFb.ShuttlePresent;
			if(gStationsIf.Cmd.RequestTip){
				tipsStFb.NextDestination = &gDests[gStationsIf.Par.Tip.RequestDestination].Destination;
				tipsStFb.Waypoint = &gWps[WP_5].Waypoint;
				tipsStFb.Send = TRUE;
				
				if(!tipsStFb.ShuttlePresent){
					tipsStFb.Send = FALSE;
					tipsState = GEN_WAIT_SHUTTLE;
				}
			}
			break;
	}
	
	//Dye Logic
	switch (dyeState)
	{
		case GEN_OFF:
			if(gStationsIf.Cmd.Enable){
				dyeStFb.Destination = &gDests[DEST_DYE].Destination;
				dyeStFb.Velocity = gSysRecipe.DefaultVel;
				dyeStFb.Accel = gSysRecipe.DefaultAccel;
				dyeStFb.WaitTime = 0;//gSysRecipe.DefaultWaitTime;
				dyeStFb.ShuttleType = SH_DYE;
				dyeStFb.Enable = TRUE;
				
				dyeState = GEN_STARTUP;
			}
			break;
		case GEN_STARTUP:
			if(dyeStFb.Active){
				dyeState = GEN_READY;
			}
			break;
		case GEN_READY:
			if(gStationsIf.Cmd.Run){
				dyeStFb.Run = TRUE;
				
				dyeState = GEN_WAIT_REQUEST;
			}
			break;
		case GEN_WAIT_SHUTTLE:
			gStationsIf.Sts.DyeShuttlePresent = dyeStFb.ShuttlePresent;
			if(gStationsIf.Sts.DyeShuttlePresent){
				dyeState = GEN_WAIT_REQUEST;
			}
			break;
		case GEN_WAIT_REQUEST:
			gStationsIf.Sts.DyeShuttlePresent  = dyeStFb.ShuttlePresent;
			//Adding the Request tip check to try and prevent a deadlock
			if(gStationsIf.Cmd.RequestDye && !gStationsIf.Cmd.RequestTip){
				dyeStFb.NextDestination = &gDests[gStationsIf.Par.Dye.RequestDestination].Destination;
				dyeStFb.Waypoint = &gWps[WP_5].Waypoint;
				dyeStFb.Send = TRUE;
				
				if(!dyeStFb.ShuttlePresent){
					dyeStFb.Send = FALSE;
					dyeState = GEN_WAIT_SHUTTLE;
				}
			}
			break;
	}
	stnGeneric(&dyeStFb);
	stnGeneric(&tipsStFb);
}
