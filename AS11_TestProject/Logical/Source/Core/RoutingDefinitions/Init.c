
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif
#define TRUE 1
#define FALSE 0

void _INIT ProgramInit(void)
{
	//Define the destinations & their paths
	
	//*******Incubator Destination
	brsstrcpy(&gDests[DEST_INCUBATOR].StationName,&"Incubator");
	gDests[DEST_INCUBATOR].Position.X = 0.06;
	gDests[DEST_INCUBATOR].Position.Y = 0.06;
	gDests[DEST_INCUBATOR].RecoveryPts.X = 0.06;
	gDests[DEST_INCUBATOR].RecoveryPts.Y = 0.06;
	
	brsstrcpy(&gDests[DEST_INCUBATOR].Entry.Name,&"Incubator Entry");
	gDests[DEST_INCUBATOR].Entry.PathPoints[0].X = 0.18;
	gDests[DEST_INCUBATOR].Entry.PathPoints[0].Y = 0.3;
	gDests[DEST_INCUBATOR].Entry.PathPoints[1].X = 0.18;
	gDests[DEST_INCUBATOR].Entry.PathPoints[1].Y = 0.06;
	gDests[DEST_INCUBATOR].Entry.Length = 2;
	
	brsstrcpy(&gDests[DEST_INCUBATOR].Exit.Name,&"Incubator Exit");
	gDests[DEST_INCUBATOR].Exit.PathPoints[0].X = 0.06;
	gDests[DEST_INCUBATOR].Exit.PathPoints[0].Y = 0.42;
	gDests[DEST_INCUBATOR].Exit.Length = 1;
		
	
	//******Analyzer Destination
	brsstrcpy(&gDests[DEST_ANALYZER].StationName,&"Analyzer");
	gDests[DEST_ANALYZER].Position.X = 0.66;
	gDests[DEST_ANALYZER].Position.Y = 0.06;
	gDests[DEST_ANALYZER].RecoveryPts.X = 0.66;
	gDests[DEST_ANALYZER].RecoveryPts.Y = 0.06;
	
	brsstrcpy(&gDests[DEST_ANALYZER].Entry.Name,&"Analyzer Entry");
	gDests[DEST_ANALYZER].Entry.PathPoints[0].X = 0.06;
	gDests[DEST_ANALYZER].Entry.PathPoints[0].Y = 0.42;
	gDests[DEST_ANALYZER].Entry.Length = 1;
	
	brsstrcpy(&gDests[DEST_ANALYZER].Exit.Name,&"Analyzer Exit");
	gDests[DEST_ANALYZER].Exit.PathPoints[0].X = 0.18;
	gDests[DEST_ANALYZER].Exit.PathPoints[0].Y = 0.66;
	gDests[DEST_ANALYZER].Entry.PathPoints[1].X = 0.42;
	gDests[DEST_ANALYZER].Entry.PathPoints[1].Y = 0.42;
	gDests[DEST_ANALYZER].Exit.Length = 2;
	
	//******Pipette 1 Destination
	brsstrcpy(&gDests[DEST_PIPETTE_1].StationName,&"Pipette 1");
	gDests[DEST_PIPETTE_1].Position.X = 0.36;
	gDests[DEST_PIPETTE_1].Position.Y = 0.60;
	gDests[DEST_PIPETTE_1].RecoveryPts.X = 0.36;
	gDests[DEST_PIPETTE_1].RecoveryPts.Y = 0.60;
	
	brsstrcpy(&gDests[DEST_PIPETTE_1].Entry.Name,&"Pipette 1 Entry");
	gDests[DEST_PIPETTE_1].Entry.PathPoints[0].X = 0.30;
	gDests[DEST_PIPETTE_1].Entry.PathPoints[0].Y = 0.60;
	gDests[DEST_PIPETTE_1].Entry.Length = 1;
	
	brsstrcpy(&gDests[DEST_PIPETTE_1].Exit.Name,&"Pipette 1 Exit");
	gDests[DEST_PIPETTE_1].Exit.PathPoints[0].X = 0.42;
	gDests[DEST_PIPETTE_1].Exit.PathPoints[0].Y = 0.60;
	gDests[DEST_PIPETTE_1].Exit.Length = 1;
	
	//******Pipette 2 Destination
	brsstrcpy(&gDests[DEST_PIPETTE_2].StationName,&"Pipette 2");
	gDests[DEST_PIPETTE_2].Position.X = 0.36;
	gDests[DEST_PIPETTE_2].Position.Y = 0.12;
	gDests[DEST_PIPETTE_2].RecoveryPts.X = 0.36;
	gDests[DEST_PIPETTE_2].RecoveryPts.Y = 0.12;
	
	brsstrcpy(&gDests[DEST_PIPETTE_2].Entry.Name,&"Pipette 2 Entry");
	gDests[DEST_PIPETTE_2].Entry.PathPoints[0].X = 0.30;
	gDests[DEST_PIPETTE_2].Entry.PathPoints[0].Y = 0.12;
	gDests[DEST_PIPETTE_2].Entry.Length = 1;
	
	brsstrcpy(&gDests[DEST_PIPETTE_2].Exit.Name,&"Pipette 2 Exit");
	gDests[DEST_PIPETTE_2].Exit.PathPoints[0].X = 0.42;
	gDests[DEST_PIPETTE_2].Exit.PathPoints[0].Y = 0.12;
	gDests[DEST_PIPETTE_2].Exit.Length = 1;
	
	//******Tips Destination
	brsstrcpy(&gDests[DEST_TIPS].StationName,&"Tips");
	gDests[DEST_TIPS].Position.X = 0.66;
	gDests[DEST_TIPS].Position.Y = 0.30;
	gDests[DEST_TIPS].RecoveryPts.X = 0.66;
	gDests[DEST_TIPS].RecoveryPts.Y = 0.30;
	
	brsstrcpy(&gDests[DEST_TIPS].Entry.Name,&"Tips Entry");
	gDests[DEST_TIPS].Entry.PathPoints[0].X = 0.54;
	gDests[DEST_TIPS].Entry.PathPoints[0].Y = 0.30;
	gDests[DEST_TIPS].Entry.Length = 1;
	
	brsstrcpy(&gDests[DEST_TIPS].Exit.Name,&"Tips Exit");
	gDests[DEST_TIPS].Exit.PathPoints[0].X = 0.54;
	gDests[DEST_TIPS].Exit.PathPoints[0].Y = 0.30;
	gDests[DEST_TIPS].Exit.Length = 1;
	
	//******Dye Destination
	brsstrcpy(&gDests[DEST_DYE].StationName,&"Dye");
	gDests[DEST_DYE].Position.X = 0.66;
	gDests[DEST_DYE].Position.Y = 0.42;
	gDests[DEST_DYE].RecoveryPts.X = 0.66;
	gDests[DEST_DYE].RecoveryPts.Y = 0.42;
	
	brsstrcpy(&gDests[DEST_DYE].Entry.Name,&"Dye Entry");
	gDests[DEST_DYE].Entry.PathPoints[0].X = 0.54;
	gDests[DEST_DYE].Entry.PathPoints[0].Y = 0.42;
	gDests[DEST_DYE].Entry.Length = 1;
	
	brsstrcpy(&gDests[DEST_DYE].Exit.Name,&"Dye Exit");
	gDests[DEST_DYE].Exit.PathPoints[0].X = 0.54;
	gDests[DEST_DYE].Exit.PathPoints[0].Y = 0.42;
	gDests[DEST_DYE].Exit.Length = 1;
	
	//Define the Waypoints
	
	//******** Way Point 1
	brsstrcpy(&gWps[WP_1].Name,&"Waypoint 1");
	gWps[WP_1].Position.X = 0.30;
	gWps[WP_1].Position.Y = 0.42;
	gWps[WP_1].NextWaypoint = WP_2;
	gWps[WP_1].Destinations[0] = DEST_PIPETTE_1;
	
	//******** Way Point 2
	brsstrcpy(&gWps[WP_2].Name,&"Waypoint 2");
	gWps[WP_2].Position.X = 0.42;
	gWps[WP_2].Position.Y = 0.42;
	gWps[WP_2].NextWaypoint = WP_3;
	gWps[WP_2].Destinations[0] = DEST_DYE;
	gWps[WP_2].Destinations[1] = DEST_TIPS;
	
	//******** Way Point 3
	brsstrcpy(&gWps[WP_3].Name,&"Waypoint 3");
	gWps[WP_3].Position.X = 0.42;
	gWps[WP_3].Position.Y = 0.30;
	gWps[WP_3].NextWaypoint = WP_4;
	gWps[WP_3].Destinations[0] = DEST_PIPETTE_2;
	
	//******** Way Point 4
	brsstrcpy(&gWps[WP_4].Name,&"Waypoint 4");
	gWps[WP_4].Position.X = 0.30;
	gWps[WP_4].Position.Y = 0.30;
	gWps[WP_4].NextWaypoint = WP_1;
	gWps[WP_4].Destinations[0] = DEST_ANALYZER;
	gWps[WP_4].Destinations[1] = DEST_INCUBATOR;
	
	//******** Way Point 5
	brsstrcpy(&gWps[WP_5].Name,&"Waypoint 5");
	gWps[WP_5].Position.X = 0.54;
	gWps[WP_5].Position.Y = 0.30;
	gWps[WP_5].NextWaypoint = WP_3;
}
