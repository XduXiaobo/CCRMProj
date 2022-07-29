#include "stdafx.h"
#include <iomanip>      // std::setprecision
#include <iostream>
#include<conio.h> 
#include <fstream>
#include <string>
#include <ctime>
#include <windows.h>
#include <math.h>
#include "phidget22.h"
#include <ctype.h>
#include "IDSLib.h"
#include "PhidgetHelperFunctions.h"
#include <cstdlib>
#include <sys/timeb.h>
#include <time.h>


namespace UFCam
{

	  //static, but non-const data members should be defined outside of the class definition and inside the namespace enclosing the class.
	  //static double duration;

	  //static std::clock_t t_start;
	  class PhidgetsMotor
	  {

	  private:
		    double velocityLimit;
			double accelerationLimit;
			//std::clock_t time_start;
			//std::clock_t time_end;
			//int max_velocity = PhidgetStepper_Velocity;
			int max_velocity = PhidgetStepper_getMaxVelocityLimit(ch, &velocityLimit);
			int max_rev = PhidgetStepper_Revolution;
			//max_rev has no use 
			//int max_acceleration = PhidgetStepper_MaxAcceleration;
			int max_acceleration = PhidgetStepper_getMaxAcceleration(ch, &accelerationLimit);
			int max_current = PhidgetStepper_Current;
			int phidget_serial_number = PhidgetStepper_SerialNum;
			int phidget_channel = PhidgetStepper_Channel;
			char buf[100];
			char* endptr;
			int end = 0;
			double position = 0.0;
			double minPosition = 0.0;
			double maxPosition = 0.0;
			int stepper_engaged = 0.0;

			double degreePerStep = 0.0041860;
			double stepsPerDegree = 238.8; // 27/1 gear ratio
			double curr_pos;
			int isMotorMoving;
			int stopped;
			int g_NewPos = 0;
			double stepstoMove = 0;
			double degreesToMove = 0;

			ChannelInfo channelInfo; //Information from AskForDeviceParameters(). May be removed when hard-coding parameters.
			PhidgetReturnCode prc; //Used to catch error codes from each Phidget function call
			PhidgetStepperHandle ch = NULL;
			PhidgetEncoderHandle stpEnc;

#pragma region default Handlers

			/// make the member function static
			/**
			* Sets the DataInterval and Engages the Stepper
			* Displays info about the attached Phidget channel.
			* Fired when a Phidget channel with onAttachHandler registered attaches
			*
			* @param ph The Phidget channel that fired the attach event
			* @param *ctx Context pointer. Used to pass information to the event handler.
			*/
			static void CCONV onAttachHandler(PhidgetHandle ph, void *ctx) {

				  PhidgetReturnCode prc; //Used to catch error codes from each Phidget function call
				  Phidget_DeviceClass deviceClass;
				  const char* channelClassName;
				  int32_t serialNumber;
				  int32_t hubPort;
				  int32_t channel;

				  /*
				  * Engage the Stepper inside of the attach handler to allow the motor to move to its target position
				  * The motor will only track a target position if it is engaged.
				  * Engaged can be set to True to enable the servo, or False to disable it.
				  */

				  prc = PhidgetStepper_setEngaged((PhidgetStepperHandle)ph, 1);
				  OutputDebugStringW(L"Phidgets is attached +++++++++++++++++++++++++++++");

			}

			/**
			* Displays info about the detached Phidget channel.
			* Fired when a Phidget channel with onDetachHandler registered detaches
			*
			* @param ph The Phidget channel that fired the detach event
			* @param *ctx Context pointer. Used to pass information to the event handler.
			*/
			static void CCONV onDetachHandler(PhidgetHandle ph, void *ctx) {

				  PhidgetReturnCode prc; //Used to catch error codes from each Phidget function call
				  prc = PhidgetStepper_setEngaged((PhidgetStepperHandle)ph, false);

			}

			static void CCONV onErrorHandler(PhidgetHandle ph, void *ctx, Phidget_ErrorEventCode errorCode, const char *errorString) {

				  fprintf(stderr, "[Phidget Error Event] -> %s (%d)\n", errorString, errorCode);
			}

