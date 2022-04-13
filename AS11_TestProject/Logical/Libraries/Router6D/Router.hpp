#ifndef __RL6D_ROUTER_H__
#define __RL6D_ROUTER_H__

#include <memory>
#include <algorithm>
#include <cstring>

#include <CheckRef.hpp>
#include <Path.hpp>
#include <Waypoint.hpp>
#include <Shuttle.hpp>
#include <SemMan.hpp>
#include <Logging.hpp>

struct RouterCmd{
    bool add_sh_from_asm = 0;
    bool recover_shuttles = 0;
    rl6dRecoverModeEnum recover_shuttles_mode = rl6dRECOVER_MODE_BASIC;
    bool error_reset = 0;
};

struct RouterStatus{
    bool cmd_ready = 0;
    bool done = 0;
    bool error = 0;
    DINT error_id = 0;
};

struct ShDestAssignment{
    std::shared_ptr<Shuttle> shuttle;
    std::shared_ptr<Destination> destination;
};

/**
 \brief A Router manages shuttles to destinations
*/
class Router : public SemManBase{
    public:
    //! constructor for the Router
    Router(Mc6DAssemblyType* assembly, UDINT user_data_size);
    
    //! destructor for the Router
    ~Router(void);  

    // members

    // methods
    //! Get the Router Type ID
    UDINT get_type_id(void);
    //! Create a new path
    std::shared_ptr<Path> create_path(std::string name, std::shared_ptr<RouterSem> router_sem);
    //! Delete a path
    void delete_path(std::shared_ptr<Path>);
    //! Get a path by name
    std::shared_ptr<Path> get_path_by_name(std::string name);
    //! Create a destination
    std::shared_ptr<Destination> create_destination(std::string name, std::shared_ptr<RouterSem> router_sem, rl6dPositionType position, std::shared_ptr<Path> path_in, std::shared_ptr<Path> path_out);
    //! Create a waypoint
    std::shared_ptr<Waypoint> create_waypoint(std::string name, std::shared_ptr<RouterSem> router_sem, rl6dPositionType position);
    //! Add a shuttle
    std::shared_ptr<Shuttle> add_shuttle(Mc6DShuttleType shuttle, std::shared_ptr<RouterSem> router_semaphore, UINT shuttle_id);
    //! Remove a shuttle
    void delete_shuttle(Mc6DShuttleType shuttle);
    //! Acquire path semaphore
    bool acquire_path_semaphore(void);
    //! Release path semaphore
    void release_path_semaphore(void);
    //! Acquire waypoint semaphore
    bool acquire_waypoint_semaphore(void);
    //! Release waypoint semaphore
    void release_waypoint_semaphore(void);
    //! Acquire shuttle semaphore
    bool acquire_shuttle_semaphore(void);
    //! Release shuttle semaphore
    void release_shuttle_semaphore(void);
    //! Acquire destination semaphore
    bool acquire_destination_semaphore(void);
    //! Release destination semaphore
    void release_destination_semaphore(void);
    //! Acquire command semaphore
    bool acquire_cmd_semaphore(void);
    //! Release command semaphore
    void release_cmd_semaphore(void);
    //! Set add shuttles from assembly
    void set_add_sh_from_asm();
    //! Reset add shuttles from assembly
    void reset_add_sh_from_asm(void);
    //! Set recover shuttles
    void set_recover_shuttles(rl6dRecoverModeEnum mode, REAL velocity, REAL acceleration);
    //! Reset recover shuttles
    void reset_recover_shuttles(void);
    //! Reset router errors
    void set_error_reset(void);
    //! get the number of shuttles found on the assembly
    UINT get_num_asm_sh(void);
    //! Get ready status
    bool get_cmd_ready(void);
    //! Get Done status
    bool get_done(void);
    //! Get error status
    bool get_error(void);
    //! Get error ID
    DINT get_error_id(void);
    //! Get initialized status
    bool get_initialized(void);
    //! Cyclic execution
    void cyclic(void);
    
    private:
    
