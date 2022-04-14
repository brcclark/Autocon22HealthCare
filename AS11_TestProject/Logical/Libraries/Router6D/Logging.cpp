#include "logging.hpp"

plcstring router_6d_log_name [] = "rl6dLog";

UDINT rl6d_log_buffer_min_size = 40;
UDINT rl6d_log_buffer_size_udint = 5;

bool log_routed_move_start(UINT shuttle, std::string destination){

    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    UDINT buffer_size = destination.length() + 1;
    buffer_size += rl6d_log_buffer_min_size + rl6d_log_buffer_size_udint;
    char buffer [buffer_size];
    status = ArEventLogAddDataInit(reinterpret_cast<UDINT>(buffer), buffer_size, arEVENTLOG_ADDFORMAT_CODED);
    if(status){
        status = ArEventLogAddDataUdint(reinterpret_cast<UDINT>(buffer), buffer_size, shuttle);
    }
    if(status){
        status = ArEventLogAddDataString(reinterpret_cast<UDINT>(buffer), buffer_size, reinterpret_cast<UDINT>(destination.c_str()));
    }
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    if(status){
        ArEventLogGetIdent(&get_ident);
        get_ident.Execute = 1;
        ArEventLogGetIdent(&get_ident);
        status = get_ident.Done;
        if(!status){
            get_ident.Execute = 0;
            ArEventLogGetIdent(&get_ident);
        }
    }
    
    std::memset(&write, 0, sizeof(write));
    
    std::string object = "RouterShuttle";
    object.append(std::to_string(shuttle));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), object.c_str());
    write.EventID = rl6dINFO_ROUTED_MOVE_START;
    write.AddData = reinterpret_cast<UDINT>(buffer);
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;
    
}

bool log_direct_move_start(UINT shuttle, std::string destination){

    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    UDINT buffer_size = destination.length() + 1;
    buffer_size += rl6d_log_buffer_min_size + rl6d_log_buffer_size_udint;
    char buffer [buffer_size];
    status = ArEventLogAddDataInit(reinterpret_cast<UDINT>(buffer), buffer_size, arEVENTLOG_ADDFORMAT_CODED);
    if(status){
        status = ArEventLogAddDataUdint(reinterpret_cast<UDINT>(buffer), buffer_size, shuttle);
    }
    if(status){
        status = ArEventLogAddDataString(reinterpret_cast<UDINT>(buffer), buffer_size, reinterpret_cast<UDINT>(destination.c_str()));
    }
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    if(status){
        ArEventLogGetIdent(&get_ident);
        get_ident.Execute = 1;
        ArEventLogGetIdent(&get_ident);
        status = get_ident.Done;
        if(!status){
            get_ident.Execute = 0;
            ArEventLogGetIdent(&get_ident);
        }
    }
    
    std::memset(&write, 0, sizeof(write));
    
    std::string object = "RouterShuttle";
    object.append(std::to_string(shuttle));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), object.c_str());
    write.EventID = rl6dINFO_DIRECT_MOVE_START;
    write.AddData = reinterpret_cast<UDINT>(buffer);
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;
    
}

bool log_destination_arrival(UINT shuttle, std::string destination){

    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    UDINT buffer_size = destination.length() + 1;
    buffer_size += rl6d_log_buffer_min_size + rl6d_log_buffer_size_udint;
    char buffer [buffer_size];
    status = ArEventLogAddDataInit(reinterpret_cast<UDINT>(buffer), buffer_size, arEVENTLOG_ADDFORMAT_CODED);
    if(status){
        status = ArEventLogAddDataUdint(reinterpret_cast<UDINT>(buffer), buffer_size, shuttle);
    }
    if(status){
        status = ArEventLogAddDataString(reinterpret_cast<UDINT>(buffer), buffer_size, reinterpret_cast<UDINT>(destination.c_str()));
    }
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    if(status){
        ArEventLogGetIdent(&get_ident);
        get_ident.Execute = 1;
        ArEventLogGetIdent(&get_ident);
        status = get_ident.Done;
        if(!status){
            get_ident.Execute = 0;
            ArEventLogGetIdent(&get_ident);
        }
    }
    
    std::memset(&write, 0, sizeof(write));
    
    std::string object = "RouterShuttle";
    object.append(std::to_string(shuttle));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), object.c_str());
    write.EventID = rl6dINFO_DESTINATION_ARRIVE;
    write.AddData = reinterpret_cast<UDINT>(buffer);
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;
    
}

