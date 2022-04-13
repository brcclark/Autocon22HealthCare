#include "Router.hpp"

extern "C" _BUR_PUBLIC void rl6dPathGetData(struct rl6dPathGetData* inst){
    
    if(inst == nullptr){
        return;
    }
    
    Path* p;
    std::shared_ptr<Path> path;
    DINT error;
    
    enum state{
        state_idle,
        state_get_data,
        state_done,
        state_error
    };
    
    switch(inst->Internal.State){
        
        case state_idle:
            
            if(inst->Execute){
                inst->Busy = 1;
                inst->Internal.DataAddress = inst->DataAddress;
                inst->Internal.DataSize = inst->DataSize;
                inst->Internal.State = state_get_data;
            }
            
            break;
        
        case state_get_data:
            
            error = check_path_reference(inst->Path);
            if(error == ERR_OK){
                p = reinterpret_cast<Path*>(inst->Path->Ident);
                path = p->get_shared_ptr();
                if(path->sem_acquire()){
                    std::vector<rl6dPositionType> positions = path->get_positions();
                    path->sem_release();
                    UDINT available_size = inst->Internal.DataSize / sizeof(rl6dPositionType);
                    UDINT offset = 0;
                    for(auto & position : positions){
                        if(offset < available_size){
                            rl6dPositionType* array_position = reinterpret_cast<rl6dPositionType*>(inst->Internal.DataAddress + offset*sizeof(rl6dPositionType));
                            *array_position = position;
                            offset++;
                        }
                        else{
                            break;
                        }
                    }
                    inst->PathDataSize = positions.size() * sizeof(rl6dPositionType);
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
                std::memset(reinterpret_cast<void*>(inst->Internal.DataAddress), 0, inst->Internal.DataSize);
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