    enum _state_enum
    {
        STATE_INIT,
        STATE_EVENT_LOG_CREATE,
        STATE_WAIT_SEM,
        STATE_CREATE_PATH_SEM,
        STATE_CREATE_WAYPOINT_SEM_READY,
        STATE_CREATE_WAYPOINT_SEM,
        STATE_CREATE_SHUTTLE_SEM_READY,
        STATE_CREATE_SHUTTLE_SEM,
        STATE_CREATE_DESTINATION_SEM_READY,
        STATE_CREATE_DESTINATION_SEM,
        STATE_CREATE_CMD_SEM_READY,
        STATE_CREATE_CMD_SEM,
        STATE_ACTIVE,
        STATE_ADD_SH_ASM,
        STATE_ADD_SH_ASM_SEM,
        STATE_ADD_SH_ASM_NEXT,
        STATE_ADD_SH_ASM_DONE,
        STATE_RECOVER_SHUTTLES_GET_DEST,
        STATE_RECOVER_SHUTTLES_GET_POS,
        STATE_RECOVER_SHUTTLES_ASSIGN,
        STATE_RECOVER_SHUTTLES,
        STATE_RECOVER_DESTINATION,
        STATE_RECOVER_DESTINATION_WAIT,
        STATE_RECOVER_SHUTTLES_DONE,
        STATE_ERROR
    };

    //! type identifier to ensure that pointer is valid
    UDINT _type_id = rl6dROUTER_TYPE_ID;
    //! state
    _state_enum _state = STATE_INIT;
    //! the list of paths that are part of the router
    std::vector<std::shared_ptr<Path>> _paths;
    //! the list of waypoints that are part of the router
    std::vector<std::shared_ptr<Waypoint>> _waypoints;
    //! the list of destinations that are part of the router
    std::vector<std::shared_ptr<Destination>> _destinations;
    //! the list of destinations to recover to
    std::vector<std::shared_ptr<Destination>> _recovery_destinations;
    //! current detination recovered shuttles are being assigned
    std::shared_ptr<Destination> _current_recovery_destination;
    //! current positions recovered shuttles are being assigned
    std::vector<rl6dPositionType> _current_recovery_positions;
    //! the list of shuttles that are part of the router
    std::vector<std::shared_ptr<Shuttle>> _shuttles;
    //! the list of shuttles to recover
    std::vector<std::shared_ptr<Shuttle>> _recovery_shuttles;
    //! current shuttle being recovered
    std::shared_ptr<Shuttle> _current_recovery_shuttle;
    //! current recovery index
    unsigned int _current_recovery_index = 0;
    //! current recovery position
    rl6dPositionType _current_recovery_position;
    //! the list of final recovery moves to make
    std::vector<ShDestAssignment> _shuttle_destination_assignment;
    //! current recovery shuttle interface
    std::shared_ptr<ShuttleIf> _current_recovery_shuttle_interface;
    //! the list of recovery shuttle interfaces
    std::vector<std::shared_ptr<ShuttleIf>> _shuttle_recovery_interfaces;
    //! Path semaphore
    std::shared_ptr<RouterSem> _path_semaphore;
    //! Waypoint semaphore
    std::shared_ptr<RouterSem> _waypoint_semaphore;
    //! Destination semaphore
    std::shared_ptr<RouterSem> _destination_semaphore;
    //! Shuttle semaphore
    std::shared_ptr<RouterSem> _shuttle_semaphore;
    //! Command semaphore
    std::shared_ptr<RouterSem> _command_semaphore;
    //! Router commands
    RouterCmd _cmd;
    //! Router status
    RouterStatus _status;
    //! Get shuttles from assembly
    struct MC_BR_AsmGetShuttle_Acp6D _asm_get_shuttle;
    //! Recover shuttles on assembly
    struct MC_BR_MoveInPlaneAsync_Acp6D _move_in_plane_async;
    //! Recorded number of shuttles remaining from assembly
    UDINT _old_remaining_count = 0;
    //! Recorded number of shuttles found on the assembly
    UDINT _asm_shuttles = 0;
    //! Newly created semaphore
    std::shared_ptr<RouterSem> _created_semaphore;
    //! Flag that initial semaphore creation is complete
    bool _router_sem_complete = 0;
    //! User data size for all shuttles
    UDINT _user_data_size;
    //! Initialization is complete
    bool _init_done = 0;
    //! Error
    bool _error = 0;
    //! Error ID
    DINT _error_id = ERR_OK;
    //EventLog Create
    struct ArEventLogCreate _ar_event_log_create;
    
};

#endif