bool log_destination_update(UINT shuttle, std::string destination){

    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    UDINT buffer_size = destination.length() + 1;
    buffer_size += rl6d_log_buffer_min_size + rl6d_log_buffer_size_udint;
    char buffer [buffer_size];
    status = ArEventLogAddDataInit(reinterpret_cast<UDINT>(buffer), buffer_size, arEVENTLOG_ADDFORMAT_CODED);
    if(status){
        status = ArEventLogAddDataUdint(reinterpret_cast<UDINT>(buffer), buffer_size, shuttle);
    }
    if(status){
        status = ArEventLogAddDataString(reinterpret_cast<UDINT>(buffer), buffer_size, reinterpret_cast<UDINT>(destination.c_str()));
    }
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    if(status){
        ArEventLogGetIdent(&get_ident);
        get_ident.Execute = 1;
        ArEventLogGetIdent(&get_ident);
        status = get_ident.Done;
        if(!status){
            get_ident.Execute = 0;
            ArEventLogGetIdent(&get_ident);
        }
    }
    
    std::memset(&write, 0, sizeof(write));
    
    std::string object = "RouterShuttle";
    object.append(std::to_string(shuttle));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), object.c_str());
    write.EventID = rl6dINFO_DESTINATION_UPDATE;
    write.AddData = reinterpret_cast<UDINT>(buffer);
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;
    
}

bool log_path_start(UINT shuttle, std::string path){

    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    UDINT buffer_size = path.length() + 1;
    buffer_size += rl6d_log_buffer_min_size + rl6d_log_buffer_size_udint;
    char buffer [buffer_size];
    status = ArEventLogAddDataInit(reinterpret_cast<UDINT>(buffer), buffer_size, arEVENTLOG_ADDFORMAT_CODED);
    if(status){
        status = ArEventLogAddDataUdint(reinterpret_cast<UDINT>(buffer), buffer_size, shuttle);
    }
    if(status){
        status = ArEventLogAddDataString(reinterpret_cast<UDINT>(buffer), buffer_size, reinterpret_cast<UDINT>(path.c_str()));
    }
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    if(status){
        ArEventLogGetIdent(&get_ident);
        get_ident.Execute = 1;
        ArEventLogGetIdent(&get_ident);
        status = get_ident.Done;
        if(!status){
            get_ident.Execute = 0;
            ArEventLogGetIdent(&get_ident);
        }
    }
    
    std::memset(&write, 0, sizeof(write));
    
    std::string object = "RouterShuttle";
    object.append(std::to_string(shuttle));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), object.c_str());
    write.EventID = rl6dINFO_MOVE_PATH_START;
    write.AddData = reinterpret_cast<UDINT>(buffer);
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;
    
}

bool log_waypoint_move(UINT shuttle, std::string waypoint){

    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    UDINT buffer_size = waypoint.length() + 1;
    buffer_size += rl6d_log_buffer_min_size + rl6d_log_buffer_size_udint;
    char buffer [buffer_size];
    status = ArEventLogAddDataInit(reinterpret_cast<UDINT>(buffer), buffer_size, arEVENTLOG_ADDFORMAT_CODED);
    if(status){
        status = ArEventLogAddDataUdint(reinterpret_cast<UDINT>(buffer), buffer_size, shuttle);
    }
    if(status){
        status = ArEventLogAddDataString(reinterpret_cast<UDINT>(buffer), buffer_size, reinterpret_cast<UDINT>(waypoint.c_str()));
    }
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    if(status){
        ArEventLogGetIdent(&get_ident);
        get_ident.Execute = 1;
        ArEventLogGetIdent(&get_ident);
        status = get_ident.Done;
        if(!status){
            get_ident.Execute = 0;
            ArEventLogGetIdent(&get_ident);
        }
    }
    
    std::memset(&write, 0, sizeof(write));
    
    std::string object = "RouterShuttle";
    object.append(std::to_string(shuttle));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), object.c_str());
    write.EventID = rl6dINFO_MOVE_WAYPOINT;
    write.AddData = reinterpret_cast<UDINT>(buffer);
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;
    
}

