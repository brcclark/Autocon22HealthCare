#ifndef __RL6D_PATH_H__
#define __RL6D_PATH_H__

#include <string>
#include <vector>
#include <Router6D.h>
#include <SemMan.hpp>

/**
 \brief A Path is a vector of positions that the shuttle can travel along bust cannot leave
*/
class Path : public RouterSemBase, public std::enable_shared_from_this<Path>{
    public:
    //! constructor for the Path
    Path(std::string name, std::shared_ptr<RouterSem> router_sem);
    
    //! destructor for the Path
    ~Path(void);

    // members
    
    // methods
    //! Get the path type ID
    UDINT get_type_id(void);
    //! Get the path name
    std::string get_name(void);
    //! Add a position to the vector
    void add_position(rl6dPositionType position);
    //! Get position vector
    std::vector<rl6dPositionType> get_positions(void);
    //! Get existing shared pointer
    std::shared_ptr<Path> get_shared_ptr(void);
    
    private:
    
    //! the list of positions that are part of the path
    std::vector<rl6dPositionType> _positions;
    //! type identifier to ensure that pointer is valid
    UDINT _type_id = rl6dPATH_TYPE_ID;
    //! name of the path
    std::string _name;

};

#endif
