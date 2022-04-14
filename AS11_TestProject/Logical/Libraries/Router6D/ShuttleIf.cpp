#include "ShuttleIf.hpp"

ShuttleIf::ShuttleIf() = default;
    
//destructor
ShuttleIf::~ShuttleIf(){
    this->_type_id = rl6dNO_TYPE_ID;
}

// methods
UDINT ShuttleIf::get_type_id(){
    return this->_type_id;
}
void ShuttleIf::set_destination(std::shared_ptr<Destination> destination, REAL velocity, REAL acceleration){
    this->_destination = destination;
    this->_velocity = velocity;
    this->_acceleration = acceleration;
    this->_move_destination = 1;
}
void ShuttleIf::set_destination_waypoint(std::shared_ptr<Destination> destination, std::weak_ptr<Waypoint> waypoint,REAL velocity, REAL acceleration){
    this->_destination = destination;
    this->_waypoint = waypoint;
    this->_velocity = velocity;
    this->_acceleration = acceleration;
    this->_move_destination = 1;
}
bool ShuttleIf::get_destination_command(){
    return this->_move_destination;
}
void ShuttleIf::set_destination_direct(std::shared_ptr<Destination> destination, REAL velocity, REAL acceleration){
    this->_destination = destination;
    this->_velocity = velocity;
    this->_acceleration = acceleration;
    this->_move_destination_direct = 1;
}
bool ShuttleIf::get_destination_direct_command(){
    return this->_move_destination_direct;
}
std::shared_ptr<Destination> ShuttleIf::get_destination(){
    return this->_destination;
}
std::weak_ptr<Waypoint> ShuttleIf::get_waypoint(){
    return this->_waypoint;
}
void ShuttleIf::set_aborted(){
    this->_aborted = 1;
}
bool ShuttleIf::get_aborted(){
    return this->_aborted;
}
void ShuttleIf::set_error(DINT error){
    this->_error = 1;
    this->_error_id = error;
}
bool ShuttleIf::get_error(){
    return this->_error;
}
DINT ShuttleIf::get_error_id(){
    return this->_error_id;
}
void ShuttleIf::set_move_done(){
    this->_move_done = 1;
}
bool ShuttleIf::get_move_done(){
    return this->_move_done;
}
void ShuttleIf::set_move_active(){
    this->_move_active = 1;
}
void ShuttleIf::reset_move_active(){
    this->_move_active = 0;
}
bool ShuttleIf::get_move_active(){
    return this->_move_active;
}
void ShuttleIf::set_delete(){
    this->_delete = 1;
}
bool ShuttleIf::get_delete(){
    return this->_delete;
}
REAL ShuttleIf::get_velocity(){
    return this->_velocity;
}
REAL ShuttleIf::get_acceleration(){
    return this->_acceleration;
}
std::shared_ptr<ShuttleIf> ShuttleIf::get_shared_ptr(){
    return shared_from_this();
}
