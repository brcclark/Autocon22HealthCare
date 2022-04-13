#include "Router.hpp"

extern "C" _BUR_PUBLIC void rl6dWaypointAddDestination(struct rl6dWaypointAddDestination* inst){
    
    if(inst == nullptr){
        return;
    }
    
    Waypoint* w;
    std::shared_ptr<Waypoint> waypoint;
    Destination* d;
    Path* p;
    std::shared_ptr<Path> path;
    DINT error;
    
    enum state{
        state_idle,
        state_add_destination,
        state_done,
        state_error
    };
    
    switch(inst->Internal.State){
        
        case state_idle:
            
            if(inst->Execute){
                inst->Busy = 1;
                inst->Internal.State = state_add_destination;
            }
            
            break;
        
        case state_add_destination:
            
            error = check_waypoint_reference(inst->Waypoint);
            if(error == ERR_OK){
                error = check_destination_reference(inst->Destination);
            }
            if((error == ERR_OK) && (inst->DeparturePath != nullptr)){
                error = check_path_reference(inst->DeparturePath);
            }
            if(error == ERR_OK){
                w = reinterpret_cast<Waypoint*>(inst->Waypoint->Ident);
                waypoint = w->get_shared_ptr();
                if(waypoint->sem_acquire()){
                    if(inst->DeparturePath != nullptr){
                        p = reinterpret_cast<Path*>(inst->Waypoint->Ident);
                        path = p->get_shared_ptr();
                    }
                    d = reinterpret_cast<Destination*>(inst->Destination->Ident);
                    waypoint->add_destination(d->get_shared_ptr(), path);
                    waypoint->sem_release();
                    inst->Done = 1;
                    inst->Internal.State = state_done;
                }
            }
            else{
                inst->ErrorID = error;
                inst->Error = 1;
                inst->Internal.State = state_error;
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
