#include "stdafx.h"
#include <iostream>
#include <iomanip>      // std::setprecision
#include <direct.h>
#include <fstream>
#include <string>
#include <math.h>
#include "uEye.h"
#include <opencv2/core.hpp>
#include "opencv2/opencv_modules.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <ctime>
#include <ctype.h>
#include "IDSLib.h"
#include <process.h>
#include "opencv2/opencv_modules.hpp"
#include <chrono>
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat)
#include <opencv2/videoio.hpp>  // Video write
#include <vector>
#include <algorithm>
#include "resource.h"
//#include <future>

#ifdef HAVE_OPENCV_CUDAARITHM
#  include "opencv2/cudaarithm.hpp"
#endif

#ifdef HAVE_OPENCV_CUDAIMGPROC
#  include "opencv2/cudaimgproc.hpp"
#endif

//#include "afxdialogex.h"

using namespace cv;
using namespace std;
using namespace System::Threading::Tasks;

namespace UFCam
{

	class IDSCamera
	{

	public:


	private:

		HWND GetSafeHwnd() const;
		// uEye varibles
		HIDS	m_hCam;			// handle to camera
		HWND	m_hWndDisplay;	// handle to diplay window
		INT		m_Ret;			// return value of uEye SDK functions
		INT		m_colorMode;	// Y8/RGB16/RGB24/REG32
		INT		m_nBitsPerPixel = 32;// number of bits needed store one pixel
		INT		m_nSizeX;		// width of video 
		INT		m_nSizeY;		// height of video
		INT		m_lMemoryId;	// grabber memory - buffer ID
		char* m_pcImageMemory;// grabber memory - pointer to buffer
		char* m_LockedMemory = nullptr;
		INT     m_nRenderMode;  // render  mode
		void* pMem;
		VOID* pMem_b;
		SENSORINFO m_sInfo;	    // sensor information struct

		std::vector<INT> m_viSeqMemId;		// camera memory - buffer IDs
		std::vector<char*> m_vpcSeqImgMem;	// camera memory - pointers to buffer
		int		m_nNumberOfBuffers;			// the number of buffers we use
			// image acquisition thread variables
		HANDLE  m_hThreadImageQueue;
		BOOL    m_boRunThreadImageQueue;
		BOOL    m_boTerminatedThreadImageQueue;
		DWORD   m_dwThreadIDImageQueue;
		//std::thread Thread;
		cv::Mat imgMat;

		double curr_frameRate; //fps
		double curr_exposure;
		double def_exposure = 900;
		double def_frameRate = 30;


		double def_gain = 1; //1x


		double curr_gain = 0; //1x
		bool stop_live_view = false;
		bool image_in_frame = false;

		int aread_of_inter_px = 1920;
		int aread_of_inter_py = 1200;

		//int live_w = 1504; // 1504, 1200, 1008, 800, 512
		//int live_h = 512; // 800,  600,  400,  400, 512
		int live_w = 1504; // 1504, 1200, 1008, 800, 512
		int live_h = 800; // 800,  600,  400,  400, 512
		int sc_w = 304;
		int sc_h = 304;

#pragma region Camera functions 


	private:



		INT InitCamera(HIDS* hCam, HWND hWnd)
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

				std::string Str1, Str2, Str3;
				Str1 = "This camera requires a new firmware. The upload will take about";
				Str2 = "seconds. Please wait ...";
				printf("%s %d %s", Str1, nUploadTime / 1000, Str2);

				// Try again to open the camera. This time we allow the automatic upload of the firmware by
				// specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
				*hCam = (HIDS)(((INT)*hCam) | IS_ALLOW_STARTER_FW_UPLOAD);
				nRet = is_InitCamera(hCam, hWnd);
			}

