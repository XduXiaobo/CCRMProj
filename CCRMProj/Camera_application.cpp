#include "stdafx.h"
#include "Camera_application.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QLabel>
#include <qgraphicsview.h>
#include <qimage.h>
#include <iostream>
#include <conio.h>           // may have to modify this line if not using Windows
#include "SerialPort.h" 
#include <QDebug>
#include <chrono>
#include <thread>
#include <ctime>


using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

// Note: class definition is in Camera_application.h file meaning all the #include files for motor / camera mus be implemneted in there
double res = 0;
double scanDegree = 1.8;  // 9 * 0.2
bool image_in_frame = false;
bool stop_live_view = false;
bool frameCleared = false;
bool pdStateCHanged = false;
double speed = 2000;
double acc = 100000;


// variables for serial com with Arduino
char *port = "\\\\.\\COM3";
//char output[MAX_DATA_LENGTH];
float lightInt = 0;
//char incomingData[MAX_DATA_LENGTH];
auto prevState = "l"s;
time_t currentTime;
struct tm *localTime;
std::vector<int> tStampsMin;
std::vector<int> tStampsSec;

void ThreadProcImageQueue();

// Ctor
Camera_application::Camera_application(QWidget *parent): QMainWindow(parent)  {
	

	ui.setupUi(this);
	
	InitialiseBtn = ui.InitialiseBtn;
	connect(InitialiseBtn, SIGNAL(released()), this, SLOT(InitialiseBtnHandle()));
	
	CaptureBtn = ui.CaptureBtn;
	connect(CaptureBtn, SIGNAL(released()), this, SLOT(CaptureBtnHandle()));

	CalibBtn = ui.CalibBtn;
	connect(CalibBtn, SIGNAL(released()), this, SLOT(CalibBtnHandle()));
	
	ScanBtn = ui.ScanBtn;
	connect(ScanBtn, SIGNAL(released()), this, SLOT(ScanBtnHandle()));

	MoveBtn = ui.MoveBtn;
	connect(MoveBtn, SIGNAL(released()), this, SLOT(MoveBtnHandle()));

	SaveBtn = ui.SaveBtn;
	connect(SaveBtn, SIGNAL(released()), this, SLOT(SaveBtnHandle()));

	SetCamBtn = ui.SetCamBtn;
	connect(SetCamBtn, SIGNAL(released()), this, SLOT(SetCamBtnHandle()));

	GetCamBtn = ui.GetCamBtn;
	connect(GetCamBtn, SIGNAL(released()), this, SLOT(GetCamBtnHandle()));

	LiveStartBtn = ui.LiveStartBtn;
	connect(LiveStartBtn, SIGNAL(released()), this, SLOT(LiveStartBtnHandle()));

	LiveStopBtn = ui.LiveStopBtn;
	connect(LiveStopBtn, SIGNAL(released()), this, SLOT(LiveStopBtnHandle()));

	SetMotorSpeedBtn = ui.SetMotorSpeedBtn;
	connect(SetMotorSpeedBtn, SIGNAL(released()), this, SLOT(SetMotorSpeedBtnHandle()));
	  
	SetMotorAccBtn = ui.SetMotorAccBtn;
	connect(SetMotorAccBtn, SIGNAL(released()), this, SLOT(SetMotorAccBtnHandle()));

	InitialiseBtnHandle();

}

Camera_application::~Camera_application() {

	   IDS_Cam.CloseCamera();
	   bool reset_pos = false;
	   reset_pos = ph_motor.resetPosition();
	   if (reset_pos) {
	      ph_motor.disengageMotor();
	   }
}

void Camera_application::InitialiseBtnHandle() {


	  ui.Camera_found_label->setText("...");
	  ui.Motor_found_label->setText("...");

	  qApp->processEvents(); //  forces update on Qt GUI

	  bool cam_init = IDS_Cam.OpenCamera();
	  bool motor_init = ph_motor.Stepper_Initialise();


	  if (cam_init) {
			ui.Camera_found_label->setText("Found IDS ");
			ui.Camera_found_label->setStyleSheet("color: green");
			GetCamBtnHandle(); // if camera was initilised, then we set the default values in the Ctor, then we show them in their place holder
	  }
	  else {
			ui.Camera_found_label->setText("No device");
	  }

	  if (motor_init) {
			ui.Motor_found_label->setText("Found motor");
			ui.Motor_found_label->setStyleSheet("color: green");
	  }
	  else {
			ui.Motor_found_label->setText("No device ");
	  }

	  if (cam_init && motor_init) {
			//cam_init && motor_init
			ui.InitialiseBtn->setEnabled(false);
			ui.InitialiseBtn->setText("initialized");
			ui.InitialiseBtn->setStyleSheet("color: grey");
	  }

	  //QMessageBox msgBox;
	  //msgBox.setText("The document has been modified.");
	  //msgBox.exec();

  
}

