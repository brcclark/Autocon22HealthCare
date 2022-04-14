#include "Router.hpp"

Router::Router(Mc6DAssemblyType* assembly, UDINT user_data_size){
    std::memset(&this->_asm_get_shuttle, 0, sizeof(this->_asm_get_shuttle));
    std::memset(&this->_move_in_plane_async, 0, sizeof(this->_move_in_plane_async));
    std::memset(&this->_ar_event_log_create, 0, sizeof(this->_ar_event_log_create));
    this->_asm_get_shuttle.Assembly = assembly;
    this->_move_in_plane_async.Assembly = assembly;
    this->_user_data_size = user_data_size;
}
    
//destructor
Router::~Router(){
    this->_type_id = rl6dNO_TYPE_ID;
}

// methods
UDINT Router::get_type_id(){
    return this->_type_id;
}
void Router::set_add_sh_from_asm(){
    this->_cmd.add_sh_from_asm = 1;
}
void Router::reset_add_sh_from_asm(){
    this->_cmd.add_sh_from_asm = 0;
}
void Router::set_error_reset(){
    this->_cmd.error_reset = 1;
}
UINT Router::get_num_asm_sh(){
    return this->_asm_shuttles;
}
void Router::set_recover_shuttles(rl6dRecoverModeEnum mode, REAL velocity, REAL acceleration){
    this->_move_in_plane_async.Parameters.Velocity = velocity;
    this->_move_in_plane_async.Parameters.Acceleration = acceleration;
    this->_cmd.recover_shuttles_mode = mode;
    this->_cmd.recover_shuttles = 1;
}
void Router::reset_recover_shuttles(){
    this->_cmd.recover_shuttles = 0;
    this->_cmd.recover_shuttles_mode = rl6dRECOVER_MODE_BASIC;
}
bool Router::get_cmd_ready(){
    return this->_status.cmd_ready;
}
bool Router::get_done(){
    return this->_status.done;
}
std::shared_ptr<Path> Router::create_path(std::string name, std::shared_ptr<RouterSem> router_sem){
    auto it = std::find_if(this->_paths.begin(), this->_paths.end(), [name](std::shared_ptr<Path> i){return i->get_name() == name;});
    if(it == this->_paths.end()){
        std::shared_ptr<Path> path (new Path(name, router_sem));
        this->_paths.push_back(path);
        return path;
    }
    else{
        throw std::invalid_argument("Path with given name already exists.");
    }
}
std::shared_ptr<Destination> Router::create_destination(std::string name, std::shared_ptr<RouterSem> router_sem, rl6dPositionType position, std::shared_ptr<Path> path_in, std::shared_ptr<Path> path_out){
    auto it = std::find_if(this->_destinations.begin(), this->_destinations.end(), [name](std::shared_ptr<Destination> i){return i->get_name() == name;});
        if(it == this->_destinations.end()){
        std::shared_ptr<Destination> destination (new Destination(name, router_sem, position, path_in, path_out));
        this->_destinations.push_back(destination);
        return destination;
    }
    else{
        throw std::invalid_argument("Destination with given name already exists.");
    }
}
std::shared_ptr<Waypoint> Router::create_waypoint(std::string name, std::shared_ptr<RouterSem> router_sem, rl6dPositionType position){
    auto it = std::find_if(this->_waypoints.begin(), this->_waypoints.end(), [name](std::shared_ptr<Waypoint> i){return i->get_name() == name;});
        if(it == this->_waypoints.end()){
        std::shared_ptr<Waypoint> waypoint (new Waypoint(name, router_sem, position));
        this->_waypoints.push_back(waypoint);
        return waypoint;
    }
    else{
        throw std::invalid_argument("Waypoint with given name already exists.");
    }
}
void Router::delete_path(std::shared_ptr<Path> path){
    auto it = std::find_if(this->_paths.begin(), this->_paths.end(), [path](std::shared_ptr<Path> i){return i == path;});
    if(it == this->_paths.end()){
        throw std::invalid_argument("Path not defined in Router.");
    }
    else{
        this->_paths.erase(it);
    }
}
std::shared_ptr<Path> Router::get_path_by_name(std::string name){

    auto it = std::find_if(this->_paths.begin(), this->_paths.end(), [name](std::shared_ptr<Path> i){return i->get_name() == name;});
    if(it == this->_paths.end()){
        throw std::invalid_argument("Path with given name does not exist.");
    }
    else{
        return *it;
    }
}
std::shared_ptr<Shuttle> Router::add_shuttle(Mc6DShuttleType shuttle, std::shared_ptr<RouterSem> router_semaphore, UINT shuttle_id){
    for (auto & existing_shuttle : this->_shuttles){
        if (existing_shuttle->get_shuttle_reference()->controlif == shuttle.controlif){
            throw std::invalid_argument("Shuttle already in router.");
        }
    }
    std::shared_ptr<Shuttle> routed_shuttle (new Shuttle(shuttle, router_semaphore, this->_user_data_size, shuttle_id));
    this->_shuttles.push_back(routed_shuttle);
    return routed_shuttle;
}
bool Router::acquire_path_semaphore(){
    return this->_path_semaphore->sem_acquire();
}
void Router::release_path_semaphore(){
    this->_path_semaphore->sem_release();
}
bool Router::acquire_waypoint_semaphore(){
    return this->_waypoint_semaphore->sem_acquire();
}
void Router::release_waypoint_semaphore(){
    this->_waypoint_semaphore->sem_release();
}
bool Router::acquire_shuttle_semaphore(){
    return this->_shuttle_semaphore->sem_acquire();
}
void Router::release_shuttle_semaphore(){
    this->_shuttle_semaphore->sem_release();
}
bool Router::acquire_destination_semaphore(){
    return this->_destination_semaphore->sem_acquire();
}
void Router::release_destination_semaphore(){
    this->_destination_semaphore->sem_release();
}
bool Router::acquire_cmd_semaphore(){
    return this->_command_semaphore->sem_acquire();
}
void Router::release_cmd_semaphore(){
    this->_command_semaphore->sem_release();
}
bool Router::get_initialized(){
    return this->_init_done;
}
bool Router::get_error(){
    return this->_error;
}
DINT Router::get_error_id(){
    return this->_error_id;
}
void Router::cyclic(){
    
    bool done;

    switch(this->_state){
    
        case STATE_INIT:
            
            std::strcpy(this->_ar_event_log_create.Name, router_6d_log_name);
            this->_ar_event_log_create.Size = 10000; //TODO
            this->_ar_event_log_create.Persistence = arEVENTLOG_PERSISTENCE_PERSIST; //TODO
            this->_ar_event_log_create.Execute = 1;
            this->_state = STATE_EVENT_LOG_CREATE;
            
            break;
            
        case STATE_EVENT_LOG_CREATE:
            
            if(this->_ar_event_log_create.Done || this->_ar_event_log_create.Error){
                this->_ar_event_log_create.Execute = 0;
                this->_state = STATE_WAIT_SEM;
            }
            
            break;
            
        case STATE_WAIT_SEM:
            
            if(this->_semaphore_manager.get_initialized()){
                if(this->sem_acquire()){
                    if(this->get_sem_cmd_ready()){
                        this->set_create_sem();
                        this->_state = STATE_CREATE_PATH_SEM;
                    }
                    else{
                        this->sem_release();
                    }
                }
            }
            
            break;
        
        case STATE_CREATE_PATH_SEM:
            
            if(this->get_sem_done()){
                this->_path_semaphore = this->get_created_sem();
                this->reset_create_sem();
                this->_state = STATE_CREATE_WAYPOINT_SEM_READY;
            }
            
            break;
        
        case STATE_CREATE_WAYPOINT_SEM_READY:
            
            if(this->get_sem_cmd_ready()){
                this->set_create_sem();
                this->_state = STATE_CREATE_WAYPOINT_SEM;
            }
            
            break;
        
        case STATE_CREATE_WAYPOINT_SEM:
            
            if(this->get_sem_done()){
                this->_waypoint_semaphore = this->get_created_sem();
                this->reset_create_sem();
                this->_state = STATE_CREATE_SHUTTLE_SEM_READY;
            }
            
            break;
        
        case STATE_CREATE_SHUTTLE_SEM_READY:
            
            if(this->get_sem_cmd_ready()){
                this->set_create_sem();
                this->_state = STATE_CREATE_SHUTTLE_SEM;
            }
            
            break;
        
        case STATE_CREATE_SHUTTLE_SEM:
            
            if(this->get_sem_done()){
                this->_shuttle_semaphore = this->get_created_sem();
                this->reset_create_sem();
                this->_state = STATE_CREATE_DESTINATION_SEM_READY;
            }
            
            break;
        
        case STATE_CREATE_DESTINATION_SEM_READY:
            
            if(this->get_sem_cmd_ready()){
                this->set_create_sem();
                this->_state = STATE_CREATE_DESTINATION_SEM;
            }
            
            break;
        
        case STATE_CREATE_DESTINATION_SEM:
            
            if(this->get_sem_done()){
                this->_destination_semaphore = this->get_created_sem();
                this->reset_create_sem();
                this->_state = STATE_CREATE_CMD_SEM_READY;
            }
            
            break;
    
        case STATE_CREATE_CMD_SEM_READY:
                
            if(this->get_sem_cmd_ready()){
                this->set_create_sem();
                this->_state = STATE_CREATE_CMD_SEM;
            }
                
        break;
    
        case STATE_CREATE_CMD_SEM:
                
            if(this->get_sem_done()){
                this->_command_semaphore = this->get_created_sem();
                this->reset_create_sem();
                this->sem_release();
                this->_init_done = 1;
                this->_status.cmd_ready = 1;
                this->_state = STATE_ACTIVE;
            }
                
        break;
                
        case STATE_ACTIVE:
        
            this->_router_sem_complete = 1;
            if(this->_cmd.add_sh_from_asm){
                log_asm_get_sh_start();
                this->_status.cmd_ready = 0;
                this->_old_remaining_count = 0;
                this->_asm_get_shuttle.Enable = 1;
                this->_state = STATE_ADD_SH_ASM;
            }
            else if(this->_cmd.recover_shuttles){
                if(this->acquire_shuttle_semaphore()){
                    log_asm_recovery_start(this->_cmd.recover_shuttles_mode);
                    this->_recovery_shuttles = this->_shuttles;
                    this->release_shuttle_semaphore();
                    this->_status.cmd_ready = 0;
                    this->_state = STATE_RECOVER_SHUTTLES_GET_DEST;
                }
            }
    
            break;
        
        case STATE_ADD_SH_ASM:
        
            if(this->_asm_get_shuttle.Error){
                log_asm_get_sh_error(this->_asm_get_shuttle.ErrorID);
                this->_asm_get_shuttle.Next = 0;
                this->_asm_get_shuttle.Enable = 0;
                this->_error_id = this->_asm_get_shuttle.ErrorID;
                this->_shuttles.clear();
                this->_state = STATE_ERROR;
            }
            else if(this->_asm_get_shuttle.Valid){
                if(this->_asm_get_shuttle.TotalCount == 0){
                    this->_status.done = 1;
                    log_asm_get_sh_done(this->_asm_get_shuttle.TotalCount);
                    this->_asm_shuttles = 0;
                    this->_state = STATE_ADD_SH_ASM_DONE;
                }
                else if((this->_asm_get_shuttle.RemainingCount != this->_old_remaining_count) || (this->_asm_get_shuttle.TotalCount == 1)){
                    this->_asm_get_shuttle.Next = 0;
                        if(this->sem_acquire()){
                        if(this->get_sem_cmd_ready()){
                            this->set_create_sem();
                            this->_state = STATE_ADD_SH_ASM_SEM;
                        }
                        else{
                            this->sem_release();
                        }
                    }
                }
            }
                    
            break;
                
        case STATE_ADD_SH_ASM_SEM:
            
            if(this->get_sem_done()){
                this->_created_semaphore = this->get_created_sem();
                this->reset_create_sem();
                this->sem_release();
                this->_state = STATE_ADD_SH_ASM_NEXT;
            }
            
            break;
        
        case STATE_ADD_SH_ASM_NEXT:
        
            if(this->acquire_shuttle_semaphore()){
                this->add_shuttle(this->_asm_get_shuttle.Shuttle, this->_created_semaphore, this->_asm_get_shuttle.AdditionalInfo.ShuttleID);
                this->release_shuttle_semaphore();
                this->_created_semaphore.reset();
                if(this->_asm_get_shuttle.RemainingCount == 0){
                    log_asm_get_sh_done(this->_asm_get_shuttle.TotalCount);
                    this->_asm_get_shuttle.Enable = 0;
                    this->_asm_shuttles = this->_asm_get_shuttle.TotalCount;
                    this->_status.done = 1;
                    this->_state = STATE_ADD_SH_ASM_DONE;
                }
                else{
                    this->_old_remaining_count = this->_asm_get_shuttle.RemainingCount;
                    this->_asm_get_shuttle.Next = 1;
                    this->_state = STATE_ADD_SH_ASM;
                }
            }
        
            break;
        
        case STATE_ADD_SH_ASM_DONE:
            
            if(!this->_cmd.add_sh_from_asm){
                this->_asm_shuttles = 0;
                this->_status.done = 0;
                this->_status.cmd_ready = 1;
                this->_state = STATE_ACTIVE;
            }
        
            break;
        
        case STATE_RECOVER_SHUTTLES_GET_DEST:
            
            if(this->acquire_destination_semaphore()){
                this->_recovery_destinations = this->_destinations;
                this->release_destination_semaphore();
                if(this->_recovery_destinations.empty()){
                    this->_status.done = 1;
                    log_asm_recovery_done();
                    this->_state = STATE_RECOVER_SHUTTLES_DONE;
                }
                else{
                    this->_current_recovery_destination = this->_recovery_destinations.back();
                    this->_recovery_destinations.pop_back();
                    this->_current_recovery_index = 0;
                    this->_state = STATE_RECOVER_SHUTTLES_GET_POS;
                }
            }
            
            break;
            
        case STATE_RECOVER_SHUTTLES_GET_POS:
            
            if(this->_current_recovery_destination->sem_acquire()){
                this->_current_recovery_positions = this->_current_recovery_destination->get_recovery_positions();
                this->_current_recovery_destination->sem_release();
                this->_state = STATE_RECOVER_SHUTTLES_ASSIGN;
            }
        
            break;
        
        case STATE_RECOVER_SHUTTLES_ASSIGN:
        
            while(this->_current_recovery_index < rl6dMAX_RECOVER_SHUTTLES){
                if(this->_recovery_shuttles.empty()){
                    break;
                }
                if(this->_current_recovery_positions.empty()){
                    break;
                }
                this->_current_recovery_shuttle = this->_recovery_shuttles.back();
                log_asm_sh_assignment(this->_current_recovery_shuttle->get_shuttle_id(), this->_current_recovery_destination->get_name());
                this->_shuttle_destination_assignment.push_back(ShDestAssignment{this->_current_recovery_shuttle, this->_current_recovery_destination});
                this->_recovery_shuttles.pop_back();
                this->_current_recovery_position = this->_current_recovery_positions.back();
                this->_current_recovery_positions.pop_back();
                this->_move_in_plane_async.Parameters.AsyncPar[this->_current_recovery_index].Shuttle = this->_current_recovery_shuttle->get_shuttle_reference();
                this->_move_in_plane_async.Parameters.AsyncPar[this->_current_recovery_index].X = this->_current_recovery_position.X;
                this->_move_in_plane_async.Parameters.AsyncPar[this->_current_recovery_index].Y = this->_current_recovery_position.Y;
                this->_current_recovery_index++;
            }
            if(this->_recovery_shuttles.empty()){
                //All shuttles have been assigned
                if(this->_current_recovery_index > 0){
                    //There is at least one shuttle to recover
                    this->_move_in_plane_async.Execute = 1;
                    this->_state = STATE_RECOVER_SHUTTLES;
                }
                else{
                    //There are no shuttles to recover
                    this->_recovery_destinations.clear();
                    this->_current_recovery_positions.clear();
                    this->_current_recovery_shuttle.reset();
                    this->_current_recovery_destination.reset();
                    this->_state = STATE_RECOVER_DESTINATION;
                }
            }
            else if(this->_current_recovery_index >= (rl6dMAX_RECOVER_SHUTTLES - 1)){
                //The Async function block has all indices assigned
                this->_move_in_plane_async.Execute = 1;
                this->_state = STATE_RECOVER_SHUTTLES;
            }
            else{
                //We are out of recovery positions on this destination
                if(this->_recovery_destinations.empty()){
                    if(this->_current_recovery_index > 0){
                        //There is at least one position we can recover to
                        this->_move_in_plane_async.Execute = 1;
                        this->_state = STATE_RECOVER_SHUTTLES;
                    }
                    else{
                        //There are no positions we can recover to
                        this->_recovery_shuttles.clear();
                        this->_current_recovery_positions.clear();
                        this->_current_recovery_shuttle.reset();
                        this->_current_recovery_destination.reset();
                        this->_state = STATE_RECOVER_DESTINATION;
                    }
                }
                else{
                    this->_current_recovery_destination = this->_recovery_destinations.back();
                    this->_recovery_destinations.pop_back();
                    this->_state = STATE_RECOVER_SHUTTLES_GET_POS;
                }
            }
            
            break;
        
        case STATE_RECOVER_SHUTTLES:
        
            if(this->_move_in_plane_async.Done){
                this->_move_in_plane_async.Execute = 0;
                this->_current_recovery_index = 0;
                this->_state = STATE_RECOVER_SHUTTLES_ASSIGN;
            }
            else if(this->_move_in_plane_async.Error){
                log_move_in_plane_error(this->_move_in_plane_async.ErrorID);
                this->_move_in_plane_async.Execute = 0;
                this->_status.error_id = this->_move_in_plane_async.ErrorID;
                this->_status.error = 1;
                this->_state = STATE_ERROR;
            }
            
            break;
        
        case STATE_RECOVER_DESTINATION:
            
            while(!this->_shuttle_destination_assignment.empty()){
                this->_current_recovery_shuttle = this->_shuttle_destination_assignment.back().shuttle;
                this->_current_recovery_destination = this->_shuttle_destination_assignment.back().destination;
                if(this->_current_recovery_shuttle->sem_acquire()){
                    this->_current_recovery_shuttle_interface = this->_current_recovery_shuttle->create_interface();
                
                    this->_current_recovery_shuttle_interface->set_destination_direct(this->_current_recovery_destination, 
                                                                                      this->_move_in_plane_async.Parameters.Velocity, 
                                                                                      this->_move_in_plane_async.Parameters.Acceleration);
                    
                this->_shuttle_recovery_interfaces.push_back(this->_current_recovery_shuttle_interface);
                    this->_shuttle_destination_assignment.pop_back();
                    this->_current_recovery_shuttle->sem_release();
                }
                else{
                    break;
                }
            }
            if(this->_shuttle_destination_assignment.empty()){
                this->_current_recovery_shuttle.reset();
                this->_current_recovery_destination.reset();
                this->_current_recovery_shuttle_interface.reset();
                this->_state = STATE_RECOVER_DESTINATION_WAIT;
            }
            
            break;
        
        case STATE_RECOVER_DESTINATION_WAIT:
            
            done = 1;
            for(auto & interface : this->_shuttle_recovery_interfaces){
                if(interface->get_type_id() == rl6dSHUTTLE_IF_TYPE_ID){
                    if(interface->get_error()){
                        this->_status.error_id = interface->get_error_id();
                        this->_status.error = 1;
                        for(auto & interface : this->_shuttle_recovery_interfaces){
                            if(interface->get_type_id() == rl6dSHUTTLE_IF_TYPE_ID){
                                interface->set_delete();
                            }
                        }
                        this->_shuttle_recovery_interfaces.clear();
                        this->_state = STATE_ERROR;
                        break;
                    }
                    else if(!(interface->get_move_active() || interface->get_move_done() || interface->get_aborted())){
                        done = 0;
                    }
                }
                //TODO: Add handling for if the list of interfaces contains an invalid interface
            }
            if(done){
                for(auto & interface : this->_shuttle_recovery_interfaces){
                    if(interface->get_type_id() == rl6dSHUTTLE_IF_TYPE_ID){
                        interface->set_delete();
                    }
                }
                this->_shuttle_recovery_interfaces.clear();
                this->_status.done = 1;
                log_asm_recovery_done();
                this->_state = STATE_RECOVER_SHUTTLES_DONE;
            }
            
            break;
        
        case STATE_RECOVER_SHUTTLES_DONE:
            
            if(!this->_cmd.recover_shuttles){
                this->_status.done = 0;
                this->_status.cmd_ready = 1;
                this->_state = STATE_ACTIVE;
            }
            
            break;
    
        case STATE_ERROR:
        
            this->_error = 1;
            if(this->_cmd.error_reset){
                this->_error_id = ERR_OK;
                this->_error = 0;
            }
        
            break;
        
    }
    
    this->_semaphore_manager.cyclic();
    if(this->_router_sem_complete){
        if(this->acquire_shuttle_semaphore()){
            for(auto & shuttle : this->_shuttles){
                shuttle->cyclic();
            }
            this->release_shuttle_semaphore();
        }
        if(this->acquire_destination_semaphore()){
            for(auto & destination : this->_destinations){
                destination->cyclic();
            }
            this->release_destination_semaphore();
        }
    }
    MC_BR_AsmGetShuttle_Acp6D(&this->_asm_get_shuttle);
    MC_BR_MoveInPlaneAsync_Acp6D(&this->_move_in_plane_async);
    ArEventLogCreate(&this->_ar_event_log_create);

    this->_cmd.error_reset = 0;
    
}
