
// uEyeImageQueue.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CuEyeImageQueueApp:
// See uEyeImageQueue.cpp for the implementation of this class
//

class CuEyeImageQueueApp : public CWinApp
{
public:
	  CuEyeImageQueueApp();

	  // Overrides
public:
	  virtual BOOL InitInstance();
	  virtual int ExitInstance();

	  // Implementation


};

extern CuEyeImageQueueApp theApp;