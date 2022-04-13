#include "Shuttle.hpp"
ShuttleRoutePositionGenerator::ShuttleRoutePositionGenerator(){
    std::memset(&this->position, 0, sizeof(this->position));
    this->_previous_destination.reset();
    this->_next_destination.reset();
    this->_route_waypoint.reset();
    this->_current_waypoint.reset();
    this->_departure_path.reset();
}
ShuttleRoutePositionGenerator::ShuttleRoutePositionGenerator(std::shared_ptr<Destination> previous_destination, std::shared_ptr<Destination> next_destination){
    std::memset(&this->position, 0, sizeof(this->position));
    this->_previous_destination = previous_destination;
    this->_next_destination = next_destination;
    this->_route_waypoint.reset();
    this->_current_waypoint.reset();
    this->_departure_path.reset();
}
ShuttleRoutePositionGenerator::ShuttleRoutePositionGenerator(std::shared_ptr<Destination> previous_destination, std::shared_ptr<Destination> next_destination, std::weak_ptr<Waypoint> route_waypoint){
    std::memset(&this->position, 0, sizeof(this->position));
    this->_previous_destination = previous_destination;
    this->_next_destination = next_destination;
    this->_route_waypoint = route_waypoint;
    this->_current_waypoint.reset();
    this->_departure_path.reset();
}

ShuttleRoutePositionGenerator::~ShuttleRoutePositionGenerator() = default;

set_path_status_enum ShuttleRoutePositionGenerator::_set_path(std::shared_ptr<Path> path){
    this->_path = path;
    if(this->_path){
        if(this->_path->sem_acquire()){
            this->_path_positions = this->_path->get_positions();
            this->_path->sem_release();
            this->_path_iterator = this->_path_positions.begin();
            if(this->_path_iterator == this->_path_positions.end()){
                return set_path_status_invalid;
            }
            else{
                return set_path_status_valid;
            }
        }
        else{
            return set_path_status_wait_sem;
        }
    }
    else{
        return set_path_status_invalid;
    }
}

