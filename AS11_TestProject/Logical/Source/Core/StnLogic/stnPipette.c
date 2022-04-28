
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
void stnPipette(struct stnPipette* fb)
{
	switch (fb->internal.state)
	{
		//******************** Off State
		case STN_PIPETTE_OFF:
			if(fb->Enable){
				fb->internal.fbs.destMonFb.Destination = fb->Destination;
				fb->internal.fbs.destMonFb.Enable = TRUE;
				fb->internal.state = STN_PIPETTE_INIT;
			}
			break;
		//******************** Initalize State
		case STN_PIPETTE_INIT:
			
			if(fb->internal.fbs.destMonFb.Valid){
				fb->Active = TRUE;
				fb->internal.state = STN_PIPETTE_WAIT_FIRST;
			}
			break;
		//******************** Waiting For the First Shuttle States
		case STN_PIPETTE_WAIT_FIRST:
			if(fb->internal.fbs.destMonFb.ShuttlePresent){
				fb->internal.fbs.moveDestFb.RouterShuttle = &fb->internal.fbs.destMonFb.RouterShuttle;
				fb->internal.fbs.moveDestFb.Destination = fb->FirstDestination;
				fb->internal.fbs.moveDestFb.Waypoint = fb->Waypoint;
				fb->internal.fbs.moveDestFb.Velocity = fb->Velocity;
				fb->internal.fbs.moveDestFb.Acceleration = fb->Accel;
				fb->internal.fbs.moveDestFb.Execute = TRUE;
				
				fb->ShUserData.shId = fb->InitialId;
				rl6dShCopyUserDataSync(&fb->internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_WRITE,&fb->ShUserData,sizeof(fb->ShUserData));
				
				fb->internal.state = STN_PIPETTE_SEND_FIRST;
			}
			break;
		//******************** Sending the first shuttle state
		case STN_PIPETTE_SEND_FIRST:
			if(fb->internal.fbs.moveDestFb.Active){
				fb->internal.fbs.destMonFb.AcknowledgeShuttle = TRUE;
				if(!fb->internal.fbs.destMonFb.ShuttlePresent){
					fb->internal.fbs.moveDestFb.Execute = FALSE;
					fb->internal.fbs.destMonFb.AcknowledgeShuttle = FALSE;
					fb->internal.state = STN_PIPETTE_READY;
				}
			}
			break;
		//******************** Ready and Waiting For a Shuttle State
		case STN_PIPETTE_READY:
			fb->ShuttlePresent = fb->internal.fbs.destMonFb.ShuttlePresent;
			if(fb->ShuttlePresent)
				rl6dShCopyUserDataSync(&fb->internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_READ,&fb->ShUserData,sizeof(fb->ShUserData));
			if(fb->internal.fbs.destMonFb.ShuttlePresent && fb->Operate){
				fb->Shuttle = fb->internal.fbs.destMonFb.RouterShuttle;
				rl6dShCopyUserDataSync(&fb->internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_READ,&fb->ShUserData,sizeof(fb->ShUserData));
				if(fb->ShUserData.ShType != SH_WELL){
					fb->ShuttlePresent = TRUE;
					fb->internal.fbs.waitFb.PT = fb->WaitTime;
					fb->internal.fbs.waitFb.IN = TRUE;
				
					fb->internal.state = STN_PIPETTE_DO_WAIT;	
				}
				else{
					USINT sampleIndex;
					sampleIndex = fb->ShUserData.Recipe.CurrentSample;
					fb->ShUserData.Recipe.Samples[sampleIndex].CurrentCount = 0;
					
					if(!fb->DyePresent){
						fb->internal.fillPos.Y = fb->ShUserData.Recipe.SampleWellCount / fb->WellPlateInfo.NumColumns;
						fb->internal.fillPos.X = fb->ShUserData.Recipe.SampleWellCount - (fb->internal.fillPos.Y * fb->WellPlateInfo.NumColumns);
					
					}
					else{
						fb->internal.fillPos.Y = fb->ShUserData.Recipe.DyeWellCount / fb->WellPlateInfo.NumColumns;
						fb->internal.fillPos.X = fb->ShUserData.Recipe.DyeWellCount - (fb->internal.fillPos.Y * fb->WellPlateInfo.NumColumns);
					}
					fb->internal.fbs.moveInPlaneFb.Shuttle = &fb->internal.fbs.destMonFb.Mc6DShuttle;
					fb->internal.fbs.moveInPlaneFb.Parameters.Velocity = fb->Velocity;
					fb->internal.fbs.moveInPlaneFb.Parameters.Acceleration = fb->Accel;
					fb->internal.fbs.moveInPlaneFb.Parameters.Path = mcACP6D_PATH_DIRECT;
					fb->internal.fbs.moveInPlaneFb.Parameters.Position.X = (fb->WellPlateInfo.OffsetX - fb->internal.fillPos.X * fb->WellPlateInfo.ColumnGap);
					fb->internal.fbs.moveInPlaneFb.Parameters.Position.Y = (fb->WellPlateInfo.OffsetY - fb->internal.fillPos.Y * fb->WellPlateInfo.RowGap);
					fb->internal.fbs.moveInPlaneFb.Parameters.Mode = mcACP6D_MOVE_RELATIVE;
					fb->internal.fbs.moveInPlaneFb.Execute = TRUE;
					
					fb->internal.fillState = FILL_MOVE;
					fb->internal.state = STN_PIPETTE_PROCESS;
				}
			}
			break;
		//******************** "Wait" process State
		case STN_PIPETTE_DO_WAIT:
			rl6dShCopyUserDataSync(&fb->internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_READ,&fb->ShUserData,sizeof(fb->ShUserData));
			fb->Waiting = TRUE;
			if(fb->internal.fbs.waitFb.Q){
				fb->Waiting = FALSE;
				fb->internal.fbs.waitFb.IN = FALSE;
				fb->WaitComplete = TRUE;
				
				fb->internal.state = STN_PIPETTE_WAIT_FOR_CMD;
			}
			
			break;
		//******************** Processing State
		case STN_PIPETTE_PROCESS:
			//in this state we're going to make the indexing movements around to fill up the shuttle with product
			
			switch (fb->internal.fillState)
			{
				case FILL_MOVE:
					if(fb->internal.fbs.moveInPlaneFb.Done){
						fb->internal.fbs.moveInPlaneFb.Execute = FALSE;
						
						fb->internal.fbs.waitFb.IN = TRUE;
						fb->internal.fbs.waitFb.PT = fb->WaitTime;
						
						fb->internal.fillState = FILL_FILL;
					}
					break;

				case FILL_FILL:
					fb->Waiting = TRUE;
					if(fb->internal.fbs.waitFb.Q){
						fb->Waiting = FALSE;
						fb->internal.fbs.waitFb.IN = FALSE;
						
						if(fb->DyePresent){
							fb->internal.fbs.moveArcFb.Shuttle = &fb->internal.fbs.destMonFb.Mc6DShuttle;
							fb->internal.fbs.moveArcFb.Parameters.Acceleration = 10.0;
							fb->internal.fbs.moveArcFb.Parameters.Velocity = 0.2;
							fb->internal.fbs.moveArcFb.Parameters.Angular.Center.X = -0.003;
							fb->internal.fbs.moveArcFb.Parameters.Angular.Center.Y = -0.003;
							fb->internal.fbs.moveArcFb.Parameters.Angular.Angle = 3.14*2*5;
							fb->internal.fbs.moveArcFb.Parameters.Mode = mcACP6D_MOVE_RELATIVE;
							fb->internal.fbs.moveArcFb.Parameters.Option = mcACP6D_ARC_ANGULAR;
							fb->internal.fbs.moveArcFb.Execute = TRUE;
							fb->internal.fillState = FILL_SWIRL;
						}
						else{
							fb->ShUserData.Wells[fb->ShUserData.Recipe.SampleWellCount].Present = TRUE;
							fb->ShUserData.Wells[fb->ShUserData.Recipe.SampleWellCount].Color = fb->ShUserData.Recipe.Samples[fb->ShUserData.Recipe.CurrentSample].Sample;
							fb->ShUserData.Recipe.SampleWellCount++;
							fb->ShUserData.Recipe.Samples[fb->ShUserData.Recipe.CurrentSample].CurrentCount++;
							fb->internal.fillState = FILL_CHECK_RECIPE;
						}
					}
					break;
				case FILL_SWIRL:
					if(fb->internal.fbs.moveArcFb.Done){
						fb->internal.fbs.moveArcFb.Execute = FALSE;
						fb->ShUserData.Wells[fb->ShUserData.Recipe.DyeWellCount].Color += 1;
						fb->ShUserData.Recipe.DyeWellCount++;
						fb->ShUserData.Recipe.Samples[fb->ShUserData.Recipe.CurrentSample].CurrentCount++;
						fb->internal.fillState = FILL_CHECK_RECIPE;
					}
					break;
				case FILL_CHECK_RECIPE:
					if(fb->ShUserData.Recipe.Samples[fb->ShUserData.Recipe.CurrentSample].CurrentCount == fb->ShUserData.Recipe.Samples[fb->ShUserData.Recipe.CurrentSample].SampleCount){
						//Finished filling this sample if the dye has present only (we apply the sample then the dye)
						if(fb->DyePresent){
							fb->ShUserData.Recipe.CurrentSample++;
						}
						fb->OperateDone = TRUE;
						fb->internal.state = STN_PIPETTE_WAIT_FOR_CMD;
					}
					else{
						fb->internal.fillPos.X++;
						if(fb->internal.fillPos.X == fb->WellPlateInfo.NumColumns){
							fb->internal.fbs.moveInPlaneFb.Parameters.Position.X = (fb->WellPlateInfo.NumColumns - 1)* fb->WellPlateInfo.ColumnGap;
							fb->internal.fbs.moveInPlaneFb.Parameters.Position.Y = -fb->WellPlateInfo.RowGap;
							fb->internal.fillPos.Y++;
							fb->internal.fillPos.X = 0;
						}
						else{
							fb->internal.fbs.moveInPlaneFb.Parameters.Position.X = -fb->WellPlateInfo.ColumnGap;
							fb->internal.fbs.moveInPlaneFb.Parameters.Position.Y = 0;
						}
						fb->internal.fbs.moveInPlaneFb.Execute = TRUE;
						fb->internal.fillState = FILL_MOVE;
					}
					break;
			}
			rl6dShCopyUserDataSync(&fb->internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_WRITE,&fb->ShUserData,sizeof(fb->ShUserData));
			break;
		//******************** Waiting for the send command State
		case STN_PIPETTE_WAIT_FOR_CMD:
			rl6dShCopyUserDataSync(&fb->internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_READ,&fb->ShUserData,sizeof(fb->ShUserData));
			if(fb->Send){
				//Update the user data
				rl6dShCopyUserDataSync(&fb->internal.fbs.destMonFb.RouterShuttle,rl6dSH_COPY_USER_DATA_WRITE,&fb->ShUserData,sizeof(fb->ShUserData));
			
				fb->internal.fbs.moveDestFb.RouterShuttle = &fb->internal.fbs.destMonFb.RouterShuttle;
				fb->internal.fbs.moveDestFb.Destination = fb->NextDestination;
				fb->internal.fbs.moveDestFb.Waypoint = fb->Waypoint;
				fb->internal.fbs.moveDestFb.Velocity = fb->Velocity;
				fb->internal.fbs.moveDestFb.Acceleration = fb->Accel;
				fb->internal.fbs.moveDestFb.Execute = TRUE;
				
				fb->internal.state = STN_PIPETTE_SEND;
			}
			break;
		//******************** Send Shuttle State State
		case STN_PIPETTE_SEND:
			if(fb->internal.fbs.moveDestFb.Error){
				fb->internal.errorState = fb->internal.state;
				fb->Error = TRUE;
				fb->ErrorID = fb->internal.fbs.moveDestFb.ErrorID;
				fb->internal.state = STN_PIPETTE_ERROR;
			}
			else if(fb->internal.fbs.moveDestFb.Active){
				fb->internal.fbs.destMonFb.AcknowledgeShuttle = TRUE;
				if(!fb->internal.fbs.destMonFb.ShuttlePresent){
					fb->internal.fbs.destMonFb.AcknowledgeShuttle = FALSE;
					fb->internal.fbs.moveDestFb.Execute = FALSE;
					fb->ShuttlePresent = FALSE;
					fb->WaitComplete = FALSE;
					fb->OperateDone = FALSE;
					brsmemset(&fb->ShUserData,0,sizeof(fb->ShUserData));
					brsmemset(&fb->Shuttle,0,sizeof(fb->Shuttle));
					fb->internal.state = STN_PIPETTE_DONE;
				}
			}
			break;
		//******************** Waiting for the Send Cmd To Drop State
		case STN_PIPETTE_DONE:
			if(!fb->Send){
				fb->internal.state = STN_PIPETTE_READY;
			}
			break;
		//******************** Error State
		case STN_PIPETTE_ERROR:
			if(!fb->Enable){
				
			}
			break;
		//******************** Reset State
		case STN_PIPETTE_RESET:
			
			break;
	}
    
	TON(&fb->internal.fbs.waitFb);
	MC_BR_MoveInPlane_Acp6D(&fb->internal.fbs.moveInPlaneFb);
	MC_BR_MoveArc_Acp6D(&fb->internal.fbs.moveArcFb);
	rl6dDestinationMonitor(&fb->internal.fbs.destMonFb);
	rl6dShuttleMoveDestination(&fb->internal.fbs.moveDestFb);
}
