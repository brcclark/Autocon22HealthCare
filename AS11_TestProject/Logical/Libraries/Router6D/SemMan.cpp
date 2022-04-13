#include "SemMan.hpp"

RouterSem::RouterSem(UDINT semaphore){
    this->_sem = semaphore;
}

//destructor
RouterSem::~RouterSem(){
    this->_type_id = rl6dNO_TYPE_ID;
}

UDINT RouterSem::get_type_id(void){
    return this->_type_id;
}
UDINT RouterSem::get_semaphore_ident(void){
    return this->_sem;
}
void RouterSem::set_delete(void){
    this->_delete = 1;
}
bool RouterSem::get_delete(void){
    return this->_delete;
}
bool RouterSem::sem_acquire(void){
    return SemAcquire(this->_sem) == ERR_OK;
}
void RouterSem::sem_release(void){
    SemRelease(this->_sem);
}
std::shared_ptr<RouterSem> RouterSem::get_shared_ptr(){
    return shared_from_this();
}

RouterSemBase::RouterSemBase(std::shared_ptr<RouterSem> router_sem){
    this->_router_semaphore = router_sem;
}

//destructor
RouterSemBase::~RouterSemBase(){
    this->_router_semaphore->set_delete();
}

bool RouterSemBase::sem_acquire(void){
    return this->_router_semaphore->sem_acquire();
}
void RouterSemBase::sem_release(void){
    this->_router_semaphore->sem_release();
}

SemMan::SemMan(){
    std::memset(&this->_create_semaphore, 0, sizeof(this->_create_semaphore));
    std::memset(&this->_delete_semaphore, 0, sizeof(this->_delete_semaphore));
}
    
//destructor
SemMan::~SemMan(){
    this->_type_id = rl6dNO_TYPE_ID;
}