bool ShuttleRoutePositionGenerator::operator++(int){
    
    std::shared_ptr<Waypoint> waypoint;
    rl6dPositionType position;
    
    if(this->_phase == ROUTED_MOVE_PHASE_EXIT_DEST_START){ 
        //PHASE_EXIT_DEST_START: Attempt to get the exit path
        
        if(this->_previous_destination){
            this->_exit_path_status = this->_set_path(this->_previous_destination->get_path_out());
            if(this->_exit_path_status == set_path_status_valid){
                this->position.X = this->_path_iterator->X;
                this->position.Y = this->_path_iterator->Y;
                this->_path_iterator++;
                this->_phase = ROUTED_MOVE_PHASE_EXIT_DEST;
                this->event.path = this->_path;
                this->event.waypoint.reset();
                this->event.event = ROUTED_MOVE_EVENT_PATH_START;
                return true;
            }
            else if(this->_exit_path_status == set_path_status_wait_sem){
                this->event.path.reset();
                this->event.waypoint.reset();
                this->event.event = ROUTED_MOVE_EVENT_NONE;
                return false;
            }
        }
        this->_phase = ROUTED_MOVE_PHASE_WAYPOINT_START; //There is no exit path
        
    }
    if(this->_phase == ROUTED_MOVE_PHASE_EXIT_DEST){
        //PHASE_EXIT_DEST: Follow the exit path
        
        if(this->_path_iterator < this->_path_positions.end()){
            this->position.X = this->_path_iterator->X;
            this->position.Y = this->_path_iterator->Y;
            this->_path_iterator++;
            this->event.path.reset();
            this->event.waypoint.reset();
            this->event.event = ROUTED_MOVE_EVENT_NONE;
            return true;
        }
        this->_phase = ROUTED_MOVE_PHASE_WAYPOINT_START; //There are no points left on the exit path
        
    }
    if(this->_phase == ROUTED_MOVE_PHASE_WAYPOINT_START){
        //PHASE_WAYPOINT_START: Attempt to get the waypoint
        
        if(!this->_route_waypoint.expired()){
            this->_current_waypoint = this->_route_waypoint;
        }
        else if(this->_previous_destination){
            this->_current_waypoint = this->_previous_destination->get_waypoint();
        }
        if(!this->_current_waypoint.expired()){
            waypoint = this->_current_waypoint.lock();
            position = waypoint->get_position();
            this->position.X = position.X;
            this->position.Y = position.Y;
            this->_phase = ROUTED_MOVE_PHASE_WAYPOINT;
            this->event.path.reset();
            this->event.waypoint = this->_current_waypoint;
            this->event.event = ROUTED_MOVE_EVENT_WAYPOINT;
            return true;
        }
        this->_phase = ROUTED_MOVE_PHASE_ENTER_DEST_START; //There is no waypoint
        
    }
    if(this->_phase == ROUTED_MOVE_PHASE_WAYPOINT){
        //PHASE_WAYPOINT: Follow waypoints
        
        if(!this->_current_waypoint.expired()){
            waypoint = this->_current_waypoint.lock();
            if(waypoint->sem_acquire()){
                bool destination_accessible = waypoint->check_destination_accessible(this->_next_destination);
                if(destination_accessible){
                    this->_departure_path = waypoint->get_destination_departure_path(this->_next_destination);
                    this->_phase = ROUTED_MOVE_PHASE_DEPART_WAYPOINT_START;
                }
                waypoint->sem_release();
                if(!destination_accessible){
                    this->_current_waypoint = waypoint->get_next_waypoint();
                    if(!this->_current_waypoint.expired()){
                        waypoint = this->_current_waypoint.lock();
                        position = waypoint->get_position();
                        this->position.X = position.X;
                        this->position.Y = position.Y;
                        this->event.path.reset();
                        this->event.waypoint = this->_current_waypoint;
                        this->event.event = ROUTED_MOVE_EVENT_WAYPOINT;
                        return true;
                    }
                }
            }
            else{
                this->event.path.reset();
                this->event.waypoint.reset();
                this->event.event = ROUTED_MOVE_EVENT_NONE;
                return false;
            }
        }
        this->_phase = ROUTED_MOVE_PHASE_ENTER_DEST_START; //Something went wrong here
        
    }
    if(this->_phase == ROUTED_MOVE_PHASE_DEPART_WAYPOINT_START){
        //PHASE_DEPART_WAYPOINT_START: Attempt to get the departure path
        
        this->_departure_path_status = this->_set_path(this->_departure_path);
        if(this->_departure_path_status == set_path_status_valid){
            this->position.X = this->_path_iterator->X;
            this->position.Y = this->_path_iterator->Y;
            this->_path_iterator++;
            this->_phase = ROUTED_MOVE_PHASE_DEPART_WAYPOINT;
            this->event.path = this->_path;
            this->event.waypoint.reset();
            this->event.event = ROUTED_MOVE_EVENT_PATH_START;
            return true;
        }
        else if(this->_departure_path_status == set_path_status_wait_sem){
            this->event.path.reset();
            this->event.waypoint.reset();
            this->event.event = ROUTED_MOVE_EVENT_NONE;
            return false;
        }
        this->_phase = ROUTED_MOVE_PHASE_ENTER_DEST_START; //There is no departure path
        
    }
    if(this->_phase == ROUTED_MOVE_PHASE_DEPART_WAYPOINT){
        //PHASE_DEPART_WAYPOINT_START: Follow the departure path
        
        if(this->_departure_path_status == set_path_status_valid){
            if(this->_path_iterator < this->_path_positions.end()){
                this->position.X = this->_path_iterator->X;
                this->position.Y = this->_path_iterator->Y;
                this->_path_iterator++;
                this->event.path.reset();
                this->event.waypoint.reset();
                this->event.event = ROUTED_MOVE_EVENT_NONE;
                return true;
            }
        }
        this->_phase = ROUTED_MOVE_PHASE_ENTER_DEST_START; //There are no points left on the departure path
        
    }
    if(this->_phase == ROUTED_MOVE_PHASE_ENTER_DEST_START){
        //ROUTED_MOVE_PHASE_ENTER_DEST_START: Attempt to get the entry path
        
        this->_entry_path_status = this->_set_path(this->_next_destination->get_path_in());
        if(this->_entry_path_status == set_path_status_valid){
            this->position.X = this->_path_iterator->X;
            this->position.Y = this->_path_iterator->Y;
            this->_path_iterator++;
            this->_phase = ROUTED_MOVE_PHASE_ENTER_DEST;
            this->event.path = this->_path;
            this->event.waypoint.reset();
            this->event.event = ROUTED_MOVE_EVENT_PATH_START;
            return true;
        }
        else if(this->_entry_path_status == set_path_status_wait_sem){
            this->event.path.reset();
            this->event.waypoint.reset();
            this->event.event = ROUTED_MOVE_EVENT_NONE;
            return false;
        }
        this->_phase = ROUTED_MOVE_PHASE_DEST; //There is no entry path
        
    }
    if(this->_phase == ROUTED_MOVE_PHASE_ENTER_DEST){
        //ROUTED_MOVE_PHASE_ENTER_DEST: Follow the entry path
        
        if(this->_entry_path_status == set_path_status_valid){
            if(this->_path_iterator < this->_path_positions.end()){
                this->position.X = this->_path_iterator->X;
                this->position.Y = this->_path_iterator->Y;
                this->_path_iterator++;
                this->event.path.reset();
                this->event.waypoint.reset();
                this->event.event = ROUTED_MOVE_EVENT_NONE;
                return true;
            }
        }
        this->_phase = ROUTED_MOVE_PHASE_DEST; //There are no points left on the entry path
        
    }
    if(this->_phase == ROUTED_MOVE_PHASE_DEST){
        //ROUTED_MOVE_PHASE_DEST: Move to destination        
        
        position = this->_next_destination->get_position();
        this->position.X = position.X;
        this->position.Y = position.Y;
        if(this->_updated_destination){
            this->_previous_destination = this->_next_destination;
            this->_next_destination = this->_updated_destination;
            this->_updated_destination.reset();
            this->_phase = ROUTED_MOVE_PHASE_EXIT_DEST_START;
        }
        else{
            this->_done = 1;
            this->_phase = ROUTED_MOVE_PHASE_ARRIVE;
        }
        this->event.path.reset();
        this->event.waypoint.reset();
        this->event.event = ROUTED_MOVE_EVENT_NONE;
        return true;
        
    }
    
    return false;

}
   
