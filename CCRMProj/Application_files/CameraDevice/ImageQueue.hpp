#include "stdafx.h"
#pragma once
//===========================================================================//
//                                                                           //
//  Copyright (C) 2004 - 2018                                                //
//  IDS Imaging GmbH                                                         //
//  Dimbacherstr. 6-8                                                        //
//  D-74182 Obersulm-Willsbach                                               //
//                                                                           //
//  The information in this document is subject to change without            //
//  notice and should not be construed as a commitment by IDS Imaging GmbH.  //
//  IDS Imaging GmbH does not assume any responsibility for any errors       //
//  that may appear in this document.                                        //
//                                                                           //
//  This document, or source code, is provided solely as an example          //
//  of how to utilize IDS software libraries in a sample application.        //
//  IDS Imaging GmbH does not assume any responsibility for the use or       //
//  reliability of any portion of this document or the described software.   //
//                                                                           //
//  General permission to copy or modify, but not for profit, is hereby      //
//  granted,  provided that the above copyright notice is included and       //
//  reference made to the fact that reproduction privileges were granted	 //
//	by IDS Imaging GmbH.				                                     //
//                                                                           //
//  IDS cannot assume any responsibility for the use or misuse of any        //
//  portion of this software for other than its intended diagnostic purpose	 //
//  in calibrating and testing IDS manufactured cameras and software.		 //
//                                                                           //
//===========================================================================//


// uEyeImageQueueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uEyeImageQueue.h"
#include "uEyeImageQueueDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include <process.h>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CuEyeImageQueueDlg dialog

// CuEyeImageQueueDlg dialog
unsigned WINAPI threadProcImageQueue(void* pv);



// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.



class ImageQu : public CDialogEx
{


private:

	  // camera variables
	  HIDS	m_hCam;					// handle to camera
	  HWND	m_hWndDisplayLive;		// handle to display window
	  INT		m_nColorMode;			// Y8/RGB16/RGB24/REG32
	  INT		m_nBitsPerPixel;		// number of bits needed store one pixel
	  INT		m_nSizeX;				// width of video 
	  INT		m_nSizeY;				// height of video
	  SENSORINFO m_sInfo;				// sensor information struct


	  // memory and sequence buffers
	  std::vector<INT> m_viSeqMemId;		// camera memory - buffer IDs
	  std::vector<char*> m_vpcSeqImgMem;	// camera memory - pointers to buffer
	  int		m_nNumberOfBuffers;			// the number of buffers we use


	  // image acquisition thread variables
	  HANDLE  m_hThreadImageQueue;
	  BOOL    m_boRunThreadImageQueue;
	  BOOL    m_boTerminatedThreadImageQueue;
	  DWORD   m_dwThreadIDImageQueue;


public:
	  INT InitCamera(HIDS *hCam, HWND hWnd)
	  {
			INT nRet = is_InitCamera(hCam, hWnd);
			/************************************************************************************************/
			/*                                                                                              */
			/*  If the camera returns with "IS_STARTER_FW_UPLOAD_NEEDED", an upload of a new firmware       */
			/*  is necessary. This upload can take several seconds. We recommend to check the required      */
			/*  time with the function is_GetDuration().                                                    */
			/*                                                                                              */
			/*  In this case, the camera can only be opened if the flag "IS_ALLOW_STARTER_FW_UPLOAD"        */
			/*  is "OR"-ed to m_hCam. This flag allows an automatic upload of the firmware.                 */
			/*                                                                                              */
			/************************************************************************************************/
			if (nRet == IS_STARTER_FW_UPLOAD_NEEDED)
			{
				  // Time for the firmware upload = 25 seconds by default
				  INT nUploadTime = 25000;
				  is_GetDuration(*hCam, IS_STARTER_FW_UPLOAD, &nUploadTime);

				  CString Str1, Str2, Str3;
				  Str1 = _T("This camera requires a new firmware. The upload will take about");
				  Str2 = _T("seconds. Please wait ...");
				  Str3.Format(_T("%s %d %s"), Str1, nUploadTime / 1000, Str2);
				  AfxMessageBox(Str3, MB_ICONWARNING);

				  // Set mouse to hourglass
				  SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

				  // Try again to open the camera. This time we allow the automatic upload of the firmware by
				  // specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
				  *hCam = (HIDS)(((INT)*hCam) | IS_ALLOW_STARTER_FW_UPLOAD);
				  nRet = is_InitCamera(hCam, hWnd);
			}

			return nRet;
	  }



