
TYPE
	RoutingDefineEnum : 
		(
		DEF_IDLE,
		DEF_INIT_ROUTER,
		DEF_INIT_ENTRY_PATHS,
		DEF_INIT_EXIT_PATHS,
		DEF_INIT_DESTINATIONS,
		DEF_INIT_RECOVERY_POINTS,
		DEF_INIT_WAYPOINTS,
		DEF_CHECK_WAYPOINT_DESTS,
		DEF_LINK_WAYPOINTS_TO_DESTS,
		DEF_LINK_WAYPOINTS_TO_WAYPOINTS,
		DEF_DONE,
		DEF_ADD_SHUTTLES,
		DEF_READY,
		DEF_RECOVERY,
		DEF_SEND_TO_START,
		DEF_RUNNING,
		DEF_ERROR
		);
	InitFbsTyp : 	STRUCT 
		routerFb : rl6dRouterMan;
		addPathFb : rl6dRouterAddPathFromData;
		addRecPointFb : rl6dDestAddRecoveryPosFromData;
		createDestFb : rl6dRouterAddDestination;
		createWaypointFb : rl6dRouterAddWaypoint;
		linkWaypointFb : rl6dWaypointSetNextWaypoint;
		linkWayPointDestFb : rl6dWaypointAddDestination;
		addShToRouter : rl6dRouterAddShFromAsm;
		recoverFb : rl6dRouterExecuteRecovery;
	END_STRUCT;
END_TYPE
