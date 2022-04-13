#include "Shuttle.hpp"
#include "CheckRef.hpp"

extern "C" _BUR_PUBLIC signed long rl6dShCopyUserDataSync(struct rl6dShuttleType* RouterShuttle, enum rl6dShCopyUserDataModeEnum Mode, unsigned long DataAddress, unsigned long DataSize){

    Shuttle* s;
    std::shared_ptr<Shuttle> shuttle;
    DINT error;
    
    error = check_shuttle_reference(RouterShuttle);
    if(error == ERR_OK){
        if(DataAddress == 0){
            error = rl6dERR_USER_DATA_NULL_POINTER;
        }
    }
    if(error == ERR_OK){
        if(DataSize == 0){
            error = rl6dERR_USER_DATA_SIZE_ZERO;
        }
    }
    if(error == ERR_OK){
        s = reinterpret_cast<Shuttle*>(RouterShuttle->Ident);
        shuttle = s->get_shared_ptr();
        ReadUserDataResult result = shuttle->read_user_data();
        if(result.size == DataSize){
            if(Mode == rl6dSH_COPY_USER_DATA_READ){
                std::memcpy(reinterpret_cast<char*>(DataAddress), result.value, DataSize);
            }
            else if(Mode == rl6dSH_COPY_USER_DATA_WRITE){
                shuttle->write_user_data(reinterpret_cast<char*>(DataAddress), DataSize);
            }     
        }
        else{
            error = rl6dERR_USER_DATA_SIZE_MISMATCH;
        }
    }
    
    return error;
    
}
