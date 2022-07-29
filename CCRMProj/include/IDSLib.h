#pragma once
// MathLibrary.h - Contains declarations of math functions
#pragma once

#ifdef IDS_EXPORTS
#define IDS_API __declspec(dllexport)
#else
#define IDS_API __declspec(dllimport)
#endif

#include "stdafx.h"
#include <iostream>
#include <vector> // for 2D vector 
#include "uEye.h"

using namespace std;




// ----------------------------------------------------------------------------
// function exports
// ----------------------------------------------------------------------------
#ifdef __LINUX__
IDSEXP is_WaitEvent(HIDS hCam, INT which, INT nTimeout);
#endif

//-------------------------------------------------------------------------
//common function
//-------------------------------------------------------------------------
extern "C" IDS_API void     Is_SetSaturation(HIDS hCam, INT ChromU, INT ChromV, INT Options);
extern "C" IDS_API void     Is_PrepareStealVideo(HIDS hCam, int Mode, ULONG StealColorMode, INT Options);
extern "C" IDS_API void     Is_StopLiveVideo(HIDS hCam, INT Wait, INT Options);
extern "C" IDS_API void     Is_FreezeVideo(HIDS hCam, INT Wait, INT Options);
extern "C" IDS_API void     Is_CaptureVideo(HIDS hCam, INT Wait, INT Options);
extern "C" IDS_API void     Is_IsVideoFinish(HIDS hCam, INT* pValue, INT Options);
extern "C" IDS_API void     Is_HasVideoStarted(HIDS hCam, BOOL* pbo, INT Options);
extern "C" IDS_API void     Is_AllocImageMem(HIDS hCam, INT width, INT height, INT bitspixel, char** ppcImgMem, int* pid, INT Options);
extern "C" IDS_API void     Is_SetImageMem(HIDS hCam, char* pcMem, int id, INT Options);
extern "C" IDS_API void     Is_FreeImageMem(HIDS hCam, char* pcMem, int id, INT Options);
extern "C" IDS_API void     Is_GetImageMem(HIDS hCam, VOID** pMem, INT Options);
extern "C" IDS_API void     Is_GetActiveImageMem(HIDS hCam, char** ppcMem, int* pnID, INT Options);
extern "C" IDS_API void     Is_InquireImageMem(HIDS hCam, char* pcMem, int nID, int* pnX, int* pnY, int* pnBits, int* pnPitch, INT Options);
extern "C" IDS_API void     Is_GetImageMemPitch(HIDS hCam, INT* pPitch, INT Options);
extern "C" IDS_API void     Is_SetAllocatedImageMem(HIDS hCam, INT width, INT height, INT bitspixel, char* pcImgMem, int* pid, INT Options);
extern "C" IDS_API void     Is_CopyImageMem(HIDS hCam, char* pcSource, int nID, char* pcDest, INT Options);
extern "C" IDS_API void     Is_CopyImageMemLines(HIDS hCam, char* pcSource, int nID, int nLines, char* pcDest, INT Options);
extern "C" IDS_API void     Is_AddToSequence(HIDS hCam, char* pcMem, INT nID, INT Options);
extern "C" IDS_API void     Is_ClearSequence(HIDS hCam, INT Options);
extern "C" IDS_API void     Is_GetActSeqBuf(HIDS hCam, INT* pnNum, char** ppcMem, char** ppcMemLast, INT Options);
extern "C" IDS_API void     Is_LockSeqBuf(HIDS hCam, INT nNum, char* pcMem, INT Options);
extern "C" IDS_API void     Is_UnlockSeqBuf(HIDS hCam, INT nNum, char* pcMem, INT Options);
extern "C" IDS_API void     Is_GetError(HIDS hCam, INT* pErr, IS_CHAR** ppcErr, INT Options);
extern "C" IDS_API void     Is_SetErrorReport(HIDS hCam, INT Mode, INT Options);
extern "C" IDS_API void     Is_SetColorMode(HIDS hCam, INT Mode, INT Options);
extern "C" IDS_API void     Is_GetColorDepth(HIDS hCam, INT* pnCol, INT* pnColMode, INT Options);
extern "C" IDS_API void     Is_RenderBitmap(HIDS hCam, INT nMemID, HWND hwnd, INT nMode, INT Options);
extern "C" IDS_API void     Is_SetDisplayMode(HIDS hCam, INT Mode, INT Options);
extern "C" IDS_API void     Is_SetDisplayPos(HIDS hCam, INT x, INT y, INT Options);
extern "C" IDS_API void     Is_SetHwnd(HIDS hCam, HWND hwnd, INT Options);
extern "C" IDS_API void     Is_GetVsyncCount(HIDS hCam, long* pIntr, long* pActIntr, INT Options);
extern "C" IDS_API void     Is_InitEvent(HIDS hCam, HANDLE hEv, INT which, INT Options);
extern "C" IDS_API void     Is_ExitEvent(HIDS hCam, INT which, INT Options);
extern "C" IDS_API void     Is_EnableEvent(HIDS hCam, INT which, INT Options);
extern "C" IDS_API void     Is_DisableEvent(HIDS hCam, INT which, INT Options);
extern "C" IDS_API void     Is_SetExternalTrigger(HIDS hCam, INT nTriggerMode, INT Options);
extern "C" IDS_API void     Is_SetTriggerCounter(HIDS hCam, INT nValue, INT Options);
extern "C" IDS_API void     Is_SetRopEffect(HIDS hCam, INT effect, INT param, INT reserved, INT Options);
extern "C" IDS_API void     Is_InitCamera(HIDS* phCam, HWND hWnd, INT Options);
extern "C" IDS_API void     Is_ExitCamera(HIDS hCam, INT Options);
extern "C" IDS_API void     Is_GetCameraInfo(HIDS hCam, PCAMINFO pInfo, INT Options);
extern "C" IDS_API void     Is_GetCameraStatus(HIDS hCam, INT Options);
extern "C" IDS_API void     Is_captureFrame(HIDS hCam, INT Options);
extern "C" IDS_API void     Is_captureSingleFrame(HIDS hCam, INT Options);
extern "C" IDS_API void     Is_ShowLive(HIDS hCam, INT Options);
extern "C" IDS_API void     Is_ShowLive_Window(HIDS hCam, INT Options, int img_w, int img_h);
extern "C" IDS_API void     Is_RecoverParams(HIDS hCam, INT Options);
extern "C" IDS_API void     Is_StopLive(HIDS hCam, INT Options);
extern "C" IDS_API void     Is_GetCameraType(HIDS hCam, INT Options);
extern "C" IDS_API void     Is_GetNumberOfCameras(INT* pnNumCams, INT Options);
extern "C" IDS_API void     Is_GetUsedBandwidth(HIDS hCam, INT Options);
extern "C" IDS_API void     Is_GetFrameTimeRange(HIDS hCam, double *min, double *max, double *intervall, INT Options);
extern "C" IDS_API void     Is_SetFrameRate(HIDS hCam, double FPS, double* newFPS, INT Options);
extern "C" IDS_API void     Is_GetFramesPerSecond(HIDS hCam, double *dblFPS, INT Options);
extern "C" IDS_API void     Is_GetSensorInfo(HIDS hCam, PSENSORINFO pInfo, INT Options);
extern "C" IDS_API void     Is_GetRevisionInfo(HIDS hCam, PREVISIONINFO prevInfo, INT Options);
extern "C" IDS_API void     Is_EnableAutoExit(HIDS hCam, INT nMode, INT Options);
extern "C" IDS_API void     Is_EnableMessage(HIDS hCam, INT which, HWND hWnd, INT Options);
extern "C" IDS_API void     Is_SetHardwareGain(HIDS hCam, INT nMaster, INT nRed, INT nGreen, INT nBlue, INT Options);
extern "C" IDS_API void     Is_SetWhiteBalance(HIDS hCam, INT nMode, INT Options);
extern "C" IDS_API void     Is_SetWhiteBalanceMultipliers(HIDS hCam, double dblRed, double dblGreen, double dblBlue, INT Options);
extern "C" IDS_API void     Is_GetWhiteBalanceMultipliers(HIDS hCam, double *pdblRed, double *pdblGreen, double *pdblBlue, INT Options);
extern "C" IDS_API void     Is_SetColorCorrection(HIDS hCam, INT nEnable, double *factors, INT Options);
extern "C" IDS_API void     Is_SetSubSampling(HIDS hCam, INT mode, INT Options);
extern "C" IDS_API void     Is_ForceTrigger(HIDS hCam, INT Options);
// with driver version 1.12.0006
extern "C" IDS_API void     Is_GetBusSpeed(HIDS hCam, INT Options);
// with driver version 1.12.0015
extern "C" IDS_API void     Is_SetBinning(HIDS hCam, INT mode, INT Options);
// with driver version 1.12.0017
extern "C" IDS_API void     Is_ResetToDefault(HIDS hCam, INT Options);
// with driver version 1.14.0008
extern "C" IDS_API void     Is_SetCameraID(HIDS hCam, INT nID, INT Options);
extern "C" IDS_API void     Is_SetBayerConversion(HIDS hCam, INT nMode, INT Options);
// with driver version 1.14.0009
extern "C" IDS_API void     Is_SetHardwareGamma(HIDS hCam, INT nMode, INT Options);
// with driver version 2.00.0001
extern "C" IDS_API void     Is_GetCameraList(PUEYE_CAMERA_LIST pucl, INT Options);
extern "C" IDS_API void     Is_SetAutoParameter(HIDS hCam, INT param, double *pval1, double *pval2, INT Options);
extern "C" IDS_API void     Is_GetAutoInfo(HIDS hCam, UEYE_AUTO_INFO *pInfo, INT Options);
extern "C" IDS_API void     Is_GetImageHistogram(HIDS hCam, int nID, INT ColorMode, DWORD* pHistoMem, INT Options);
extern "C" IDS_API void     Is_SetTriggerDelay(HIDS hCam, INT nTriggerDelay, INT Options);
//with driver version 2.21.0000
extern "C" IDS_API void     Is_SetGainBoost(HIDS hCam, INT mode, INT Options);
extern "C" IDS_API void     Is_SetGlobalShutter(HIDS hCam, INT mode, INT Options);
extern "C" IDS_API void     Is_SetExtendedRegister(HIDS hCam, INT index, WORD value, INT Options);
extern "C" IDS_API void     Is_GetExtendedRegister(HIDS hCam, INT index, WORD *pwValue, INT Options);
//with driver version 2.22.0002
extern "C" IDS_API void     Is_SetHWGainFactor(HIDS hCam, INT nMode, INT nFactor, INT Options);
extern "C" IDS_API void     Is_Renumerate(HIDS hCam, INT nMode, INT Options);
extern "C" IDS_API void     Is_WriteI2C(HIDS hCam, INT nDeviceAddr, INT nRegisterAddr, BYTE* pbData, INT nLen, INT Options);
extern "C" IDS_API void     Is_ReadI2C(HIDS hCam, INT nDeviceAddr, INT nRegisterAddr, BYTE* pbData, INT nLen, INT Options);



