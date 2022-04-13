#include "Router.hpp"

extern "C" _BUR_PUBLIC void rl6dRouterAddWaypoint(struct rl6dRouterAddWaypoint* inst){
    
    if(inst == nullptr){
        return;
    }
    
    Router* r;
    RouterSem* router_sem;
    std::shared_ptr<Waypoint> waypoint;
    DINT error;
    
    enum state{
        state_idle,
        state_create_sem_start,
        state_create_sem_end,
        state_create_waypoint,
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
                    inst->Internal.State = state_create_waypoint;
                }
            }
            else{
                inst->ErrorID = error;
                inst->Error = 1;
                inst->Internal.State = state_error;
            }
            
            break;
        
        case state_create_waypoint:
            
            error = check_router_reference(inst->Router);
            if(error == ERR_OK){
            error = check_router_sem_reference(inst->Internal.Reference);
            }
            if(error == ERR_OK){
                if(inst->Name[0] == 0){
                    error = rl6dERR_NO_OBJ_NAME;
                }
            }
            if(error == ERR_OK){
                r = reinterpret_cast<Router*>(inst->Router->Ident);
                if(r->acquire_waypoint_semaphore()){
                    router_sem = reinterpret_cast<RouterSem*>(inst->Internal.Reference);
                    try{
                        waypoint = r->create_waypoint(inst->Name, router_sem->get_shared_ptr(), inst->Position);
                    }
                    catch (const std::invalid_argument& ia) {
                        inst->ErrorID = rl6dERR_OBJ_NAME_EXISTS;
                        inst->Error = 1;
                        inst->Internal.State = state_error;
                        r->release_waypoint_semaphore();
                        router_sem->set_delete();
                        return;
                    }
                    inst->Waypoint.Ident = reinterpret_cast<std::uintptr_t>(waypoint.get());
                    r->release_waypoint_semaphore();
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
                inst->Waypoint.Ident = 0;
                inst->Busy = 0;
                inst->Internal.State = 0;
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
