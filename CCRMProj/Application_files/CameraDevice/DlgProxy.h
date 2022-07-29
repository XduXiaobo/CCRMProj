
// DlgProxy.h: header file
//

#pragma once

class CuEyeImageQueueDlg;


// CuEyeImageQueueDlgAutoProxy command target

class CuEyeImageQueueDlgAutoProxy : public CCmdTarget
{


  // Attributes
public:
	  CuEyeImageQueueDlg* m_pDialog;

	  // Operations
public:

	  // Overrides
public:
	  virtual void OnFinalRelease();

	  // Implementation
protected:
	  virtual ~CuEyeImageQueueDlgAutoProxy();

	  // Generated message map functions



};