			return nRet;
		}

	public:

		// Ctor
		IDSCamera()
		{
			//OpenCamera();
		}


		// calls--> InitCamera() calls --> is_InitCamera() which is a default function
		bool OpenCamera(bool reinit = false)
		{


			if (!reinit) {
				if (m_hCam != 0)
				{
					//free old image mem.
					is_FreeImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
					is_ExitCamera(m_hCam);
				}
				// init camera
				m_hCam = (HIDS)0; // open next camera
				m_Ret = InitCamera(&m_hCam, NULL); // init camera - no window handle required
			}

			is_GetSensorInfo(m_hCam, &m_sInfo);
			m_Ret = initDisplayMode(m_hCam);
			if (m_Ret == IS_SUCCESS)
			{
				try {
					//// Get sensor info
					//is_GetSensorInfo(m_hCam, &m_sInfo);

					//GetMaxImageSize(&m_nSizeX, &m_nSizeY); // rather than using all the vertical pixels in CMOS, we cut it down to 350
					//m_nSizeX = aread_of_inter_px;
					//m_nSizeY = aread_of_inter_py;


					//INT colorMode = IS_CM_BGRA8_PACKED;  /*IS_CM_SENSOR_RAW8 or IS_CM_MONO8*/
					//is_SetColorMode(m_hCam, colorMode);


					INT nGamma = 0;
					INT nRet = is_Gamma(m_hCam, IS_GAMMA_CMD_SET, (void*)&nGamma, sizeof(nGamma));

					nRet = is_SetSubSampling(m_hCam, IS_SUBSAMPLING_DISABLE);

					is_SetExternalTrigger(m_hCam, IS_SET_TRIGGER_SOFTWARE);
					// Set the flash to a high active pulse for each image in the trigger mode
					UINT nMode = IO_FLASH_MODE_TRIGGER_HI_ACTIVE;
					is_IO(m_hCam, IS_IO_CMD_FLASH_SET_MODE, (void*)&nMode, sizeof(nMode));

					UINT nEdgeEnhancement = 7;
					nRet = is_EdgeEnhancement(m_hCam, IS_EDGE_ENHANCEMENT_CMD_SET, (void*)&nEdgeEnhancement, sizeof(nEdgeEnhancement));

					UINT nSharpness = 9;
					nRet = is_Sharpness(m_hCam, SHARPNESS_CMD_SET_VALUE, (void*)&nSharpness, sizeof(nSharpness));

					UINT desired_pix_clock = get_desired_pixelClock();
					set_pixel_clock(desired_pix_clock);
					INT current_pixel_clock = get_pixel_clock();

					set_Gain(def_gain);
					set_FrameRate(def_frameRate);

					return true;
				}
				catch (std::exception e) {
					return false;
				}

			}
			else {
				return false;
			}

		}


		int initDisplayMode(HIDS m_hCam)
		{

			INT nRet = IS_NO_SUCCESS;

			if (m_hCam == NULL)
				return IS_NO_SUCCESS;

			if (m_pcImageMemory != NULL)
			{
				is_FreeImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
			}

			m_pcImageMemory = NULL;

			// Set display mode to DIB
			nRet = is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);
			//if (m_sInfo.nColorMode == IS_COLORMODE_BAYER)
			//{
				  //// setup the color depth to the current windows setting
				  //is_GetColorDepth(*_hCam, &m_nBitsPerPixel, &m_colorMode);
			//}
			//else if (m_sInfo.nColorMode == IS_COLORMODE_CBYCRY)
			//{
				  //// for color camera models use RGB32 mode
				  //m_colorMode = IS_CM_BGRA8_PACKED;
				  //m_nBitsPerPixel = 32;
			//}
			//else
			//{
				  //// for monochrome camera models use Y8 mode
				  //m_colorMode = IS_CM_MONO8;el = 8;
			//}
				  //m_nBitsPerPix

			// allocate an image memory.
			is_AllocImageMem(m_hCam, aread_of_inter_px, aread_of_inter_py, m_nBitsPerPixel, &m_pcImageMemory, &m_lMemoryId);
			is_AddToSequence(m_hCam, m_pcImageMemory, m_lMemoryId);
			is_SetImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
			m_nBitsPerPixel = 32;
			m_colorMode = IS_CM_RGBA8_PACKED;
			is_SetColorMode(m_hCam, m_colorMode);

			//is_SetColorMode(*_hCam, m_colorMode);
			IS_SIZE_2D imageSize;
			imageSize.s32Width = aread_of_inter_px;
			imageSize.s32Height = aread_of_inter_py;

			is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));


			return nRet;
		}



		// close camera to free up memory (use this after captures ?)
		bool CloseCamera() {

			if (m_hCam != 0)
			{
				//free old image memory.
				is_FreeImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
				is_ExitCamera(m_hCam);
			}

			return true;
		}

		// capture a single image 
		void CameraCapture() {

			// OpenCamera(true);

			if (m_hCam != 0)
			{
				//static std::clock_t start = 0;
				//is_SetExternalTrigger(m_hCam, IS_SET_TRIGGER_SOFTWARE);
				//BOOL bWasLive = (BOOL)(is_CaptureVideo(m_hCam, IS_GET_LIVE));
				//if (bWasLive)
					  //is_StopLiveVideo(m_hCam, IS_FORCE_VIDEO_STOP);
				// Set the flash to a high active pulse for each image in the trigger mode


				// Returns the AOI position and size by using an object of the IS_RECT type.
				// IS_RECT rectAOI;

				//INT nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI));
				//if (nRet == IS_SUCCESS)
				//{
					//INT x = rectAOI.s32X;
					//INT y = rectAOI.s32Y;
					//INT width = rectAOI.s32Width;
					//INT height = rectAOI.s32Height;
				//}


				//Is_captureSingleFrame(m_hCam, 1);

				//INT aa = get_ExposureTime();

			  //// Get sensor info
					  //is_GetSensorInfo(m_hCam, &m_sInfo);

					  //GetMaxImageSize(&m_nSizeX, &m_nSizeY); // rather than using all the vertical pixels in CMOS, we cut it down to 350
					  //m_nSizeX = aread_of_inter_px;
					  //m_nSizeY = aread_of_inter_py;


					  //INT colorMode = IS_CM_BGRA8_PACKED;  /*IS_CM_SENSOR_RAW8 or IS_CM_MONO8*/
					  //is_SetColorMode(m_hCam, colorMode);

				//INT m_Ret = initDisplayMode(m_hCam);
				//is_AddToSequence(m_hCam, m_pcImageMemory, m_lMemoryId);
				//is_SetImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
				//m_nBitsPerPixel = 32;
				//m_colorMode = IS_CM_RGBA8_PACKED;
				//is_SetColorMode(m_hCam, m_colorMode);

				////is_SetColorMode(*_hCam, m_colorMode);
				//IS_SIZE_2D imageSize;
				//imageSize.s32Width = aread_of_inter_px;
				//imageSize.s32Height = aread_of_inter_py;

				//is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));
				INT nRet = is_FreezeVideo(m_hCam, IS_WAIT);
				//save images
				IMAGE_FILE_PARAMS ImageFileParams;
				ImageFileParams.pnImageID = NULL;
				ImageFileParams.ppcImageMem = NULL;
				ImageFileParams.pwchFileName = NULL;
				ImageFileParams.nFileType = IS_IMG_BMP;
				nRet = is_ImageFile(m_hCam, IS_IMAGE_FILE_CMD_SAVE, (void*)&ImageFileParams, sizeof(ImageFileParams));
				//UINT nMode = IO_FLASH_MODE_TRIGGER_HI_ACTIVE;
				//is_IO(m_hCam, IS_IO_CMD_FLASH_SET_MODE, (void*)&nMode, sizeof(nMode));
				//if (is_FreezeVideo(m_hCam, IS_DONT_WAIT) == IS_SUCCESS)
				//{

				//	  // printing the capture duration
				//	 /* OutputDebugStringW(L"\n----------Image captured ");
				//	  double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
				//	  std::string s_duration = " the capturer duration is: " + std::to_string(duration);
				//	  std::wstring stemp = std::wstring(s_duration.begin(), s_duration.end());
				//	  LPCWSTR sw = stemp.c_str();
				//	  OutputDebugString(sw);*/

				//	  //// continue to save
				//	  IMAGE_FILE_PARAMS ImageFileParams;
				//	  ImageFileParams.pwchFileName = NULL;
				//	  ImageFileParams.pnImageID = NULL;
				//	  ImageFileParams.ppcImageMem = NULL;
				//	  ImageFileParams.nQuality = 0;

				//	  // Save bitmap from active memory to file (with file open dialog)

				//	  //Mat frame = getSeqFrames(&m_hCam);
				///*	  cv::imshow("new img",frame);*/
				//	  ImageFileParams.nFileType = IS_IMG_BMP;

				//	  INT nRet = is_ImageFile(m_hCam, IS_IMAGE_FILE_CMD_SAVE, (void*)&ImageFileParams,
				//			sizeof(ImageFileParams));

				//}

			}


		}

		void setupMemSequence() {

			if (m_hCam == 0)
				OpenCamera();

			//is_AllocImageMem(m_hCam, aread_of_inter_px, aread_of_inter_py, 8, &pMem, &memID);
			is_AllocImageMem(m_hCam,
				aread_of_inter_px,
				aread_of_inter_py,
				32,
				&m_pcImageMemory,
				&m_lMemoryId);
			is_SetImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
		}



		void CheckSettingsOnCamera() {

			BOOL bWasLive = (BOOL)(is_CaptureVideo(m_hCam, IS_GET_LIVE));
			if (bWasLive)
				is_StopLiveVideo(m_hCam, IS_FORCE_VIDEO_STOP);

			Is_GetBusSpeed(m_hCam, m_lMemoryId);

		}



		// use live view as a video capture tool ie save the reference frames in the memory
		void CaptureLive() {

			BOOL bWasLive = (BOOL)(is_CaptureVideo(m_hCam, IS_GET_LIVE));
			if (bWasLive)
				is_StopLiveVideo(m_hCam, IS_FORCE_VIDEO_STOP);

			// realloc image mem with actual sizes and depth.
			//is_FreeImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
			Is_ShowLive_Window(m_hCam, m_lMemoryId, live_w, live_h);

		}


		// load image 

		void Load_image() {

			//QString fileName = QFileDialog::getOpenFileName(this,
			//	  tr("Open Image"), "/home/jana", tr("Image Files (*.bmp)"));
			//std::string file_path = fileName.toStdString().c_str();

			Mat capturedImgOrg, capturedImg;
			// following is needed however is commented out due to opencv issue
			//capturedImgOrg = imread("C:\\Users/local/Desktop/A.bmp", CV_LOAD_IMAGE_COLOR);
			//cv::cvtColor(capturedImgOrg, capturedImgOrg, CV_BGR2GRAY);

			if (!capturedImgOrg.data)  // Check for invalid input
			{
				cout << "Could not open or find the image" << std::endl;
			}

			cv::resize(capturedImgOrg, capturedImg, Size(), 0.5, 0.5);
			imshow("Captured image", capturedImg);
			waitKey(0);
		}



		// setting up LUT profile
		void setupLUT(std::string lutName) {

			if (lutName == "Live") {


				//// setting LUT
				wchar_t* pFilename = L"D:\\HW-sourceCode\\UF_camera\\Camera_application\\Color_Milk.xml"; //microFlu
				INT nRet = is_LUT(m_hCam, IS_LUT_CMD_LOAD_FILE, (void*)pFilename, NULL);

				IS_LUT_ENABLED_STATE nLutEnabled = IS_LUT_ENABLED;
				nRet = is_LUT(m_hCam, IS_LUT_CMD_SET_ENABLED, (void*)&nLutEnabled, sizeof(nLutEnabled));


			}
			else if (lutName == "Scan") {
				//// setting LUT
				wchar_t* pFilename = L"D:\\HW-sourceCode\\UF_camera\\Camera_application\\Color_Milk.xml";
				INT nRet = is_LUT(m_hCam, IS_LUT_CMD_LOAD_FILE, (void*)pFilename, NULL);

				IS_LUT_ENABLED_STATE nLutEnabled = IS_LUT_ENABLED;
				nRet = is_LUT(m_hCam, IS_LUT_CMD_SET_ENABLED, (void*)&nLutEnabled, sizeof(nLutEnabled));
			}
		}


		Mat copyFromMemory(cv::Mat& mat) {


			Mat emMat;
			if (m_pcImageMemory == NULL) { return emMat; }
			try {
				memcpy(mat.ptr(), m_pcImageMemory, mat.cols * mat.rows);
				return mat;
			}
			catch (std::exception e) {
				return emMat;
			}

		}

		// live view start (free run)
		void CameraLive_Start() {


			if (m_hCam != 0) {
				//free old image mem.
				is_FreeImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
				is_ExitCamera(m_hCam);
			}
			return;

		}

		// live view stop
		void CameraLive_Stop() {


			if (m_hCam != 0)
			{
				Is_StopLive(m_hCam, m_lMemoryId);

				// Free the allocated buffer
				if (m_pcImageMemory != NULL)
					is_FreeImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
				m_pcImageMemory = NULL;
			}

			stop_live_view = true;
		}


