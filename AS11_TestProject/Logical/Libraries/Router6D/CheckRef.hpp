#ifndef __RL6D_CHECK_REF_H__
#define __RL6D_CHECK_REF_H__

#include <Router.hpp>
#include <SemMan.hpp>
#include <Destination.hpp>
#include <DestinationIf.hpp>
#include <Path.hpp>
#include <Shuttle.hpp>
#include <ShuttleIf.hpp>

//Forward Declarations
class Router;
class SemMan;
class RouterSem;
class Destination;
class DestinationIf;
class Path;
class Shuttle;
class ShuttleIf;

DINT check_null_pointer(UDINT reference);

DINT check_router_reference(rl6dRouterType* reference);

DINT check_sem_man_reference(UDINT reference);

DINT check_router_sem_reference(UDINT reference);

DINT check_destination_reference(rl6dDestinationType* reference);

DINT check_destination_if_reference(UDINT reference);

DINT check_path_reference(rl6dPathType* reference);

DINT check_waypoint_reference(rl6dWaypointType* reference);

DINT check_shuttle_reference(rl6dShuttleType* reference);

DINT check_shuttle_if_reference(UDINT reference);

#endif

