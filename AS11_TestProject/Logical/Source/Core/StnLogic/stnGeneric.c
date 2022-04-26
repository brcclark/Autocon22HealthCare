
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
#include "StnLogic.h"
#ifdef __cplusplus
	};
#endif

#define TRUE 1
#define FALSE 0

/* TODO: Add your comment here */
void stnGeneric(struct stnGeneric* fb)
{
	switch (fb->internal.state)
	{
		//******************** Off State
		case STN_GENERIC_OFF:
			if(fb->Enable){
				fb->internal.fbs.destMonFb.Destination = fb->Destination;
				fb->internal.fbs.destMonFb.Enable = TRUE;
				fb->internal.state = STN_GENERIC_INIT;
			}
			break;
		//******************** Initalize State
		case STN_GENERIC_INIT:
			
			if(fb->internal.fbs.destMonFb.Valid){
				fb->Active = TRUE;
			}
			
			if(fb->internal.fbs.destMonFb.Valid && fb->Run){
				fb->internal.state = STN_GENERIC_READY;
			}
			
			break;
		//******************** Ready and Waiting For a Shuttle State
		case STN_GENERIC_READY:
			if(fb->internal.fbs.destMonFb.ShuttlePresent){
				fb->ShuttlePresent = TRUE;
				fb->internal.fbs.waitFb.PT = fb->WaitTime;
				fb->internal.fbs.waitFb.IN = TRUE;
				rl6dShCopyUserDataSync(&fb->internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_READ,&fb->ShUserData,sizeof(fb->ShUserData));
				
				fb->internal.state = STN_GENERIC_PROCESS;	
			}
			break;
		//******************** Processing State
		case STN_GENERIC_PROCESS:
			rl6dShCopyUserDataSync(&fb->internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_READ,&fb->ShUserData,sizeof(fb->ShUserData));
			if(fb->internal.fbs.waitFb.Q){
				fb->internal.fbs.waitFb.IN = FALSE;
				fb->WaitComplete = TRUE;
				
				fb->internal.state = STN_GENERIC_WAIT_FOR_CMD;
			}
			
			break;
		//******************** Waiting for the send command State
		case STN_GENERIC_WAIT_FOR_CMD:
			rl6dShCopyUserDataSync(&fb->internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_READ,&fb->ShUserData,sizeof(fb->ShUserData));
			if(fb->Send){
				//Update the user data
				if(fb->ResetData){
					brsmemset(&fb->ShUserData,0,sizeof(fb->ShUserData));
					fb->ErrorID = rl6dShCopyUserDataSync(&fb->internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_WRITE,&fb->ShUserData,sizeof(fb->ShUserData));
				}
				if(fb->ApplyRecipe){
					fb->ShUserData.Recipe = fb->Recipe;
				}
				fb->ShUserData.CurrentColor = fb->ShColor;
				fb->ShUserData.ShType = fb->ShuttleType;
				fb->ErrorID = rl6dShCopyUserDataSync(&fb->internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_WRITE,&fb->ShUserData,sizeof(fb->ShUserData));
			
				fb->internal.fbs.moveDestFb.RouterShuttle = &fb->internal.fbs.destMonFb.RouterShuttle;
				fb->internal.fbs.moveDestFb.Destination = fb->NextDestination;
				fb->internal.fbs.moveDestFb.Waypoint = fb->Waypoint;
				fb->internal.fbs.moveDestFb.Velocity = fb->Velocity;
				fb->internal.fbs.moveDestFb.Acceleration = fb->Accel;
				fb->internal.fbs.moveDestFb.Execute = TRUE;
				
				fb->internal.state = STN_GENERIC_SEND;
			}
			break;
		//******************** Send Shuttle State State
		case STN_GENERIC_SEND:
			if(fb->internal.fbs.moveDestFb.Error){
				fb->internal.errorState = fb->internal.state;
				fb->Error = TRUE;
				fb->ErrorID = fb->internal.fbs.moveDestFb.ErrorID;
				fb->internal.state = STN_GENERIC_ERROR;
			}
			else if(fb->internal.fbs.moveDestFb.Active){
				fb->internal.fbs.destMonFb.AcknowledgeShuttle = TRUE;
				if(!fb->internal.fbs.destMonFb.ShuttlePresent){
					fb->internal.fbs.destMonFb.AcknowledgeShuttle = FALSE;
					fb->internal.fbs.moveDestFb.Execute = FALSE;
					fb->ShuttlePresent = FALSE;
					fb->WaitComplete = FALSE;
					brsmemset(&fb->ShUserData,0,sizeof(fb->ShUserData));

					fb->internal.state = STN_GENERIC_DONE;
				}
			}
			break;
		//******************** Waiting for the Send Cmd To Drop State
		case STN_GENERIC_DONE:
			if(!fb->Send){
				fb->internal.state = STN_GENERIC_READY;
			}
			break;
		//******************** Error State
		case STN_GENERIC_ERROR:
			if(!fb->Enable){
				
			}
			break;
		//******************** Reset State
		case STN_GENERIC_RESET:
			
			break;
	}
    
	TON(&fb->internal.fbs.waitFb);
	rl6dDestinationMonitor(&fb->internal.fbs.destMonFb);
	rl6dShuttleMoveDestination(&fb->internal.fbs.moveDestFb);
}
