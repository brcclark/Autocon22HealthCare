
FUNCTION_BLOCK rl6dRouterMan (*Router manager*)
	VAR_INPUT
		Assembly : REFERENCE TO Mc6DAssemblyType; (*Assembly to manage shuttles on*)
		UserDataSize : UDINT; (*Size of the user data*)
		Enable : BOOL; (*Enable shuttle routing*)
	END_VAR
	VAR_OUTPUT
		Busy : BOOL; (*The function block is busy and must continue to be called*)
		Active : BOOL; (*The function block is active*)
		Error : BOOL; (*An error occurred*)
		ErrorID : DINT; (*Error that occurred*)
		Router : rl6dRouterType; (*Router handle*)
	END_VAR
	VAR
		Internal : rl6dRouterManInternalType; (*Internal data*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK rl6dRouterAddPathFromData (*Add a path from an array of positions*)
	VAR_INPUT
		Router : REFERENCE TO rl6dRouterType; (*Router to add path to*)
		DataAddress : UDINT; (*Pointer to array of rl6dPositionType*)
		DataSize : UDINT; (*Size of the array of positions in Bytes*)
		Name : STRING[255]; (*Name of the path*)
		Execute : BOOL; (*Add the path*)
	END_VAR
	VAR_OUTPUT
		Busy : BOOL; (*The function block is busy and must continue to be called*)
		Path : rl6dPathType; (*Path handle*)
		Done : BOOL; (*The path has been created*)
		Error : BOOL; (*An error occurred*)
		ErrorID : DINT; (*Error that occurred*)
	END_VAR
	VAR
		Internal : rl6dRouterAddPathInternalType; (*Internal data*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK rl6dPathGetData (*Get a path to an array of positions*)
	VAR_INPUT
		Path : REFERENCE TO rl6dPathType; (*Path handle*)
		DataAddress : UDINT; (*Pointer to array of rl6dPositionType*)
		DataSize : UDINT; (*Size of the array of positions in Bytes*)
		Execute : BOOL; (*Get the path*)
	END_VAR
	VAR_OUTPUT
		Busy : BOOL; (*The function block is busy and must continue to be called*)
		PathDataSize : UDINT; (*Actual size of path data*)
		Done : BOOL; (*The path has been created*)
		Error : BOOL; (*An error occurred*)
		ErrorID : DINT; (*Error that occurred*)
	END_VAR
	VAR
		Internal : rl6dGetPathInternalType; (*Internal data*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK rl6dRouterAddDestination (*Add a path from an array of positions*)
	VAR_INPUT
		Router : REFERENCE TO rl6dRouterType; (*Router to add destination to*)
		EntryPath : REFERENCE TO rl6dPathType; (*Optional reference to the destination entry path*)
		ExitPath : REFERENCE TO rl6dPathType; (*Optional reference to the destination exit path*)
		Position : rl6dPositionType; (*Position of the destination*)
		Name : STRING[255]; (*Name of the destination*)
		Execute : BOOL; (*Add the destination*)
	END_VAR
	VAR_OUTPUT
		Busy : BOOL; (*The function block is busy and must continue to be called*)
		Destination : rl6dDestinationType; (*Destination handle*)
		Done : BOOL; (*The destination has been created*)
		Error : BOOL; (*An error occurred*)
		ErrorID : DINT; (*Error that occurred*)
	END_VAR
	VAR
		Internal : rl6dRouterAddPathInternalType; (*Internal data*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK rl6dDestAddRecoveryPosFromData (*Add recovery positions from an array of positions*)
	VAR_INPUT
		Destination : REFERENCE TO rl6dDestinationType; (*Destination to add recovery points to*)
		DataAddress : UDINT; (*Pointer to array of rl6dPositionType*)
		DataSize : UDINT; (*Size of the array of positions in Bytes*)
		Execute : BOOL; (*Add the recovery points*)
	END_VAR
	VAR_OUTPUT
		Busy : BOOL; (*The function block is busy and must continue to be called*)
		Done : BOOL; (*The path has been created*)
		Error : BOOL; (*An error occurred*)
		ErrorID : DINT; (*Error that occurred*)
	END_VAR
	VAR
		Internal : rl6dDestAddRecPosInternalType; (*Internal data*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK rl6dDestinationSetWaypoint (*Set the waypoint of the destination*)
	VAR_INPUT
		Destination : REFERENCE TO rl6dDestinationType; (*Destination to set waypoint*)
		Waypoint : REFERENCE TO rl6dWaypointType; (*Waypoint to set*)
		Execute : BOOL; (*Set the next waypoint on a rising edge*)
	END_VAR
	VAR_OUTPUT
		Busy : BOOL; (*The function block is busy and must continue to be called*)
		Done : BOOL; (*The waypoint has been set*)
		Error : BOOL; (*An error occurred*)
		ErrorID : DINT; (*Error that occurred*)
	END_VAR
	VAR
		Internal : rl6dDestSetWaypntInternalType; (*Internal data*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK rl6dRouterAddWaypoint (*Add a waypoint from a position*)
	VAR_INPUT
		Router : REFERENCE TO rl6dRouterType; (*Router to add waypoint to*)
		Position : rl6dPositionType; (*Position of waypoint*)
		Name : STRING[255]; (*Name of waypoint*)
		Execute : BOOL; (*Add waypoint on rising edge*)
	END_VAR
	VAR_OUTPUT
		Busy : BOOL; (*The function block is busy and must continue to be called*)
		Waypoint : rl6dWaypointType; (*Waypoint handle*)
		Done : BOOL; (*The waypoint has been created*)
		Error : BOOL; (*An error occurred*)
		ErrorID : DINT; (*Error that occurred*)
	END_VAR
	VAR
		Internal : rl6dRouterAddWaypntInternalType; (*Internal data*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK rl6dWaypointAddDestination (*Add Destination to Waypoint*)
	VAR_INPUT
		Waypoint : REFERENCE TO rl6dWaypointType; (*Waypoint to add destination to*)
		Destination : REFERENCE TO rl6dDestinationType; (*Destination to add to waypoint*)
		DeparturePath : REFERENCE TO rl6dPathType; (*Optional departure path to follow when leaving waypoint for destination*)
		Execute : BOOL; (*Add the destination*)
	END_VAR
	VAR_OUTPUT
		Busy : BOOL; (*The function block is busy and must continue to be called*)
		Done : BOOL; (*The Destination has been added to the Waypoint*)
		Error : BOOL; (*An error occurred*)
		ErrorID : DINT; (*Error that occurred*)
	END_VAR
	VAR
		Internal : rl6dWaypointAddDestInternalType; (*Internal data*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK rl6dWaypointSetNextWaypoint (*Set the next waypoint of the target waypoint*)
	VAR_INPUT
		Waypoint : REFERENCE TO rl6dWaypointType; (*Waypoint to set next waypoint of*)
		NextWaypoint : REFERENCE TO rl6dWaypointType; (*Next waypoint*)
		Execute : BOOL; (*Set the next waypoint on a rising edge*)
	END_VAR
	VAR_OUTPUT
		Busy : BOOL; (*The function block is busy and must continue to be called*)
		Done : BOOL; (*The next waypoint has been set*)
		Error : BOOL; (*An error occurred*)
		ErrorID : DINT; (*Error that occurred*)
	END_VAR
	VAR
		Internal : rl6dWaypntSetWaypntInternalType; (*Internal data*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK rl6dRouterAddShFromAsm (*Add shuttles from the assembly*)
	VAR_INPUT
		Router : REFERENCE TO rl6dRouterType; (*Router to add shuttles from assembly to*)
		Execute : BOOL; (*Add shuttles*)
	END_VAR
	VAR_OUTPUT
		Busy : BOOL; (*The function block is busy and must continue to be called*)
		Done : BOOL; (*The shuttles have been added*)
		NumShuttles : UDINT; (*Number of shuttles found on the assembly*)
		Error : BOOL; (*An error occurred*)
		ErrorID : DINT; (*Error that occurred*)
	END_VAR
	VAR
		Internal : rl6dRtrAddShFromAsmInternalType; (*Internal data*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK rl6dRouterExecuteRecovery (*Execute recovery of shuttles*)
	VAR_INPUT
		Router : REFERENCE TO rl6dRouterType; (*Router to add shuttles from assembly to*)
		Velocity : REAL; (*Recovery Velocity*)
		Acceleration : REAL; (*Recovery Acceleration*)
		Mode : rl6dRecoverModeEnum; (*Mode to use when recovering*)
		Execute : BOOL; (*Add shuttles*)
	END_VAR
	VAR_OUTPUT
		Busy : BOOL; (*The function block is busy and must continue to be called*)
		Done : BOOL; (*The shuttles have been added*)
		Error : BOOL; (*An error occurred*)
		ErrorID : DINT; (*Error that occurred*)
	END_VAR
	VAR
		Internal : rl6dRtrAddShFromAsmInternalType; (*Internal data*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK rl6dDestinationMonitor (*Monitor destination for shuttles*)
	VAR_INPUT
		Destination : REFERENCE TO rl6dDestinationType; (*Destination to monitor*)
		Enable : BOOL; (*Enable monitoring*)
		AcknowledgeShuttle : BOOL; (*Acknowledge current shuttle*)
	END_VAR
	VAR_OUTPUT
		Busy : BOOL; (*The function block is busy and must continue to be called*)
		Valid : BOOL; (*Outputs are valid*)
		ShuttlePresent : BOOL; (*A shuttle is present at the destination*)
		ShuttleID : UINT; (*Shuttle ID*)
		RouterShuttle : rl6dShuttleType; (*Shuttle for use with router system*)
		Mc6DShuttle : Mc6DShuttleType; (*Shuttle for use with mapp 6D*)
		Error : BOOL; (*An error occurred*)
		ErrorID : DINT; (*Error that occurred*)
	END_VAR
	VAR
		Internal : rl6dDestinationMonInternalType; (*Internal data*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK rl6dShuttleMoveDestination (*Move shuttle to destination*)
	VAR_INPUT
		RouterShuttle : REFERENCE TO rl6dShuttleType; (*Shuttle to control*)
		Destination : REFERENCE TO rl6dDestinationType; (*Destination to move to*)
		Velocity : REAL; (*Velocity of shuttle movement*)
		Acceleration : REAL; (*Acceleration of shuttle movement*)
		Execute : BOOL; (*Start moving shuttle to destination*)
	END_VAR
	VAR_OUTPUT
		Busy : BOOL; (*The function block is busy and must continue to be called*)
		Active : BOOL; (*Move is active*)
		Aborted : BOOL; (*Move has been aborted*)
		Done : BOOL; (*Move has completed*)
		Error : BOOL; (*An error occurred*)
		ErrorID : DINT; (*Error that occurred*)
	END_VAR
	VAR
		Internal : rl6dShMoveDestInternalType; (*Internal data*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK rl6dShCopyUserData (*Copy shuttle user data asynchronously*)
	VAR_INPUT
		RouterShuttle : REFERENCE TO rl6dShuttleType; (*Shuttle to copy user data*)
		Mode : rl6dShCopyUserDataModeEnum; (*Mode of copy user data*)
		DataAddress : UDINT; (*Address of data to copy*)
		DataSize : UDINT; (*Start moving shuttle to destination*)
		Execute : BOOL; (*Copy user data*)
	END_VAR
	VAR_OUTPUT
		Busy : BOOL; (*The function block is busy and must continue to be called*)
		Done : BOOL; (*Move has completed*)
		SizeCopied : UDINT; (*Size of the data copied*)
		Error : BOOL; (*An error occurred*)
		ErrorID : DINT; (*Error that occurred*)
	END_VAR
	VAR
		Internal : rl6dShCopyUserDataInternalType; (*Internal Data*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION rl6dShCopyUserDataSync : DINT (*Copy shuttle user data synchronously*)
	VAR_INPUT
		RouterShuttle : rl6dShuttleType; (*Shuttle to copy user data*)
		Mode : rl6dShCopyUserDataModeEnum; (*Mode of copy*)
		DataAddress : UDINT; (*Data address*)
		DataSize : UDINT; (*Data size*)
	END_VAR
END_FUNCTION
