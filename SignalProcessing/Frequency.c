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
#include "ProiectAPD.h"
//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables
int nrPointsInterval = 0;
int second = 0; 
int winType = 0;
double *filteredSignal;
int filterType;

double windowParameter = 0;
WindowConst winConst;
double df = 0;
double searchFreq = 0;
double freqPeak = 0;
double powerPeak = 0;

double windowParameter2 = 0;
WindowConst winConst2;
double df2 = 0;
double searchFreq2 = 0;
double freqPeak2 = 0;
double powerPeak2 = 0;
int saveGraphs = 0;

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
			free(filteredSignal);
			QuitUserInterface(0);
			break;
	}
	return 0;
}


int CVICALLBACK startTimerCB (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int pressed = 0;
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
	//verificam daca e bifat checkbox-ul pt salvare
	GetCtrlVal(panel, FREQ_PANEL_SAVE_GRAPHS, &saveGraphs); 
	GetCtrlIndex(panel, FREQ_PANEL_FILTER_TYPE, &filterType);
	GetCtrlIndex(panel, FREQ_PANEL_WINDOW_TYPE, &winType);
	
	
	GetCtrlVal(panel, FREQ_PANEL_NR_OF_POINTS, &nrPointsInterval);
	
	char unit[32]="V";
	
	//initializam semnalul filtrat
	filteredSignal = (double *) calloc(nrPointsInterval, sizeof(double));
	
	
	double * autoSpectrum = (double *) calloc (npoints / 2, sizeof(double));
	double *convertedSpectrum = (double *) calloc (npoints / 2, sizeof(double));
	
	double * autoSpectrum2 = (double *) calloc (npoints / 2, sizeof(double));
	double *convertedSpectrum2 = (double *) calloc (npoints / 2, sizeof(double));
	

	int nrOfIntervals =floor( npoints / nrPointsInterval) + 1;
	int nrPointsLeft = npoints % nrPointsInterval;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			second++;//incrementam secunda
			if(second < nrOfIntervals)
				SetCtrlAttribute(panel, FREQ_PANEL_SECOND, ATTR_CTRL_VAL, second);
			
			if(second < nrOfIntervals - 1)
			{
				
				//semnal nefiltrat
				DeleteGraphPlot (panel,  FREQ_PANEL_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);
				
				//afisam nrPointsInterval din semnal inainte de filtrare
				DeleteGraphPlot (panel,  FREQ_PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
				PlotY(panel,FREQ_PANEL_GRAPH, waveData + second * nrPointsInterval, nrPointsInterval, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
	 			
				
				ScaledWindowEx(waveData + second * nrPointsInterval, nrPointsInterval, RECTANGLE, windowParameter, &winConst);
				AutoPowerSpectrum(waveData + second * nrPointsInterval, nrPointsInterval, 1.0 / sampleRate, autoSpectrum, &df);
				PowerFrequencyEstimate(autoSpectrum, nrPointsInterval / 2, searchFreq, winConst, df, 7, &freqPeak, &powerPeak);
				SpectrumUnitConversion(autoSpectrum, nrPointsInterval / 2, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit); 
				
				//afisam spectrul, inainte de filtrare
				PlotY(panel, FREQ_PANEL_GRAPH_FILTERED, convertedSpectrum, nrPointsInterval / 2, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS , VAL_DK_RED);
				
				
				SetCtrlVal(panel, FREQ_PANEL_FREQ_PEAK, freqPeak);
				SetCtrlVal(panel, FREQ_PANEL_POWER_PEAK, powerPeak);
																			 
				//semnal filtrat
				
				//alegem tipul ferestrei 
				
				if(winType == 0)
					HamWin(waveData + second * nrPointsInterval, nrPointsInterval);
				else
					WelchWin(waveData + second * nrPointsInterval, nrPointsInterval);
				
				//vedem ce funtie de filtrare trebuie folosita				   
				if(filterType == 0)
					Bw_LPF(waveData + second * nrPointsInterval, nrPointsInterval, sampleRate, sampleRate / 6, 4, filteredSignal);
				else
					InvCh_LPF(waveData + second * nrPointsInterval, nrPointsInterval, sampleRate, sampleRate / 6, 40.0, 4, filteredSignal);				
				//DebugPrintf("%f", filterType);
				
				DeleteGraphPlot (panel,  FREQ_PANEL_GRAPH_FILTERED_2, -1, VAL_IMMEDIATE_DRAW);
				DeleteGraphPlot (panel,  FREQ_PANEL_GRAPH_SPECTRUM, -1, VAL_IMMEDIATE_DRAW);
				
				//afisam semnalul filtrat
				PlotY(panel,FREQ_PANEL_GRAPH_FILTERED_2, filteredSignal, nrPointsInterval, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				

				AutoPowerSpectrum(filteredSignal, nrPointsInterval, 1.0 / sampleRate, autoSpectrum2, &df2);
				PowerFrequencyEstimate(autoSpectrum2, nrPointsInterval / 2, searchFreq2, winConst2, df2, 7, &freqPeak2, &powerPeak2);
				SpectrumUnitConversion(autoSpectrum2, nrPointsInterval / 2, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df2, winConst2, convertedSpectrum2, unit); 

				//afisam spectrul semnalului filtrat 
				PlotY(panel, FREQ_PANEL_GRAPH_SPECTRUM, convertedSpectrum2, nrPointsInterval / 2, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_RED);
			   
				if(saveGraphs){
					//construim numele fisierului cu imaginea semnalului filtrat
					char fileName[100] = "Signal_";
					if(filterType == 0)
						strcat(fileName, "Butterworth_");
					else
						strcat(fileName, "Chebysev_");
					if(winType == 0)
						strcat(fileName, "Hamming_");
					else
						strcat(fileName, "Welch_");
					
					char sec[5];
					if(sprintf(&sec, "%d", second) == -1)
						perror("");
					else
					{
						strcat(fileName, sec);
					}
					//salvam imaginea
					int imgHandle = 0;
					GetCtrlDisplayBitmap(panel, FREQ_PANEL_GRAPH_FILTERED_2, 1, &imgHandle);
					SaveBitmapToJPEGFile(imgHandle, fileName, JPEG_INTERLACE, 50);
					DiscardBitmap(imgHandle);
					
					//construim numele fisierului cu imaginea spectrului semnalului filtrat
					char fileName2[100] = "Spectrum_";
					if(filterType == 0)
						strcat(fileName2, "Butterworth_");
					else
						strcat(fileName2, "Chebysev_");
					if(winType == 0)
						strcat(fileName2, "Hamming_");
					else
						strcat(fileName2, "Welch_");
					
					
					strcat(fileName2, sec);
					
					//salvam imaginea
					imgHandle = 0;
					GetCtrlDisplayBitmap(panel, FREQ_PANEL_GRAPH_SPECTRUM, 1, &imgHandle);
					SaveBitmapToJPEGFile(imgHandle, fileName2, JPEG_INTERLACE, 50);
					DiscardBitmap(imgHandle);
					
				
				  
					
				}
				
			}
			else if (second == nrOfIntervals - 1){
				
				
				DeleteGraphPlot (panel,  FREQ_PANEL_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);
				//afisam nrPointsLeft din semnal inainte de filtrare
				DeleteGraphPlot (panel,  FREQ_PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
				PlotY(panel,FREQ_PANEL_GRAPH, waveData + second * nrPointsLeft, nrPointsLeft, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
	 			
				
				ScaledWindowEx(waveData + second * nrPointsLeft, nrPointsLeft, RECTANGLE, windowParameter, &winConst);
				AutoPowerSpectrum(waveData + second * nrPointsLeft, nrPointsLeft, 1.0 / sampleRate, autoSpectrum, &df);
				PowerFrequencyEstimate(autoSpectrum, nrPointsLeft / 2, searchFreq, winConst, df, 7, &freqPeak, &powerPeak);
				SpectrumUnitConversion(autoSpectrum, nrPointsLeft / 2, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit); 
				
				//afisam spectrul, inainte de filtrare
				PlotY(panel, FREQ_PANEL_GRAPH_FILTERED, convertedSpectrum, nrPointsLeft / 2, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_RED);
				
				
				SetCtrlVal(panel, FREQ_PANEL_FREQ_PEAK, freqPeak);
				SetCtrlVal(panel, FREQ_PANEL_POWER_PEAK, powerPeak);
				
				
				//semnal filtrat
				
				//alegem tipul ferestrei 
				
				if(winType == 0)
					HamWin(waveData + second * nrPointsInterval, nrPointsLeft);
				else
					WelchWin(waveData + second * nrPointsInterval, nrPointsLeft);
				
				//vedem ce funtie de filtrare trebuie folosita				   
				if(filterType == 0)
					Bw_LPF(waveData + second * nrPointsInterval, nrPointsLeft, sampleRate, sampleRate / 6, 4, filteredSignal);
				else																									    
					InvCh_LPF(waveData + second * nrPointsInterval, nrPointsLeft, sampleRate, sampleRate / 6, 40.0, 4, filteredSignal);
				//DebugPrintf("%f", filterType);
				
				DeleteGraphPlot (panel,  FREQ_PANEL_GRAPH_FILTERED_2, -1, VAL_IMMEDIATE_DRAW);
				DeleteGraphPlot (panel,  FREQ_PANEL_GRAPH_SPECTRUM, -1, VAL_IMMEDIATE_DRAW);
				
				//afisam semnalul filtrat
				PlotY(panel,FREQ_PANEL_GRAPH_FILTERED_2, filteredSignal, nrPointsLeft, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				

				AutoPowerSpectrum(filteredSignal, nrPointsLeft, 1.0 / sampleRate, autoSpectrum2, &df2);
				PowerFrequencyEstimate(autoSpectrum2, nrPointsLeft / 2, searchFreq2, winConst2, df2, 7, &freqPeak2, &powerPeak2);
				SpectrumUnitConversion(autoSpectrum2, nrPointsLeft / 2, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df2, winConst2, convertedSpectrum2, unit); 

				//afisam spectrul semnalului filtrat 
				PlotY(panel, FREQ_PANEL_GRAPH_SPECTRUM, convertedSpectrum2, nrPointsInterval / 2, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_RED);
			   
			}
			else{
				SetCtrlAttribute(panel, FREQ_PANEL_TIMER, ATTR_ENABLED, 0);
			}
			
			
			free(autoSpectrum);
			free(autoSpectrum2);
			free(convertedSpectrum);
			free(convertedSpectrum2);
			break;
	}
	return 0;
}




