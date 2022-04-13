#ifndef __RL6D_SHUTTLE_IF_H__
#define __RL6D_SHUTTLE_IF_H__

#include <Router6D.h>

#include <Destination.hpp>

class Destination; //Forward Declaration

/**
 \brief A ShuttleIf is an interface that can be used to command shuttles
*/
class ShuttleIf : public std::enable_shared_from_this<ShuttleIf>{
    public:
    //! constructor for the ShuttleIf
    ShuttleIf(void);
    
    //! destructor for the ShuttleIf
    ~ShuttleIf(void);

    // members
    
    // methods
    //! Get the ShuttleIf type ID
    UDINT get_type_id(void);
    //! Issue move destination command
    void set_destination(std::shared_ptr<Destination> destination, REAL velocity, REAL acceleration);
    //! Check for move destination command
    bool get_destination_command(void);
    //! Issue move destination direct command
    void set_destination_direct(std::shared_ptr<Destination> destination, REAL velocity, REAL acceleration);
    //! Check for move destination direct command
    bool get_destination_direct_command(void);
    //! Get the destination commanded
    std::shared_ptr<Destination> get_destination(void);
    //! Set aborted status
    void set_aborted(void);
    //! Get aborted status
    bool get_aborted(void);
    //! Set error
    void set_error(DINT error);
    //! Get error status
    bool get_error(void);
    //! Get error id
    DINT get_error_id(void);
    //! Set move complete
    void set_move_done(void);
    //! Get move complete
    bool get_move_done(void);
    //! Set move active
    void set_move_active(void);
    //! Reset move active
    void reset_move_active(void);
    //! Get move active
    bool get_move_active(void);
    //! Set delete
    void set_delete(void);
    //! Get delete
    bool get_delete(void);
    //! Get velocity
    REAL get_velocity(void);
    //! Get acceleration
    REAL get_acceleration(void);
    //! Get existing shared pointer
    std::shared_ptr<ShuttleIf> get_shared_ptr(void);    
    
    private:
    
    //! type identifier to ensure that pointer is valid
    UDINT _type_id = rl6dSHUTTLE_IF_TYPE_ID;
    //! Command Aborted
    bool _aborted = 0;
    //! Error
    bool _error = 0;
    //! Error ID
    DINT _error_id = 0;
    //! Move Destination
    bool _move_destination = 0;
    //! Move Destination Direct
    bool _move_destination_direct = 0;
    //! Destination
    std::shared_ptr<Destination> _destination;
    //! Velocity
    REAL _velocity;
    //! Acceleration
    REAL _acceleration;
    //! Move Done
    bool _move_done = 0;
    //! Move active
    bool _move_active = 0;
    //! Delete interface
    bool _delete = 0;

};

#endif
