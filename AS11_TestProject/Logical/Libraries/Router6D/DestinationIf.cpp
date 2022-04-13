#include "DestinationIf.hpp"

DestinationIf::DestinationIf() = default;
    
//destructor
DestinationIf::~DestinationIf(){
    this->_type_id = rl6dNO_TYPE_ID;
}

// methods
UDINT DestinationIf::get_type_id(){
    return this->_type_id;
}
std::shared_ptr<DestinationIf> DestinationIf::get_shared_ptr(){
    return shared_from_this();
}
void DestinationIf::set_shuttle_present(std::weak_ptr<Shuttle> shuttle){
    this->_shuttle = shuttle;
    this->_shuttle_present = 1;
}
bool DestinationIf::shuttle_is_present(void){
    return this->_shuttle_present;
}
void DestinationIf::acknowledge_shuttle(){
    this->_shuttle.reset();
    this->_shuttle_present = 0;
}
std::weak_ptr<Shuttle> DestinationIf::get_shuttle_present(){
    return this->_shuttle;
}
void DestinationIf::set_delete(){
    this->_delete = 1;
}
bool DestinationIf::get_delete(){
    return this->_delete;
}