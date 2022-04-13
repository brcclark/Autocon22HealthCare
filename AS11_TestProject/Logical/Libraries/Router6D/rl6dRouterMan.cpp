#include "Router.hpp"

uint32_t bur_heap_size = rl6dHEAP_SIZE;

extern "C" _BUR_PUBLIC void rl6dRouterMan(struct rl6dRouterMan* inst){
    
    if(inst == nullptr){
        return;
    }
    
    Router* r;
    DINT error;
    
    enum state{
        state_idle,
        state_active,
        state_error
    };
    
    switch(inst->Internal.State){
        
        case state_idle:
        
            if(inst->Enable){
                inst->Busy = 1;
                if(inst->UserDataSize > 0){
                    inst->Router.Ident = reinterpret_cast<std::uintptr_t>(new Router(inst->Assembly, inst->UserDataSize));
                    inst->Active = 1;
                    inst->Internal.State = state_active;
                }
                else{
                    inst->ErrorID = rl6dERR_USER_DATA_SIZE_ZERO;
                    inst->Error = 1;
                    inst->Internal.State = state_error;
                }
            }
    
            break;
        
        case state_active:
        
            error = check_router_reference(&inst->Router);
            if(error == ERR_OK){
                r = reinterpret_cast<Router*>(inst->Router.Ident);
                r->cyclic();
                if(!inst->Enable){
                    inst->Active = 0;
                    inst->Router.Ident = 0;
                    delete r;
                    inst->Busy = 0;
                    inst->Internal.State = state_idle;
                    }
            }
            else{
                inst->Active = 0;
                inst->ErrorID = error;
                inst->Error = 1;
                inst->Internal.State = state_error;
            }
    
            break;
    
        case state_error:
        
            if(!inst->Enable){
                error = check_router_reference(&inst->Router);
                if(error == ERR_OK){
                    r = reinterpret_cast<Router*>(inst->Router.Ident);
                    delete r;
                }
                inst->Router.Ident = 0;
                inst->ErrorID = ERR_OK;
                inst->Error = 0;
                inst->Busy = 0;
                inst->Internal.State = state_idle;
            }
    
            break;
    
    }
      
    //TODO: Allow for cleanup of semaphores before router deletion
    
}