#pragma endregion 


#pragma region Helper functions



		// Capture a frame and push it in a OpenCV mat element
		Mat getFrame(HIDS* hCam, int width, int height, cv::Mat& mat, char* pMemm, VOID* pMem_b, int memID) {
			// Allocate memory for image

			try {

				// Activate the image memory for storing the frame captured
				// Grabbing the image
				// Getting the data of the frame and push it in a Mat element

				is_FreezeVideo(*hCam, IS_WAIT);
				int retInt = is_GetImageMem(*hCam, &pMem_b);
				if (retInt != IS_SUCCESS) {
					std::cout << "Image data could not be read from memory!" << std::endl;
				}
				is_SetImageMem(*hCam, m_pcImageMemory, memID);
				memcpy(mat.ptr(), pMem_b, mat.cols * mat.rows);


				return mat;
			}
			catch (const std::exception& e) {

			}

		}

		Mat getSeqFrames(HIDS* hCam, cv::Mat& mat) {

			int retInt = is_GetImageMem(*hCam, &pMem_b);
			if (retInt != IS_SUCCESS) {
				std::cout << "Image data could not be read from memory!" << std::endl;
			}
			memcpy(mat.ptr(), pMem_b, mat.cols * mat.rows);

			return mat;
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
		bool CamSeqBuilt()
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
			bool bRet = true;

			// create image queue thread
			m_boRunThreadImageQueue = TRUE;

			//std::thread t1(ThreadProcImageQueue);
			//Thread = std::thread([this] { this->ThreadProcImageQueue(); });
			//Thread.join();
			//m_hThreadImageQueue = (HANDLE)_beginthreadex(NULL, 0, threadProcImageQueue, (void*)this, 0, (UINT*)&m_dwThreadIDImageQueue);
			//if (m_hThreadImageQueue == NULL)
			//{
				  ////AfxMessageBox(_T("ERROR: Cannot create image queue thread!"), MB_ICONEXCLAMATION, 0);
				  //m_boRunThreadImageQueue = FALSE;
			//}
			//else
			//{
				  //// image queue thread must now be active
				  //bRet = true;
			//}


			// we really shouldn't do that !!!!
			//SetThreadPriority (m_hThreadEvent, THREAD_PRIORITY_TIME_CRITICAL);


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
			char* pBuffer = NULL;
			INT nRet;
			Mat current_image(aread_of_inter_py, aread_of_inter_px, CV_8UC1); //CV_8UC1
			// run the the image queue acquisition
			do
			{
				m_boTerminatedThreadImageQueue = false;

				nRet = is_WaitForNextImage(m_hCam, 1000, &pBuffer, &nMemID);
				if (nRet == IS_SUCCESS)
				{
					// do some processing, e.g. image display
					//is_RenderBitmap(m_hCam, nMemID, m_hWndDisplayLive, IS_RENDER_FIT_TO_WINDOW);
					Mat frame = getSeqFrames(&m_hCam, current_image);
					waitKey(200);
					cv::imshow("new frame", frame);
					// do not forget to unlock the buffer, when all buffers are locked we cannot receive images any more
					is_UnlockSeqBuf(m_hCam, nMemID, pBuffer);
				}

			} while (m_boRunThreadImageQueue);

			// that's it
			m_boTerminatedThreadImageQueue = true;
		}

		///////////////////////////////////////////////////////////////////////////////
		//
		// METHOD threadProcImageQueuec(void* pv)
		//
		// DESCRIPTION: Init thread for image queue handling
		//
		///////////////////////////////////////////////////////////////////////////////
		void threadProcImageQueue()
		{
			IDSCamera* p;

			p->ThreadProcImageQueue();

			_endthreadex(0);

			//return 0;
		}

		bool clearFrameForCalib() {

			Mat current_image(aread_of_inter_py, aread_of_inter_px, CV_8UC1); //CV_8UC1
			Mat frame = getFrame(&m_hCam, aread_of_inter_px, aread_of_inter_py, current_image, m_pcImageMemory, pMem_b, m_lMemoryId);
			Mat frame_out_of_calib = Mat(frame.size(), frame.type(), Scalar::all(0));
			Rect out_of_calib(0, 0, aread_of_inter_px - 10, aread_of_inter_py);
			frame(out_of_calib).copyTo(frame_out_of_calib(out_of_calib));

			cv::Point min_loc, min_loc_OutCalib, max_loc, max_loc_OutCalib;
			double minval, minval_outCalib, maxval, maxval_OutCalib;
			cv::minMaxLoc(frame, &minval, &maxval, &min_loc, &max_loc);
			cv::minMaxLoc(frame_out_of_calib, &minval_outCalib, &maxval_OutCalib, &min_loc_OutCalib, &max_loc_OutCalib);

			std::string maxvalDisplay = " \n -----------  Clear frame ------------ \n maxVal at calib area " + std::to_string(maxval) + "\n maxVal out of calib area " + std::to_string(maxval_OutCalib);
			std::wstring stemp = std::wstring(maxvalDisplay.begin(), maxvalDisplay.end());
			LPCWSTR sw = stemp.c_str();
			OutputDebugString(sw);

			if (maxval > 30) {
				// we have spotted the image in the frame
				return false;

			}

			// PERFORM YOUR OPENCV PROCESSING HERE!
			// Visualise the data
			waitKey(200);
			imshow("test_interface_2", current_image);
			return true;

		}

		// detect if there is an image in the current frame
		bool detect_frame_in_live() {

			Mat current_image(aread_of_inter_py, aread_of_inter_px, CV_8UC1); //CV_8UC1
			Mat frame = getFrame(&m_hCam, aread_of_inter_px, aread_of_inter_py, current_image, m_pcImageMemory, pMem_b, m_lMemoryId);

			Mat frame_out_of_calib = Mat(frame.size(), frame.type(), Scalar::all(0));
			Rect out_of_calib(0, 0, aread_of_inter_px - 10, aread_of_inter_py);
			frame(out_of_calib).copyTo(frame_out_of_calib(out_of_calib));

			Mat frame_in_calib = Mat(frame.size(), frame.type(), Scalar::all(0));
			Rect in_calib(aread_of_inter_px - 10, 0, 10, aread_of_inter_py);
			frame(in_calib).copyTo(frame_in_calib(in_calib));

			//double l1 = frame.at<uchar>(100, 0); double l2 = frame.at<uchar>(200, 0);
			//double l3 = frame.at<uchar>(0, 100); double l4 = frame.at<uchar>(0, 200);
			/*  ColumSum = cv::sum(greyFrame)[0] / (frame.rows*frame.cols);*/
			//for (int row_index = 1; row_index < frame.rows; row_index++) { //
				  //l1 = frame.at<uchar>((uchar)row_index, (uchar)aread_of_inter_px);
				  //if (l1 > 100) {
				  //	  return true;
				  //}
				  //
			//}
			cv::Point min_loc, min_loc_OutCalib, max_loc, max_loc_OutCalib;
			double minval, minval_outCalib, maxval, maxval_OutCalib;

			cv::minMaxLoc(frame_in_calib, &minval, &maxval, &min_loc, &max_loc);
			cv::minMaxLoc(frame_out_of_calib, &minval_outCalib, &maxval_OutCalib, &min_loc_OutCalib, &max_loc_OutCalib);


			std::string maxvalDisplay = " \n ---------- Locating the image ------------ \n maxVal at calib area " + std::to_string(maxval) + "\n maxVal out of calib area " + std::to_string(maxval_OutCalib);
			std::wstring stemp = std::wstring(maxvalDisplay.begin(), maxvalDisplay.end());
			LPCWSTR sw = stemp.c_str();
			OutputDebugString(sw);

			if (maxval > 150) {
				// we have spotted the image in the frame
				if (max_loc.x > 1895) {
					// it is within the accpetable range 
					return true;
				}

			}


			// PERFORM YOUR OPENCV PROCESSING HERE!
			// Visualise the data
			waitKey(200);
			imshow("test_interface_2", current_image);
			return false;

		}

		void Save_Scan() {

			int sf = 1;
			for (int L = 0; L < sf; L++) {

				if (true) {


					//std:string fileName = "../Scans/subScans/Image_" + std::to_string(L) + ".jpg";
					//	  imwrite(fileName, frames[L]);
						  //// continue to save
						  //	  IMAGE_FILE_PARAMS ImageFileParams;
						  //	  ImageFileParams.pwchFileName = NULL;
						  //	  ImageFileParams.pnImageID = NULL;
						  //	  ImageFileParams.ppcImageMem = NULL;
						  //	  ImageFileParams.nQuality = 0;

						  //	  // Save bitmap from active memory to file (with file open dialog)

						  //	  //Mat frame = getSeqFrames(&m_hCam);
						  ///*	  cv::imshow("new img",frame);*/
						  //	  ImageFileParams.nFileType = IS_IMG_BMP;

						  //	  INT nRet = is_ImageFile(m_hCam, IS_IMAGE_FILE_CMD_SAVE, (void*)&frames[L],
						  //			sizeof(frames[L]));


				}
			}


		}



		// get camera physical size

		void GetMaxImageSize(INT* pnSizeX, INT* pnSizeY)
		{
			// Check if the camera supports an arbitrary AOI
			// Only the ueye xs does not support an arbitrary AOI
			INT nAOISupported = 0;
			BOOL bAOISupported = TRUE;
			if (is_ImageFormat(m_hCam,
				IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED,
				(void*)&nAOISupported,
				sizeof(nAOISupported)) == IS_SUCCESS)
			{
				bAOISupported = (nAOISupported != 0);
			}

			if (bAOISupported)
			{
				// All other sensors
				// Get maximum image size
				SENSORINFO sInfo;
				is_GetSensorInfo(m_hCam, &sInfo);
				*pnSizeX = sInfo.nMaxWidth;
				*pnSizeY = sInfo.nMaxHeight;
			}
			else
			{
				// Only ueye xs
				// Get image size of the current format
				IS_SIZE_2D imageSize;
				is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

				*pnSizeX = imageSize.s32Width;
				*pnSizeY = imageSize.s32Height;
			}
		}