// methods
UDINT SemMan::get_type_id(){
    return this->_type_id;
}
void SemMan::set_create_sem(){
    this->_cmd.create_sem = 1;
}
void SemMan::reset_create_sem(){
    this->_cmd.create_sem = 0;
}
std::shared_ptr<RouterSem> SemMan::get_created_sem(){
    return this->_created_semaphore;
}
bool SemMan::get_cmd_ready(){
    return this->_status.cmd_ready;
}
bool SemMan::get_done(){
    return this->_status.done;
}
bool SemMan::sem_acquire(){
    return SemAcquire(this->_sem) == ERR_OK;
}
void SemMan::sem_release(){
    SemRelease(this->_sem);
}
bool SemMan::get_initialized(){
    return this->_init_done;
}
void SemMan::cyclic(){

    switch(this->_state){
        
        case STATE_START:
            
            this->_create_semaphore.enable = 0;
            if(this->_create_semaphore.status == ERR_FUB_ENABLE_FALSE){
                this->_state = STATE_INIT;
            }
            
            break;
        
        case STATE_INIT:
            
            this->_create_semaphore.maxCount = 1;
            this->_create_semaphore.initCount = 1;
            this->_create_semaphore.enable = 1;
            if(this->_create_semaphore.status == ERR_OK){
                this->_sem = this->_create_semaphore.ident;
                this->_create_semaphore.enable = 0;
                this->_status.cmd_ready = 1;
                this->_init_done = 1;
                this->_state = STATE_IDLE;
            }
            else if(!((this->_create_semaphore.status == ERR_FUB_ENABLE_FALSE)||(this->_create_semaphore.status == ERR_FUB_BUSY))){
                this->_create_semaphore.enable = 0;
            }
            
            break;
        
        case STATE_IDLE:
            
            if(this->_cmd.delete_all){
                this->_status.cmd_ready = 0;
                // start deleting all semaphores to prepare for destruction of object
                this->_sem_iterator = this->_semaphores.begin();
                this->_state = STATE_DELETE_ALL_SEM_NEXT;
            }
            else if(this->_cmd.create_sem){
                this->_status.cmd_ready = 0;
                this->_state = STATE_CREATE_SEM;
            }
            else{
                auto it = std::find_if(this->_semaphores.begin(), this->_semaphores.end(), [](std::shared_ptr<RouterSem> i){return i->get_delete();});
                if(it != this->_semaphores.end()){
                    this->_status.cmd_ready = 0;
                    this->_sem_iterator = it;
                    std::shared_ptr<RouterSem> sem = *it;
                    this->_delete_semaphore.ident = sem->get_semaphore_ident();
                    this->_delete_semaphore.enable = 1;
                    this->_state = STATE_DELETE_SEM;
                }
            }
        
            break;
        
        case STATE_CREATE_SEM:
            
            this->_create_semaphore.enable = 1;
            
            if(this->_create_semaphore.status == ERR_OK){
                std::shared_ptr<RouterSem> new_sem (new RouterSem(this->_create_semaphore.ident));
                this->_created_semaphore = new_sem;
                this->_semaphores.push_back(new_sem);
                this->_create_semaphore.enable = 0;
                this->_status.done = 1;
                this->_state = STATE_CREATE_SEM_DONE;
            }
            else if(!((this->_create_semaphore.status == ERR_FUB_ENABLE_FALSE)||(this->_create_semaphore.status == ERR_FUB_BUSY))){
                this->_create_semaphore.enable = 0;
            }
        
            break;
        
        case STATE_CREATE_SEM_DONE:
            
            if(!this->_cmd.create_sem){
                this->_status.done = 0;
                this->_created_semaphore.reset();
                this->_status.cmd_ready = 1;
                this->_state = STATE_IDLE;
            }
                
            break;
        
        case STATE_DELETE_SEM:
            
            if(!((this->_delete_semaphore.status == ERR_FUB_ENABLE_FALSE)||(this->_delete_semaphore.status == ERR_FUB_BUSY))){
                this->_delete_semaphore.enable = 0;
                this->_semaphores.erase(this->_sem_iterator);
                this->_status.cmd_ready = 1;
                this->_state = STATE_IDLE;
            }
        
            break;
                
        case STATE_DELETE_ALL_SEM:
            
            if(!((this->_delete_semaphore.status == ERR_FUB_ENABLE_FALSE)||(this->_delete_semaphore.status == ERR_FUB_BUSY))){
                this->_delete_semaphore.enable = 0;
                this->_sem_iterator++;
                this->_state = STATE_DELETE_ALL_SEM_NEXT;
            }
            
            break;
        
        case STATE_DELETE_ALL_SEM_NEXT:
            
            if(this->_sem_iterator < this->_semaphores.end()){
                std::shared_ptr<RouterSem> sem = *this->_sem_iterator;
                this->_delete_semaphore.ident = sem->get_semaphore_ident();
                this->_delete_semaphore.enable = 1;
                this->_state = STATE_DELETE_ALL_SEM;
            }
            else{
                this->_delete_semaphore.ident = this->_sem;
                this->_delete_semaphore.enable = 1;
                this->_state = STATE_DELETE_ALL_SEM_LAST;
            }
            
            break;
        
        case STATE_DELETE_ALL_SEM_LAST:
            if(!((this->_delete_semaphore.status == ERR_FUB_ENABLE_FALSE)||(this->_delete_semaphore.status == ERR_FUB_BUSY))){
                this->_delete_semaphore.enable = 0;
                this->_semaphores.clear();
                this->_status.done = 1;
                this->_state = STATE_DELETE_ALL_SEM_DONE;
            }
            break;
        
        case STATE_DELETE_ALL_SEM_DONE:
            //At this point, the object must be destroyed
            break;
        
    }
    
    SemCreate(&this->_create_semaphore);
    SemDelete(&this->_delete_semaphore);
        
}

SemManBase::SemManBase() = default;
    
//destructor
SemManBase::~SemManBase() = default;

// methods
void SemManBase::set_create_sem(){
    this->_semaphore_manager.set_create_sem();
}
void SemManBase::reset_create_sem(){
    this->_semaphore_manager.reset_create_sem();
}
std::shared_ptr<RouterSem> SemManBase::get_created_sem(){
    return this->_semaphore_manager.get_created_sem();
}
bool SemManBase::get_sem_cmd_ready(){
    return this->_semaphore_manager.get_cmd_ready();
}
bool SemManBase::get_sem_done(){
    return this->_semaphore_manager.get_done();
}
bool SemManBase::sem_acquire(){
    return this->_semaphore_manager.sem_acquire();
}
void SemManBase::sem_release(){
    this->_semaphore_manager.sem_release();
}
