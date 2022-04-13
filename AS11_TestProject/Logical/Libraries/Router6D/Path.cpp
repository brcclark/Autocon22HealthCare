#include "Path.hpp"

Path::Path(std::string name, std::shared_ptr<RouterSem> router_sem) : RouterSemBase{router_sem}{
    this->_name = name;
}
    
//destructor
Path::~Path(){
    this->_type_id = rl6dNO_TYPE_ID;
}

// methods
UDINT Path::get_type_id(){
    return this->_type_id;
}
void Path::add_position(rl6dPositionType position){
    this->_positions.push_back(position);
}
std::string Path::get_name(){
    return this->_name;
}
std::shared_ptr<Path> Path::get_shared_ptr(){
    return shared_from_this();
}
std::vector<rl6dPositionType> Path::get_positions(){
    return this->_positions;
}
