//---------------------------------------------------------------------------

#ifndef asendic_ColorH
#define asendic_ColorH
//---------------------------------------------------------------------------

extern int iLoadNewColorTrayToCarTask;
extern int iColorTrayToFrontTask;
extern int iAutoColorReceiveTask;
extern int iAutoColorTask;
extern int iUnLoadNewColorTrayTask;
extern int iColorTrayToRearTask;
extern int iColorTrayReadCIDTask;

extern void InitAutoColorTask();
extern void InitAutoColorReceiveTask();
extern void InitColorTrayReadCIDTask();
extern void InitColorTrayToRearTask();

extern void DoAutoColor();
extern bool DoLoadNewColorTrayToCar();
extern int  DoColorTrayReadCID();
extern void DoAutoColorReceive();
extern bool ForTERAPOWERCheckColorSensor(int iPos);
extern bool DoColorTrayToRear();
#endif
