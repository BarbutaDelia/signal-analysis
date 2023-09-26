#ifndef errChk
#define errChk(fCall) if (error = (fCall), error < 0) \
{goto Error;} else
#endif

#ifndef nullChk
#define nullChk(fCall) if ((fCall) == 0) \
{error = UIEOutOfMemory; goto Error;} else
#endif
#include <analysis.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "ProiectAPD.h"
#define SAMPLE_RATE		0
#define NPOINTS			1


//to do 1: vezi cu optiunea implicita pt filtrare, problema cu null pointer
//to do 2: pune delete graph
//to do 3: intreaba de intervale
//to do 4: dealoca memoria!!!
//to do 5: anvelopa semnalului
//to do 6: derivata

int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;
static int mainPanel;
static int freqPanel;
double maximumValue, minimumValue;
int maximumIndex, minimumIndex;
double mean;
long double S = 0;
double median;
double dispersion;
int filterOption = 0;
double alpha = 0;
int nrZeros = 0;
double* filteredData = 0;
int start = 0;
int stop = 0; 
int seconds;
double* envelopeData = 0;

int main (int argc, char *argv[])
{
	
		int error = 0;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (mainPanel = LoadPanel (0, "ProiectAPD.uir", MAIN_PANEL));
	errChk (freqPanel = LoadPanel (0, "ProiectAPD.uir", FREQ_PANEL));
	/* display the panel and run the user interface */
	errChk (DisplayPanel (mainPanel));
	errChk (RunUserInterface ());
Error: 
	/* clean up */
	if (mainPanel > 0)
		DiscardPanel (mainPanel);
	return 0;
}

int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
switch (event)
	{
		case EVENT_COMMIT:
			//executa script python pentru conversia unui fisierului .wav in .txt
			//LaunchExecutable("main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			//Delay(0.5);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			seconds = npoints / sampleRate;
			//afisare pe grapf
			PlotY(panel, MAIN_PANEL_GRAPH, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			//DebugPrintf("%f", sampleRate);
			
			
			//calcul minim, maxim, index min, index max
			MaxMin1D(waveData, npoints, &maximumValue, &maximumIndex, &minimumValue, &minimumIndex);
			
			//calcul medie
			double mean = 0;
			for (int i = 0; i<npoints; i++)
				S = S + waveData[i];
			mean = S/npoints; 
			//DebugPrintf("%f", mean);
			
			//calcul mediana + dispersie
			Median(waveData, npoints, &median);
			dispersion = (S * S) / (npoints - mean * mean);
			
			for (int i = 0; i < npoints; i++)
				if((waveData[i] < 0 && waveData[i+1] > 0) || (waveData[i] > 0 && waveData[i+1] < 0))
					nrZeros ++;
		
		
			
			SetCtrlVal(panel, MAIN_PANEL_MAXIM, maximumValue);
			SetCtrlVal(panel,MAIN_PANEL_MINIM, minimumValue);
			SetCtrlVal(panel,MAIN_PANEL_INDEX_MAXIM, maximumIndex);
			SetCtrlVal(panel,MAIN_PANEL_INDEX_MINIM, minimumIndex);
			SetCtrlVal(panel,MAIN_PANEL_MEDIE, mean);
			SetCtrlVal(panel,MAIN_PANEL_MEDIANA, median);
			SetCtrlVal(panel,MAIN_PANEL_DISPERSIE, dispersion);
			SetCtrlVal(panel, MAIN_PANEL_NR_ZEROS, nrZeros);




			break;
	}
	return 0;
}

int CVICALLBACK OnMainPanel (int panel, int event, void *callbackData,
							 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			free(waveData);
			free(filteredData);
			QuitUserInterface(0);

			break;
	}
	return 0;
}


