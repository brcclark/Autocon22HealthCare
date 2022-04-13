#ifndef __RL6D_DESTINATION_IF_H__
#define __RL6D_DESTINATION_IF_H__

#include <memory>

#include <Router6D.h>
#include <Shuttle.hpp>

class Shuttle; //Forward Declaration

/**
 \brief A DestinationIf is an interface that can be monitored for shuttles
*/
class DestinationIf: public std::enable_shared_from_this<DestinationIf>{
    public:
    //! constructor for the DestinationIf
    DestinationIf(void);
    
    //! destructor for the DestinationIf
    ~DestinationIf(void);

    // members
    
    // methods
    //! Get the DestinationIf type ID
    UDINT get_type_id(void);
    //! Get existing shared pointer
    std::shared_ptr<DestinationIf> get_shared_ptr(void);
    //! Set shuttle status
    void set_shuttle_present(std::weak_ptr<Shuttle> shuttle);
    //! Get shuttle status
    bool shuttle_is_present(void);
    //! Get currently present shuttle
    std::weak_ptr<Shuttle> get_shuttle_present(void);
    //! Acknowledge shuttle present
    void acknowledge_shuttle(void);
    //! Set interface for deletion
    void set_delete(void);
    //! Check interface for deletion
    bool get_delete(void);
    
    private:
    
    //! type identifier to ensure that pointer is valid
    UDINT _type_id = rl6dDESTINATION_IF_TYPE_ID;
    //! Destination status of shuttle present
    bool _shuttle_present = 0;
    //! Shuttle present at Destination
    std::weak_ptr<Shuttle> _shuttle;
    //! Interface can be deleted
    bool _delete = 0;

};

#endif
