/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  FREQ_PANEL                       1       /* callback function: onFreqPanelCB */
#define  FREQ_PANEL_SWITCHPANEL_FQ        2       /* control type: binary, callback function: switchPanelCB */
#define  FREQ_PANEL_GRAPH_SPECTRUM        3       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_FILTERED_2      4       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_FILTERED        5       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH                 6       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_SECOND                7       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_START_TIMER_BT        8       /* control type: textButton, callback function: startTimerCB */
#define  FREQ_PANEL_FREQ_PEAK             9       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_POWER_PEAK            10      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_NR_OF_POINTS          11      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_TIMER                 12      /* control type: timer, callback function: timerCB */
#define  FREQ_PANEL_WINDOW_TYPE           13      /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_FILTER_TYPE           14      /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_SAVE_GRAPHS           15      /* control type: radioButton, callback function: (none) */

#define  MAIN_PANEL                       2       /* callback function: OnMainPanel */
#define  MAIN_PANEL_BINARYSWITCH          2       /* control type: binary, callback function: OnLoadButtonCB */
#define  MAIN_PANEL_GRAPH_FILTERED        3       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_GRAPH                 4       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_MAXIM                 5       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_INDEX_MAXIM           6       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_STOP                  7       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_START                 8       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_INDEX_MINIM           9       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MEDIANA               10      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NR_ZEROS              11      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_DISPERSIE             12      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MEDIE                 13      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MINIM                 14      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_FILTER                15      /* control type: ring, callback function: (none) */
#define  MAIN_PANEL_ALPHA                 16      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NEXT                  17      /* control type: command, callback function: nextCB */
#define  MAIN_PANEL_PREV                  18      /* control type: command, callback function: prevCB */
#define  MAIN_PANEL_APPLY_ALPHA           19      /* control type: command, callback function: applyAlphaCB */
#define  MAIN_PANEL_APPLY                 20      /* control type: command, callback function: applyCB */
#define  MAIN_PANEL_DERIVATIVE            21      /* control type: command, callback function: derivativeCB */
#define  MAIN_PANEL_ENVELOPE              22      /* control type: command, callback function: envelopeCB */
#define  MAIN_PANEL_HISTOGRAM             23      /* control type: command, callback function: histogramCB */
#define  MAIN_PANEL_SAVE                  24      /* control type: command, callback function: saveCB */
#define  MAIN_PANEL_FILE_NAME             25      /* control type: string, callback function: (none) */
#define  MAIN_PANEL_SWITCHPANEL           26      /* control type: binary, callback function: switchPanelCB */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK applyAlphaCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK applyCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK derivativeCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK envelopeCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK histogramCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK nextCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onFreqPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnMainPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK prevCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK saveCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK startTimerCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK switchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK timerCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
