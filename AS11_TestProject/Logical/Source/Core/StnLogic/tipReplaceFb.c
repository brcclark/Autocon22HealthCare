
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
void tipReplace(struct tipReplace* fb){
	//If Tip needs replace, move to old tip
	//Move Tip Down
	//Wait
	//Move Tip up
	//Move to new tip
	//Move tip Down
	//Move Tip Up
	switch (fb->internal.state)
	{
		case TIP_IDLE:
			if(fb->Execute){
				fb->TipZPosition = fb->TipHomeZPosition;
				fb->Active = TRUE;
				fb->internal.fbs.movePlaneFb.Shuttle = fb->Shuttle;
				fb->internal.fbs.movePlaneFb.Parameters.Velocity = fb->ShuttleVel;
				fb->internal.fbs.movePlaneFb.Parameters.Acceleration = fb->ShuttleAccel;
				fb->internal.fbs.movePlaneFb.Parameters.Mode = mcACP6D_MOVE_RELATIVE;
				fb->internal.fbs.movePlaneFb.Parameters.Path = mcACP6D_PATH_DIRECT;
				fb->internal.fbs.movePlaneFb.Execute = TRUE;
				if(fb->TipNeedsReplaced){
					//Move to old tip
					fb->internal.fbs.movePlaneFb.Parameters.Position.X = fb->OldTipPosition.X;
					fb->internal.fbs.movePlaneFb.Parameters.Position.Y = fb->OldTipPosition.Y;
					fb->internal.state = TIP_MOVE_OLD;					
				}
				else{
					fb->NoTip = TRUE;
					fb->internal.fbs.movePlaneFb.Parameters.Position.X = fb->NewTipPosition.X;
					fb->internal.fbs.movePlaneFb.Parameters.Position.Y = fb->NewTipPosition.Y;
					fb->internal.state = TIP_MOVE_NEW;					
				}
			}
			break;
		case TIP_MOVE_OLD:
			if(fb->internal.fbs.movePlaneFb.Done){
				fb->internal.fbs.movePlaneFb.Execute = FALSE;
				fb->internal.state = TIP_LOWER_TIP;				
			}
			break;
		case TIP_MOVE_NEW:
			if(fb->internal.fbs.movePlaneFb.Done){
				fb->internal.fbs.movePlaneFb.Execute = FALSE;
				fb->internal.state = TIP_LOWER_TIP;				
			}
			break;
		case TIP_LOWER_TIP:
			fb->TipZPosition -= fb->TipZVelocity;
			if(fb->TipZPosition <= fb->TipZSwapHeight){
				fb->internal.fbs.waitTon.PT = fb->TipSwapTime;
				fb->internal.fbs.waitTon.IN = TRUE;
				
				if(fb->TipNeedsReplaced)
					fb->NoTip = !fb->NoTip;
				else
					fb->NoTip = FALSE;
				fb->internal.state = TIP_WAIT;
			}
			break;
		case TIP_RAISE_TIP:
			fb->TipZPosition += fb->TipZVelocity;
			
			if(fb->TipZPosition >= fb->TipHomeZPosition){
				if(!fb->internal.tipReplaced && fb->TipNeedsReplaced){
					fb->internal.tipReplaced = TRUE;
					fb->internal.fbs.movePlaneFb.Parameters.Position.X = fb->NewTipPosition.X - fb->OldTipPosition.X;
					fb->internal.fbs.movePlaneFb.Parameters.Position.Y = fb->NewTipPosition.Y;
					fb->internal.fbs.movePlaneFb.Execute = TRUE;
					fb->internal.state = TIP_MOVE_NEW;	
				}
				else{
					fb->NoTip = FALSE;
					fb->Done = TRUE;
					fb->internal.state = TIP_DONE;	
				}
			}
			break;
		case TIP_WAIT:
			if(fb->internal.fbs.waitTon.Q){
				fb->internal.fbs.waitTon.IN = FALSE;
				fb->internal.state = TIP_RAISE_TIP;
			}
			break;
		case TIP_DONE:
			if(!fb->Execute){
				fb->Active = FALSE;
				fb->Done = FALSE;
				fb->internal.tipReplaced = FALSE;
				fb->internal.state = TIP_IDLE;
			}
			break;
	}
	MC_BR_MoveInPlane_Acp6D(&fb->internal.fbs.movePlaneFb);
	TON(&fb->internal.fbs.waitTon);
}
