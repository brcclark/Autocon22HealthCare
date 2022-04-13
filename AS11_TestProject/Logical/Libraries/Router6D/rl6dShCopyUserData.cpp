#include "Shuttle.hpp"
#include "CheckRef.hpp"

extern "C" _BUR_PUBLIC void rl6dShCopyUserData(struct rl6dShCopyUserData* inst){
    
    if(inst == nullptr){
        return;
    }
    
    Shuttle* s;
    std::shared_ptr<Shuttle> shuttle;
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
                error = check_shuttle_reference(inst->RouterShuttle);
                if(error == ERR_OK){
                    if(inst->DataAddress == 0){
                        error = rl6dERR_USER_DATA_NULL_POINTER;
                    }
                }
                if(error == ERR_OK){
                    if(inst->DataSize == 0){
                        error = rl6dERR_USER_DATA_SIZE_ZERO;
                    }
                }
                if(error == ERR_OK){
                    s = reinterpret_cast<Shuttle*>(inst->RouterShuttle->Ident);
                    shuttle = s->get_shared_ptr();
                    ReadUserDataResult result = shuttle->read_user_data();
                    if(result.size == inst->DataSize){
                        if(inst->Mode == rl6dSH_COPY_USER_DATA_READ){
                            std::memcpy(reinterpret_cast<char*>(inst->DataAddress), result.value, result.size);
                            inst->SizeCopied = result.size;
                        }
                        else if(inst->Mode == rl6dSH_COPY_USER_DATA_WRITE){
                            inst->SizeCopied = shuttle->write_user_data(reinterpret_cast<char*>(inst->DataAddress), inst->DataSize);
                        }
                        inst->Done = 1;
                        inst->Internal.State = state_done;
                    }
                    else{
                        inst->ErrorID = rl6dERR_USER_DATA_SIZE_MISMATCH;
                        inst->Error = 1;
                        inst->Internal.State = state_error;
                    }
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
                inst->SizeCopied = 0;
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