	  ///////////////////////////////////////////////////////////////////////////////
	  //
	  // METHOD CuEyeImageQueueDlg::CamOpen() 
	  //
	  // DESCRIPTION: Opens a handle to a connected camera
	  //				Init the image queue
	  //				Run the image capture
	  //
	  ///////////////////////////////////////////////////////////////////////////////
	  bool CamOpen()
	  {
			// variables
			INT nRet;

			if (m_hCam != 0)
				  is_ExitCamera(m_hCam);

			// init camera
			m_hCam = (HIDS)0;  // open next camera
			nRet = InitCamera(&m_hCam, NULL);
			if (nRet == IS_SUCCESS)
			{
				  // get sensor info
				  is_GetSensorInfo(m_hCam, &m_sInfo);

				  // enable/disable the dialog based error report
				  nRet = is_SetErrorReport(m_hCam, IS_DISABLE_ERR_REP); // or IS_ENABLE_ERR_REP);
				  if (nRet != IS_SUCCESS)
				  {
						AfxMessageBox(_T("ERROR: Can not enable the automatic uEye error report!"), MB_ICONEXCLAMATION, 0);
						return false;
				  }

				  // query camera information
				  SENSORINFO SensorInfo;
				  is_GetSensorInfo(m_hCam, &SensorInfo);
				  CAMINFO CamInfo;
				  is_GetCameraInfo(m_hCam, &CamInfo);


				  // use color depth according to monochrome or color camera
				  if (SensorInfo.nColorMode == IS_COLORMODE_MONOCHROME)
				  {
						// monochrome camera
						m_nBitsPerPixel = 8;
						m_nColorMode = IS_CM_MONO8;
				  }
				  else
				  {
						// color camera
						m_nBitsPerPixel = 24;
						m_nColorMode = IS_CM_BGR8_PACKED;
				  }
				  is_SetColorMode(m_hCam, m_nColorMode);


				  // display initialization
				 
				  is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);


				  // allocate memory and built the sequence
				  CamSeqBuild();


				  // run the image queue
				  CamRunImageQueue();
				  is_CaptureVideo(m_hCam, IS_DONT_WAIT);



			}
			else
			{
				  AfxMessageBox(_T("ERROR: Cannot open uEye camera!"), MB_ICONEXCLAMATION, 0);
				  PostQuitMessage(0);
			}

