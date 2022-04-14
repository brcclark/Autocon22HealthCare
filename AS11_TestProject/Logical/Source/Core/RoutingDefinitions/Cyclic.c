
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif
#define TRUE 1 
#define FALSE 0


void _CYCLIC ProgramCyclic(void)
{
	//On Command: Initalize the configuration for the routing library
	switch (state)
	{
		case DEF_IDLE:
			if(Init){
				fbs.routerFb.Assembly = &g6dAssembly;
				fbs.routerFb.UserDataSize = 4;
				fbs.routerFb.Enable = TRUE;
				
				//Starting from 1 because the 0 index is going to be the "emtpy or default" index for most items
				i = 1;
				
				state = DEF_INIT_ROUTER;
			}
			break;
		case DEF_INIT_ROUTER:
			if(fbs.routerFb.Error){
				
			}
			else if(fbs.routerFb.Active){
				fbs.addPathFb.Router = &fbs.routerFb.Router;
				fbs.addPathFb.DataAddress = &gDests[i].Entry.PathPoints;
				fbs.addPathFb.DataSize = sizeof(gDests[i].Entry.PathPoints[0]) * gDests[i].Entry.Length;
				brsstrcpy(&fbs.addPathFb.Name,&gDests[i].Entry.Name);
				fbs.addPathFb.Execute = TRUE;
				
				state = DEF_INIT_ENTRY_PATHS;
			}
			break;
		case DEF_INIT_ENTRY_PATHS:
			if(fbs.addPathFb.Error){
				state = DEF_ERROR;
			}
			else if(fbs.addPathFb.Done){
				gDests[i].Entry.Path = fbs.addPathFb.Path;
				fbs.addPathFb.Execute = FALSE;
				
				i += 1;
				if(i > numDestinations){
					i = 1;
					
					fbs.addPathFb.DataAddress = &gDests[i].Exit.PathPoints;
					fbs.addPathFb.DataSize = sizeof(gDests[i].Exit.PathPoints[0]) * gDests[i].Exit.Length;
					brsstrcpy(&fbs.addPathFb.Name,&gDests[i].Exit.Name);
					
					state = DEF_INIT_EXIT_PATHS;
				}
				else{
					fbs.addPathFb.DataAddress = &gDests[i].Entry.PathPoints;
					fbs.addPathFb.DataSize = sizeof(gDests[i].Entry.PathPoints[0]) * gDests[i].Entry.Length;
					brsstrcpy(&fbs.addPathFb.Name,&gDests[i].Entry.Name);
				}
				
			}
			else{
				fbs.addPathFb.Execute = TRUE;
			}
			break;
		case DEF_INIT_EXIT_PATHS:
			if(fbs.addPathFb.Error){
				state = DEF_ERROR;
			}
			else if(fbs.addPathFb.Done){
				gDests[i].Exit.Path = fbs.addPathFb.Path;
				fbs.addPathFb.Execute = FALSE;
				
				i += 1;
				if(i > numDestinations){
					i = 1;
					
					fbs.createDestFb.Router = &fbs.routerFb.Router;
					fbs.createDestFb.EntryPath = &gDests[i].Entry.Path;
					fbs.createDestFb.ExitPath = &gDests[i].Exit.Path;
					fbs.createDestFb.Position = gDests[i].Position;
					brsstrcpy(&fbs.createDestFb.Name,&gDests[i].StationName);
					fbs.createDestFb.Execute = TRUE;
					
					state = DEF_INIT_DESTINATIONS;
				}
				else{
					fbs.addPathFb.DataAddress = &gDests[i].Exit.PathPoints;
					fbs.addPathFb.DataSize = sizeof(gDests[i].Exit.PathPoints[0]) * gDests[i].Exit.Length;
					brsstrcpy(&fbs.addPathFb.Name,&gDests[i].Exit.Name);
				}
				
			}
			else{
				fbs.addPathFb.Execute = TRUE;
			}
			break;
		case DEF_INIT_DESTINATIONS:
			if(fbs.createDestFb.Error){
				state = DEF_ERROR;
			}
			else if(fbs.createDestFb.Done){
				gDests[i].Destination = fbs.createDestFb.Destination;
				fbs.createDestFb.Execute = FALSE;
				
				i++;
				//Check to see if we've gone through all destinations
				if(i > numDestinations){
					i = 1;
					
					//Setup the recovery points
					fbs.addRecPointFb.Destination = &gDests[i].Destination;
					fbs.addRecPointFb.DataAddress = &gDests[i].RecoveryPts;
					fbs.addRecPointFb.DataSize = sizeof(gDests[i].RecoveryPts);
					fbs.addRecPointFb.Execute = TRUE;
					
					state = DEF_INIT_RECOVERY_POINTS;
				}
				else{
					//Haven't finished the destinations, so configure to complete the next destination
					fbs.createDestFb.Router = &fbs.routerFb.Router;
					fbs.createDestFb.EntryPath = &gDests[i].Entry.Path;
					fbs.createDestFb.ExitPath = &gDests[i].Exit.Path;
					fbs.createDestFb.Position = gDests[i].Position;
					brsstrcpy(&fbs.createDestFb.Name,&gDests[i].StationName);
				}
				
			}
			else{
				//Re-execute the block to handle the new parameter set
				fbs.createDestFb.Execute = TRUE;
			}
			break;
		case DEF_INIT_RECOVERY_POINTS:
			if(fbs.addRecPointFb.Error){
				state = DEF_ERROR;
			}
			else if(fbs.addRecPointFb.Done){
				fbs.addRecPointFb.Execute = FALSE;
				
				i++;
				//Check to see if we've gone through all destinations
				if(i > numDestinations){
					i = 1;
					
					//Setup the waypoints
					fbs.createWaypointFb.Router = &fbs.routerFb.Router;
					fbs.createWaypointFb.Position = gWps[i].Position;
					brsstrcpy(&fbs.createWaypointFb.Name,gWps[i].Name);
					fbs.createWaypointFb.Execute = TRUE;
					
					state = DEF_INIT_WAYPOINTS;
				}
				else{
					//Haven't finished the recovery points, so configure to complete the next destination
					fbs.addRecPointFb.Destination = &gDests[i].Destination;
					fbs.addRecPointFb.DataAddress = &gDests[i].RecoveryPts;
					fbs.addRecPointFb.DataSize = sizeof(gDests[i].RecoveryPts);
				}
				
			}
			else{
				//Re-execute the block to handle the new parameter set
				fbs.addRecPointFb.Execute = TRUE;
			}
			break;
		case DEF_INIT_WAYPOINTS:
			if(fbs.createWaypointFb.Error){
				state = DEF_ERROR;
			}
			else if(fbs.createWaypointFb.Done){
				gWps[i].Waypoint = fbs.createWaypointFb.Waypoint;
				fbs.createWaypointFb.Execute = FALSE;
				
				i++;
				//Check to see if we've gone through all waypoints
				if(i > numWaypoints){
					i = 1;
					j = 0;
					
					if(gWps[i].Destinations[j] != DEST_DEFAULT){
						//We have, so now we need to add all destinations for each waypoint
						fbs.linkWayPointDestFb.Destination = &gDests[gWps[i].Destinations[j]].Destination;
						fbs.linkWayPointDestFb.Waypoint = &gWps[i].Waypoint;
						fbs.linkWayPointDestFb.Execute = TRUE;
					
						state = DEF_LINK_WAYPOINTS_TO_DESTS;
					}
					else{
						i++;
						state = DEF_LINK_WAYPOINTS_TO_DESTS;
					}
				}
				else{
					//Haven't finished the waypoints, so configure to complete the next waypoint
					fbs.createWaypointFb.Router = &fbs.routerFb.Router;
					fbs.createWaypointFb.Position = gWps[i].Position;
					brsstrcpy(&fbs.createWaypointFb.Name,gWps[i].Name);
					fbs.createWaypointFb.Execute = TRUE;
				}
				
			}
			else{
				//Re-execute the block to handle the new parameter set
				fbs.createWaypointFb.Execute = TRUE;
			}
			break;
		case DEF_CHECK_WAYPOINT_DESTS:
			//Check to see if we've finished iterating through all waypoints
			if(i > numWaypoints){
				//We're done, move onto the next init state
				i = 1;
				j = 0;
				fbs.linkWaypointFb.Waypoint = &gWps[i].Waypoint;
				fbs.linkWaypointFb.NextWaypoint = &gWps[gWps[i].NextWaypoint].Waypoint;
				fbs.linkWaypointFb.Execute = TRUE;
				
				state = DEF_LINK_WAYPOINTS_TO_WAYPOINTS;
			}
			else{
				//Check to see if the next destination is defined
				if(gWps[i].Destinations[j] != DEST_DEFAULT && j < 6){ //Added 5 here as that's the current size (6) of the array. This is a temp fix
					//The destination has been defined let's add it now
					fbs.linkWayPointDestFb.Destination = &gDests[gWps[i].Destinations[j]].Destination;
					fbs.linkWayPointDestFb.Waypoint = &gWps[i].Waypoint;
					fbs.linkWayPointDestFb.Execute = TRUE;
					
					state = DEF_LINK_WAYPOINTS_TO_DESTS;
				}
				else{
					//The destination is not defined, we therefore are done with this waypoint
					i++;
					j = 0;
				}
			}
			break;
		case DEF_LINK_WAYPOINTS_TO_DESTS:
			if(fbs.linkWayPointDestFb.Error){
				state = DEF_ERROR;
			}
			else if(fbs.linkWayPointDestFb.Done){
				fbs.linkWayPointDestFb.Execute = FALSE;
				j++;
				state = DEF_CHECK_WAYPOINT_DESTS;
			}
			break;
		case DEF_LINK_WAYPOINTS_TO_WAYPOINTS:
			if(fbs.linkWaypointFb.Error){
				state = DEF_ERROR;
			}
			else if(fbs.linkWaypointFb.Done){
				fbs.linkWaypointFb.Execute = FALSE;
				
				i++;
				//Check to see if we've gone through all waypoints
				if(i == numWaypoints){
					i = 1;
					//We're all done!
					state = DEF_DONE;	
				}
				else{
					//Haven't finished the waypoints, so configure to complete the next waypoint
					fbs.linkWaypointFb.Waypoint = &gWps[i].Waypoint;
					fbs.linkWaypointFb.NextWaypoint = &gWps[gWps[i].NextWaypoint].Waypoint;
				}
				
			}
			else{
				//Re-execute the block to handle the new parameter set
				fbs.linkWaypointFb.Execute = TRUE;
			}
			break;
		case DEF_DONE:
			
			Init = FALSE;
			if(gAcp6DIf.Sts.AssemblyReady){
			
				fbs.addShToRouter.Router = &fbs.routerFb.Router;
				fbs.addShToRouter.Execute = TRUE;
			
				state = DEF_ADD_SHUTTLES;
			}
			break;
		case DEF_ADD_SHUTTLES:
			if(fbs.addShToRouter.Error){
			}
			else if(fbs.addShToRouter.Done){
				fbs.addShToRouter.Execute = FALSE;
				state = DEF_RUNNING;
			}
			break;
		case DEF_RUNNING:
			fbs.recoverFb.Velocity = 1.0;
			fbs.recoverFb.Acceleration = 10.0;
			fbs.recoverFb.Router = &fbs.routerFb.Router;
			break;
		case DEF_ERROR:
			break;
	}
	rl6dRouterMan(&fbs.routerFb);
	rl6dRouterAddPathFromData(&fbs.addPathFb);
	rl6dRouterAddDestination(&fbs.createDestFb);
	rl6dRouterAddWaypoint(&fbs.createWaypointFb);
	rl6dWaypointAddDestination(&fbs.linkWayPointDestFb);
	rl6dWaypointSetNextWaypoint(&fbs.linkWaypointFb);
	rl6dRouterAddShFromAsm(&fbs.addShToRouter);
	rl6dDestAddRecoveryPosFromData(&fbs.addRecPointFb);
	rl6dRouterExecuteRecovery(&fbs.recoverFb);
}
