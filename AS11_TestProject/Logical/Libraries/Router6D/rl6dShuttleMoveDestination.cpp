#include "Router.hpp"

extern "C" _BUR_PUBLIC void rl6dShuttleMoveDestination(struct rl6dShuttleMoveDestination* inst){
    
    if(inst == nullptr){
        return;
    }
    
    Destination* d;
    std::shared_ptr<Destination> destination;
    Waypoint* w;
    std::weak_ptr<Waypoint> waypoint;
    Shuttle* s;
    std::shared_ptr<Shuttle> shuttle;
    ShuttleIf* i;
    std::shared_ptr<ShuttleIf> interface;
    DINT error;
    
    enum state{
        state_idle,
        state_send_cmd,
        state_move,
        state_done,
        state_error
    };
    
    switch(inst->Internal.State){
        
        case state_idle:
            
            if(inst->Execute){
                inst->Busy = 1;
                inst->Internal.State = state_send_cmd;
            }
            
            break;
        
        case state_send_cmd:
            
            error = check_shuttle_reference(inst->RouterShuttle);
            if(error == ERR_OK){
                error = check_destination_reference(inst->Destination);
            }
            if(error == ERR_OK){
                s = reinterpret_cast<Shuttle*>(inst->RouterShuttle->Ident);
                shuttle = s->get_shared_ptr();
                if(shuttle->sem_acquire()){
                    inst->Internal.Reference = reinterpret_cast<std::uintptr_t>(shuttle->create_interface().get());
                    shuttle->sem_release();
                    d = reinterpret_cast<Destination*>(inst->Destination->Ident);
                    destination = d->get_shared_ptr();
                    error = check_shuttle_if_reference(inst->Internal.Reference);
                    if(error == ERR_OK){
                        i = reinterpret_cast<ShuttleIf*>(inst->Internal.Reference);
                        interface = i->get_shared_ptr();
                        if (inst->Waypoint != nullptr){
                            error = check_waypoint_reference(inst->Waypoint);
                            if(error == ERR_OK){
                                w = reinterpret_cast<Waypoint*>(inst->Waypoint->Ident);
                                waypoint = w->get_shared_ptr();
                                interface->set_destination_waypoint(destination, waypoint, inst->Velocity, inst->Acceleration);
                            }
                            else{
                                inst->ErrorID = error;
                                inst->Error = 1;
                                inst->Internal.State = state_error;
                            }
                        }
                        else{
                            interface->set_destination(destination, inst->Velocity, inst->Acceleration);
                            inst->Internal.State = state_move;
                        }
                    }
                    else{
                        inst->ErrorID = error;
                        inst->Error = 1;
                        inst->Internal.State = state_error;
                    }
                }
            }
            else{
                inst->ErrorID = error;
                inst->Error = 1;
                inst->Internal.State = state_error;
            }
            
            break;
        
        case state_move:
            
            error = check_shuttle_if_reference(inst->Internal.Reference);
            if(error == ERR_OK){
                i = reinterpret_cast<ShuttleIf*>(inst->Internal.Reference);
                interface = i->get_shared_ptr();
                if(inst->Execute){
                    if(interface->get_error()){
                        inst->ErrorID = interface->get_error_id();
                        inst->Error = 1;
                        inst->Active = 0;
                        inst->Aborted = 0;
                        inst->Done = 0;
                        inst->Internal.State = state_error;
                    }
                    else{
                        inst->Active = interface->get_move_active();
                        inst->Aborted = interface->get_aborted();
                        inst->Done = interface->get_move_done();
                    }
                }
                else{
                    inst->Active = 0;
                    inst->Aborted = 0;
                    inst->Done = 0;
                    interface->set_delete();
                    inst->Internal.Reference = 0;
                    inst->Busy = 0;
                    inst->Internal.State = state_idle;
                }
            }
            else{
                inst->Active = 0;
                inst->Aborted = 0;
                inst->Done = 0;
                inst->ErrorID = error;
                inst->Error = 1;
                inst->Internal.State = state_error;
            }
            
            break;
        
        case state_error:
            
            if(!inst->Execute){
                error = check_shuttle_if_reference(inst->Internal.Reference);
                if(error == ERR_OK){
                    interface->set_delete();
                    inst->Internal.Reference = 0;
                }
                inst->ErrorID = ERR_OK;
                inst->Error = 0;
                inst->Busy = 0;
                inst->Internal.State = state_idle;
            }
            
            break;
        
    }
    
}
