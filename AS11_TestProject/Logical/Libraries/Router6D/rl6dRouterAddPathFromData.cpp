#include "Router.hpp"

extern "C" _BUR_PUBLIC void rl6dRouterAddPathFromData(struct rl6dRouterAddPathFromData* inst){
    
    if(inst == nullptr){
        return;
    }
    
    Router* r;
    RouterSem* router_sem;
    std::shared_ptr<Path> p;
    rl6dPositionType* position;
    DINT error;
    
    enum state{
        state_idle,
        state_create_sem_start,
        state_create_sem_end,
        state_add_positions,
        state_done,
        state_error
    };
    
    switch(inst->Internal.State){
        
        case state_idle:
            
            if(inst->Execute){
                inst->Busy = 1;
                inst->Internal.State = state_create_sem_start;
            }
            
            break;
        
        case state_create_sem_start:
            
            error = check_router_reference(inst->Router);
            if(error == ERR_OK){
                if(inst->DataSize % sizeof(rl6dPositionType) != 0){
                    error = rl6dERR_DATA_ALIGNMENT;
                }
            }
            if(error == ERR_OK){
                if(inst->Name[0] == 0){
                    error = rl6dERR_NO_OBJ_NAME;
                }
            }
            if(error == ERR_OK){
                r = reinterpret_cast<Router*>(inst->Router->Ident);
                if(r->get_initialized()){
                    if(r->sem_acquire()){
                        if(r->get_sem_cmd_ready()){
                            r->set_create_sem();
                            inst->Internal.State = state_create_sem_end;
                        }
                        else{
                            r->sem_release();
                        }
                    }
                }
            }
            else{
                inst->ErrorID = error;
                inst->Error = 1;
                inst->Internal.State = state_error;
            }
            break;
        
        case state_create_sem_end:
            
            error = check_router_reference(inst->Router);
            if(error == ERR_OK){
                r = reinterpret_cast<Router*>(inst->Router->Ident);
                if(r->get_sem_done()){
                    inst->Internal.Reference = reinterpret_cast<std::uintptr_t>(r->get_created_sem().get());
                    r->reset_create_sem();
                    r->sem_release();
                    inst->Internal.State = state_add_positions;
                }
            }
            else{
                inst->ErrorID = error;
                inst->Error = 1;
                inst->Internal.State = state_error;
            }
            
            break;
        
        case state_add_positions:
            
            error = check_router_reference(inst->Router);
            if(error == ERR_OK){
                error = check_router_sem_reference(inst->Internal.Reference);
            }
            if(error == ERR_OK){
                if(inst->DataSize % sizeof(rl6dPositionType) != 0){
                    error = rl6dERR_DATA_ALIGNMENT;
                }
            }
            if(error == ERR_OK){
                if(inst->Name[0] == 0){
                    error = rl6dERR_NO_OBJ_NAME;
                }
            }
            if(error == ERR_OK){
                r = reinterpret_cast<Router*>(inst->Router->Ident);
                if(r->acquire_path_semaphore()){
                    router_sem = reinterpret_cast<RouterSem*>(inst->Internal.Reference);
                    try{
                        p = r->create_path(inst->Name, router_sem->get_shared_ptr());
                    }
                    catch (const std::invalid_argument& ia) {
                        inst->ErrorID = rl6dERR_OBJ_NAME_EXISTS;
                        inst->Error = 1;
                        inst->Internal.State = state_error;
                        r->release_path_semaphore();
                        router_sem->set_delete();
                        return;
                    }
                    for(UDINT offset = 0; offset < inst->DataSize; offset += sizeof(rl6dPositionType)){
                        position = reinterpret_cast<rl6dPositionType*>(inst->DataAddress + offset);
                        p->add_position(*position);
                    }
                    inst->Path.Ident = reinterpret_cast<std::uintptr_t>(p.get());
                    r->release_path_semaphore();
                    inst->Done = 1;
                    inst->Internal.State = state_done;
                }
            }
            else{
                inst->ErrorID = error;
                inst->Error = 1;
                if(check_router_sem_reference(inst->Internal.Reference) == ERR_OK){
                    router_sem = reinterpret_cast<RouterSem*>(inst->Internal.Reference);
                    router_sem->set_delete();
                }
                inst->Internal.State = state_error;
            }
            
            break;
        
        case state_done:
            
            if(!inst->Execute){
                inst->Done = 0;
                inst->Path.Ident = 0;
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
