#include "Router.hpp"

extern "C" _BUR_PUBLIC void rl6dDestinationMonitor(struct rl6dDestinationMonitor* inst){
    
    if(inst == nullptr){
        return;
    }
    
    Destination* d;
    std::shared_ptr<Destination> destination;
    DestinationIf* i;
    std::shared_ptr<DestinationIf> interface;
    std::shared_ptr<Shuttle> shuttle;
    DINT error;
    
    enum state{
        state_idle,
        state_get_interface,
        state_wait_shuttle,
        state_wait_not_acknowledge,
        state_wait_acknowledge,
        state_error
    };
    
    switch(inst->Internal.State){
        
        case state_idle:
            
            if(inst->Enable){
                inst->Busy = 1;
                inst->Internal.State = state_get_interface;
            }
            
            break;
        
        case state_get_interface:
            
            error = check_destination_reference(inst->Destination);
            if(error == ERR_OK){
                d = reinterpret_cast<Destination*>(inst->Destination->Ident);
                destination = d->get_shared_ptr();
                if(destination->sem_acquire()){
                    inst->Internal.Reference = reinterpret_cast<std::uintptr_t>(destination->create_interface().get());
                    destination->sem_release();
                    inst->Internal.State = state_wait_shuttle;
                }
            }
            else{
                inst->ErrorID = error;
                inst->Error = 1;
                inst->Internal.State = state_error;
            }
                
            break;
        
        case state_wait_shuttle:
            
            error = check_destination_if_reference(inst->Internal.Reference);
            if(error == ERR_OK){
                if(inst->Enable){
                    i = reinterpret_cast<DestinationIf*>(inst->Internal.Reference);
                    interface = i->get_shared_ptr();
                    if(interface->shuttle_is_present()){
                        shuttle = interface->get_shuttle_present().lock();
                        inst->ShuttleID = shuttle->get_shuttle_id();
                        inst->RouterShuttle.Ident = reinterpret_cast<std::uintptr_t>(shuttle.get());
                        inst->Mc6DShuttle = *shuttle->get_shuttle_reference();
                        inst->ShuttlePresent = 1;
                        interface->acknowledge_shuttle();
                        if(inst->AcknowledgeShuttle){
                            inst->Internal.State = state_wait_not_acknowledge;
                        }
                        else{
                            inst->Internal.State = state_wait_acknowledge;
                        }
                    }
                    inst->Valid = 1;
                }
                else{
                    i = reinterpret_cast<DestinationIf*>(inst->Internal.Reference);
                    interface = i->get_shared_ptr();
                    interface->set_delete();
                    inst->Valid = 0;
                    inst->ShuttlePresent = 0;
                    inst->ShuttleID = 0;
                    std::memset(&inst->Mc6DShuttle, 0, sizeof(inst->Mc6DShuttle));
                    std::memset(&inst->RouterShuttle, 0, sizeof(inst->RouterShuttle));
                    inst->Busy = 0;
                    inst->Internal.State = state_idle;
                }
            }
            else{
                inst->ErrorID = error;
                inst->Error = 1;
                inst->Internal.State = state_error;
            }
            
            break;
        
        case state_wait_not_acknowledge:
            
            if(inst->Enable){
                if(!inst->AcknowledgeShuttle){
                    inst->Internal.State = state_wait_acknowledge;
                }
            }
            else{
                error = check_destination_if_reference(inst->Internal.Reference);
                if(error == ERR_OK){
                    i = reinterpret_cast<DestinationIf*>(inst->Internal.Reference);
                    interface = i->get_shared_ptr();
                    interface->set_delete();
                    inst->Valid = 0;
                    inst->ShuttlePresent = 0;
                    inst->ShuttleID = 0;
                    std::memset(&inst->Mc6DShuttle, 0, sizeof(inst->Mc6DShuttle));
                    std::memset(&inst->RouterShuttle, 0, sizeof(inst->RouterShuttle));
                    inst->Busy = 0;
                    inst->Internal.State = state_idle;
                }
                else{
                    inst->Valid = 0;
                    inst->ShuttlePresent = 0;
                    inst->ShuttleID = 0;
                    std::memset(&inst->Mc6DShuttle, 0, sizeof(inst->Mc6DShuttle));
                    std::memset(&inst->RouterShuttle, 0, sizeof(inst->RouterShuttle));
                    inst->ErrorID = error;
                    inst->Error = 1;
                    inst->Internal.State = state_error;
                }
            }
            
            break;
        
        case state_wait_acknowledge:
            
            if(inst->Enable){
                if(inst->AcknowledgeShuttle){
                    inst->ShuttlePresent = 0;
                    inst->ShuttleID = 0;
                    std::memset(&inst->Mc6DShuttle, 0, sizeof(inst->Mc6DShuttle));
                    std::memset(&inst->RouterShuttle, 0, sizeof(inst->RouterShuttle));
                    inst->Internal.State = state_wait_shuttle;
                }
            }
            else{
                error = check_destination_if_reference(inst->Internal.Reference);
                if(error == ERR_OK){
                    i = reinterpret_cast<DestinationIf*>(inst->Internal.Reference);
                    interface = i->get_shared_ptr();
                    interface->set_delete();
                    inst->Valid = 0;
                    inst->ShuttlePresent = 0;
                    inst->ShuttleID = 0;
                    std::memset(&inst->Mc6DShuttle, 0, sizeof(inst->Mc6DShuttle));
                    std::memset(&inst->RouterShuttle, 0, sizeof(inst->RouterShuttle));
                    inst->Busy = 0;
                    inst->Internal.State = state_idle;
                }
                else{
                    inst->Valid = 0;
                    inst->ShuttlePresent = 0;
                    inst->ShuttleID = 0;
                    std::memset(&inst->Mc6DShuttle, 0, sizeof(inst->Mc6DShuttle));
                    std::memset(&inst->RouterShuttle, 0, sizeof(inst->RouterShuttle));
                    inst->ErrorID = error;
                    inst->Error = 1;
                    inst->Internal.State = state_error;
                }
            }
            
            break;
        
        case state_error:
        
            if(!inst->Enable){
                inst->ErrorID = ERR_OK;
                inst->Error = 0;
                inst->Busy = 0;
                inst->Internal.State = state_idle;
            }
            
            break;
        
    }
    
}
