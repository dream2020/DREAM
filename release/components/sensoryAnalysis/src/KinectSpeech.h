/*
20161214
*/
#ifndef _KINECT_SPEECH_CLASS
#define _KINECT_SPEECH_CLASS

#include <iostream>
#include <string>
#include <time.h>
#include <opencv2/opencv.hpp> 
#include <windows.h>
#include "NuiApi.h"

////speech recognition
#include "AudioExplorer.h"
#include "KinectAudioStream.h"
#include <wmcodecdsp.h>
#include <uuids.h>
#include <sapi.h>
#include <sphelper.h>
#include <Shlobj.h>
#include <mmreg.h>
#include "StdAfx.h"

//////////////kinect audio speech////////////////
#include <shlobj.h>
#include <wchar.h>
#include <devicetopology.h>
#include "WASAPICapture.h"
////////////////////////////////////////


using namespace std;


class KinectSpeech
{
public:
	HRESULT hr;

	IMMDevice *device;
	HANDLE waveFile;
	CWASAPICapture *capturer;

	INuiSensor *m_pNuiSensor; //the first kinect, assume to be the middle kinect	

	//for speech recognition
	LPCWSTR          GrammarFileName;
	KinectAudioStream*      m_pKinectAudioStream;	
	INuiAudioBeam*          m_pNuiAudioSource;
	//IPropertyStore*         m_pPropertyStore;
	ISpStream*              m_pSpeechStream; // Stream given to speech recognition engine
	ISpRecognizer*          m_pSpeechRecognizer; //// Speech recognizer
	ISpRecoContext*         m_pSpeechContext;// Speech recognizer context
	ISpRecoGrammar*         m_pSpeechGrammar; // Speech grammar	
	HANDLE                  m_hSpeechEvent; // Event triggered when we detect speech recognition

	CStaticMediaBuffer      m_captureBuffer;
	// Media object from which Kinect audio stream is captured

	///////////////////////////////////speech audio/////////////////////////
	HRESULT GetGlobalId(IMMDevice *pDevice, wchar_t **ppszGlobalId);
	bool IsMatchingAudioDevice(INuiSensor *pNuiSensor, wchar_t *pszGlobalId);
	HRESULT WriteWaveHeader(HANDLE waveFile, const WAVEFORMATEX *pWaveFormat, DWORD dataSize);
	HRESULT CaptureAudio(CWASAPICapture *capturer, HANDLE waveFile, const wchar_t *waveFileName);
	HRESULT GetMatchingAudioDevice(INuiSensor *pNuiSensor, IMMDevice **ppDevice);
	bool StartAudio_C(string pathname, string audioname);
	bool StopAudio_C();
	bool LoadGrammar(string speech_filename, INuiSensor *sensorp);

	bool savingflag;
public:
	KinectSpeech();
	~KinectSpeech();

	//for speech recognition
	HRESULT                 InitializeAudioStream(); /// Initialize Kinect audio stream object.
	HRESULT                 CreateSpeechRecognizer();/// Create speech recognizer that will read Kinect audio stream data. 
	HRESULT                 LoadSpeechGrammar(); /// Load speech recognition grammar into recognizer.    
	HRESULT                 StartSpeechRecognition();/// Start recognizing speech asynchronously. 
	bool                    ProcessSpeech(string &outputstring);
};
//extern KinectSpeech kinectspeech;

#endif