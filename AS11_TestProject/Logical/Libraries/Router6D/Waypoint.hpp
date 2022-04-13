#ifndef __RL6D_WAYPOINT_H__
#define __RL6D_WAYPOINT_H__

#include <Router6D.h>
#include <Path.hpp>
#include <Destination.hpp>

class Destination; //Forward declaration

struct WaypointDestination{
    std::shared_ptr<Destination> destination;
    std::shared_ptr<Path> departure_path;
};

/**
 \brief A Waypoint is a position that the shuttle will go through on its way to a destination, if defined
*/
class Waypoint : public RouterSemBase, public std::enable_shared_from_this<Waypoint>{
    public:
    //! constructor for the Waypoint
    Waypoint(std::string name, std::shared_ptr<RouterSem> router_sem);
    Waypoint(std::string name, std::shared_ptr<RouterSem> router_sem, rl6dPositionType position);
    
    //! destructor for the Waypoint
    ~Waypoint(void);

    // members
    
    // methods
    //! Get the waypoint type ID
    UDINT get_type_id(void);
    //! Get the waypoint name
    std::string get_name(void);
    
    //! Add an accessible destination to the waypoint
    void add_destination(std::shared_ptr<Destination> destination, std::shared_ptr<Path> departure_path);
    //! Check if a destination is accessible
    bool check_destination_accessible(std::shared_ptr<Destination> destination);
    //! Get destination departure path
    std::shared_ptr<Path> get_destination_departure_path(std::shared_ptr<Destination> destination);
    //! Get destination vector
    std::vector<WaypointDestination> get_destinations(void);
    //! Get position
    rl6dPositionType get_position(void);
    //! Get existing shared pointer
    std::shared_ptr<Waypoint> get_shared_ptr(void);
    //! Get next waypoint
    std::weak_ptr<Waypoint> get_next_waypoint(void);
    //! Set next waypoint
    void set_next_waypoint(std::weak_ptr<Waypoint> waypoint);
    //! Reset next waypoint
    void reset_next_waypoint(void);
    
    private:
    
    //! the next waypoint
    std::weak_ptr<Waypoint> _next_waypoint;
    //! the list of destinations accessible from the waypoint
    std::vector<WaypointDestination> _destinations;
    //! type identifier to ensure that pointer is valid
    UDINT _type_id = rl6dWAYPOINT_TYPE_ID;
    //! name of the waypoint
    std::string _name;
    //! position of the waypoint
    rl6dPositionType _position;

};

#endif