			/**
			* Outputs the Stepper's most recently detected position change
			* Fired when a Stepper channel with onPositionChangeHandler registered detects a position change
			*
			* @param ph The Stepper channel that fired the PositionChange event
			* @param *ctx Context pointer. Used to pass information to the event handler.
			* @param position The reported position from the Stepper channel
			*/
			static void CCONV onPositionChangeHandler(PhidgetStepperHandle ph, void *ctx, double position) {

				  //If you are unsure how to use more than one Phidget channel with this event, we recommend going to
				  //www.phidgets.com/docs/Using_Multiple_Phidgets for information

				  //std::string output = "Position changed to :  " + std::to_string(position) ;
				  //OutputDebugStringW(L"Position changed");
			}

			static void CCONV onStopped(PhidgetStepperHandle ch, void *ctx) {

				  PhidgetsMotor PhObj;
				  PhObj.motor_has_stopped();

			}


#pragma endregion

	  public:

			PhidgetsMotor()
			{
				  // Stepper_Initialise();
			}

			bool Stepper_Initialise()
			{


				  prc = PhidgetStepper_getEngaged(ch, &stepper_engaged);

				  if (stepper_engaged != 1) {
						/*
						* Allocate a new Phidget Channel object
						*/

						// setup new obj of ch with serial number, and the hubport
						prc = PhidgetStepper_create(&ch);
						if (prc != EPHIDGET_OK) {
							  return 0;
						}
						prc = Phidget_openWaitForAttachment((PhidgetHandle)ch, 5000); //kw:!!!
						if (prc != EPHIDGET_OK) {
							return 0;
						}
						prc = Phidget_setDeviceSerialNumber((PhidgetHandle)ch, phidget_serial_number);
						if (prc != EPHIDGET_OK) {
							  return 0;
						}
						//prc = Phidget_setHubPort((PhidgetHandle)ch, -1);
						//if (prc != EPHIDGET_OK) {
						//return 0;
						//}
						prc = Phidget_setChannel((PhidgetHandle)ch, 0);
						if (prc != EPHIDGET_OK) {
							  return 0;
						}


						/*
						* Add event handlers before calling open so that no events are missed.
						*/
						prc = Phidget_setOnAttachHandler((PhidgetHandle)ch, onAttachHandler, NULL);
						if (prc != EPHIDGET_OK) {
							  return 0;
						}
						prc = Phidget_setOnDetachHandler((PhidgetHandle)ch, onDetachHandler, NULL);
						if (prc != EPHIDGET_OK) {
							  return 0;
						}
						prc = Phidget_setOnErrorHandler((PhidgetHandle)ch, onErrorHandler, NULL);
						if (prc != EPHIDGET_OK) {
							  return 0;
						}
						prc = PhidgetStepper_setOnPositionChangeHandler(ch, onPositionChangeHandler, NULL);
						if (prc != EPHIDGET_OK) {
							  return 0;
						}


						///*
						//* Attach to the motor and wait for the feedback
						//*/
						//prc = Phidget_openWaitForAttachment((PhidgetHandle)ch, 5000);
						//if (prc != EPHIDGET_OK) {
						//	  return 0;
						//}


						/*
						* get min and max possible position and put them in their address. Setup initial parameters
						*/
						prc = PhidgetStepper_getMaxPosition(ch, &maxPosition);
						if (prc != EPHIDGET_OK) {
							  return 0;
						}
						prc = PhidgetStepper_getMinPosition(ch, &minPosition);
						if (prc != EPHIDGET_OK) {
							  return 0;
						}
						prc = PhidgetStepper_setVelocityLimit(ch, max_velocity);
						if (prc != EPHIDGET_OK) {
							  return 0;
						}
						prc = PhidgetStepper_setCurrentLimit(ch, max_current);
						if (prc != EPHIDGET_OK) {
							  return 0;
						}
						prc = PhidgetStepper_setAcceleration(ch, max_acceleration);
						if (prc != EPHIDGET_OK) {
							  return 0;
						}


						/*
						* if all goes well, return 1
						*/
						PhidgetStepper_setEngaged(ch, 1);
						return 1;
				  }
				  else {
						return 1;
				  }

			}


#pragma region methods to override from motorBase 


			int getMilliCount() {
				  timeb tb;
				  ftime(&tb);
				  int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
				  return nCount;
			}