void Camera_application::CheckArduino() {

	  //double val = 0;
	  //SerialPort arduino(port);
	  //if (arduino.isConnected()) {
			//cout << "connected";
	  //}
	  //else {
			//cout << "Error in port name" << endl << endl;
	  //}

	  //while (true) {

			////arduino.readSerialPort(output, MAX_DATA_LENGTH);

			//string outPut = convertToString(output, 1);
			//if (outPut == "h" && prevState != outPut) {

			//	  time(&currentTime);                   // Get the current time
			//	  localTime = localtime(&currentTime);  // Convert the current time to the local time

			///*	  int Min = localTime->tm_min;
			//	  int Sec = localTime->tm_sec;
			//	  tStampsMin.push_back(Min);
			//	  tStampsSec.push_back(Sec);*/
			//	  //qInfo() << QString::fromStdString(outPut);

			//	  std::this_thread::sleep_for(std::chrono::nanoseconds(1));
			//	  // then call the camera to capture
			//}

			//waitKey(20);
			//prevState = outPut;
			//	 
	  //}

}

string Camera_application::convertToString(char* a, int size)
{
	  int i;
	  string s = "";
	  for (i = 0; i < size; i++) {
			s = s + a[i];
	  }
	  return s;
}


void Camera_application::ScanBtnHandle() {


	  if (ui.gainBoost->isChecked()) {
			IDS_Cam.set_GainBoost(true);
	  }
	  else {
			IDS_Cam.set_GainBoost(false);
	  }


	  QString text = ui.scanDegreeInp->toPlainText();
	  //IDS_Cam.setupLUT("Scan");
	  double NewscanDegree = text.split(" ")[0].toDouble();
	  if (NewscanDegree != 0) {
			scanDegree = NewscanDegree;
	  }
	  // first start moving the motor(by degrees) and at the same time expose the camera to the scene 

	  //res = ph_motor.setStepperSpeed(speed);
	  //res = ph_motor.setStepperAcc(acc);
	  //double scanDefPos = ph_motor.getPosition();
	  //res = ph_motor.moveTo(scanDefPos + scanDegree);
	  IDS_Cam.CameraSeqCapture();
	 
	  waitKey(2);
	  // back to origial position
	  //res = ph_motor.moveTo(scanDefPos);
	  //while (ph_motor.isStepperMoving()) {
			//waitKey(1);
	  //}


	  //.......................................................................................................

	  //IDS_Cam.setupLUT("Scan");
	  //QString text = ui.scanDegreeInp->toPlainText();
	  //IDS_Cam.setupLUT("Scan");
	  //double NewscanDegree = text.split(" ")[0].toDouble();
	  //if (NewscanDegree != 0) {
			//scanDegree = NewscanDegree;
	  //}
	  //// first start moving the motor(by degrees) and at the same time expose the camera to the scene 


	  //// wait 100 ms to allocate memory
	  //std::this_thread::sleep_for(std::chrono::milliseconds(100));
	  //double scanDefPos = ph_motor.getPosition();
	  //res = ph_motor.moveTo(scanDefPos + scanDegree);
	  //IDS_Cam.CameraSeqCapture();

	  //while (ph_motor.isStepperMoving()) {

	  //}
	  //res = ph_motor.moveTo(scanDefPos);

	  //IDS_Cam.CaptureLive();
	  //waitKey(20);
	  //// back to origial position
	  //res = ph_motor.moveTo(scanDefPos);
	  //while (ph_motor.isStepperMoving()) {
			//waitKey(1);
	  //}

}

void Camera_application::MoveBtnHandle() {

	  QString text = ui.movePosInp->toPlainText();
	  double position = text.split(" ")[0].toDouble();
	  double defPos = ph_motor.getPosition();
	  double res = ph_motor.moveTo(defPos + position);

}