#pragma endregion



#pragma region	  Camera settings

		// get current exposure time
		double get_ExposureTime() {

			if (m_hCam == NULL)
				return 0;

			UINT nExposureTime;
			double dTemp = 0.0;
			dTemp = (double)nExposureTime;
			m_Ret = is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, (void*)&dTemp, sizeof(dTemp));
			curr_exposure = dTemp;
			return dTemp;

		}

		// set new exposure time 
		bool set_ExposureTime(double nExposureTime) {

			//double nExposureTime = 1000;
			m_Ret = is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&nExposureTime, sizeof(nExposureTime));
			if (m_Ret != IS_SUCCESS) {
				return false;
			}

			m_Ret = is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, (void*)&curr_exposure, sizeof(curr_exposure));
			return true;
		}

		// get current frame rate
		double get_FrameRate() {

			if (m_hCam == NULL)
				return 0;

			return curr_frameRate;

		};

		// set frame rate to 1 seceond
		bool set_FrameRate(double nFrameRate) {

			double newExpo = (1 / nFrameRate) * 1000;
			if (newExpo > 1000) { newExpo = 1000; };
			if (m_hCam == NULL)
				return 0;
			m_Ret = set_ExposureTime(newExpo);
			m_Ret = is_SetFrameRate(m_hCam, nFrameRate, &curr_frameRate);


			return true;
		};

		// get Gain value
		double get_Gain() {

			if (m_hCam == NULL)
				return 0;

			curr_gain = is_SetHWGainFactor(m_hCam, IS_GET_MASTER_GAIN_FACTOR, NULL);
			curr_gain = curr_gain / 100;
			return curr_gain;
		}

		// set Gain value
		bool set_Gain(double nGainFactor) {

			if (m_hCam == NULL)
				return 0;

			//double nGainFactor = 1;
			INT ret = is_SetHWGainFactor(m_hCam, IS_SET_MASTER_GAIN_FACTOR, nGainFactor * 100);
			curr_gain = nGainFactor;
			return true;
		}

		// detect mode
		void set_frame_detect_mode()
		{

			// set very high frame rate
			// start capturing at high frame rate
			setupMemSequence();
			set_FrameRate(def_frameRate);

			UINT nEnable = EXTENDED_PIXELCLOCK_RANGE_OFF;
			INT nRet = is_DeviceFeature(m_hCam, IS_DEVICE_FEATURE_CMD_SET_EXTENDED_PIXELCLOCK_RANGE_ENABLE, (void*)&nEnable, sizeof(nEnable));
			set_pixel_clock(200);
		}


		// single capture
		void set_single_capture_mode() {
			set_Gain(0);
			m_Ret = set_FrameRate(def_frameRate);
		}

		UINT get_desired_pixelClock() {

			// normal pixel clock rate is 30-MHz
			UINT nRange[3];
			ZeroMemory(nRange, sizeof(nRange));
			INT nRet = is_PixelClock(m_hCam, IS_PIXELCLOCK_CMD_GET_RANGE, (void*)nRange, sizeof(nRange));

			if (nRet == IS_SUCCESS)
			{
				UINT nMin = nRange[0];
				UINT nMax = nRange[1];
				UINT nInc = nRange[2];
				return nMax;
			}
			else {
				return 0;
			}
		}

		// set pixel clock 
		void set_pixel_clock(UINT max_pix_clk) {

			INT  nRet = is_PixelClock(m_hCam, IS_PIXELCLOCK_CMD_SET, (void*)&max_pix_clk, sizeof(max_pix_clk));
		}

		// get the current pixel clock
		INT get_pixel_clock() {
			UINT nPixelClock;
			INT nRet = is_PixelClock(m_hCam, IS_PIXELCLOCK_CMD_GET, (void*)&nPixelClock, sizeof(nPixelClock));
			return nPixelClock;
		}

		// get the Gamma value
		double get_Gamma() {
			INT  nGamma;
			INT nRet = is_Gamma(m_hCam, IS_GAMMA_CMD_GET, (void*)&nGamma, sizeof(nGamma));
			return nGamma / 100;
		}

		// set the Gamma value
		void set_Gamma(INT Gamma) {

			INT nGamma = Gamma * 100;
			//Gamma value to be set, multiplied by 100 (Range: 1?000)
			//INT nRet = is_Gamma(m_hCam, IS_GAMMA_CMD_SET, (void*)&nGamma, sizeof(nGamma));
		}

		// set the gain boost for the cases where the light intensity is very low
		void set_GainBoost(bool GainBoost) {

			if (GainBoost) {
				is_SetGainBoost(m_hCam, IS_SET_GAINBOOST_ON);
			}
			else {
				is_SetGainBoost(m_hCam, IS_SET_GAINBOOST_OFF);
			}
		}


	};

#pragma endregion


}
