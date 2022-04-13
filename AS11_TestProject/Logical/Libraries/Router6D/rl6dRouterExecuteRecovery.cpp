#include "Router.hpp"

extern "C" _BUR_PUBLIC void rl6dRouterExecuteRecovery(struct rl6dRouterExecuteRecovery* inst){
    
    if(inst == nullptr){
        return;
    }
    
    Router* r;
    DINT error;
    
    enum state{
        state_idle,
        state_send_cmd,
        state_recover,
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
            
            error = check_router_reference(inst->Router);
            if(error == ERR_OK){
                r = reinterpret_cast<Router*>(inst->Router->Ident);
                if(r->get_initialized()){
                    if(r->acquire_cmd_semaphore()){
                        if(r->get_cmd_ready()){
                            r->set_recover_shuttles(inst->Mode, inst->Velocity, inst->Acceleration);
                            inst->Internal.State = state_recover;
                        }
                        else{
                            r->release_cmd_semaphore();
                        }
                    }
                }
            }
            else{
                inst->ErrorID  = error;
                inst->Error = 1;
                inst->Internal.State = state_error;
            }
            
            break;
        
        case state_recover:
            
            error = check_router_reference(inst->Router);
            if(error == ERR_OK){
                r = reinterpret_cast<Router*>(inst->Router->Ident);
                if(r->get_done()){
                    r->reset_recover_shuttles();
                    r->release_cmd_semaphore();
                    inst->Done = 1;
                    inst->Internal.State = state_done;
                }
                else if(r->get_error()){
                    r->reset_recover_shuttles();
                    inst->ErrorID  = r->get_error_id();
                    inst->Error = 1;
                    r->set_error_reset();
                    r->release_cmd_semaphore();
                    inst->Internal.State = state_error;
                }
            }
            else{
                inst->ErrorID  = error;
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