bool log_routed_move_waypoint(UINT shuttle, std::string destination, std::string waypoint){

    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    UDINT buffer_size = destination.length() + 1;
    buffer_size += waypoint.length() +1;
    buffer_size += rl6d_log_buffer_min_size + rl6d_log_buffer_size_udint;
    char buffer [buffer_size];
    status = ArEventLogAddDataInit(reinterpret_cast<UDINT>(buffer), buffer_size, arEVENTLOG_ADDFORMAT_CODED);
    if(status){
        status = ArEventLogAddDataUdint(reinterpret_cast<UDINT>(buffer), buffer_size, shuttle);
    }
    if(status){
        status = ArEventLogAddDataString(reinterpret_cast<UDINT>(buffer), buffer_size, reinterpret_cast<UDINT>(destination.c_str()));
    }
    if(status){
        status = ArEventLogAddDataString(reinterpret_cast<UDINT>(buffer), buffer_size, reinterpret_cast<UDINT>(waypoint.c_str()));
    }
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    if(status){
        ArEventLogGetIdent(&get_ident);
        get_ident.Execute = 1;
        ArEventLogGetIdent(&get_ident);
        status = get_ident.Done;
        if(!status){
            get_ident.Execute = 0;
            ArEventLogGetIdent(&get_ident);
        }
    }
    
    std::memset(&write, 0, sizeof(write));
    
    std::string object = "RouterShuttle";
    object.append(std::to_string(shuttle));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), object.c_str());
    write.EventID = rl6dINFO_ROUTED_MOVE_WAYPOINT;
    write.AddData = reinterpret_cast<UDINT>(buffer);
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;
    
}

bool log_asm_get_sh_start(){
    
    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    ArEventLogGetIdent(&get_ident);
    get_ident.Execute = 1;
    ArEventLogGetIdent(&get_ident);
    status = get_ident.Done;
    if(!status){
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    std::memset(&write, 0, sizeof(write));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), "Router");
    write.EventID = rl6dINFO_GET_SHUTTLES_START;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;

}
bool log_asm_get_sh_done(UINT shuttles){

    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    UDINT buffer_size = rl6d_log_buffer_min_size + rl6d_log_buffer_size_udint;
    char buffer [buffer_size];
    status = ArEventLogAddDataInit(reinterpret_cast<UDINT>(buffer), buffer_size, arEVENTLOG_ADDFORMAT_CODED);
    if(status){
        status = ArEventLogAddDataUdint(reinterpret_cast<UDINT>(buffer), buffer_size, shuttles);
    }
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    if(status){
        ArEventLogGetIdent(&get_ident);
        get_ident.Execute = 1;
        ArEventLogGetIdent(&get_ident);
        status = get_ident.Done;
        if(!status){
            get_ident.Execute = 0;
            ArEventLogGetIdent(&get_ident);
        }
    }
    
    std::memset(&write, 0, sizeof(write));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), "Router");
    write.EventID = rl6dINFO_GET_SHUTTLES_DONE;
    write.AddData = reinterpret_cast<UDINT>(buffer);
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;
    
}

