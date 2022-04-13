#include "Destination.hpp"

Destination::Destination(std::string name, std::shared_ptr<RouterSem> router_sem): RouterSemBase{router_sem}{
    this->_name = name;
}
Destination::Destination(std::string name, std::shared_ptr<RouterSem> router_sem, rl6dPositionType position): RouterSemBase{router_sem}{
    this->_name = name;
    this->_position = position;
}
Destination::Destination(std::string name, std::shared_ptr<RouterSem> router_sem, rl6dPositionType position, std::shared_ptr<Path> path_in, std::shared_ptr<Path> path_out): RouterSemBase{router_sem}{
    this->_name = name;
    this->_position = position;
    this->_path_in = path_in;
    this->_path_out = path_out;
}
    
//destructor
Destination::~Destination(){
    this->_type_id = rl6dNO_TYPE_ID;
}

// methods
UDINT Destination::get_type_id(){
    return this->_type_id;
}
std::string Destination::get_name(){
    return this->_name;
}
rl6dPositionType Destination::get_position(){
    return this->_position;
}
void Destination::set_position(rl6dPositionType position){
    this->_position = position;
}
std::shared_ptr<Path> Destination::get_path_in(){
    return this->_path_in;
}
void Destination::set_path_in(std::shared_ptr<Path> path){
    this->_path_in = path;
}
void Destination::clear_path_in(void){
    this->_path_in.reset();
}
std::shared_ptr<Path> Destination::get_path_out(){
    return this->_path_out;
}
void Destination::set_path_out(std::shared_ptr<Path> path){
    this->_path_out = path;
}
void Destination::clear_path_out(){
    this->_path_out.reset();
}
void Destination::set_waypoint(std::weak_ptr<Waypoint> waypoint){
    this->_waypoint = waypoint;
}
std::weak_ptr<Waypoint> Destination::get_waypoint(){
    return this->_waypoint;
}
void Destination::reset_waypoint(){
    this->_waypoint.reset();
}
std::shared_ptr<Destination> Destination::get_shared_ptr(){
    return shared_from_this();
}
std::shared_ptr<DestinationIf> Destination::create_interface(){
    std::shared_ptr<DestinationIf> destination_if (new DestinationIf);
    this->_interfaces.push_back(destination_if);
    return destination_if;
}
void Destination::add_recovery_position(rl6dPositionType position){
    this->_recovery_positions.push_back(position);
}
std::vector<rl6dPositionType> Destination::get_recovery_positions(){
    return this->_recovery_positions;
}
void Destination::set_shuttle_present(std::weak_ptr<Shuttle> shuttle){
    this->_shuttle = shuttle;
    this->_shuttle_present = 1;
}
void Destination::cyclic(){

    if(this->_shuttle_present){
        if(this->sem_acquire()){
            for(auto & interface : this->_interfaces){
                interface->set_shuttle_present(this->_shuttle);
            }
            this->sem_release();
            this->_shuttle_present = 0;
            this->_shuttle.reset();
        }
    }
    
    if(this->sem_acquire()){
        auto it = std::find_if(this->_interfaces.begin(), this->_interfaces.end(), [](std::shared_ptr<DestinationIf> i){return i->get_delete();});
            if(it != this->_interfaces.end()){
            this->_interfaces.erase(it);
        }
        this->sem_release();
    }
    
}
