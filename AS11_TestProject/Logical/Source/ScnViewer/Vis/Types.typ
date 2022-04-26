
TYPE
	VisTyp : 	STRUCT 
		Shuttles : ARRAY[0..MAX_SH_IDX]OF ShuttleVisTyp;
	END_STRUCT;
	ShuttleVisTyp : 	STRUCT 
		Visible : BOOL;
		Position : VisPositionTyp;
		ShuttleColor : ColorEnum;
		Wells : ARRAY[0..17]OF WellTyp;
		ToolVisible : BOOL;
	END_STRUCT;
	VisPositionTyp : 	STRUCT 
		X : REAL;
		Y : REAL;
		Z : REAL;
	END_STRUCT;
END_TYPE
