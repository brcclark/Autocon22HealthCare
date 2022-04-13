#include "Waypoint.hpp"

Waypoint::Waypoint(std::string name, std::shared_ptr<RouterSem> router_sem) : RouterSemBase{router_sem}{
    this->_name = name;
}
Waypoint::Waypoint(std::string name, std::shared_ptr<RouterSem> router_sem, rl6dPositionType position) : RouterSemBase{router_sem}{
    this->_name = name;
    this->_position = position;
}
    
//destructor
Waypoint::~Waypoint(){
    this->_type_id = rl6dNO_TYPE_ID;
}

// methods
UDINT Waypoint::get_type_id(){
    return this->_type_id;
}
std::string Waypoint::get_name(){
    return this->_name;
}
std::shared_ptr<Waypoint> Waypoint::get_shared_ptr(){
    return shared_from_this();
}
void Waypoint::add_destination(std::shared_ptr<Destination> destination, std::shared_ptr<Path> departure_path){
    WaypointDestination waypoint_destination = {destination, departure_path};
    this->_destinations.push_back(waypoint_destination);
}
std::vector<WaypointDestination> Waypoint::get_destinations(void){
    return this->_destinations;
}
rl6dPositionType Waypoint::get_position(void){
    return this->_position;
}
std::weak_ptr<Waypoint> Waypoint::get_next_waypoint(void){
    return this->_next_waypoint;
}
void Waypoint::set_next_waypoint(std::weak_ptr<Waypoint> waypoint){
    this->_next_waypoint = waypoint;
}
void Waypoint::reset_next_waypoint(void){
    this->_next_waypoint.reset();
}
bool Waypoint::check_destination_accessible(std::shared_ptr<Destination> destination){
    for (auto & accessible_destination : this->_destinations){
        if(accessible_destination.destination == destination){
            return true;
        }
    }
    return false;
}
std::shared_ptr<Path> Waypoint::get_destination_departure_path(std::shared_ptr<Destination> destination){
    std::shared_ptr<Path> path;
    auto it = std::find_if(this->_destinations.begin(), this->_destinations.end(), [destination](WaypointDestination i){return i.destination == destination;});
    if(it != this->_destinations.end()){
        path = it->departure_path;
    }
    return path;
}