			return true;
	  }



	  void OnBnClickedButtonExit()
	  {
			// stop image acquisition and related threads
			CamTerminateImageQueue();

			// memory and events are automatically released
			is_ExitCamera(m_hCam);

			// terminate
			PostQuitMessage(0);
	  }




	  ///////////////////////////////////////////////////////////////////////////////
	  //
	  // METHOD:      CuEyeImageQueueDlg::CamSeqBuilt()
	  //
	  // DESCRIPTION: Built a sequence for acquisition into the sequence buffer
	  //				The number of buffers covers 1 second
	  //				At least 3 buffers are used
	  //				Use the image queue for save acquisition
	  //
	  ///////////////////////////////////////////////////////////////////////////////
	  bool CamSeqBuild()
	  {
			// variables
			bool bRet = false;
			INT nRet;
			int i;


			// how many buffers are required?
			double FrameTimeMin, FrameTimeMax, FrameTimeIntervall;
			nRet = is_GetFrameTimeRange(m_hCam, &FrameTimeMin, &FrameTimeMax, &FrameTimeIntervall);
			if (nRet == IS_SUCCESS)
			{
				  double maxBuffers;
				  maxBuffers = (1.0f / FrameTimeMin) + 0.5f;
				  m_nNumberOfBuffers = (int)(maxBuffers);

				  if (m_nNumberOfBuffers < 3)
				  {
						m_nNumberOfBuffers = 3;
				  }

			}
			else
				  return false;


			// calculate the image buffer width and height , watch if an (absolute) AOI is used
			IS_SIZE_2D imageSize;
			is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));
			INT nAllocSizeX = 0;
			INT nAllocSizeY = 0;
			m_nSizeX = nAllocSizeX = imageSize.s32Width;
			m_nSizeY = nAllocSizeY = imageSize.s32Height;
			UINT nAbsPosX = 0;
			UINT nAbsPosY = 0;
			is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_X_ABS, (void*)&nAbsPosX, sizeof(nAbsPosX));
			is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_Y_ABS, (void*)&nAbsPosY, sizeof(nAbsPosY));
			if (nAbsPosX)
			{
				  nAllocSizeX = m_sInfo.nMaxWidth;
			}
			if (nAbsPosY)
			{
				  nAllocSizeY = m_sInfo.nMaxHeight;
			}


			// allocate buffers (memory) in a loop
			for (i = 0; i < m_nNumberOfBuffers; i++)
			{
				  INT iImgMemID = 0;
				  char* pcImgMem = 0;

				  // allocate a single buffer memory
				  nRet = is_AllocImageMem(m_hCam,
						nAllocSizeX,
						nAllocSizeY,
						m_nBitsPerPixel,
						&pcImgMem,
						&iImgMemID);
				  if (nRet != IS_SUCCESS)
				  {
						break;  // it makes no sense to continue
				  }

				  // put memory into the sequence buffer management
				  nRet = is_AddToSequence(m_hCam, pcImgMem, iImgMemID);
				  if (nRet != IS_SUCCESS)
				  {
						// free latest buffer
						is_FreeImageMem(m_hCam, pcImgMem, iImgMemID);
						break;  // it makes no sense to continue
				  }

				  m_viSeqMemId.push_back(iImgMemID);
				  m_vpcSeqImgMem.push_back(pcImgMem);

			}

			// store current number buffers in case we did not match to get the desired number
			m_nNumberOfBuffers = i;

			// enable the image queue
			nRet = is_InitImageQueue(m_hCam, 0);
			if (nRet == IS_SUCCESS)
			{
				  // we got buffers in the image queue
				  if (m_nNumberOfBuffers >= 3)
						bRet = true;
			}


			return bRet;
	  }




	  ///////////////////////////////////////////////////////////////////////////////
	  //
	  // METHOD:      CuEyeImageQueueDlg::CamSeqKill()
	  //
	  // DESCRIPTION: Release the sequence and memory of the image buffers
	  //
	  ///////////////////////////////////////////////////////////////////////////////
	  bool CamSeqKill()
	  {
			// exit image queue and release buffers from sequence
			is_ExitImageQueue(m_hCam);
			is_ClearSequence(m_hCam);

			// free buffers memory
			int i;
			for (i = (m_nNumberOfBuffers - 1); i >= 0; i--)
			{
				  // free buffers
				  if (is_FreeImageMem(m_hCam, m_vpcSeqImgMem.at(i), m_viSeqMemId.at(i)) != IS_SUCCESS)
				  {
						return false;
				  }
			}

			// no valid buffers any more
			m_viSeqMemId.clear();
			m_vpcSeqImgMem.clear();
			m_nNumberOfBuffers = 0;

			return true;
	  }



	  ///////////////////////////////////////////////////////////////////////////////
	  //
	  // METHOD:      CuEyeImageQueueDlg::CamRunImageQueue()
	  //
	  // DESCRIPTION: Install the image queue thread and run it
	  //
	  ///////////////////////////////////////////////////////////////////////////////
	  bool CamRunImageQueue()
	  {
			// variables
			bool bRet = false;

			// create image queue thread
			m_boRunThreadImageQueue = TRUE;
			m_hThreadImageQueue = (HANDLE)_beginthreadex(NULL, 0, threadProcImageQueue, (void*)this, 0, (UINT*)&m_dwThreadIDImageQueue);
			if (m_hThreadImageQueue == NULL)
			{
				  AfxMessageBox(_T("ERROR: Cannot create image queue thread!"), MB_ICONEXCLAMATION, 0);
				  m_boRunThreadImageQueue = FALSE;
			}
			else
			{
				  // image queue thread must now be active
				  bRet = true;
			}


			// we really shouldn't do that !!!!
			//SetThreadPriority (m_hThreadEvent, THREAD_PRIORITY_TIME_CRITICAL);


			return bRet;
	  }



	  ///////////////////////////////////////////////////////////////////////////////
	  //
	  // METHOD:      CuEyeImageQueueDlg::CamTerminateImageQueue()
	  //
	  // DESCRIPTION: Stop the image acquisition
	  //				Terminate the image queue thread.
	  //
	  ///////////////////////////////////////////////////////////////////////////////
	  bool CamTerminateImageQueue()
	  {
			// variables
			bool bRet = false;

			// stop the thread loop 
			m_boRunThreadImageQueue = false;


			// finally terminate thread
			if (m_boTerminatedThreadImageQueue)
			{
				  TerminateThread(m_hThreadImageQueue, 0);
				  bRet = true;
			}

			// stop the image acquisition
			is_StopLiveVideo(m_hCam, IS_DONT_WAIT);

			CloseHandle(m_hThreadImageQueue);
			m_hThreadImageQueue = NULL;

			return bRet;
	  }






	  ///////////////////////////////////////////////////////////////////////////////
	  //
	  // METHOD CuEyeImageQueueDlg::ThreadProcImageQueue()
	  //
	  // DESCRIPTION: Collect buffers of the image queue
	  //
	  ///////////////////////////////////////////////////////////////////////////////
	  void ThreadProcImageQueue()
	  {
			INT nMemID = 0;
			char *pBuffer = NULL;
			INT nRet;

			// run the the image queue acquisition
			do
			{
				  m_boTerminatedThreadImageQueue = false;

				  nRet = is_WaitForNextImage(m_hCam, 1000, &pBuffer, &nMemID);
				  if (nRet == IS_SUCCESS)
				  {
						// do some processing, e.g. image display
						is_RenderBitmap(m_hCam, nMemID, m_hWndDisplayLive, IS_RENDER_FIT_TO_WINDOW);

						// do not forget to unlock the buffer, when all buffers are locked we cannot receive images any more
						is_UnlockSeqBuf(m_hCam, nMemID, pBuffer);
				  }

			} while (m_boRunThreadImageQueue);

			// that's it
			m_boTerminatedThreadImageQueue = true;
	  }

};

///////////////////////////////////////////////////////////////////////////////
//
// METHOD threadProcImageQueuec(void* pv)
//
// DESCRIPTION: Init thread for image queue handling
//
///////////////////////////////////////////////////////////////////////////////
unsigned WINAPI threadProcImageQueue(void* pv)
{
	  CuEyeImageQueueDlg* p = (CuEyeImageQueueDlg*)pv;

	  p->ThreadProcImageQueue();

	  _endthreadex(0);

	  return 0;
}