int CVICALLBACK applyCB (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//mediere pe 16/32 de elemente sau cu element de ordinul 1
			 
			GetCtrlIndex(panel, MAIN_PANEL_FILTER, &filterOption);
			DebugPrintf("%d", filterOption);
			
			double sum = 0;
			
			filteredData = (double*) malloc(npoints * sizeof(double));
			
			if (filterOption == 0)
			{
				
				for(int i = 0; i < 15; i++){
					filteredData[i] = (sum + waveData[i])/(i + 1);
					sum = sum + waveData[i];
				}
				
				for(int i = 15; i<npoints; i++)
				{
					filteredData[i] = 0;
					for(int j = 0; j < 16; j++)
					{
						filteredData[i] = filteredData[i] + waveData[i - j]; 
						
					}
					filteredData[i] = filteredData[i]/16;
				}
				
			}
			else if(filterOption == 1)
			{
				
				for(int i = 0; i < 31; i++)
				{
					filteredData[i] = (sum + waveData[i])/(i + 1);
					sum = sum + waveData[i];
				}
			
				for(int i = 31; i<npoints; i++)
				{
					filteredData[i] = 0;
				for(int j = 0; j < 32; j++)
				{
					filteredData[i] = filteredData[i] + waveData[i - j]; 
					
				}
					filteredData[i] = filteredData[i]/32;
					
				}
			}
				
			else{
				filteredData[0] = waveData[0];
				for(int i = 1; i < npoints; i++)
					filteredData[i] = (1 - alpha) * filteredData[i-1] + alpha * waveData[i];
			}
	
			
			
			
			DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);
			
			PlotY(panel, MAIN_PANEL_GRAPH_FILTERED, filteredData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);

			break;
	}
	
	return 0;
}

int CVICALLBACK applyAlphaCB (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
				GetCtrlVal(panel, MAIN_PANEL_ALPHA, &alpha);
				
			break;
	}
	return 0;
}

int CVICALLBACK prevCB (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panel, MAIN_PANEL_START, &start);
			GetCtrlVal(panel, MAIN_PANEL_STOP, &stop);
			
			int difference = stop - start;
			
			double* partialWaveRaw = (double*) calloc(npoints/seconds * difference, sizeof(double));
			double* partialWaveFiltered = (double*) calloc(npoints/seconds * difference, sizeof(double)); 
			
			if(start >= 1 && stop >= 2)
			{
				stop = start;
				if(stop - difference >= 0)
					start = stop - difference;
				else{
					start = 0;
					difference = stop -start;
				}
			
				SetCtrlVal(panel, MAIN_PANEL_STOP, stop);
				SetCtrlVal(panel, MAIN_PANEL_START, start);
			
				for(int i = 0; i < npoints/seconds * difference; i++)
				{
					partialWaveRaw[i] = waveData[start * npoints / seconds + i];
				}
				
				for(int i = 0; i < npoints/seconds * difference; i++)
				{
					partialWaveFiltered[i] = filteredData[start * npoints / seconds + i];
				}
				
				DeleteGraphPlot(panel, MAIN_PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
				PlotY(panel, MAIN_PANEL_GRAPH, partialWaveRaw, npoints / seconds * difference, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1,VAL_BLUE );
				
				DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);
				PlotY(panel, MAIN_PANEL_GRAPH_FILTERED, partialWaveFiltered, npoints / seconds * difference, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1,VAL_BLUE );
			
			free(partialWaveFiltered);
			free(partialWaveRaw);
			}
			break;
	}
	return 0;
}