void ShuttleRoutePositionGenerator::update_destination(std::shared_ptr<Destination> destination){
    if(this->_phase < ROUTED_MOVE_PHASE_DEPART_WAYPOINT_START){ //We have not yet left for the current destination
        this->_next_destination = destination;
    }
    else{
        this->_updated_destination = destination;
    }
}
 
void ShuttleRoutePositionGenerator::update_destination(std::shared_ptr<Destination> destination, std::weak_ptr<Waypoint> waypoint){
    if(!waypoint.expired()){
        this->_route_waypoint = waypoint;
    }
    if(this->_phase < ROUTED_MOVE_PHASE_DEPART_WAYPOINT_START){ //We have not yet left for the current destination
        this->_next_destination = destination;
    }
    else{
        this->_updated_destination = destination;
    }
}

routed_move_phase_enum ShuttleRoutePositionGenerator::get_phase(){
    return this->_phase;
}
bool ShuttleRoutePositionGenerator::get_done(){
    return this->_done;
}

Shuttle::Shuttle(Mc6DShuttleType shuttle, std::shared_ptr<RouterSem> router_sem, UDINT user_data_size, UINT shuttle_id) : RouterSemBase{router_sem}{
    std::memset(&this->_move_in_plane, 0, sizeof(this->_move_in_plane));
    this->_shuttle = shuttle;
    this->_move_in_plane.Shuttle = &this->_shuttle;
    this->_move_in_plane.Parameters.Mode = mcACP6D_MOVE_ABSOLUTE;
    this->_user_data = std::unique_ptr<char[]>(new char[user_data_size]);
    std::memset(this->_user_data.get(), 0, user_data_size);
    this->_user_data_size = user_data_size;
    this->_shuttle_id = shuttle_id;
}
    
//destructor
Shuttle::~Shuttle(){
    this->_type_id = rl6dNO_TYPE_ID;
}

