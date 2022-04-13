#include "CheckRef.hpp"

DINT check_null_reference(UDINT reference){
    if(reference == 0){
        return rl6dERR_NULL_POINTER;
    }
    else{
        return ERR_OK;
    }
}

DINT check_router_reference(rl6dRouterType* reference){
    if(reference == nullptr){
        return rl6dERR_NULL_POINTER;
    }
    if(check_null_reference(reference->Ident) == ERR_OK){
        Router* router = reinterpret_cast<Router*>(reference->Ident);
        if(router->get_type_id() == rl6dROUTER_TYPE_ID){
            return ERR_OK;
        }
        else{
            return rl6dERR_ROUTER_INVALID;
        }
    }
    else{
        return rl6dERR_ROUTER_INVALID;
    }
}

DINT check_sem_man_reference(UDINT reference){
    DINT error;
    SemMan* sem_man;
    error = check_null_reference(reference);
    if(error == ERR_OK){
        sem_man = reinterpret_cast<SemMan*>(reference);
        if(sem_man->get_type_id() != rl6dSEM_MAN_TYPE_ID){
            error = rl6dERR_SEM_MAN_INVALID;
        }
    }
    return error;
}

DINT check_router_sem_reference(UDINT reference){
    DINT error;
    RouterSem* router_sem;
    error = check_null_reference(reference);
    if(error == ERR_OK){
        router_sem = reinterpret_cast<RouterSem*>(reference);
        if(router_sem->get_type_id() != rl6dROUTER_SEM_TYPE_ID){
            error = rl6dERR_ROUTER_SEM_INVALID;
        }
    }
    return error;
}

DINT check_destination_reference(rl6dDestinationType* reference){
    if(reference == nullptr){
        return rl6dERR_NULL_POINTER;
    }
    if(check_null_reference(reference->Ident) == ERR_OK){
        Destination* destination = reinterpret_cast<Destination*>(reference->Ident);
        if(destination->get_type_id() == rl6dDESTINATION_TYPE_ID){
            return ERR_OK;
        }
        else{
            return rl6dERR_DESTINATION_INVALID;
        }
    }
    else{
        return rl6dERR_DESTINATION_INVALID;
    }
}

DINT check_destination_if_reference(UDINT reference){
    DINT error;
    DestinationIf* destination_if;
    error = check_null_reference(reference);
    if(error == ERR_OK){
        destination_if = reinterpret_cast<DestinationIf*>(reference);
        if(destination_if->get_type_id() != rl6dDESTINATION_IF_TYPE_ID){
            error = rl6dERR_DESTINATION_IF_INVALID;
        }
    }
    return error;
}

DINT check_path_reference(rl6dPathType* reference){
    if(reference == nullptr){
        return rl6dERR_NULL_POINTER;
    }
    if(check_null_reference(reference->Ident) == ERR_OK){
        Path* path = reinterpret_cast<Path*>(reference->Ident);
        if(path->get_type_id() == rl6dPATH_TYPE_ID){
            return ERR_OK;
        }
        else{
            return rl6dERR_PATH_INVALID;
        }
    }
    else{
        return rl6dERR_PATH_INVALID;
    }
}

DINT check_waypoint_reference(rl6dWaypointType* reference){
    if(reference == nullptr){
        return rl6dERR_NULL_POINTER;
    }
    if(check_null_reference(reference->Ident) == ERR_OK){
        Waypoint* waypoint = reinterpret_cast<Waypoint*>(reference->Ident);
        if(waypoint->get_type_id() == rl6dWAYPOINT_TYPE_ID){
            return ERR_OK;
        }
        else{
            return rl6dERR_WAYPOINT_INVALID;
        }
    }
    else{
        return rl6dERR_WAYPOINT_INVALID;
    }
}

DINT check_shuttle_reference(rl6dShuttleType* reference){
    if(reference == nullptr){
        return rl6dERR_NULL_POINTER;
    }
    if(check_null_reference(reference->Ident) == ERR_OK){
        Shuttle* shuttle = reinterpret_cast<Shuttle*>(reference->Ident);
        if(shuttle->get_type_id() == rl6dSHUTTLE_TYPE_ID){
            return ERR_OK;
        }
        else{
            return rl6dERR_SHUTTLE_INVALID;
        }
    }
    else{
        return rl6dERR_SHUTTLE_INVALID;
    }
}

DINT check_shuttle_if_reference(UDINT reference){
    DINT error;
    ShuttleIf* shuttle_if;
    error = check_null_reference(reference);
    if(error == ERR_OK){
        shuttle_if = reinterpret_cast<ShuttleIf*>(reference);
        if(shuttle_if->get_type_id() != rl6dSHUTTLE_IF_TYPE_ID){
            error = rl6dERR_SHUTTLE_IF_INVALID;
        }
    }
    return error;
}