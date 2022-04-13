#include "Router.hpp"

extern "C" _BUR_PUBLIC void rl6dDestinationSetWaypoint(struct rl6dDestinationSetWaypoint* inst){
    
    if(inst == nullptr){
        return;
    }
    
    Destination* d;
    std::shared_ptr<Destination> destination;
    Waypoint* w;
    DINT error;
    
    enum state{
        state_idle,
        state_done,
        state_error
    };
    
    switch(inst->Internal.State){
        
        case state_idle:
            
            if(inst->Execute){
                inst->Busy = 1;
                error = check_destination_reference(inst->Destination);
                if(error == ERR_OK){
                    error = check_waypoint_reference(inst->Waypoint);
                }
                if(error == ERR_OK){
                    d = reinterpret_cast<Destination*>(inst->Destination->Ident);
                    destination = d->get_shared_ptr();
                    w = reinterpret_cast<Waypoint*>(inst->Waypoint->Ident);
                    destination->set_waypoint(w->get_shared_ptr());
                    inst->Done = 1;
                    inst->Internal.State = state_done;
                }
                else{
                    inst->ErrorID = error;
                    inst->Error = 1;
                    inst->Internal.State = state_error;
                }
            }
            
            break;
        
        case state_done:
            
            if(!inst->Execute){
                inst->Done = 0;
                inst->Busy = 0;
                inst->Internal.State = state_idle;
            }
            
            break;
        
        case state_error:
        
            if(!inst->Execute){
                inst->ErrorID = ERR_OK;
                inst->Error = 0;
                inst->Busy = 0;
                inst->Internal.State = state_idle;
            }
            
            break;
    }
    
}
