#include "Router.hpp"

extern "C" _BUR_PUBLIC void rl6dDestAddRecoveryPosFromData(struct rl6dDestAddRecoveryPosFromData* inst){
    
    if(inst == nullptr){
        return;
    }
    
    Destination* d;
    std::shared_ptr<Destination> destination;
    rl6dPositionType* position;
    DINT error;
    
    enum state{
        state_idle,
        state_add_recovery_pos,
        state_done,
        state_error
    };
    
    switch(inst->Internal.State){
        
        case state_idle:
            
            if(inst->Execute){
                inst->Busy = 1;
                inst->Internal.State = state_add_recovery_pos;
            }
            
            break;
        
        case state_add_recovery_pos:
            
            error = check_destination_reference(inst->Destination);
            if(error == ERR_OK){
                if(inst->DataSize % sizeof(rl6dPositionType) != 0){
                    error = rl6dERR_DATA_ALIGNMENT;
                }
            }
            if(error == ERR_OK){
                d = reinterpret_cast<Destination*>(inst->Destination->Ident);
                destination = d->get_shared_ptr();
                if(destination->sem_acquire()){
                    for(UDINT offset = 0; offset < inst->DataSize; offset += sizeof(rl6dPositionType)){
                        position = reinterpret_cast<rl6dPositionType*>(inst->DataAddress + offset);
                        destination->add_recovery_position(*position);
                    }
                    destination->sem_release();
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
