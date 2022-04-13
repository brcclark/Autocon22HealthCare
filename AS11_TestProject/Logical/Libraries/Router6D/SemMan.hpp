#ifndef __RL6D_SEMMAN_H__
#define __RL6D_SEMMAN_H__

#include <memory>
#include <vector>
#include <algorithm>
#include <cstring>

#include <Router6D.h>

/**
 \brief A RouterSem contains a semaphore
*/
class RouterSem : public std::enable_shared_from_this<RouterSem>{
    public:
    //! constructor for the RouterSem
    RouterSem(UDINT semaphore);
    
    //! destructor for the RouterSem
    ~RouterSem(void);  

    // members

    // methods
    //! Get the RouterSem Type ID
    UDINT get_type_id(void);
    //! Get the semaphore ident
    UDINT get_semaphore_ident(void);
    //! Delete this semaphore
    void set_delete(void);
    //! Check if this semaphore is to be deleted
    bool get_delete(void);
    //! Acquire the semaphore
    bool sem_acquire(void);
    //! Release the semaphore
    void sem_release(void);
    //! Get existing shared pointer
    std::shared_ptr<RouterSem> get_shared_ptr(void);
    
    private:

    //! type identifier to ensure that pointer is valid
    UDINT _type_id = rl6dROUTER_SEM_TYPE_ID;
    //! Semaphore to access
    UDINT _sem;
    //! Flag if this semaphore is to be deleted
    bool _delete = 0;
    
};

/**
 \brief A RouterSemBase gives a derived class the public methods and attributes to interface with the contained RouterSem
*/
class RouterSemBase{
    public:
    //! constructor for the RouterSem
    RouterSemBase(std::shared_ptr<RouterSem>);
    
    //! destructor for the RouterSem
    ~RouterSemBase(void);  

    // members

    // methods
    //! Acquire the semaphore
    bool sem_acquire(void);
    //! Release the semaphore
    void sem_release(void);
    
    protected:

    //! Router semaphore
    std::shared_ptr<RouterSem> _router_semaphore;
    
};

struct SemManCmd{
    bool create_sem = 0;
    bool delete_all = 0;
};

struct SemManStatus{
    bool cmd_ready = 0;
    bool done = 0;
    bool error = 0;
    DINT error_id = 0;
};

/**
 \brief A SemMan mangages semaphores
*/
class SemMan{
    public:
    //! constructor for the SemMan
    SemMan(void);
    
    //! destructor for the SemMan
    ~SemMan(void);  

    // members

    // methods
    //! Get the SemMan Type ID
    UDINT get_type_id(void);
    //! Create a new semaphore
    void set_create_sem(void);
    //! Reset creating a new semaphore
    void reset_create_sem(void);
    //! Return the newly created semaphore
    std::shared_ptr<RouterSem> get_created_sem(void);
    //! Get Done status
    bool get_done(void);
    //! Get Command Ready status
    bool get_cmd_ready(void);
    //! Get Error status
    bool get_error(void);
    //! Get Error ID
    DINT get_error_id(void);
    //! Delete a semaphore
    void delete_sem(UDINT);
    //! Acquire the semaphore to use other methods
    bool sem_acquire(void);
    //! Release the semaphore
    void sem_release(void);
    //! Get if the initialization is done
    bool get_initialized(void);
    //! Cyclic execution
    void cyclic(void);
    
    private:
    
    enum _state_enum
    {
        STATE_START,
        STATE_INIT,
        STATE_IDLE,
        STATE_CREATE_SEM,
        STATE_CREATE_SEM_DONE,
        STATE_DELETE_SEM,
        STATE_DELETE_ALL_SEM,
        STATE_DELETE_ALL_SEM_NEXT,
        STATE_DELETE_ALL_SEM_LAST,
        STATE_DELETE_ALL_SEM_DONE
    };

    //! type identifier to ensure that pointer is valid
    UDINT _type_id = rl6dSEM_MAN_TYPE_ID;
    //! state
    _state_enum _state = STATE_START;
    //! Semaphore to access manager
    UDINT _sem;
    //! Initialization is complete
    bool _init_done = 0;
    //! Commands
    SemManCmd _cmd;
    //! Status
    SemManStatus _status;
    //! Created semaphore
    std::shared_ptr<RouterSem> _created_semaphore;
    //! the list of semaphores being managed
    std::vector<std::shared_ptr<RouterSem>> _semaphores;
    //! Redo so that the iterator is not needed
    std::vector<std::shared_ptr<RouterSem>>::iterator _sem_iterator;
    //! semaphore generator
    struct SemCreate _create_semaphore;
    //! semaphore destructor
    struct SemDelete _delete_semaphore;
    
};

/**
 \brief A SemManBase gives a derived class the public methods and attributes to interface with the contained SemMan
*/
class SemManBase{
    public:
    //! constructor for the SemManBase
    SemManBase(void);
    
    //! destructor for the SemManBase
    ~SemManBase(void);  

    // members

    // methods
    //! Create a new semaphore
    void set_create_sem(void);
    //! Reset creating a new semaphore
    void reset_create_sem(void);
    //! Return the newly created semaphore
    std::shared_ptr<RouterSem> get_created_sem(void);
    //! Get Done status
    bool get_sem_done(void);
    //! Get Command Ready status
    bool get_sem_cmd_ready(void);
    //! Get Error status
    bool get_sem_error(void);
    //! Get Error ID
    DINT get_sem_error_id(void);
    //! Acquire the semaphore to use other methods
    bool sem_acquire(void);
    //! Release the semaphore
    void sem_release(void);
    
    protected:
   
    //! SemMan
    SemMan _semaphore_manager;
    
};

#endif

