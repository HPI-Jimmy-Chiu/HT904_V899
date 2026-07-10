//---------------------------------------------------------------------------
//  SECSGEM/SecsEventReport.cpp -- Sim-first EventReport() entry point
//
//  Integration wave: W5-comms INTEGRATE
//  Author: AI(W5-comms-Integrate) 20260710
//  See SecsEventReport.h for the full rationale (why this is a NEW file, not
//  a golden translation, and why it deliberately does not touch HTGem).
//---------------------------------------------------------------------------

#include "SecsEventReport.h"

unsigned      g_SimLastEventReportCeid = 0;
unsigned long g_SimEventReportCount    = 0;

void EventReport(unsigned Ceid)
{
    g_SimLastEventReportCeid = Ceid;
    ++g_SimEventReportCount;
}

void ResetSimEventReport()
{
    g_SimLastEventReportCeid = 0;
    g_SimEventReportCount    = 0;
}
