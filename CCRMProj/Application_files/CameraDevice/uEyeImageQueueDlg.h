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


// uEyeImageQueueDlg.h : header file
//

#pragma once

#include <vector>
#include "uEye.h"

class CuEyeImageQueueDlgAutoProxy;


// CuEyeImageQueueDlg dialog
class CuEyeImageQueueDlg : public CDialogEx
{

	  friend class CuEyeImageQueueDlgAutoProxy;

	  // Construction
public:
	  CuEyeImageQueueDlg(CWnd* pParent = NULL);	// standard constructor
	  virtual ~CuEyeImageQueueDlg();


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


	  // camera functions
	  INT InitCamera(HIDS *hCam, HWND hWnd);
	  bool CamOpen();
	  bool CamRunImageQueue(void);
	  bool CamTerminateImageQueue(void);
	  bool CamSeqBuild();
	  bool CamSeqKill();

	  // image acquisition thread function
	  void ThreadProcImageQueue();



	  // Dialog Data
	  enum { IDD = IDD_UEYEIMAGEQUEUE_DIALOG };

protected:
	  virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


  // Implementation
protected:
	  CuEyeImageQueueDlgAutoProxy* m_pAutoProxy;
	  HICON m_hIcon;

	  BOOL CanExit();

	  // Generated message map functions
	  virtual BOOL OnInitDialog();

	  virtual void OnOK();
	  virtual void OnCancel();
	
public:
	  CString m_strCamType;

};
