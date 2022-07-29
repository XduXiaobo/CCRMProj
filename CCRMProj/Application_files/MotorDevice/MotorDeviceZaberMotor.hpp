#pragma once
#undef ERROR
#undef min
#include "stdafx.h"
#include <iomanip>      // std::setprecision
#include <iostream>
#include <conio.h> 
#include <fstream>
#include <string>
#include <ctime>
#include <windows.h>
#include <math.h>
#include <ctype.h>
#include "C:\Users\ZHOU Zhaokang\Desktop\CCRMProj_v2.0\CCRMProj\CCRMProj\IDSLib.h"
#include <cstdlib>
#include <sys/timeb.h>
#include <time.h>
#undef ERROR
#undef min
#include <zaber/motion/ascii.h>
#include <zaber/motion/ascii/axis_settings.h>




namespace UFCam
{
	using namespace zaber::motion;
	using namespace zaber::motion::ascii;
	//static, but non-const data members should be defined outside of the class definition and inside the namespace enclosing the class.
	//static double duration;

	//static std::clock_t t_start;
	class ZaberMotor
	{

	private:

		//std::clock_t time_start;
		//std::clock_t time_end;
		int max_velocity = 640000;
		int velocity = max_velocity;
		int acceleration = max_acceleration;
		int max_current = 0;
		int max_acceleration = 0;
		int end = 0;
		double stepsPerDegree = 35.56;
		double microSetpSize = 0.028125; //0.028125 degree
		int position;
		double curr_pos;
		int isMotorMoving;
		double stepstoMove = 0;
		double degreesToMove = 0;

		double pi = acos(-1);
		double L = 0.1; //orthogonal distance between the mirror and detector surface
		double pixel = 5.86e-6;//width of each pixel
		double fps_velocity = 0.0;//After input FPS, the real-time speed of the motor is obtained.

	public:
		Connection connection = Connection::openSerialPort("COM3");
		std::vector<Device> deviceList = connection.detectDevices();
		Device device = deviceList[0];
		Axis axis = device.getAxis(1);


		ZaberMotor()
		{
			// Stepper_Initialise();
		}

		bool Stepper_Initialise()
		{
			//open a serial port
			Connection connection = Connection::openSerialPort("COM3");
			std::vector<Device> deviceList = connection.detectDevices();
			Device device = deviceList[0];
			Axis axis = device.getAxis(1);

			max_current = axis.getSettings().get("motor.current.max", Units::NATIVE);
			axis.getSettings().set("motor.current.max", max_current, Units::NATIVE);

			position = axis.getPosition(Units::NATIVE);

			//set velocity and acceleration		
			max_acceleration = axis.getSettings().get("motion.accelonly", Units::NATIVE);
			axis.moveVelocity(max_velocity, Units::NATIVE, max_acceleration, Units::NATIVE);

			return 1;

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


			// move to target position 
			//time_start = getMilliCount();

			degreesToMove = pos_degree;
			stepstoMove = pos_degree / microSetpSize;

			axis.moveRelative(stepstoMove, Units::NATIVE, true, velocity, Units::NATIVE, acceleration, Units::NATIVE);


			//prc = PhidgetStepper_setOnStoppedHandler(ch, onStopped, NULL);


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

			velocity = speed;
			axis.moveVelocity(velocity, Units::NATIVE);
		}

		bool setStepperAcc(double Acc) {
			acceleration = Acc;
			axis.moveVelocity(velocity, Units::NATIVE, acceleration, Units::NATIVE);
			return 1;
		}

		double getPosition() {


			curr_pos = axis.getPosition(Units::NATIVE);

			return curr_pos * microSetpSize;
		}

		bool resetPosition() {
			axis.moveAbsolute(0, Units::NATIVE, true, velocity, Units::NATIVE, acceleration, Units::NATIVE);
		}

		double getDegreeResolution() {

			return microSetpSize;
		}

		bool disengageMotor() {

			axis.park();
			return 1;
		}

		bool setCurrPosZero() {

			return 1;

		}

		bool isStepperMoving() {

			isMotorMoving = axis.isBusy();
			return isMotorMoving;
		}


		//After input FPS, the  speed of the motor is obtained.
		double setFps(double fps) {
			double motorSpeed = (fps * pixel) / (2 * pi * L) * 360 * stepsPerDegree;
			axis.getSettings().set("maxspeed", motorSpeed, Units::NATIVE);
			fps_velocity = axis.getSettings().get("maxspeed", Units::NATIVE);


			return fps_velocity;

		}


		/*
				double VeltoPhidgetVel(double phVel) {

					if (phVel > 30)
						phVel = 30;
					return (phVel * max_velocity) / 30;

				}
		*/
#pragma endregion 

	};

}