// methods
UDINT Shuttle::get_type_id(){
    return this->_type_id;
}
UINT Shuttle::get_shuttle_id(){
    return this->_shuttle_id;
}
Mc6DShuttleType* Shuttle::get_shuttle_reference(){
    return &this->_shuttle;
}
std::shared_ptr<Destination> Shuttle::get_current_destination(){
    return this->_next_destination;
}
std::shared_ptr<Destination> Shuttle::get_previous_destination(){
    return this->_previous_destination;
}
void Shuttle::clear_previous_destination(){
    this->_previous_destination.reset();
}
std::shared_ptr<ShuttleIf> Shuttle::create_interface(){
    std::shared_ptr<ShuttleIf> shuttle_if (new ShuttleIf);
    this->_interfaces.push_back(shuttle_if);
    return shuttle_if;
}
std::shared_ptr<Shuttle> Shuttle::get_shared_ptr(){
    return shared_from_this();
}
ReadUserDataResult Shuttle::read_user_data(){
    ReadUserDataResult result = {this->_user_data.get(), this->_user_data_size};
    return result;
}
UDINT Shuttle::write_user_data(char* value, UDINT size){
    if(size < this->_user_data_size){
        std::memcpy(this->_user_data.get(), value, size);
        return size;
    }
    else{
        std::memcpy(this->_user_data.get(), value, this->_user_data_size);
        return this->_user_data_size;
    }
}
bool Shuttle::_update_interfaces(){
    if(this->_current_interface){
        if(this->_interfaces.empty()){ //The current interface is defined, but no interfaces are being used
            this->_current_interface.reset();
            return 1; //Indicate that the interface has changed
        }
        else if(this->_current_interface != this->_interfaces.back()){ //A newer interface is available
            std::shared_ptr<ShuttleIf> interface = this->_current_interface;
            interface->reset_move_active();
            interface->set_aborted();
            this->_current_interface = this->_interfaces.back();
            return 1; //Indicate that the interface has changed
        }
    }
    else if(!this->_interfaces.empty()){ //No interface is defined, but an interface is available
        this->_current_interface = this->_interfaces.back();
        return 1; //Indicate that the interface has changed
    }
    return 0; //Indicate that the interface has not changed
}

void Shuttle::_log_event(RoutedMoveEvent event){

    switch(event.event){
                
        case ROUTED_MOVE_EVENT_NONE:
                        
            break;
                    
        case ROUTED_MOVE_EVENT_PATH_START:
            log_path_start(this->get_shuttle_id(), this->_route_position_generator.event.path->get_name());
            break;
                    
        case ROUTED_MOVE_EVENT_WAYPOINT:
            log_waypoint_move(this->get_shuttle_id(), this->_route_position_generator.event.waypoint.lock()->get_name());
            break;
                    
    }
    
    return;
    
}

