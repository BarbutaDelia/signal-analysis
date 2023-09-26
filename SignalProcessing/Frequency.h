//==============================================================================
//
// Title:		Frequency.h
// Purpose:		A short description of the interface.
//
// Created on:	10/12/2021 at 17:21:56 by delia barbuta.
// Copyright:	Universitatea Tehnica „Gheorghe Asachi” din Ia?i. All Rights Reserved.
//
//==============================================================================

#ifndef __Frequency_H__
#define __Frequency_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants

//==============================================================================
// Types
double *waveData;
double sampleRate;
int npoints;
static int freqPanel;
//==============================================================================
// External variables

//==============================================================================
// Global functions

int Declare_Your_Functions_Here (int x);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Frequency_H__ */
