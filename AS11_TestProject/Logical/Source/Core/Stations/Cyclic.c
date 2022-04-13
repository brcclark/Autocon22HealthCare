
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

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
	
	moveShFb.Velocity = 1.0;
	moveShFb.Acceleration = 10.0;
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
	rl6dDestinationMonitor(&incubatorFb);
	rl6dDestinationMonitor(&analyzerFb);
	rl6dDestinationMonitor(&pipette1Fb);
	rl6dDestinationMonitor(&pipette2Fb);
	rl6dDestinationMonitor(&tipsFb);
	rl6dDestinationMonitor(&dyeFb);
	rl6dShuttleMoveDestination(&moveShFb);
}
