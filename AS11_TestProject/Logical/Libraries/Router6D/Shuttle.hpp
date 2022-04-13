#ifndef __RL6D_SHUTTLE_H__
#define __RL6D_SHUTTLE_H__

#include <Router6D.h>
#include <McAcp6D.h>
#include <memory>
#include <vector>
#include <algorithm>
#include <cstring>

#include <Destination.hpp>
#include <Waypoint.hpp>
#include <Path.hpp>
#include <ShuttleIf.hpp>
#include <Logging.hpp>

class Destination; //Forward Declaration
class Waypoint; //Forward Declaration
class ShuttleIf; //Forward Declaration

struct ReadUserDataResult{
    char* value;
    UDINT size;
};

enum set_path_status_enum
{
    set_path_status_valid,
    set_path_status_invalid,
    set_path_status_wait_sem
};

enum routed_move_phase_enum
{
    ROUTED_MOVE_PHASE_EXIT_DEST_START,
    ROUTED_MOVE_PHASE_EXIT_DEST,
    ROUTED_MOVE_PHASE_WAYPOINT_START,
    ROUTED_MOVE_PHASE_WAYPOINT,
    ROUTED_MOVE_PHASE_DEPART_WAYPOINT_START,
    ROUTED_MOVE_PHASE_DEPART_WAYPOINT,
    ROUTED_MOVE_PHASE_ENTER_DEST_START,
    ROUTED_MOVE_PHASE_ENTER_DEST,
    ROUTED_MOVE_PHASE_DEST,
    ROUTED_MOVE_PHASE_ARRIVE
};

enum routed_move_event_enum
{
    ROUTED_MOVE_EVENT_NONE,
    ROUTED_MOVE_EVENT_PATH_START,
    ROUTED_MOVE_EVENT_WAYPOINT
};

struct RoutedMoveEvent{
    routed_move_event_enum event = ROUTED_MOVE_EVENT_NONE;
    std::shared_ptr<Path> path;
    std::weak_ptr<Waypoint> waypoint;
};

/**
 \brief A ShuttleRoutePositionGenerator outputs the positions on the shuttle route
*/
class ShuttleRoutePositionGenerator{
    public:
    //Constructor
    ShuttleRoutePositionGenerator(void);
    ShuttleRoutePositionGenerator(std::shared_ptr<Destination> previous_destination, std::shared_ptr<Destination> next_destination);
    ShuttleRoutePositionGenerator(std::shared_ptr<Destination> previous_destination, std::shared_ptr<Destination> next_destination, std::weak_ptr<Waypoint> route_waypoint);
    //Destructor
    ~ShuttleRoutePositionGenerator(void);
    
    //members

    //attributes
    rl6dPositionType position;
    RoutedMoveEvent event;
    
    //methods
    //! Increment the position along the route, return true if the route position has been incremented
    bool operator++(int);
    //! Update the destination
    void update_destination(std::shared_ptr<Destination> destination);
    void update_destination(std::shared_ptr<Destination> destination, std::weak_ptr<Waypoint> waypoint);
    //! Reset the position generator
    void reset(void);
    //! Get the current phase of the routed move
    routed_move_phase_enum get_phase(void);
    //! Get if the routed move is done
    bool get_done(void);
    
    private:

    //! Routing complete
    bool _done = 0;
    //! Set path, return set path status
    set_path_status_enum _set_path(std::shared_ptr<Path> path);
    //! Status of the exit path
    set_path_status_enum _exit_path_status = set_path_status_invalid;
    //! Status of the departure path
    set_path_status_enum _departure_path_status = set_path_status_invalid;
    //! Status of the entry path
    set_path_status_enum _entry_path_status = set_path_status_invalid;
    //! current waypoint
    std::weak_ptr<Waypoint> _current_waypoint;
    //! The phase of the routed motion
    routed_move_phase_enum _phase = ROUTED_MOVE_PHASE_EXIT_DEST_START;
    //! The previous destination the shuttle is going from
    std::shared_ptr<Destination>  _previous_destination;
    //! The next destination the shuttle is going to
    std::shared_ptr<Destination> _next_destination;
    //! The updated destination the shuttle needs to move to
    std::shared_ptr<Destination> _updated_destination;
    //! Route waypoint to use
    std::weak_ptr<Waypoint> _route_waypoint;
    //! path
    std::shared_ptr<Path> _path;
    //! path positions
    std::vector<rl6dPositionType> _path_positions;
    //! path iterator
    std::vector<rl6dPositionType>::iterator _path_iterator;
    //! Departure path
    std::shared_ptr<Path> _departure_path;

};

/**
 \brief A Shuttle controls an ACOPOS 6D shuttle
*/
class Shuttle : public RouterSemBase, public std::enable_shared_from_this<Shuttle>{
    public:
    //! constructor for the Shuttle
    Shuttle(Mc6DShuttleType shuttle, std::shared_ptr<RouterSem> router_sem, UDINT user_data_size, UINT _shuttle_id);
    
    //! destructor for the Shuttle
    ~Shuttle(void);

    // members
    
    // methods
    //! Get the shuttle type ID
    UDINT get_type_id(void);
    //! Get the shuttle ID
    UINT get_shuttle_id(void);
    //! Get the shuttle reference
    Mc6DShuttleType* get_shuttle_reference(void);
    //! Get the current destination
    std::shared_ptr<Destination> get_current_destination(void);
    //! Get the previous destination
    std::shared_ptr<Destination> get_previous_destination(void);
    //! Clear previous destination
    void clear_previous_destination(void);
    //! Create interface
    std::shared_ptr<ShuttleIf> create_interface(void);
    //! Get existing shared pointer
    std::shared_ptr<Shuttle> get_shared_ptr(void);
    //! Read User Data
    ReadUserDataResult read_user_data(void);
    //! Write User Data
    UDINT write_user_data(char* value, UDINT size);
    //! Cyclic runtime
    void cyclic(void);
    
    private:
    
    enum _state_enum
    {
        STATE_IDLE,
        STATE_ROUTED_MOVE_START,
        STATE_ROUTED_MOVE,
        STATE_ROUTED_MOVE_NEXT,
        STATE_DIRECT_MOVE,
    };
    
    //! Update shuttle interfaces
    bool _update_interfaces(void);
    //! Log events generated by the position generator
    void _log_event(RoutedMoveEvent event);
    //! type identifier to ensure that pointer is valid
    UDINT _type_id = rl6dSHUTTLE_TYPE_ID;
    //! 6D Shuttle ID
    UINT _shuttle_id;
    //! current interface
    std::shared_ptr<ShuttleIf> _current_interface;
    //! the list of shuttle interfaces that are part of the shuttle
    std::vector<std::shared_ptr<ShuttleIf>> _interfaces;
    //! The previous destination the shuttle is going from
    std::shared_ptr<Destination>  _previous_destination;
    //! The next destination the shuttle is going to
    std::shared_ptr<Destination> _next_destination;
    //! current state of cyclic execution
    _state_enum _state = STATE_IDLE;
    //! shuttle being managed
    Mc6DShuttleType _shuttle;
    //! move shuttle in plane
    struct MC_BR_MoveInPlane_Acp6D _move_in_plane;
    //! User Data
    std::unique_ptr<char[]> _user_data;
    //! User Data Size
    UDINT _user_data_size;
    //! Route position generator
    ShuttleRoutePositionGenerator _route_position_generator;

};    

#endif
