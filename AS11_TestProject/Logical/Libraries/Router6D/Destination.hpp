#ifndef __RL6D_DESTINATION_H__
#define __RL6D_DESTINATION_H__

#include <memory>
#include <algorithm>
#include <string>
#include <vector>
#include <Router6D.h>

#include <Shuttle.hpp>
#include <Path.hpp>
#include <DestinationIf.hpp>
#include <Waypoint.hpp>

//Forward Declarations
class DestinationIf;
class Shuttle; 
class Waypoint;

/**
 \brief A Destination is a position that a shuttle can travel to and can notify of its arrival
*/
class Destination: public RouterSemBase, public std::enable_shared_from_this<Destination>{
    public:
    //! constructor for the Destination
    Destination(std::string name, std::shared_ptr<RouterSem> router_sem);
    Destination(std::string name, std::shared_ptr<RouterSem> router_sem, rl6dPositionType position);
    Destination(std::string name, std::shared_ptr<RouterSem> router_sem, rl6dPositionType position, std::shared_ptr<Path> path_in, std::shared_ptr<Path> path_out);
    
    //! destructor for the Destination
    ~Destination(void);

    // members
    
    // methods
    //! Get the destination type ID
    UDINT get_type_id(void);
    //! Get the destination name
    std::string get_name(void);
    //! Get the destination position
    rl6dPositionType get_position(void);
    //! Get the recovery positions
    std::vector<rl6dPositionType> get_recovery_positions(void);
    //! Set the destination position
    void set_position(rl6dPositionType position);
    //! Get the destination path in
    std::shared_ptr<Path> get_path_in(void);
    //! Set the destination path in
    void set_path_in(std::shared_ptr<Path> path);
    //! Clear the destination path in
    void clear_path_in(void);
    //! Get the destination path out
    std::shared_ptr<Path> get_path_out(void);
    //! Set the destination path out
    void set_path_out(std::shared_ptr<Path> path);
    //! Clear the destination path out
    void clear_path_out(void);
    //! Add recovery position
    void add_recovery_position(rl6dPositionType position);
    //! Create interface
    std::shared_ptr<DestinationIf> create_interface(void);
    //! Set waypoint
    void set_waypoint(std::weak_ptr<Waypoint> waypoint);
    //! Get waypoint
    std::weak_ptr<Waypoint> get_waypoint(void);
    //! Reset waypoint
    void reset_waypoint(void);
    //! Set shuttle present
    void set_shuttle_present(std::weak_ptr<Shuttle> shuttle);
    //! Get existing shared pointer
    std::shared_ptr<Destination> get_shared_ptr(void);
    //! Cyclic runtime
    void cyclic(void);
    
    private:
    
    //! destination position
    rl6dPositionType _position;
    //! type identifier to ensure that pointer is valid
    UDINT _type_id = rl6dDESTINATION_TYPE_ID;
    //! name of the destination
    std::string _name;
    //! path in
    std::shared_ptr<Path> _path_in;
    //! path out
    std::shared_ptr<Path> _path_out;
    //! default waypoint
    std::weak_ptr<Waypoint> _waypoint;
    //! recovery points
    std::vector<rl6dPositionType> _recovery_positions;
    //! the list of destination interfaces that are part of the destination
    std::vector<std::shared_ptr<DestinationIf>> _interfaces;
    //! shuttle present at destination
    std::weak_ptr<Shuttle> _shuttle;
    //! shuttle present status
    bool _shuttle_present = 0;

};

#endif
