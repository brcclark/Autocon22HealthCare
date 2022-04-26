
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	pipetteFb[0].Destination = &gDests[DEST_PIPETTE_1].Destination;
	pipetteFb[0].Waypoint = &gWps[WP_2].Waypoint;
	pipetteFb[0].FirstDestination = &gDests[DEST_ANALYZER].Destination;
	
	pipetteFb[1].Destination = &gDests[DEST_PIPETTE_2].Destination;
	pipetteFb[1].Waypoint = &gWps[WP_4].Waypoint;
	pipetteFb[1].FirstDestination = &gDests[DEST_INCUBATOR].Destination;

	pipetteFb[0].WellPlateInfo.ColumnGap = 0.02;
	pipetteFb[0].WellPlateInfo.RowGap = 0.02;
	pipetteFb[0].WellPlateInfo.NumColumns = 4;
	pipetteFb[0].WellPlateInfo.NumRows = 4;
	pipetteFb[0].WellPlateInfo.OffsetX = 0.031;
	pipetteFb[0].WellPlateInfo.OffsetY = 0.031;
	
	pipetteFb[1].WellPlateInfo.ColumnGap = 0.02;
	pipetteFb[1].WellPlateInfo.RowGap = 0.02;
	pipetteFb[1].WellPlateInfo.NumColumns = 4;
	pipetteFb[1].WellPlateInfo.NumRows = 4;
	pipetteFb[1].WellPlateInfo.OffsetX = 0.031;
	pipetteFb[1].WellPlateInfo.OffsetY = 0.031;
	
	gSysRecipe.WellRecipe[0].CurrentSample = 0;
	gSysRecipe.WellRecipe[0].NumberSamples = 3;
	gSysRecipe.WellRecipe[0].Samples[0].CurrentCount = 0;
	gSysRecipe.WellRecipe[0].Samples[0].Sample = COLOR_DARK_BLUE;
	gSysRecipe.WellRecipe[0].Samples[0].SampleCount = 6;
	gSysRecipe.WellRecipe[0].Samples[1].CurrentCount = 0;
	gSysRecipe.WellRecipe[0].Samples[1].Sample = COLOR_DARK_GREEN;
	gSysRecipe.WellRecipe[0].Samples[1].SampleCount = 6;
	gSysRecipe.WellRecipe[0].Samples[2].CurrentCount = 0;
	gSysRecipe.WellRecipe[0].Samples[2].Sample = COLOR_DARK_YELLOW;
	gSysRecipe.WellRecipe[0].Samples[2].SampleCount = 4;
	
	gSysRecipe.WellRecipe[1].CurrentSample = 0;
	gSysRecipe.WellRecipe[1].NumberSamples = 4;
	gSysRecipe.WellRecipe[1].Samples[0].CurrentCount = 0;
	gSysRecipe.WellRecipe[1].Samples[0].Sample = COLOR_DARK_YELLOW;
	gSysRecipe.WellRecipe[1].Samples[0].SampleCount = 3;
	gSysRecipe.WellRecipe[1].Samples[1].CurrentCount = 0;
	gSysRecipe.WellRecipe[1].Samples[1].Sample = COLOR_DARK_BLUE;
	gSysRecipe.WellRecipe[1].Samples[1].SampleCount = 6;
	gSysRecipe.WellRecipe[1].Samples[2].CurrentCount = 0;
	gSysRecipe.WellRecipe[1].Samples[2].Sample = COLOR_RED;
	gSysRecipe.WellRecipe[1].Samples[2].SampleCount = 4;
	gSysRecipe.WellRecipe[1].Samples[3].CurrentCount = 0;
	gSysRecipe.WellRecipe[1].Samples[3].Sample = COLOR_DARK_GREEN;
	gSysRecipe.WellRecipe[1].Samples[3].SampleCount = 3;
}
