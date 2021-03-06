
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#define TRUE 1
#define FALSE 0

/*
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
*/


/*
	Wait for a shuttle to get assigned
		That shuttle defines the recipe we'll be using
		When tip enroute, if shuttle is at pipette, send shuttle to none
		When tip arrives, wait time to complete, request sample from incubator, Send Tip to tip destination
		when sample arrives, wait time to complete, send sample back to incubator, Update shuttle to go to Pipette
		When well plate arrives, wait for process to complete (should be doing indexing filling at that point)
		when process completed, request Tip, send shuttle to none
		When tip arrives, wait time to complete, send tip back to tips, request dye
		when dye completes, send back to dye station, update shuttle to go back to Pipette
		when sample arrives, doing the same waiting for indexing again
		
		at this point, we've completed a sample set, so increment the sample set. 
		if recpipe is finished, send to analyzer and head back to 1st wait state
		otherwise, repeat process again (request new tip etc)
		
			
		
	
*/
void _CYCLIC ProgramCyclic(void)
{

	for(i = 0; i < 2; i++){
		
		//Pipette Logic
		switch (pipetteState[i])
		{
			case PIP_OFF:
				if(gStationsIf.Cmd.Enable){
					pipetteFb[i].Velocity = gSysRecipe.DefaultVel;
					pipetteFb[i].Accel = gSysRecipe.DefaultAccel;
					pipetteFb[i].WaitTime = gSysRecipe.DefaultWaitTime;
					pipetteFb[i].InitialId = i;
					pipetteFb[i].Enable = TRUE;
				
					pipetteState[i] = PIP_STARTUP;
				}
				break;
		
			case PIP_STARTUP:
				if(pipetteFb[i].Active){
					
					gStationsIf.Sts.PipetteActive[i] = TRUE;
					shIf[i].PipetteCtrl.Position = gSysRecipe.PipettePars.HomePosition;
					pipetteState[i] = PIP_READY;
				}
				break;
			case PIP_READY:
				startupFlag[i] = FALSE;
				pipetteState[i] = PIP_REQUEST_TIP;
				
				break;
			case PIP_REQUEST_TIP:
				//Waiting for a shuttle to arrive that is of type tip
				if(!gStationsIf.Cmd.RequestTip){
					gStationsIf.Cmd.RequestTip = 1;
					gStationsIf.Par.Tip.RequestDestination = DEST_PIPETTE_1 + i;
				}
				if(pipetteFb[i].ShuttlePresent && pipetteFb[i].ShUserData.ShType == SH_TIPS){
					//gStationsIf.Cmd.RequestTip = FALSE;
					pipetteFb[i].Operate = TRUE;
					pipetteState[i] = PIP_CHANGE_TIP;
				}
				break;
			case PIP_CHANGE_TIP:
				tipReplaceFb[i].TipNeedsReplaced = startupFlag[i];
				startupFlag[i] = TRUE;
				tipReplaceFb[i].Shuttle = &pipetteFb[i].internal.fbs.destMonFb.Mc6DShuttle;
				tipReplaceFb[i].TipHomeZPosition = gSysRecipe.PipettePars.HomePosition;
				tipReplaceFb[i].TipZVelocity = gSysRecipe.PipettePars.Vel * 0.004;
				tipReplaceFb[i].TipZSwapHeight = gSysRecipe.PipettePars.TipPosition;
				tipReplaceFb[i].ShuttleVel = gSysRecipe.DefaultVel;
				tipReplaceFb[i].ShuttleAccel = gSysRecipe.DefaultAccel;
				tipReplaceFb[i].NewTipPosition.X = -0.015;
				tipReplaceFb[i].OldTipPosition.X = 0.015;
				tipReplaceFb[i].Execute = TRUE;
				
				shIf[i].TipPresent = !tipReplaceFb[i].NoTip;
				if(!shIf[i].TipPresent)
					shIf[i].CurrentSample = COLOR_GRAY;
				if(tipReplaceFb[i].Active)
					shIf[i].PipetteCtrl.Position = tipReplaceFb[i].TipZPosition;
				
				if(tipReplaceFb[i].Done && pipetteFb[i].WaitComplete){
					tipReplaceFb[i].Execute = FALSE;
					pipetteFb[i].Operate = FALSE;
					shIf[i].TipNeedReplace = FALSE;
					pipetteFb[i].Send = TRUE;
					pipetteFb[i].NextDestination = &gDests[DEST_TIPS].Destination;
					
					if(!shIf[i].ShuttleOperating){
						//We haven't received a shuttle yet, we should go ahead and tell the system we're ready to receive a sample and shuttle and wait for one.
						gStationsIf.Sts.PipetteReadyForShuttle[i] = TRUE;
						pipetteState[i] = PIP_WAIT_SAMPLE;
					}
					else{
						if(!shIf[i].SamplePresent){
							pipetteState[i] = PIP_REQUEST_SAMPLE;
						}
						else{
							pipetteState[i] = PIP_REQUEST_DYE;
						}
					}
				}
				break;
			
			case PIP_REQUEST_SAMPLE:
				if(!gStationsIf.Cmd.RequestIncubator){
					gStationsIf.Cmd.RequestIncubator = TRUE;
					gStationsIf.Par.Incubator.SampleType = shIf[i].NextSample;
					gStationsIf.Par.Incubator.NextStation = DEST_PIPETTE_1 + i;
					pipetteState[i] = PIP_WAIT_SAMPLE;
				}
				break;
			case PIP_WAIT_SAMPLE:
				if(pipetteFb[i].ShuttlePresent && pipetteFb[i].ShUserData.ShType == SH_SAMPLE){
					shIf[i].PipetteCtrl.DestinationPosition = gSysRecipe.PipettePars.SampleDistance;
					shIf[i].PipetteCtrl.RequestState = PIP_OPERATE_SAMPLE;
					pipetteState[i] = PIP_MOVE_TIP_DOWN;
				}
				break;
			case PIP_OPERATE_SAMPLE:
				if(pipetteFb[i].WaitComplete){
					pipetteFb[i].Operate = FALSE;
					shIf[i].CurrentSample = pipetteFb[i].ShUserData.CurrentColor;
					pipetteFb[i].NextDestination = &gDests[DEST_INCUBATOR].Destination;
					
					pipetteFb[i].DyePresent = FALSE;
					
					shIf[i].PipetteCtrl.DestinationPosition = gSysRecipe.PipettePars.HomePosition;
					shIf[i].PipetteCtrl.RequestState = PIP_REQUEST_SHUTTLE;
					pipetteState[i] = PIP_MOVE_TIP_UP;
				}
				break;
			case PIP_REQUEST_SHUTTLE:
				if(!gStationsIf.Cmd.RequestAnalyzer && !shIf[i].ShuttleOperating){
					gStationsIf.Cmd.RequestAnalyzer = TRUE;
					gStationsIf.Par.Analyzer.NextStation = DEST_PIPETTE_1 + i;
					shIf[i].ShuttleOperating = TRUE;
					
					pipetteState[i] = PIP_WAIT_WELL_TO_ARRIVE;
				}
				else if(shIf[i].ShuttleOperating){
					//Shuttle is already present, so we need to request it from the shMove block
					shMoveFb[i].Destination = &gDests[DEST_PIPETTE_1 + i].Destination;
					shMoveFb[i].Velocity = gSysRecipe.DefaultVel;
					shMoveFb[i].Acceleration = gSysRecipe.DefaultAccel;
					shMoveFb[i].RouterShuttle = &shIf[i].shuttleIf;
					shMoveFb[i].Execute = TRUE;
						
					pipetteState[i] = PIP_WAIT_WELL_TO_ARRIVE;
				}
				
				break;
			case PIP_WAIT_WELL_TO_ARRIVE:
				if(pipetteFb[i].ShuttlePresent && pipetteFb[i].ShUserData.ShType == SH_WELL){
					shMoveFb[i].Execute = FALSE;
					
					gStationsIf.Cmd.RequestPipette[i] = FALSE;
					shIf[i].PipetteCtrl.DestinationPosition = gSysRecipe.PipettePars.WellDistance;
					shIf[i].PipetteCtrl.RequestState = PIP_PROCESS_SHUTTLE;
					pipetteState[i] = PIP_MOVE_TIP_DOWN;
				}
				break;
			case PIP_PROCESS_SHUTTLE:
				gStationsIf.Sts.PipetteDispensing[i] = pipetteFb[i].Waiting;
				if(pipetteFb[i].OperateDone){
					pipetteFb[i].Operate = FALSE;
					if(!shIf[i].SamplePresent){
						shIf[i].shuttleIf = pipetteFb[i].Shuttle;
						shIf[i].SamplePresent = TRUE;
						shIf[i].TipNeedReplace = TRUE;
						pipetteFb[i].NextDestination = &gDests[DEST_DEFAULT].Destination;
						
						shIf[i].PipetteCtrl.DestinationPosition = gSysRecipe.PipettePars.HomePosition;
						shIf[i].PipetteCtrl.RequestState = PIP_REQUEST_TIP;
						pipetteState[i] = PIP_MOVE_TIP_UP;
					}
					else{
						//Need to go check on the recipe now
						shIf[i].PipetteCtrl.DestinationPosition = gSysRecipe.PipettePars.HomePosition;
						shIf[i].PipetteCtrl.RequestState = PIP_CHECK_RECIPE;
						pipetteState[i] = PIP_MOVE_TIP_UP;
					}
				}
				break;
			case PIP_REQUEST_DYE:
				if(!gStationsIf.Cmd.RequestDye){
					gStationsIf.Cmd.RequestDye = TRUE;
					gStationsIf.Par.Dye.RequestDestination = DEST_PIPETTE_1 + i;
				}
				if(pipetteFb[i].ShuttlePresent && pipetteFb[i].ShUserData.ShType == SH_DYE){
					gStationsIf.Cmd.RequestDye = FALSE;
					shIf[i].PipetteCtrl.DestinationPosition = gSysRecipe.PipettePars.WellDistance;
					shIf[i].PipetteCtrl.RequestState = PIP_PROCESS_DYE;
					pipetteState[i] = PIP_MOVE_TIP_DOWN;
				}
				break;
			case PIP_PROCESS_DYE:
				if(pipetteFb[i].WaitComplete){
					pipetteFb[i].Operate = FALSE;
					shIf[i].DyePresent = TRUE;
					pipetteFb[i].NextDestination = &gDests[DEST_DYE].Destination;
					shIf[i].CurrentSample = COLOR_WHITE;
					pipetteFb[i].DyePresent = TRUE;
					
					if(shIf[i].SamplePresent){
						shMoveFb[i].Destination = &gDests[DEST_PIPETTE_1 + i].Destination;
						shMoveFb[i].Velocity = gSysRecipe.DefaultVel;
						shMoveFb[i].Acceleration = gSysRecipe.DefaultAccel;
						shMoveFb[i].RouterShuttle = &shIf[i].shuttleIf;
												
						shIf[i].PipetteCtrl.DestinationPosition = gSysRecipe.PipettePars.HomePosition;
						shIf[i].PipetteCtrl.RequestState = PIP_WAIT_WELL_TO_ARRIVE;
						pipetteState[i] = PIP_MOVE_TIP_UP;
					}
					else{
						shIf[i].PipetteCtrl.DestinationPosition = gSysRecipe.PipettePars.HomePosition;
						shIf[i].PipetteCtrl.RequestState = PIP_REQUEST_SHUTTLE;
						pipetteState[i] = PIP_MOVE_TIP_UP;
					}
				}
				break;
			case PIP_MOVE_TIP_DOWN:
				shIf[i].PipetteCtrl.Position -= (gSysRecipe.PipettePars.Vel * 0.004); //convert to mm/ms
				if(shIf[i].PipetteCtrl.Position <= shIf[i].PipetteCtrl.DestinationPosition){
					pipetteFb[i].Operate = TRUE;
					pipetteState[i] = shIf[i].PipetteCtrl.RequestState;     
				}
				break;
			case PIP_MOVE_TIP_UP:
				shIf[i].PipetteCtrl.Position += (gSysRecipe.PipettePars.Vel  * 0.004); //convert to mm/ms
				if(shIf[i].PipetteCtrl.Position >= shIf[i].PipetteCtrl.DestinationPosition){
					//The check recipe state needs to do some special things, so we should wait to hit send if we're supposed to go here and let it handle it
					if(shIf[i].PipetteCtrl.RequestState != PIP_CHECK_RECIPE) {
						//In some situations, we also need to call off hte shuttle from the roundabout location, this if condition handles that
						if(shIf[i].DyePresent && shIf[i].SamplePresent)
							shMoveFb[i].Execute = TRUE;
						pipetteFb[i].Send = TRUE;
					}
					pipetteState[i] = shIf[i].PipetteCtrl.RequestState;     
				}
				break;
			case PIP_CHECK_RECIPE:
				//If the sample and well count == numRows * numColumns, then we're done
					//Send the shuttle to the analyzer, and reset any flags to start the process over again
				//Otherwise, request the next sample and keep going on
				if(pipetteFb[i].ShUserData.Recipe.SampleWellCount == (pipetteFb[i].WellPlateInfo.NumColumns * pipetteFb[i].WellPlateInfo.NumRows) &&
						pipetteFb[i].ShUserData.Recipe.DyeWellCount == (pipetteFb[i].WellPlateInfo.NumColumns * pipetteFb[i].WellPlateInfo.NumRows)){
					shIf[i].DyePresent = FALSE;
					shIf[i].SamplePresent = FALSE;
					pipetteFb[i].DyePresent = FALSE;
					shIf[i].TipNeedReplace = FALSE;
					brsmemset(&shIf[i].shuttleIf,0,sizeof(shIf[i].shuttleIf));
					pipetteFb[i].NextDestination = &gDests[DEST_ANALYZER].Destination;
					pipetteFb[i].Send = TRUE;
					shIf[i].ShuttleOperating = FALSE;
					
					pipetteState[i] = PIP_SEND_ANALYZER;
				}
				else{
					shIf[i].DyePresent = FALSE;
					shIf[i].shuttleIf = pipetteFb[i].Shuttle;
					shIf[i].SamplePresent = FALSE;
					shIf[i].TipNeedReplace = TRUE;
					pipetteFb[i].NextDestination = &gDests[DEST_DEFAULT].Destination;
					pipetteFb[i].Send = TRUE;
					pipetteFb[i].DyePresent = FALSE;
					shIf[i].NextSample = pipetteFb[i].ShUserData.Recipe.Samples[pipetteFb[i].ShUserData.Recipe.CurrentSample].Sample;
					pipetteState[i] = PIP_REQUEST_TIP;
				}
				break;
			case PIP_SEND_ANALYZER:
				if(!pipetteFb[i].ShuttlePresent){
					pipetteFb[i].Send = FALSE;
					pipetteState[i] = PIP_REQUEST_TIP;
				}
				break;
		}    
		stnPipette(&pipetteFb[i]);
		rl6dShuttleMoveDestination(&shMoveFb[i]);
		tipReplace(&tipReplaceFb[i]);
		if(pipetteFb[i].Send && !pipetteFb[i].ShuttlePresent){
			pipetteFb[i].Send = FALSE;
		}
		if(shMoveFb[i].Active && shMoveFb[i].Execute){
			shMoveFb[i].Execute = FALSE;
		}
	}
}