			int getMilliSpan(int nTimeStart) {
				  int nSpan = getMilliCount() - nTimeStart;
				  if (nSpan < 0)
						nSpan += 0x100000 * 1000;
				  return nSpan;
			}

			void motor_has_stopped() {


				  //moveTo(0);
				  OutputDebugStringW(L"\n----------Motor stopped");
				  //double duration_time = getMilliSpan(time_start);
				  //duration = (std::clock() - time_start) / (double)CLOCKS_PER_SEC;
				  //std::string duration = " the stepper duration is: " + std::to_string(duration_time);
				  //std::wstring stemp = std::wstring(duration.begin(), duration.end());
				  //LPCWSTR sw = stemp.c_str();
				  //OutputDebugString(sw);

			}

			double moveTo(double pos_degree, bool return_round = false) {

				  // move to absolute position 
				  //time_start = getMilliCount();

				  degreesToMove = pos_degree;
				  stepstoMove = pos_degree * stepsPerDegree;

				  prc = PhidgetStepper_setTargetPosition(ch, stepstoMove);

				  if (prc != EPHIDGET_OK) {
						return 0;
				  }

				  prc = PhidgetStepper_setOnStoppedHandler(ch, onStopped, NULL);
				  if (prc != EPHIDGET_OK) {
						return 0;
				  }

				  //std::clock_t c_end = std::clock();
/*				  auto c_end = std::chrono::high_resolution_clock::now();

				  duration = (std::clock() - c_start) / (double)CLOCKS_PER_SEC;
				  std::string s_duration = " the stepper duration is: " + std::to_string(duration);
				  std::wstring stemp = std::wstring(s_duration.begin(), s_duration.end());
				  LPCWSTR sw = stemp.c_str();
				  OutputDebugString(sw);*/

				  //std::cout << std::fixed << std::setprecision(2) << "Motor:    CPU time used: "
						//<< 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
						//<< "Wall clock time passed: "
						//<< std::chrono::duration<double, std::milli>(t_end - t_start).count()
						//<< " ms\n";

				  return 1;
			}

			bool setStepperSpeed(double speed) {

				  double phSpeed = VeltoPhidgetVel(speed);
				  prc = PhidgetStepper_setVelocityLimit(ch, phSpeed);
				  if (prc != EPHIDGET_OK) {
						return 0;
				  }
				  return 1;
			}

			bool setStepperAcc(double Acc) {
				  prc = PhidgetStepper_setAcceleration(ch, Acc);
				  if (prc != EPHIDGET_OK) {
						return 0;
				  }
				  return 1;
			}

			double getPosition() {

				  prc = PhidgetStepper_getPosition(ch, &curr_pos);
				  if (prc != EPHIDGET_OK) {
						return 0;
				  }
				  return curr_pos * degreePerStep;
			}

			bool resetPosition() {

				  prc = PhidgetStepper_setTargetPosition(ch, 0);
				  Sleep(3000); // wait until motor goes back to its default position
				  if (prc != EPHIDGET_OK) {
						return 0;
				  }
				  return 1;

			}

			double getDegreeResolution() {

				  return degreePerStep;
			}

			bool disengageMotor() {

				  prc = PhidgetStepper_setEngaged((PhidgetStepperHandle)ch, false);
				  if (prc != EPHIDGET_OK) {
						return 0;
				  }
				  return 1;
			}

			bool setCurrPosZero() {

				  // set the current position to zero only if the motor is stationary

				  PhidgetStepper_getIsMoving((PhidgetStepperHandle)ch, &isMotorMoving);
				  while (isMotorMoving == 1) {
						PhidgetStepper_getIsMoving((PhidgetStepperHandle)ch, &isMotorMoving);
				  }
				  return 1;				  //prc = PhidgetEncoder_setPosition((PhidgetEncoderHandle)stpEnc, (int64_t)0);
				  //if (prc != EPHIDGET_OK) {
						//return 0;
				  //}
				  //return 1;

			}

			bool isStepperMoving() {

				  PhidgetStepper_getIsMoving((PhidgetStepperHandle)ch, &isMotorMoving);
				  return isMotorMoving;
			}


			double VeltoPhidgetVel(double phVel) {

				  if (phVel > max_velocity)
						phVel = max_velocity;
				  return phVel;

			}

#pragma endregion 

	  };

}