void Camera_application::SetCamBtnHandle() {

	  //QString text_exp = ui.camExpoInp->toPlainText();
	  //double exposure = text_exp.split(" ")[0].toDouble();

	  QString text_gain = ui.camGainInp->toPlainText();
	  double gain = text_gain.split(" ")[0].toDouble();

	  //QString text_frame = ui.camFrameInp->toPlainText();
	  //double frame = text_frame.split(" ")[0].toDouble();

	  QString text_gamma = ui.camGammaInp->toPlainText();
	  double gamma = text_gamma.split(" ")[0].toDouble();

	  //QString text_subF = ui.camSubFInp->toPlainText();
	  //double subF = text_subF.split(" ")[0].toDouble();

	  //IDS_Cam.set_ExposureTime(exposure);
	  IDS_Cam.set_Gain(gain);
	  IDS_Cam.set_FrameRate(30);
	  IDS_Cam.set_Gamma(gamma);
	  IDS_Cam.set_SubF(1000);

	  if (ui.gainBoost->isChecked()) {
			IDS_Cam.set_GainBoost(true);
	  }
	  else {
			IDS_Cam.set_GainBoost(false);
	  }


	  // update the settings on  the UI
	  Camera_application::GetCamBtnHandle();
	  
}

void Camera_application::GetCamBtnHandle() {
	 
	  double exposure = IDS_Cam.get_ExposureTime();
	  QString exposure_text = QString::number(exposure);

	  double gain = IDS_Cam.get_Gain();
	  QString gain_text = QString::number(gain);
	  
	  double frameRate = IDS_Cam.get_FrameRate();
	  QString frameRate_text = QString::number(frameRate);
	  
	  double gamma = IDS_Cam.get_Gamma();
	  QString gamma_text = QString::number(gamma);

	  double subF = IDS_Cam.get_SubF();
	  QString subF_text = QString::number(subF);

	  //ui.Cam_curr_expo->setText(exposure_text);
	  ui.Cam_curr_gain->setText(gain_text);
	  //ui.Cam_curr_frame->setText(frameRate_text);
	  ui.Cam_curr_gamma->setText(gamma_text);
	  //ui.Cam_curr_subF->setText(subF_text);
}

void Camera_application::CaptureBtnHandle() {

	 
	  IDS_Cam.set_single_capture_mode();
	  IDS_Cam.CameraCapture();


	  ////////////////////////////////////
}

void Camera_application::CalibBtnHandle() {
	  

	  double curr_expo = IDS_Cam.get_ExposureTime();
	  double curr_frameRate = IDS_Cam.get_FrameRate();

	  IDS_Cam.set_frame_detect_mode();
	  //IDS_Cam.setupLUT("Live");
	  stop_live_view = false;
	  image_in_frame = false;
	  frameCleared = false;
	  double moveIncrement = 0.02;
	  double moveDecrement = 0.1;
	  double stepBackward = ph_motor.getPosition();

	  stepBackward = stepBackward - moveDecrement;
	  ph_motor.moveTo(stepBackward);

	  while (!frameCleared && !stop_live_view) {

			stepBackward = stepBackward - moveDecrement;
			ph_motor.moveTo(stepBackward);
			frameCleared = IDS_Cam.clearFrameForCalib();
			if ((int)waitKey(200) >= 0) {
				  break;
			}
	  }

	  double stepForward = ph_motor.getPosition();
	  while (!image_in_frame && !stop_live_view) {

			if ((int)waitKey(200) >= 0) {
				  break;
			}

			stepForward = stepForward + moveIncrement;
			ph_motor.moveTo(stepForward);
			image_in_frame = IDS_Cam.detect_frame_in_live();
			
	  }
	  // final move to bring in the image from the edge
	  ph_motor.moveTo(stepForward + 0.5);

	  cv::destroyAllWindows();
	   
	  ph_motor.setCurrPosZero();


	  // return the values back to what they were set to by the operator
	  IDS_Cam.set_FrameRate(curr_frameRate);
	  IDS_Cam.CameraLive_Start();

}

void Camera_application::LiveStartBtnHandle() {
	  IDS_Cam.setupLUT("Live");

	  if (ui.gainBoost->isChecked()) {
			IDS_Cam.set_GainBoost(true);
	  }
	  else {
			IDS_Cam.set_GainBoost(false);
	  }

	  IDS_Cam.set_frame_detect_mode();

	  //IDS_Cam.threadProcImageQueue();
	  IDS_Cam.CaptureLive();
}

void Camera_application::LiveStopBtnHandle() {

	  stop_live_view = true;
	  IDS_Cam.CameraLive_Stop();
	  cv::destroyAllWindows();
}

void Camera_application::SetMotorSpeedBtnHandle() {

	  QString text = ui.MotorSpdInp->toPlainText();
	  double speed = text.split(" ")[0].toDouble();
	  ph_motor.setStepperSpeed(speed);
}

void Camera_application::SetMotorAccBtnHandle() {

	  QString text = ui.MotorAccInp->toPlainText();
	  double acceleration = text.split(" ")[0].toDouble();
	  ph_motor.setStepperAcc(acceleration);
}

void Camera_application::SaveBtnHandle() {

	 // CheckArduino();

	  IDS_Cam.Save_Scan();
}


