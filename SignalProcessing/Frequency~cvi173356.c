#include <cvirte.h>		
#include <userint.h>
#include "ProiectAPD.h"
#include <utility.h>
#include <cvirte.h>		
#include <ansi_c.h>
#include <advanlys.h>
#include <userint.h>

//==============================================================================
//
// Title:		Frequency.c
// Purpose:		A short description of the implementation.
//
// Created on:	10/12/2021 at 17:21:56 by delia barbuta.
// Copyright:	Universitatea Tehnica „Gheorghe Asachi” din Ia?i. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "Frequency.h"  
//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables
int second = 0;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
int nrPointsInterval = 0;
//==============================================================================
// Global functions


int Define_Your_Functions_Here (int x)
{
	return x;
}

int CVICALLBACK onFreqPanelCB (int panel, int event, void *callbackData,
							   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}


int CVICALLBACK startTimerCB (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int pressed ;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, FREQ_PANEL_START_TIMER_BT, &pressed);
			SetCtrlAttribute(panel, FREQ_PANEL_TIMER, ATTR_ENABLED, pressed);
			break;
	}
	return 0;
}

int CVICALLBACK timerCB (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	double windowParameter;
	WindowConst winConst;
	double * autoSpectrum = (double *) calloc (npoints / 2, sizeof(double));
	double df;
	double searchFreq;
	double freqPeak;
	double powerPeak;
	
	
	GetCtrlVal(panel, FREQ_PANEL_NR_OF_POINTS, &nrPointsInterval);
	int nrOfIntervals =floor( npoints / nrPointsInterval) + 1;
	int nrPointsLeft = npoints % nrPointsInterval;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			second++;
			SetCtrlAttribute(panel, FREQ_PANEL_SECOND, ATTR_CTRL_VAL, second);
			if(second < nrOfIntervals)
			{
				DeleteGraphPlot (panel,  FREQ_PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
				PlotY(panel,FREQ_PANEL_GRAPH, waveData + second * nrPointsInterval, nrPointsInterval, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
	 
				ScaledWindowEx(waveData + second * nrPointsInterval, nrPointsInterval, RECTANGLE, windowParameter, &winConst);
				AutoPowerSpectrum(waveData + second * nrPointsInterval, nrPointsInterval, 1.0 / nrPointsInterval, autoSpectrum, &df);
				PowerFrequencyEstimate(autoSpectrum, nrPointsInterval / 2, searchFreq, winConst, df, 7, &freqPeak, &powerPeak);
				
			}
			
			
			break;
	}
	return 0;
}
