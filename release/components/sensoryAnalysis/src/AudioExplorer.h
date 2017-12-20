/*
20161214
	modified
*/
#pragma once

#include <d2d1.h>

// For IMediaObject and related interfaces
#include <dmo.h>

// For configuring DMO properties
#include <wmcodecdsp.h>


// For WAVEFORMATEX
#include <mmreg.h>

// For FORMAT_WaveFormatEx and such
#include <uuids.h>

// For Kinect SDK APIs
#include <NuiApi.h>

#include <Shlobj.h>


// For StringCch* and such
//#include <strsafe.h>

// For INT_MAX
//#include <limits.h>

//#include "Utilities.h"

#include <iostream>



// Format of Kinect audio stream
static const WORD       AudioFormat = WAVE_FORMAT_PCM;

// Number of channels in Kinect audio stream
static const WORD       AudioChannels = 1;

// Samples per second in Kinect audio stream
static const DWORD      AudioSamplesPerSecond = 16000;

// Average bytes per second in Kinect audio stream
static const DWORD      AudioAverageBytesPerSecond = 32000;

// Block alignment in Kinect audio stream
static const WORD       AudioBlockAlign = 2;

// Bits per audio sample in Kinect audio stream
static const WORD       AudioBitsPerSample = 16;


/// <summary>
/// IMediaBuffer implementation for a statically allocated buffer.
/// </summary>
class CStaticMediaBuffer : public IMediaBuffer
{
public:
    // Constructor
    CStaticMediaBuffer() : m_dataLength(0) {}

    // IUnknown methods
    STDMETHODIMP_(ULONG) AddRef() { return 2; }
    STDMETHODIMP_(ULONG) Release() { return 1; }
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
        if (riid == IID_IUnknown)
        {
            AddRef();
            *ppv = (IUnknown*)this;
            return NOERROR;
        }
        else if (riid == IID_IMediaBuffer)
        {
            AddRef();
            *ppv = (IMediaBuffer*)this;
            return NOERROR;
        }
        else
        {
            return E_NOINTERFACE;
        }
    }

    // IMediaBuffer methods
    STDMETHODIMP SetLength(DWORD length) {m_dataLength = length; return NOERROR;}
    STDMETHODIMP GetMaxLength(DWORD *pMaxLength) {*pMaxLength = sizeof(m_pData); return NOERROR;}
    STDMETHODIMP GetBufferAndLength(BYTE **ppBuffer, DWORD *pLength)
    {
        if (ppBuffer)
        {
            *ppBuffer = m_pData;
        }
        if (pLength)
        {
            *pLength = m_dataLength;
        }
        return NOERROR;
    }
    void Init(ULONG ulData)
    {
        m_dataLength = ulData;
    }

protected:
    // Statically allocated buffer used to hold audio data returned by IMediaObject
    BYTE m_pData[AudioSamplesPerSecond * AudioBlockAlign];

    // Amount of data currently being held in m_pData
    ULONG m_dataLength;
};

/// <summary>
/// Main application class for AudioExplorer sample.
/// </summary>