int CVICALLBACK nextCB (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, MAIN_PANEL_START, &start);
			GetCtrlVal(panel, MAIN_PANEL_STOP, &stop); 
			
			int difference = stop - start;
			
			double* partialWaveFiltered = (double*) calloc(npoints * difference/seconds, sizeof(double));
			double* partialWaveRaw = (double*) calloc(npoints * difference/seconds, sizeof(double));

			if(start <= 4 && stop <= 5)
			{
				start = stop;
				if(difference + stop <= seconds)
					stop = stop + difference;
				else{
					stop = seconds;
					difference = stop - start;
				}
			
				SetCtrlVal(panel, MAIN_PANEL_STOP, stop);
				SetCtrlVal(panel, MAIN_PANEL_START, start);
			
				for(int i = 0; i < npoints/seconds * difference ; i++)
				{
					partialWaveFiltered[i] = filteredData[start * npoints/seconds + i];
				}
															 
			for(int i = 0; i < npoints/seconds * difference; i++)
			{
				partialWaveRaw[i] = waveData[start * npoints / seconds + i];
			}
			
			DeleteGraphPlot(panel, MAIN_PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
			PlotY(panel, MAIN_PANEL_GRAPH, partialWaveRaw, npoints / seconds * difference, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1,VAL_BLUE );

			
			DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);
			PlotY(panel, MAIN_PANEL_GRAPH_FILTERED, partialWaveFiltered, npoints / seconds * difference, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1,VAL_BLUE );
			
			free(partialWaveFiltered);
			free(partialWaveRaw);
			}

			break;
	}
	return 0;
}

int CVICALLBACK histogramCB (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);
			int intervals = 1000;
			double axisP[intervals];
			ssize_t histogram[intervals];
			Histogram(waveData, npoints, minimumValue, maximumValue, histogram, axisP, intervals);
			PlotXY(panel,MAIN_PANEL_GRAPH_FILTERED, axisP, histogram, intervals, VAL_DOUBLE, VAL_SSIZE_T, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_CYAN);
			
		
			break;
	}
	return 0;
}

int CVICALLBACK saveCB (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			char* fileName;
			char* fileName2;
			int fileNameLength;
			GetCtrlAttribute(panel,MAIN_PANEL_FILE_NAME, ATTR_STRING_TEXT_LENGTH, &fileNameLength);
			fileName = malloc ((fileNameLength + 1) * sizeof (char));
			GetCtrlVal (panel, MAIN_PANEL_FILE_NAME, fileName);
			
			int bitmapId1 = 0;
			GetCtrlDisplayBitmap(panel, MAIN_PANEL_GRAPH, 4, &bitmapId1);
			SaveBitmapToBMPFile(bitmapId1, fileName);
			
			fileName2 = malloc ((fileNameLength + 2) * sizeof (char));
			strncpy(fileName2, fileName, fileNameLength + 1);
			
			strcat(fileName2, "2");
			int bitmapId2 = 0;
			GetCtrlDisplayBitmap(panel, MAIN_PANEL_GRAPH_FILTERED, 3, &bitmapId2);
			SaveBitmapToBMPFile(bitmapId2, fileName2);
			
			free(fileName);
			free(fileName2);
			break;
	}
	return 0;
}

int CVICALLBACK envelopeCB (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			envelopeData = (double *) calloc(npoints, sizeof(double));
			FileToArray("envelopeData.txt", envelopeData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			PlotY(panel, MAIN_PANEL_GRAPH, envelopeData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLACK);
			
			free(envelopeData);

			break;
	}
	return 0;
}

int CVICALLBACK derivativeCB (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			double initial[1] = {0};
			double final[1] = {0};

			
			double* derivativeData =  (double *) calloc(npoints, sizeof(double));
			//Difference(waveData, npoints, 6, 0, npoints, derivativeData);
			DifferenceEx(waveData, npoints, 1. / sampleRate , initial, 1, final , 1, DIFF_SECOND_ORDER_CENTRAL, derivativeData);
			DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);
			PlotY(panel, MAIN_PANEL_GRAPH_FILTERED, derivativeData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_MAGENTA);
			//samplinginterval e perioada de esantionare 
			free(derivativeData);
			break;
	}
	return 0;
}

int CVICALLBACK switchPanelCB (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
		switch (event)
	{
		case EVENT_COMMIT:
			if(panel == mainPanel)
			{
				SetCtrlVal(freqPanel, FREQ_PANEL_SWITCHPANEL_FQ, 1);
				DisplayPanel(freqPanel);
				HidePanel(panel);
			}
			else
			{
				SetCtrlVal(mainPanel, MAIN_PANEL_SWITCHPANEL, 0);
				DisplayPanel(mainPanel);
				HidePanel(panel);
			}
			break;
	}
	return 0;
}