bool log_asm_recovery_start(rl6dRecoverModeEnum mode){
    
    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    UDINT buffer_size = rl6d_log_buffer_min_size + rl6d_log_buffer_size_udint;
    char buffer [buffer_size];
    status = ArEventLogAddDataInit(reinterpret_cast<UDINT>(buffer), buffer_size, arEVENTLOG_ADDFORMAT_CODED);
    if(status){
        status = ArEventLogAddDataDint(reinterpret_cast<UDINT>(buffer), buffer_size, mode);
    }
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    if(status){
        ArEventLogGetIdent(&get_ident);
        get_ident.Execute = 1;
        ArEventLogGetIdent(&get_ident);
        status = get_ident.Done;
        if(!status){
            get_ident.Execute = 0;
            ArEventLogGetIdent(&get_ident);
        }
    }
    
    std::memset(&write, 0, sizeof(write));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), "Router");
    write.EventID = rl6dINFO_RECOVERY_START;
    write.AddData = reinterpret_cast<UDINT>(buffer);
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;
    
}
bool log_asm_recovery_done(void){
    
    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    ArEventLogGetIdent(&get_ident);
    get_ident.Execute = 1;
    ArEventLogGetIdent(&get_ident);
    status = get_ident.Done;
    if(!status){
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    std::memset(&write, 0, sizeof(write));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), "Router");
    write.EventID = rl6dINFO_RECOVERY_DONE;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;
}
bool log_asm_sh_assignment(UINT shuttle, std::string destination){
    
    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    UDINT buffer_size = destination.length() + 1;
    buffer_size += rl6d_log_buffer_min_size + rl6d_log_buffer_size_udint;
    char buffer [buffer_size];
    status = ArEventLogAddDataInit(reinterpret_cast<UDINT>(buffer), buffer_size, arEVENTLOG_ADDFORMAT_CODED);
    if(status){
        status = ArEventLogAddDataUdint(reinterpret_cast<UDINT>(buffer), buffer_size, shuttle);
    }
    if(status){
        status = ArEventLogAddDataString(reinterpret_cast<UDINT>(buffer), buffer_size, reinterpret_cast<UDINT>(destination.c_str()));
    }
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    if(status){
        ArEventLogGetIdent(&get_ident);
        get_ident.Execute = 1;
        ArEventLogGetIdent(&get_ident);
        status = get_ident.Done;
        if(!status){
            get_ident.Execute = 0;
            ArEventLogGetIdent(&get_ident);
        }
    }
    
    std::memset(&write, 0, sizeof(write));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), "Router");
    write.EventID = rl6dINFO_RECOVERY_ASSIGNMENT;
    write.AddData = reinterpret_cast<UDINT>(buffer);
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;
    
}
bool log_sh_error(UINT shuttle, DINT error){

    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    UDINT buffer_size = rl6d_log_buffer_min_size + (rl6d_log_buffer_size_udint * 2);
    char buffer [buffer_size];
    status = ArEventLogAddDataInit(reinterpret_cast<UDINT>(buffer), buffer_size, arEVENTLOG_ADDFORMAT_CODED);
    if(status){
        status = ArEventLogAddDataUdint(reinterpret_cast<UDINT>(buffer), buffer_size, shuttle);
    }
    if(status){
        status = ArEventLogAddDataDint(reinterpret_cast<UDINT>(buffer), buffer_size, error);
    }
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    if(status){
        ArEventLogGetIdent(&get_ident);
        get_ident.Execute = 1;
        ArEventLogGetIdent(&get_ident);
        status = get_ident.Done;
        if(!status){
            get_ident.Execute = 0;
            ArEventLogGetIdent(&get_ident);
        }
    }
    
    std::memset(&write, 0, sizeof(write));
    
    std::string object = "RouterShuttle";
    object.append(std::to_string(shuttle));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), object.c_str());
    write.EventID = rl6dERR_SHUTTLE;
    write.AddData = reinterpret_cast<UDINT>(buffer);
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;
    
}
bool log_asm_get_sh_error(DINT error){
    
    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    UDINT buffer_size = rl6d_log_buffer_min_size + rl6d_log_buffer_size_udint;
    char buffer [buffer_size];
    status = ArEventLogAddDataInit(reinterpret_cast<UDINT>(buffer), buffer_size, arEVENTLOG_ADDFORMAT_CODED);
    if(status){
        status = ArEventLogAddDataDint(reinterpret_cast<UDINT>(buffer), buffer_size, error);
    }
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    if(status){
        ArEventLogGetIdent(&get_ident);
        get_ident.Execute = 1;
        ArEventLogGetIdent(&get_ident);
        status = get_ident.Done;
        if(!status){
            get_ident.Execute = 0;
            ArEventLogGetIdent(&get_ident);
        }
    }
    
    std::memset(&write, 0, sizeof(write));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), "Router");
    write.EventID = rl6dERR_SHUTTLE;
    write.AddData = reinterpret_cast<UDINT>(buffer);
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;

}
bool log_move_in_plane_error(DINT error){

    struct ArEventLogGetIdent get_ident;
    struct ArEventLogWrite write;
    
    bool status;
    UDINT buffer_size = rl6d_log_buffer_min_size + rl6d_log_buffer_size_udint;
    char buffer [buffer_size];
    status = ArEventLogAddDataInit(reinterpret_cast<UDINT>(buffer), buffer_size, arEVENTLOG_ADDFORMAT_CODED);
    if(status){
        status = ArEventLogAddDataDint(reinterpret_cast<UDINT>(buffer), buffer_size, error);
    }
    
    std::memset(&get_ident, 0, sizeof(get_ident));
    
    std::strcpy(get_ident.Name, router_6d_log_name);
    get_ident.Execute = 0;
    
    if(status){
        ArEventLogGetIdent(&get_ident);
        get_ident.Execute = 1;
        ArEventLogGetIdent(&get_ident);
        status = get_ident.Done;
        if(!status){
            get_ident.Execute = 0;
            ArEventLogGetIdent(&get_ident);
        }
    }
    
    std::memset(&write, 0, sizeof(write));
    
    write.Ident = get_ident.Ident;
    std::strcpy(reinterpret_cast<char*>(&write.ObjectID), "Router");
    write.EventID = rl6dERR_SHUTTLE;
    write.AddData = reinterpret_cast<UDINT>(buffer);
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
    write.Execute = 0;
    
    if(status){
        ArEventLogWrite(&write);
        write.Execute = 1;
        ArEventLogWrite(&write);
        status = write.Done;
        write.Execute = 0;
        ArEventLogWrite(&write);
        get_ident.Execute = 0;
        ArEventLogGetIdent(&get_ident);
    }
    
    return status;
    
}
