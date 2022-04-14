
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif



/*
	//Incubator station
		When requested send the shuttle away to the requested destination

	//Analyzer		
		When requested, empty the completed well plate, reload the fresh one
		Make decision on if you should send to Pipette 1, 2 or a garage station

	//Pipette 1/2
		when a shuttle arrives
			Tips or Dye
				Wait for a set amount of time 
				if pipette 1/2 Tips/Dye is needed > Send to Pipette 1/2
				else > send to tips/dye
			Well plate
				move shuttle to X/Y position corresponding to recipe (grid based position)
				wait Defined time
				go to next recipe index
				repeat until sample set completed
				if recipeIsDone
					sendToAnalyzer
				else: 
					request Tip
				When Tip request has been answered
					send shuttle to tip station
					

		The TBD destinations are something passed into the station where to send it? Maybe defined by a global recipe?
	
	//Dye and Tips stations
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
	incubatorFb.Enable = enable;
	incubatorFb.Destination = &gDests[DEST_INCUBATOR].Destination; 
	analyzerFb.Enable = enable;
	analyzerFb.Destination = &gDests[DEST_ANALYZER].Destination;
	pipette1Fb.Enable = enable;
	pipette1Fb.Destination = &gDests[DEST_PIPETTE_1].Destination;
	pipette2Fb.Enable = enable;
	pipette2Fb.Destination = &gDests[DEST_PIPETTE_2].Destination;
	tipsFb.Enable = enable;
	tipsFb.Destination = &gDests[DEST_TIPS].Destination;
	dyeFb.Enable = enable;
	dyeFb.Destination = &gDests[DEST_DYE].Destination;
	
	moveShFb.Velocity = vel;
	moveShFb.Acceleration = accel;
	moveShFb.Destination = &gDests[nextStation].Destination;
	
	switch (currentStation){
		case DEST_INCUBATOR:
			moveShFb.RouterShuttle = &incubatorFb.RouterShuttle;
			break;
		case DEST_ANALYZER:
			moveShFb.RouterShuttle = &analyzerFb.RouterShuttle;
			break;
		case DEST_PIPETTE_1:
			moveShFb.RouterShuttle = &pipette1Fb.RouterShuttle;
			break;
		case DEST_PIPETTE_2:
			moveShFb.RouterShuttle = &pipette2Fb.RouterShuttle;
			break;
		case DEST_DYE:
			moveShFb.RouterShuttle = &dyeFb.RouterShuttle;
			break;
		case DEST_TIPS:
			moveShFb.RouterShuttle = &tipsFb.RouterShuttle;
			break;		
	}
	switch (optionalWaypoint)
	{
		case WP_NONE:
			moveShFb.Waypoint = 0;
			break;
		case WP_1:
			moveShFb.Waypoint = &gWps[WP_1].Waypoint;
			break;
		case WP_2:
			moveShFb.Waypoint = &gWps[WP_2].Waypoint;
			break;
		case WP_3:
			moveShFb.Waypoint = &gWps[WP_3].Waypoint;
			break;
		case WP_4:
			moveShFb.Waypoint = &gWps[WP_4].Waypoint;
			break;
		case WP_5:
			moveShFb.Waypoint = &gWps[WP_5].Waypoint;
			break;
	}
     
	rl6dDestinationMonitor(&incubatorFb);
	rl6dDestinationMonitor(&analyzerFb);
	rl6dDestinationMonitor(&pipette1Fb);
	rl6dDestinationMonitor(&pipette2Fb);
	rl6dDestinationMonitor(&tipsFb);
	rl6dDestinationMonitor(&dyeFb);
	rl6dShuttleMoveDestination(&moveShFb);
}