void Shuttle::cyclic(){
    
    rl6dPositionType position;
    
    switch(this->_state){
        
        case STATE_IDLE:
            
            if(this->_update_interfaces()){
                if(this->_current_interface){
                    this->_move_in_plane.Parameters.Velocity = this->_current_interface->get_velocity();
                    this->_move_in_plane.Parameters.Acceleration = this->_current_interface->get_acceleration();
                    this->_current_interface->set_move_active();
                    this->_next_destination = this->_current_interface->get_destination();
                    if(this->_current_interface->get_destination_direct_command()){
                        position = this->_next_destination->get_position();
                        this->_move_in_plane.Parameters.Position.X = position.X;
                        this->_move_in_plane.Parameters.Position.Y = position.Y;
                        this->_move_in_plane.Execute = 1;
                        log_direct_move_start(this->get_shuttle_id(), this->_next_destination->get_name());
                        this->_state = STATE_DIRECT_MOVE;
                    }
                    else if(this->_current_interface->get_destination_command()){
                        this->_route_position_generator = ShuttleRoutePositionGenerator(this->_previous_destination, this->_next_destination);
                        if(this->_route_position_generator++){
                            this->_move_in_plane.Parameters.Position.X = _route_position_generator.position.X;
                            this->_move_in_plane.Parameters.Position.Y = _route_position_generator.position.Y;
                            this->_move_in_plane.Execute = 1;
                            log_routed_move_start(this->get_shuttle_id(), this->_next_destination->get_name());
                            this->_log_event(this->_route_position_generator.event);
                            this->_state = STATE_ROUTED_MOVE;
                        }
                        else{
                            this->_state = STATE_ROUTED_MOVE_START;
                        }
                    }
                }
            }
        
            break;
        
        case STATE_ROUTED_MOVE_START:
        
            if(this->_route_position_generator++){
                this->_move_in_plane.Parameters.Position.X = _route_position_generator.position.X;
                this->_move_in_plane.Parameters.Position.Y = _route_position_generator.position.Y;
                this->_move_in_plane.Execute = 1;
                log_routed_move_start(this->get_shuttle_id(), this->_next_destination->get_name());
                this->_log_event(this->_route_position_generator.event);
                this->_state = STATE_ROUTED_MOVE;
            }
            
            break;
        
        case STATE_ROUTED_MOVE:
        
            if(this->_move_in_plane.Done){
                this->_move_in_plane.Execute = 0;
                if(this->_route_position_generator.get_done()){
                    log_destination_arrival(this->get_shuttle_id(), this->_next_destination->get_name());
                    if(this->_current_interface){
                        this->_current_interface->set_move_done();
                    }
                    std::weak_ptr<Shuttle> reference = shared_from_this();
                    this->_next_destination->set_shuttle_present(reference);
                    this->_previous_destination = this->_next_destination;
                    this->_next_destination.reset();
                    this->_state = STATE_IDLE;
                }
                else{
                    this->_state = STATE_ROUTED_MOVE_NEXT;
                }
            }
            else if(this->_move_in_plane.Error){
                log_sh_error(this->get_shuttle_id(), this->_move_in_plane.ErrorID);
                this->_move_in_plane.Execute = 0;
                this->_current_interface->set_error(this->_move_in_plane.ErrorID);
                this->_state = STATE_IDLE;
            }
            
            break;
        
        case STATE_ROUTED_MOVE_NEXT:
            
            if(this->_update_interfaces()){
                if(this->_current_interface){
                    this->_move_in_plane.Parameters.Velocity = this->_current_interface->get_velocity();
                    this->_move_in_plane.Parameters.Acceleration = this->_current_interface->get_acceleration();
                    this->_current_interface->set_move_active();
                    this->_next_destination = this->_current_interface->get_destination();
                    this->_route_position_generator.update_destination(this->_next_destination);
                    log_destination_update(this->get_shuttle_id(), this->_next_destination->get_name());
                }
            }
            if(this->_route_position_generator++){
                this->_move_in_plane.Parameters.Position.X = _route_position_generator.position.X;
                this->_move_in_plane.Parameters.Position.Y = _route_position_generator.position.Y;
                this->_move_in_plane.Execute = 1;
                this->_log_event(this->_route_position_generator.event);
                this->_state = STATE_ROUTED_MOVE;
            }
        
            break;
        
        case STATE_DIRECT_MOVE:
        
            if(this->_move_in_plane.Done){
                this->_move_in_plane.Execute = 0;
                log_destination_arrival(this->get_shuttle_id(), this->_next_destination->get_name());
                if(this->_current_interface){
                    this->_current_interface->set_move_done();
                }
                std::weak_ptr<Shuttle> reference = shared_from_this();
                this->_next_destination->set_shuttle_present(reference);
                this->_previous_destination = this->_next_destination;
                this->_next_destination.reset();
                this->_state = STATE_IDLE;
            }
            else if(this->_move_in_plane.Error){
                log_sh_error(this->get_shuttle_id(), this->_move_in_plane.ErrorID);
                this->_move_in_plane.Execute = 0;
                if(this->_current_interface){
                    this->_current_interface->set_error(this->_move_in_plane.ErrorID);
                }
                this->_state = STATE_IDLE;
            }
            
            break;
        
    }
    
    if(this->sem_acquire()){
        auto it_delete = std::find_if(this->_interfaces.begin(), this->_interfaces.end(), [](std::shared_ptr<ShuttleIf> i){return i->get_delete();});
        if(it_delete != this->_interfaces.end()){
            this->_interfaces.erase(it_delete);
        }
        //Delete any invalid interfaces
        auto it_invalid = std::find_if(this->_interfaces.begin(), this->_interfaces.end(), [](std::shared_ptr<ShuttleIf> i){return (i->get_type_id() != rl6dSHUTTLE_IF_TYPE_ID);});
        if(it_invalid != this->_interfaces.end()){
            this->_interfaces.erase(it_invalid);
        }
        this->sem_release();
    }
    
    MC_BR_MoveInPlane_Acp6D(&this->_move_in_plane);
    
}
