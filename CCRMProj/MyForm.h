#pragma once
#include "stdafx.h"
#include <chrono>
#include "Application_files\MotorDevice\MotorDevicePhidgetsMotor.hpp"
//#include "Application_files\MotorDevice\MotorDeviceZaberMotor.hpp"
//#include <zaber/motion/ascii.h>
//#include <zaber/motion/ascii/axis_settings.h>
#include "Application_files\CameraDevice\CameraDeviceIDS.hpp"
#include "IDSLib.h"	
#include <ctype.h>
#include <Windows.h>
#include <opencv2/core.hpp>
#include "opencv2/opencv_modules.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cctype>
#include <vector> // for 2D vector 
namespace UFCam {
	//Zhongyao part
	//using namespace zaber::motion;
	//using namespace zaber::motion::ascii;
	//
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows;
	using namespace System::Windows::Forms;
	//using namespace std::this_thread; // sleep_for, sleep_until
	using namespace System::Threading;

	using namespace cv;

	//UFCam::PhidgetsMotor ph_motor;
	UFCam::PhidgetsMotor ph_motor;
	UFCam::IDSCamera IDS_Cam;

	// Note: class definition is in Camera_application.h file meaning all the #include files for motor / camera mus be implemneted in there
	double res = 0;
	double scanDegree = 2;  // 9 * 0.2
	bool image_in_frame = false;
	bool stop_live_view = false;
	bool frameCleared = false;
	bool pdStateCHanged = false;
	double stepspeed = 30;
	double acceleration = 100000;
	/// <summary>
	/// Zhaokang part
	/// </summary>
	double pixel_size = 0.00005;//pixel_size
	double orthogonal_distance = 0.2;//orthogonal_distance
	double Number_of_Frames = 0.00000000;
	double FPS = 0.00000000;
	double arm_length=0.1;
	double rotation_speed=0.00000000;
	double line_speed=0.00000000;
	double Rescale_factor =0.0041875;
	double maximum_step_speed = 0.00000000;
	double exposuretime = 0.00000000;
	double scan_degree;
	int par_ControlMode=1;
	//wchar_t motor1[] = L"Phidgets";
	//double size_motor1 = size(motor1);
	//wchar_t motor2[] = L"Zaber";
	//double size_motor2 = size(motor2);
	int parameter_speed=0;
	int parameter_time=0;
	int parameter_distance=0;
	// variables for serial com with Arduino
	char* port = "\\\\.\\COM3";
	//char output[MAX_DATA_LENGTH];
	float lightInt = 0;
	//char incomingData[MAX_DATA_LENGTH];
	auto prevState = "l";
	time_t currentTime;
	struct tm* localTime;
	std::vector<int> tStampsMin;
	std::vector<int> tStampsSec;


	/// <summary>
	/// 
	/// </summary>
	public ref class Work
	{
	public:Work()
	{
	}
		  void Print()
		  {
			  // obtain reference to currently executing thread
			  Thread^ current = Thread::CurrentThread;

			  // put thread to sleep for sleepTime amount of time
			  std::string msg = " starting thread ";
			  std::wstring stemp = std::wstring(msg.begin(), msg.end());
			  LPCWSTR sw = stemp.c_str();
			  OutputDebugString((LPCWSTR)sw);


			  Thread::Sleep(5000);

		  } // end method Print
	};




	void ThreadProcImageQueue();
	void InvokeDelegate(Object^ obj);
	/// <summary>
	/// Summary for MyForm
	/// </summary>

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			Setup();

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}

			IDS_Cam.CloseCamera();
			bool reset_pos = false;
			reset_pos = ph_motor.resetPosition();
			if (reset_pos) {
				ph_motor.disengageMotor();
			}

		}
	private: System::Windows::Forms::Button^ InitialiseBtn;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ Motor_found_label;
	private: System::Windows::Forms::Label^ Camera_found_label;


	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::TextBox^ MotorAccInp;
	private: System::Windows::Forms::TextBox^ MotorSpdInp;
	private: System::Windows::Forms::TextBox^ movePosInp;
	private: System::Windows::Forms::Label^ Motor_curr_pos;
	private: System::Windows::Forms::Label^ label5;

	private: System::Windows::Forms::Button^ SetMotorSpeedBtn;
	private: System::Windows::Forms::Button^ MoveBtn;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ l;
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::CheckBox^ gainBoost;
	private: System::Windows::Forms::TextBox^ camGammaInp;
	private: System::Windows::Forms::Label^ Cam_curr_gamma;
	private: System::Windows::Forms::Label^ Cam_curr_gain;
	private: System::Windows::Forms::TextBox^ camGainInp;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Button^ LiveStopBtn;
	private: System::Windows::Forms::Button^ SetCamBtn;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Button^ LiveStartBtn;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::Button^ CaptureBtn;
	private: System::Windows::Forms::Button^ ScanBtn;
	private: System::Windows::Forms::TextBox^ box_scandegree;


	private: System::Windows::Forms::Button^ CalibBtn;
	private: System::Windows::Forms::Button^ SaveBtn;
	private: System::Windows::Forms::GroupBox^ groupBox3;
	private: System::Windows::Forms::TextBox^ MotorMoveFPS;



	private: System::Windows::Forms::Label^ label12;




	private: System::Windows::Forms::TextBox^ MotorMoveFrames;





	private: System::Windows::Forms::Label^ label13;
	private: System::Windows::Forms::RichTextBox^ richTextBox1;

	private: System::Windows::Forms::TextBox^ ExposureTimeInp;
	private: System::Windows::Forms::Label^ label14;

	private: System::Windows::Forms::GroupBox^ groupBox4;
	private: System::Windows::Forms::TextBox^ box_Orthogonal_Distance;

	private: System::Windows::Forms::Label^ label15;
	private: System::Windows::Forms::TextBox^ box_Pixel_Size;



	private: System::Windows::Forms::Label^ label16;

private: System::Windows::Forms::TextBox^ box_RotationSpeed;
private: System::Windows::Forms::Label^ label18;
private: System::Windows::Forms::TextBox^ box_Arm_Length;

private: System::Windows::Forms::Label^ label17;
private: System::Windows::Forms::TextBox^ box_MaximumStepSpeed;

private: System::Windows::Forms::Label^ label19;



private: System::Windows::Forms::Button^ movemotor_3_button;
private: System::Windows::Forms::Button^ calculateall_button;
private: System::Windows::Forms::Label^ label9;
private: System::Windows::Forms::Label^ label20;
private: System::Windows::Forms::Label^ label21;
private: System::Windows::Forms::Label^ label23;
private: System::Windows::Forms::Label^ label22;
private: System::Windows::Forms::Label^ label24;
private: System::Windows::Forms::Button^ button_clear;
private: System::Windows::Forms::Label^ label25;
private: System::Windows::Forms::Button^ button1;
private: System::Windows::Forms::Label^ Text_ControlMode;
private: System::Windows::Forms::Label^ label26;
private: System::Windows::Forms::Button^ button_ControlMode;





























	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->InitialiseBtn = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->Motor_found_label = (gcnew System::Windows::Forms::Label());
			this->Camera_found_label = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->MotorSpdInp = (gcnew System::Windows::Forms::TextBox());
			this->movePosInp = (gcnew System::Windows::Forms::TextBox());
			this->Motor_curr_pos = (gcnew System::Windows::Forms::Label());
			this->MoveBtn = (gcnew System::Windows::Forms::Button());
			this->SetMotorSpeedBtn = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->l = (gcnew System::Windows::Forms::Label());
			this->ExposureTimeInp = (gcnew System::Windows::Forms::TextBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->MotorAccInp = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->gainBoost = (gcnew System::Windows::Forms::CheckBox());
			this->camGammaInp = (gcnew System::Windows::Forms::TextBox());
			this->Cam_curr_gamma = (gcnew System::Windows::Forms::Label());
			this->Cam_curr_gain = (gcnew System::Windows::Forms::Label());
			this->camGainInp = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->LiveStopBtn = (gcnew System::Windows::Forms::Button());
			this->SetCamBtn = (gcnew System::Windows::Forms::Button());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->LiveStartBtn = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->CaptureBtn = (gcnew System::Windows::Forms::Button());
			this->ScanBtn = (gcnew System::Windows::Forms::Button());
			this->box_scandegree = (gcnew System::Windows::Forms::TextBox());
			this->CalibBtn = (gcnew System::Windows::Forms::Button());
			this->SaveBtn = (gcnew System::Windows::Forms::Button());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->box_RotationSpeed = (gcnew System::Windows::Forms::TextBox());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->box_MaximumStepSpeed = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->MotorMoveFPS = (gcnew System::Windows::Forms::TextBox());
			this->MotorMoveFrames = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->box_Orthogonal_Distance = (gcnew System::Windows::Forms::TextBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->box_Pixel_Size = (gcnew System::Windows::Forms::TextBox());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->box_Arm_Length = (gcnew System::Windows::Forms::TextBox());
			this->movemotor_3_button = (gcnew System::Windows::Forms::Button());
			this->calculateall_button = (gcnew System::Windows::Forms::Button());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->button_clear = (gcnew System::Windows::Forms::Button());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->Text_ControlMode = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->button_ControlMode = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->SuspendLayout();
			// 
			// InitialiseBtn
			// 
			this->InitialiseBtn->BackColor = System::Drawing::Color::RoyalBlue;
			this->InitialiseBtn->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->InitialiseBtn->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->InitialiseBtn->Location = System::Drawing::Point(18, 47);
			this->InitialiseBtn->Margin = System::Windows::Forms::Padding(4);
			this->InitialiseBtn->Name = L"InitialiseBtn";
			this->InitialiseBtn->Size = System::Drawing::Size(150, 64);
			this->InitialiseBtn->TabIndex = 0;
			this->InitialiseBtn->Text = L"Initialise";
			this->InitialiseBtn->UseVisualStyleBackColor = false;
			this->InitialiseBtn->Click += gcnew System::EventHandler(this, &MyForm::InitialiseBtn_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->label1->Location = System::Drawing::Point(288, 47);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(116, 25);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Motor ready";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->label2->Location = System::Drawing::Point(507, 47);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(136, 25);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Camera ready";
			// 
			// Motor_found_label
			// 
			this->Motor_found_label->AutoSize = true;
			this->Motor_found_label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Motor_found_label->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->Motor_found_label->Location = System::Drawing::Point(320, 87);
			this->Motor_found_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->Motor_found_label->Name = L"Motor_found_label";
			this->Motor_found_label->Size = System::Drawing::Size(60, 25);
			this->Motor_found_label->TabIndex = 1;
			this->Motor_found_label->Text = L"False";
			// 
			// Camera_found_label
			// 
			this->Camera_found_label->AutoSize = true;
			this->Camera_found_label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Camera_found_label->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->Camera_found_label->Location = System::Drawing::Point(546, 87);
			this->Camera_found_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->Camera_found_label->Name = L"Camera_found_label";
			this->Camera_found_label->Size = System::Drawing::Size(60, 25);
			this->Camera_found_label->TabIndex = 1;
			this->Camera_found_label->Text = L"False";
			// 
			// groupBox1
			// 
			this->groupBox1->BackColor = System::Drawing::Color::Transparent;
			this->groupBox1->Controls->Add(this->label23);
			this->groupBox1->Controls->Add(this->MotorSpdInp);
			this->groupBox1->Controls->Add(this->movePosInp);
			this->groupBox1->Controls->Add(this->Motor_curr_pos);
			this->groupBox1->Controls->Add(this->MoveBtn);
			this->groupBox1->Controls->Add(this->SetMotorSpeedBtn);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->l);
			this->groupBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->groupBox1->ForeColor = System::Drawing::Color::GreenYellow;
			this->groupBox1->Location = System::Drawing::Point(18, 119);
			this->groupBox1->Margin = System::Windows::Forms::Padding(4);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(4);
			this->groupBox1->Size = System::Drawing::Size(702, 295);
			this->groupBox1->TabIndex = 2;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Motor Settings";
			this->groupBox1->Enter += gcnew System::EventHandler(this, &MyForm::groupBox1_Enter);
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8));
			this->label23->ForeColor = System::Drawing::Color::Ivory;
			this->label23->Location = System::Drawing::Point(25, 188);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(240, 20);
			this->label23->TabIndex = 19;
			this->label23->Text = L"Just move the motor, no camera.";
			// 
			// MotorSpdInp
			// 
			this->MotorSpdInp->Location = System::Drawing::Point(307, 146);
			this->MotorSpdInp->Margin = System::Windows::Forms::Padding(4);
			this->MotorSpdInp->Name = L"MotorSpdInp";
			this->MotorSpdInp->Size = System::Drawing::Size(148, 32);
			this->MotorSpdInp->TabIndex = 2;
			// 
			// movePosInp
			// 
			this->movePosInp->Location = System::Drawing::Point(306, 87);
			this->movePosInp->Margin = System::Windows::Forms::Padding(4);
			this->movePosInp->Name = L"movePosInp";
			this->movePosInp->Size = System::Drawing::Size(148, 32);
			this->movePosInp->TabIndex = 2;
			// 
			// Motor_curr_pos
			// 
			this->Motor_curr_pos->AutoSize = true;
			this->Motor_curr_pos->Location = System::Drawing::Point(302, 43);
			this->Motor_curr_pos->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->Motor_curr_pos->Name = L"Motor_curr_pos";
			this->Motor_curr_pos->Size = System::Drawing::Size(149, 26);
			this->Motor_curr_pos->TabIndex = 1;
			this->Motor_curr_pos->Text = L"motor position";
			// 
			// MoveBtn
			// 
			this->MoveBtn->BackColor = System::Drawing::Color::DarkGreen;
			this->MoveBtn->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->MoveBtn->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->MoveBtn->Location = System::Drawing::Point(506, 242);
			this->MoveBtn->Margin = System::Windows::Forms::Padding(4);
			this->MoveBtn->Name = L"MoveBtn";
			this->MoveBtn->Size = System::Drawing::Size(178, 35);
			this->MoveBtn->TabIndex = 0;
			this->MoveBtn->Text = L"Move motor";
			this->MoveBtn->UseVisualStyleBackColor = false;
			this->MoveBtn->Click += gcnew System::EventHandler(this, &MyForm::MoveBtn_Click);
			// 
			// SetMotorSpeedBtn
			// 
			this->SetMotorSpeedBtn->BackColor = System::Drawing::Color::DarkGreen;
			this->SetMotorSpeedBtn->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SetMotorSpeedBtn->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->SetMotorSpeedBtn->Location = System::Drawing::Point(508, 150);
			this->SetMotorSpeedBtn->Margin = System::Windows::Forms::Padding(4);
			this->SetMotorSpeedBtn->Name = L"SetMotorSpeedBtn";
			this->SetMotorSpeedBtn->Size = System::Drawing::Size(176, 38);
			this->SetMotorSpeedBtn->TabIndex = 0;
			this->SetMotorSpeedBtn->Text = L"Set speed";
			this->SetMotorSpeedBtn->UseVisualStyleBackColor = false;
			this->SetMotorSpeedBtn->Click += gcnew System::EventHandler(this, &MyForm::SetMotorSpeedBtn_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(15, 146);
			this->label4->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(155, 25);
			this->label4->TabIndex = 0;
			this->label4->Text = L"Set motor speed";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->ForeColor = System::Drawing::Color::White;
			this->label3->Location = System::Drawing::Point(15, 94);
			this->label3->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(153, 25);
			this->label3->TabIndex = 0;
			this->label3->Text = L"Move by degree";
			// 
			// l
			// 
			this->l->AutoSize = true;
			this->l->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->l->ForeColor = System::Drawing::Color::White;
			this->l->Location = System::Drawing::Point(15, 44);
			this->l->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->l->Name = L"l";
			this->l->Size = System::Drawing::Size(193, 25);
			this->l->TabIndex = 0;
			this->l->Text = L"Motor curent position";
			// 
			// ExposureTimeInp
			// 
			this->ExposureTimeInp->Location = System::Drawing::Point(232, 315);
			this->ExposureTimeInp->Margin = System::Windows::Forms::Padding(4);
			this->ExposureTimeInp->Name = L"ExposureTimeInp";
			this->ExposureTimeInp->Size = System::Drawing::Size(148, 32);
			this->ExposureTimeInp->TabIndex = 5;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label14->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->label14->Location = System::Drawing::Point(15, 322);
			this->label14->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(144, 25);
			this->label14->TabIndex = 4;
			this->label14->Text = L"Exposure Time";
			// 
			// MotorAccInp
			// 
			this->MotorAccInp->Location = System::Drawing::Point(295, 235);
			this->MotorAccInp->Margin = System::Windows::Forms::Padding(4);
			this->MotorAccInp->Name = L"MotorAccInp";
			this->MotorAccInp->Size = System::Drawing::Size(148, 32);
			this->MotorAccInp->TabIndex = 2;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label5->ForeColor = System::Drawing::Color::White;
			this->label5->Location = System::Drawing::Point(-5, 242);
			this->label5->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(206, 25);
			this->label5->TabIndex = 0;
			this->label5->Text = L"Set motor acceleration";
			// 
			// groupBox2
			// 
			this->groupBox2->BackColor = System::Drawing::Color::Transparent;
			this->groupBox2->Controls->Add(this->ExposureTimeInp);
			this->groupBox2->Controls->Add(this->gainBoost);
			this->groupBox2->Controls->Add(this->camGammaInp);
			this->groupBox2->Controls->Add(this->Cam_curr_gamma);
			this->groupBox2->Controls->Add(this->label14);
			this->groupBox2->Controls->Add(this->Cam_curr_gain);
			this->groupBox2->Controls->Add(this->camGainInp);
			this->groupBox2->Controls->Add(this->label7);
			this->groupBox2->Controls->Add(this->LiveStopBtn);
			this->groupBox2->Controls->Add(this->SetCamBtn);
			this->groupBox2->Controls->Add(this->label8);
			this->groupBox2->Controls->Add(this->label11);
			this->groupBox2->Controls->Add(this->LiveStartBtn);
			this->groupBox2->Controls->Add(this->label6);
			this->groupBox2->Controls->Add(this->label10);
			this->groupBox2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->groupBox2->ForeColor = System::Drawing::Color::GreenYellow;
			this->groupBox2->Location = System::Drawing::Point(742, 159);
			this->groupBox2->Margin = System::Windows::Forms::Padding(4);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Padding = System::Windows::Forms::Padding(4);
			this->groupBox2->Size = System::Drawing::Size(623, 359);
			this->groupBox2->TabIndex = 2;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Camera Settings";
			// 
			// gainBoost
			// 
			this->gainBoost->AutoSize = true;
			this->gainBoost->Location = System::Drawing::Point(231, 41);
			this->gainBoost->Margin = System::Windows::Forms::Padding(4);
			this->gainBoost->Name = L"gainBoost";
			this->gainBoost->Size = System::Drawing::Size(22, 21);
			this->gainBoost->TabIndex = 3;
			this->gainBoost->UseVisualStyleBackColor = true;
			// 
			// camGammaInp
			// 
			this->camGammaInp->Location = System::Drawing::Point(231, 220);
			this->camGammaInp->Margin = System::Windows::Forms::Padding(4);
			this->camGammaInp->Name = L"camGammaInp";
			this->camGammaInp->Size = System::Drawing::Size(148, 32);
			this->camGammaInp->TabIndex = 2;
			// 
			// Cam_curr_gamma
			// 
			this->Cam_curr_gamma->AutoSize = true;
			this->Cam_curr_gamma->Location = System::Drawing::Point(227, 174);
			this->Cam_curr_gamma->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->Cam_curr_gamma->Name = L"Cam_curr_gamma";
			this->Cam_curr_gamma->Size = System::Drawing::Size(170, 26);
			this->Cam_curr_gamma->TabIndex = 1;
			this->Cam_curr_gamma->Text = L"Camera gamma";
			// 
			// Cam_curr_gain
			// 
			this->Cam_curr_gain->AutoSize = true;
			this->Cam_curr_gain->Location = System::Drawing::Point(227, 79);
			this->Cam_curr_gain->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->Cam_curr_gain->Name = L"Cam_curr_gain";
			this->Cam_curr_gain->Size = System::Drawing::Size(137, 26);
			this->Cam_curr_gain->TabIndex = 1;
			this->Cam_curr_gain->Text = L"Camera gain";
			// 
			// camGainInp
			// 
			this->camGainInp->Location = System::Drawing::Point(231, 115);
			this->camGainInp->Margin = System::Windows::Forms::Padding(4);
			this->camGainInp->Name = L"camGainInp";
			this->camGainInp->Size = System::Drawing::Size(148, 32);
			this->camGainInp->TabIndex = 2;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label7->ForeColor = System::Drawing::Color::White;
			this->label7->Location = System::Drawing::Point(12, 226);
			this->label7->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(112, 25);
			this->label7->TabIndex = 0;
			this->label7->Text = L"Set gamma";
			// 
			// LiveStopBtn
			// 
			this->LiveStopBtn->BackColor = System::Drawing::Color::Firebrick;
			this->LiveStopBtn->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->LiveStopBtn->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->LiveStopBtn->Location = System::Drawing::Point(423, 216);
			this->LiveStopBtn->Margin = System::Windows::Forms::Padding(4);
			this->LiveStopBtn->Name = L"LiveStopBtn";
			this->LiveStopBtn->Size = System::Drawing::Size(189, 44);
			this->LiveStopBtn->TabIndex = 0;
			this->LiveStopBtn->Text = L"Stop live";
			this->LiveStopBtn->UseVisualStyleBackColor = false;
			this->LiveStopBtn->Click += gcnew System::EventHandler(this, &MyForm::LiveStopBtn_Click);
			// 
			// SetCamBtn
			// 
			this->SetCamBtn->BackColor = System::Drawing::Color::DarkGreen;
			this->SetCamBtn->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SetCamBtn->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->SetCamBtn->Location = System::Drawing::Point(423, 37);
			this->SetCamBtn->Margin = System::Windows::Forms::Padding(4);
			this->SetCamBtn->Name = L"SetCamBtn";
			this->SetCamBtn->Size = System::Drawing::Size(188, 76);
			this->SetCamBtn->TabIndex = 0;
			this->SetCamBtn->Text = L"Set Camera settings";
			this->SetCamBtn->UseVisualStyleBackColor = false;
			this->SetCamBtn->Click += gcnew System::EventHandler(this, &MyForm::SetCamBtn_Click);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label8->ForeColor = System::Drawing::Color::White;
			this->label8->Location = System::Drawing::Point(12, 174);
			this->label8->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(200, 25);
			this->label8->TabIndex = 0;
			this->label8->Text = L"Gamma (range 0-2.2)";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label11->ForeColor = System::Drawing::Color::White;
			this->label11->Location = System::Drawing::Point(15, 120);
			this->label11->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(154, 25);
			this->label11->TabIndex = 0;
			this->label11->Text = L"Set camera gain";
			// 
			// LiveStartBtn
			// 
			this->LiveStartBtn->BackColor = System::Drawing::Color::DarkSlateGray;
			this->LiveStartBtn->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->LiveStartBtn->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->LiveStartBtn->Location = System::Drawing::Point(422, 152);
			this->LiveStartBtn->Margin = System::Windows::Forms::Padding(4);
			this->LiveStartBtn->Name = L"LiveStartBtn";
			this->LiveStartBtn->Size = System::Drawing::Size(189, 44);
			this->LiveStartBtn->TabIndex = 0;
			this->LiveStartBtn->Text = L"Start live";
			this->LiveStartBtn->UseVisualStyleBackColor = false;
			this->LiveStartBtn->Click += gcnew System::EventHandler(this, &MyForm::LiveStartBtn_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label6->ForeColor = System::Drawing::Color::White;
			this->label6->Location = System::Drawing::Point(15, 80);
			this->label6->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(53, 25);
			this->label6->TabIndex = 0;
			this->label6->Text = L"Gain";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label10->ForeColor = System::Drawing::Color::White;
			this->label10->Location = System::Drawing::Point(15, 40);
			this->label10->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(106, 25);
			this->label10->TabIndex = 0;
			this->label10->Text = L"Gain boost";
			// 
			// CaptureBtn
			// 
			this->CaptureBtn->BackColor = System::Drawing::Color::DarkSlateGray;
			this->CaptureBtn->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->CaptureBtn->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->CaptureBtn->Location = System::Drawing::Point(18, 764);
			this->CaptureBtn->Margin = System::Windows::Forms::Padding(4);
			this->CaptureBtn->Name = L"CaptureBtn";
			this->CaptureBtn->Size = System::Drawing::Size(189, 64);
			this->CaptureBtn->TabIndex = 0;
			this->CaptureBtn->Text = L"Single capture";
			this->CaptureBtn->UseVisualStyleBackColor = false;
			this->CaptureBtn->Click += gcnew System::EventHandler(this, &MyForm::CaptureBtn_Click);
			// 
			// ScanBtn
			// 
			this->ScanBtn->BackColor = System::Drawing::Color::RoyalBlue;
			this->ScanBtn->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ScanBtn->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->ScanBtn->ImageAlign = System::Drawing::ContentAlignment::BottomRight;
			this->ScanBtn->Location = System::Drawing::Point(18, 832);
			this->ScanBtn->Margin = System::Windows::Forms::Padding(4);
			this->ScanBtn->Name = L"ScanBtn";
			this->ScanBtn->Size = System::Drawing::Size(189, 73);
			this->ScanBtn->TabIndex = 0;
			this->ScanBtn->Text = L"Scan";
			this->ScanBtn->UseVisualStyleBackColor = false;
			this->ScanBtn->Click += gcnew System::EventHandler(this, &MyForm::ScanBtn_Click);
			// 
			// box_scandegree
			// 
			this->box_scandegree->Location = System::Drawing::Point(973, 575);
			this->box_scandegree->Margin = System::Windows::Forms::Padding(4);
			this->box_scandegree->Name = L"box_scandegree";
			this->box_scandegree->Size = System::Drawing::Size(148, 28);
			this->box_scandegree->TabIndex = 2;
			// 
			// CalibBtn
			// 
			this->CalibBtn->BackColor = System::Drawing::Color::DarkSlateGray;
			this->CalibBtn->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->CalibBtn->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->CalibBtn->Location = System::Drawing::Point(513, 800);
			this->CalibBtn->Margin = System::Windows::Forms::Padding(4);
			this->CalibBtn->Name = L"CalibBtn";
			this->CalibBtn->Size = System::Drawing::Size(189, 44);
			this->CalibBtn->TabIndex = 0;
			this->CalibBtn->Text = L"Calibrate";
			this->CalibBtn->UseVisualStyleBackColor = false;
			this->CalibBtn->Click += gcnew System::EventHandler(this, &MyForm::CalibBtn_Click);
			// 
			// SaveBtn
			// 
			this->SaveBtn->BackColor = System::Drawing::Color::DarkSlateGray;
			this->SaveBtn->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SaveBtn->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->SaveBtn->Location = System::Drawing::Point(510, 861);
			this->SaveBtn->Margin = System::Windows::Forms::Padding(4);
			this->SaveBtn->Name = L"SaveBtn";
			this->SaveBtn->Size = System::Drawing::Size(189, 44);
			this->SaveBtn->TabIndex = 0;
			this->SaveBtn->Text = L"Save";
			this->SaveBtn->UseVisualStyleBackColor = false;
			this->SaveBtn->Click += gcnew System::EventHandler(this, &MyForm::SaveBtn_Click);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->label22);
			this->groupBox3->Controls->Add(this->box_RotationSpeed);
			this->groupBox3->Controls->Add(this->label18);
			this->groupBox3->Controls->Add(this->label19);
			this->groupBox3->Controls->Add(this->box_MaximumStepSpeed);
			this->groupBox3->Controls->Add(this->label12);
			this->groupBox3->Controls->Add(this->MotorMoveFPS);
			this->groupBox3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11));
			this->groupBox3->ForeColor = System::Drawing::Color::GreenYellow;
			this->groupBox3->Location = System::Drawing::Point(1399, 137);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(384, 395);
			this->groupBox3->TabIndex = 3;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Speed Setting";
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8));
			this->label22->ForeColor = System::Drawing::Color::Ivory;
			this->label22->Location = System::Drawing::Point(17, 35);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(253, 20);
			this->label22->TabIndex = 16;
			this->label22->Text = L"Only need one among those three.";
			// 
			// box_RotationSpeed
			// 
			this->box_RotationSpeed->Location = System::Drawing::Point(218, 128);
			this->box_RotationSpeed->Margin = System::Windows::Forms::Padding(4);
			this->box_RotationSpeed->Name = L"box_RotationSpeed";
			this->box_RotationSpeed->Size = System::Drawing::Size(148, 32);
			this->box_RotationSpeed->TabIndex = 14;
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label18->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->label18->Location = System::Drawing::Point(7, 132);
			this->label18->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(182, 25);
			this->label18->TabIndex = 13;
			this->label18->Text = L"Rotation Speed(r/s)";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label19->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->label19->Location = System::Drawing::Point(4, 174);
			this->label19->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(206, 25);
			this->label19->TabIndex = 15;
			this->label19->Text = L"Maximum Step Speed";
			// 
			// box_MaximumStepSpeed
			// 
			this->box_MaximumStepSpeed->Location = System::Drawing::Point(218, 178);
			this->box_MaximumStepSpeed->Margin = System::Windows::Forms::Padding(4);
			this->box_MaximumStepSpeed->Name = L"box_MaximumStepSpeed";
			this->box_MaximumStepSpeed->Size = System::Drawing::Size(148, 32);
			this->box_MaximumStepSpeed->TabIndex = 16;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label12->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->label12->Location = System::Drawing::Point(7, 89);
			this->label12->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(51, 25);
			this->label12->TabIndex = 3;
			this->label12->Text = L"FPS";
			// 
			// MotorMoveFPS
			// 
			this->MotorMoveFPS->Location = System::Drawing::Point(218, 82);
			this->MotorMoveFPS->Margin = System::Windows::Forms::Padding(4);
			this->MotorMoveFPS->Name = L"MotorMoveFPS";
			this->MotorMoveFPS->Size = System::Drawing::Size(148, 32);
			this->MotorMoveFPS->TabIndex = 3;
			// 
			// MotorMoveFrames
			// 
			this->MotorMoveFrames->Location = System::Drawing::Point(973, 526);
			this->MotorMoveFrames->Margin = System::Windows::Forms::Padding(4);
			this->MotorMoveFrames->Name = L"MotorMoveFrames";
			this->MotorMoveFrames->Size = System::Drawing::Size(148, 28);
			this->MotorMoveFrames->TabIndex = 6;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label13->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->label13->Location = System::Drawing::Point(757, 526);
			this->label13->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(178, 25);
			this->label13->TabIndex = 5;
			this->label13->Text = L"Number of  Frames";
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(1347, 593);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(409, 168);
			this->richTextBox1->TabIndex = 4;
			this->richTextBox1->Text = L"";
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->button1);
			this->groupBox4->Controls->Add(this->label20);
			this->groupBox4->Controls->Add(this->MotorAccInp);
			this->groupBox4->Controls->Add(this->box_Orthogonal_Distance);
			this->groupBox4->Controls->Add(this->label15);
			this->groupBox4->Controls->Add(this->box_Pixel_Size);
			this->groupBox4->Controls->Add(this->label5);
			this->groupBox4->Controls->Add(this->label16);
			this->groupBox4->Controls->Add(this->label17);
			this->groupBox4->Controls->Add(this->box_Arm_Length);
			this->groupBox4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11));
			this->groupBox4->ForeColor = System::Drawing::Color::GreenYellow;
			this->groupBox4->Location = System::Drawing::Point(30, 421);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(690, 304);
			this->groupBox4->TabIndex = 8;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Basic Parameter Setting";
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::DarkGreen;
			this->button1->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->button1->Location = System::Drawing::Point(496, 242);
			this->button1->Margin = System::Windows::Forms::Padding(4);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(176, 38);
			this->button1->TabIndex = 20;
			this->button1->Text = L"Set Parameter";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8));
			this->label20->ForeColor = System::Drawing::Color::Ivory;
			this->label20->Location = System::Drawing::Point(27, 30);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(274, 20);
			this->label20->TabIndex = 15;
			this->label20->Text = L"Those parameters have default value.";
			// 
			// box_Orthogonal_Distance
			// 
			this->box_Orthogonal_Distance->Location = System::Drawing::Point(295, 128);
			this->box_Orthogonal_Distance->Margin = System::Windows::Forms::Padding(4);
			this->box_Orthogonal_Distance->Name = L"box_Orthogonal_Distance";
			this->box_Orthogonal_Distance->Size = System::Drawing::Size(148, 32);
			this->box_Orthogonal_Distance->TabIndex = 6;
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label15->ForeColor = System::Drawing::Color::White;
			this->label15->Location = System::Drawing::Point(-5, 128);
			this->label15->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(190, 25);
			this->label15->TabIndex = 5;
			this->label15->Text = L"Orthogonal Distance";
			// 
			// box_Pixel_Size
			// 
			this->box_Pixel_Size->Location = System::Drawing::Point(295, 81);
			this->box_Pixel_Size->Margin = System::Windows::Forms::Padding(4);
			this->box_Pixel_Size->Name = L"box_Pixel_Size";
			this->box_Pixel_Size->Size = System::Drawing::Size(148, 32);
			this->box_Pixel_Size->TabIndex = 3;
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label16->ForeColor = System::Drawing::Color::White;
			this->label16->Location = System::Drawing::Point(-5, 81);
			this->label16->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(98, 25);
			this->label16->TabIndex = 3;
			this->label16->Text = L"Pixel Size";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label17->ForeColor = System::Drawing::Color::White;
			this->label17->Location = System::Drawing::Point(-5, 183);
			this->label17->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(113, 25);
			this->label17->TabIndex = 11;
			this->label17->Text = L"Arm Length";
			// 
			// box_Arm_Length
			// 
			this->box_Arm_Length->Location = System::Drawing::Point(295, 183);
			this->box_Arm_Length->Margin = System::Windows::Forms::Padding(4);
			this->box_Arm_Length->Name = L"box_Arm_Length";
			this->box_Arm_Length->Size = System::Drawing::Size(148, 32);
			this->box_Arm_Length->TabIndex = 12;
			// 
			// movemotor_3_button
			// 
			this->movemotor_3_button->BackColor = System::Drawing::Color::DarkGreen;
			this->movemotor_3_button->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->movemotor_3_button->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->movemotor_3_button->Location = System::Drawing::Point(893, 839);
			this->movemotor_3_button->Margin = System::Windows::Forms::Padding(4);
			this->movemotor_3_button->Name = L"movemotor_3_button";
			this->movemotor_3_button->Size = System::Drawing::Size(188, 44);
			this->movemotor_3_button->TabIndex = 3;
			this->movemotor_3_button->Text = L"Move motor";
			this->movemotor_3_button->UseVisualStyleBackColor = false;
			this->movemotor_3_button->Click += gcnew System::EventHandler(this, &MyForm::movemotor_3_button_Click);
			// 
			// calculateall_button
			// 
			this->calculateall_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->calculateall_button->Font = (gcnew System::Drawing::Font(L"Arial", 15, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->calculateall_button->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->calculateall_button->Location = System::Drawing::Point(847, 711);
			this->calculateall_button->Margin = System::Windows::Forms::Padding(4);
			this->calculateall_button->Name = L"calculateall_button";
			this->calculateall_button->Size = System::Drawing::Size(302, 85);
			this->calculateall_button->TabIndex = 3;
			this->calculateall_button->Text = L"Calculate All";
			this->calculateall_button->UseVisualStyleBackColor = false;
			this->calculateall_button->Click += gcnew System::EventHandler(this, &MyForm::calculateall_button_Click);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label9->ForeColor = System::Drawing::Color::Silver;
			this->label9->Location = System::Drawing::Point(754, 578);
			this->label9->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(127, 25);
			this->label9->TabIndex = 1;
			this->label9->Text = L"Scan Degree";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20));
			this->label21->ForeColor = System::Drawing::Color::Red;
			this->label21->Location = System::Drawing::Point(734, 37);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(730, 46);
			this->label21->TabIndex = 16;
			this->label21->Text = L"ATTENTION: MUST TYPE IN NUMBER";
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8));
			this->label24->ForeColor = System::Drawing::Color::Ivory;
			this->label24->Location = System::Drawing::Point(727, 800);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(617, 20);
			this->label24->TabIndex = 16;
			this->label24->Text = L"Set 2 among Speed Setting, Exposure Time, and Number of Frames to use this button"
				L"";
			// 
			// button_clear
			// 
			this->button_clear->BackColor = System::Drawing::Color::DarkGreen;
			this->button_clear->Font = (gcnew System::Drawing::Font(L"Arial", 10));
			this->button_clear->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->button_clear->Location = System::Drawing::Point(1165, 721);
			this->button_clear->Name = L"button_clear";
			this->button_clear->Size = System::Drawing::Size(148, 69);
			this->button_clear->TabIndex = 17;
			this->button_clear->Text = L"Clear All Parameter";
			this->button_clear->UseVisualStyleBackColor = false;
			this->button_clear->Click += gcnew System::EventHandler(this, &MyForm::button_clear_Click);
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label25->ForeColor = System::Drawing::Color::White;
			this->label25->Location = System::Drawing::Point(1286, 553);
			this->label25->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(171, 25);
			this->label25->TabIndex = 18;
			this->label25->Text = L"Debug Information";
			// 
			// Text_ControlMode
			// 
			this->Text_ControlMode->AutoSize = true;
			this->Text_ControlMode->BackColor = System::Drawing::Color::Transparent;
			this->Text_ControlMode->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11));
			this->Text_ControlMode->ForeColor = System::Drawing::Color::GreenYellow;
			this->Text_ControlMode->Location = System::Drawing::Point(964, 119);
			this->Text_ControlMode->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->Text_ControlMode->Name = L"Text_ControlMode";
			this->Text_ControlMode->Size = System::Drawing::Size(142, 26);
			this->Text_ControlMode->TabIndex = 19;
			this->Text_ControlMode->Text = L"Control Mode";
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label26->ForeColor = System::Drawing::Color::White;
			this->label26->Location = System::Drawing::Point(749, 119);
			this->label26->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(130, 25);
			this->label26->TabIndex = 20;
			this->label26->Text = L"Control Mode";
			// 
			// button_ControlMode
			// 
			this->button_ControlMode->BackColor = System::Drawing::Color::DarkGreen;
			this->button_ControlMode->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button_ControlMode->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->button_ControlMode->Location = System::Drawing::Point(1159, 108);
			this->button_ControlMode->Margin = System::Windows::Forms::Padding(4);
			this->button_ControlMode->Name = L"button_ControlMode";
			this->button_ControlMode->Size = System::Drawing::Size(210, 47);
			this->button_ControlMode->TabIndex = 21;
			this->button_ControlMode->Text = L"Change Control Mode";
			this->button_ControlMode->UseVisualStyleBackColor = false;
			this->button_ControlMode->Click += gcnew System::EventHandler(this, &MyForm::button_ControlMode_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 18);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::WindowFrame;
			this->ClientSize = System::Drawing::Size(1841, 924);
			this->Controls->Add(this->Text_ControlMode);
			this->Controls->Add(this->label26);
			this->Controls->Add(this->button_ControlMode);
			this->Controls->Add(this->label25);
			this->Controls->Add(this->button_clear);
			this->Controls->Add(this->label24);
			this->Controls->Add(this->label21);
			this->Controls->Add(this->MotorMoveFrames);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->calculateall_button);
			this->Controls->Add(this->movemotor_3_button);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->box_scandegree);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->Camera_found_label);
			this->Controls->Add(this->Motor_found_label);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->ScanBtn);
			this->Controls->Add(this->SaveBtn);
			this->Controls->Add(this->CalibBtn);
			this->Controls->Add(this->CaptureBtn);
			this->Controls->Add(this->InitialiseBtn);
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"MyForm";
			this->Text = L"CCRM_Controler.V2.4";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion


	private:



		void InitThreads() {

			Work^ w1 = gcnew Work();
			Thread^ newThread1 = gcnew Thread(gcnew ThreadStart(w1, &Work::Print));
			newThread1->Name = "newThread_1";

			Work^ w2 = gcnew Work();
			Thread^ newThread2 = gcnew Thread(gcnew ThreadStart(w2, &Work::Print));
			newThread2->Name = "newThread_2";

			Work^ w3 = gcnew Work();
			Thread^ newThread3 = gcnew Thread(gcnew ThreadStart(w3, &Work::Print));
			newThread3->Name = "newThread_3";


			OutputDebugString(L"Starting threads\n");
			// call each thread's Start method to place each
			// thread in Started state
			newThread1->Start();
			newThread2->Start();
			newThread3->Start();

			OutputDebugString(L"Threads started\n");
		}

		void InitialiseBtn_Click(System::Object^ sender, System::EventArgs^ e)
		{

			try {
				Camera_found_label->Text = "...";
				Motor_found_label->Text = "...";

				bool cam_init = IDS_Cam.OpenCamera();
				bool motor_init = ph_motor.Stepper_Initialise();

				Camera_found_label->Text = cam_init.ToString();
				Motor_found_label->Text = motor_init.ToString();

				// set default settings
				res = ph_motor.setStepperSpeed(stepspeed);
				res = ph_motor.setStepperAcc(acceleration);

				if (cam_init) {

					GetCamBtnHandle();
				}
			}
			catch (std::exception& ex) {

			}

		}


		void MoveBtn_Click(System::Object^ sender, System::EventArgs^ e) {
			MoveBtnHandle();
		}
		void SetMotorSpeedBtn_Click(System::Object^ sender, System::EventArgs^ e) {
			SetMotorSpeedBtnHandle();
		}
		void SetMotorAccBtn_Click(System::Object^ sender, System::EventArgs^ e) {
			SetMotorAccBtnHandle();
		}
		void SetCamBtn_Click(System::Object^ sender, System::EventArgs^ e) {
			SetCamBtnHandle();
		}
		void LiveStartBtn_Click(System::Object^ sender, System::EventArgs^ e) {
			LiveStartBtnHandle();
		}
		void LiveStopBtn_Click(System::Object^ sender, System::EventArgs^ e) {
			LiveStopBtnHandle();
		}
		void CaptureBtn_Click(System::Object^ sender, System::EventArgs^ e) {
			CaptureBtnHandle();
		}
		void ScanBtn_Click(System::Object^ sender, System::EventArgs^ e) {
			ScanBtnHandle();
		}
		void CalibBtn_Click(System::Object^ sender, System::EventArgs^ e) {
			CalibBtnHandle();
		}
		void SaveBtn_Click(System::Object^ sender, System::EventArgs^ e) {
			SaveBtnHandle();
		}


		void UpdateUI(Object^ obj)
		{
			Camera_found_label->Text = "...";
			Motor_found_label->Text = "...";
		}

		void GetCamBtnHandle() {

			double gain = IDS_Cam.get_Gain();
			double gamma = IDS_Cam.get_Gamma();

			Cam_curr_gain->Text = gain.ToString();
			Cam_curr_gamma->Text = gamma.ToString();
		}


		std::string convertToString(char* a, int size)
		{
			int i;
			string s = "";
			for (i = 0; i < size; i++) {
				s = s + a[i];
			}
			return s;
		}


		void ScanBtnHandle() {


			// Start a thread that calls a parameterized instance method.
			//Thread^ newThread = gcnew Thread(w->DoMoreWork);
			//newThread->Start();

			if (gainBoost->Checked) {
				IDS_Cam.set_GainBoost(true);
			}
			else {
				IDS_Cam.set_GainBoost(false);
			}


		


			IDS_Cam.CheckSettingsOnCamera();

			UINT a = IDS_Cam.set_FrameRate(1);
			//Zhaokang: donot set ET
			//UINT b = IDS_Cam.set_ExposureTime(100);
			double FrameRateCheck = IDS_Cam.get_FrameRate();
			double ExposureTimeCheck = IDS_Cam.get_ExposureTime();

			double scanDefPos = ph_motor.getPosition();
			res = ph_motor.moveTo(scanDefPos + scanDegree);
			Sleep(4);
			IDS_Cam.CameraCapture();
			// Thread^ MotionThread1 = gcnew Thread(gcnew ParameterizedThreadStart(MoveProj, &MotionThread::MoveProjectile));
			waitKey(2);
			// back to origial position
			res = ph_motor.moveTo(scanDefPos);
			while (ph_motor.isStepperMoving()) {
				waitKey(1);
			}

		}

		void MoveBtnHandle() {

			double position = Convert::ToDouble(movePosInp->Text);
			double defPos = ph_motor.getPosition();
			double res = ph_motor.moveTo(defPos + position);
		}

		void SetCamBtnHandle() {

			//QString text_exp = ui.camExpoInp->toPlainText();
			//double exposure = text_exp.split(" ")[0].toDouble();

			double gain = Convert::ToDouble(camGainInp->Text);

			//QString text_frame = ui.camFrameInp->toPlainText();
			//double frame = text_frame.split(" ")[0].toDouble();

			double gamma = Convert::ToDouble(camGammaInp->Text);

			//QString text_subF = ui.camSubFInp->toPlainText();
			//double subF = text_subF.split(" ")[0].toDouble();

			//IDS_Cam.set_ExposureTime(exposure);
			IDS_Cam.set_Gain(gain);
			IDS_Cam.set_FrameRate(30);
			IDS_Cam.set_Gamma(gamma);

			//Zhaokang Part
			double ExposureTime = Convert::ToDouble(ExposureTimeInp->Text);
			IDS_Cam.set_ExposureTime(ExposureTime);
			//  end

			if (gainBoost->Checked) {
				IDS_Cam.set_GainBoost(true);
			}
			else {
				IDS_Cam.set_GainBoost(false);
			}


			// update the settings on  the UI
			GetCamBtnHandle();

		}

		void CaptureBtnHandle() {

			IDS_Cam.set_single_capture_mode();
			IDS_Cam.CameraCapture();
		}

		void CalibBtnHandle() {


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
			//IDS_Cam.CameraLive_Start();
			IDS_Cam.CaptureLive();

		}

		void LiveStartBtnHandle() {

			IDS_Cam.setupLUT("Live");

			if (gainBoost->Checked) {
				IDS_Cam.set_GainBoost(true);
			}
			else {
				IDS_Cam.set_GainBoost(false);
			}

			IDS_Cam.set_frame_detect_mode();

			//IDS_Cam.threadProcImageQueue();
			IDS_Cam.CaptureLive();
		}

		void LiveStopBtnHandle() {

			stop_live_view = true;
			IDS_Cam.CameraLive_Stop();
			cv::destroyAllWindows();
			IDS_Cam.OpenCamera();
		}

		void SetMotorSpeedBtnHandle() {
			//char a = Convert::ToChar(MotorSpdInp->Text);
			//if (String::IsNumber(MotorSpdInp->Text)) {
			double speed = Convert::ToDouble(MotorSpdInp->Text);
			ph_motor.setStepperSpeed(speed);
			//this->label_motorspeed->Text = L"" + speed;
		  //}
		  //else {
			  //System::Windows::Forms::MessageBox();

		  //} 

		}

		void SetMotorAccBtnHandle() {

			acceleration = Convert::ToDouble(MotorAccInp->Text);
			ph_motor.setStepperAcc(acceleration);
			//this->label_motoracceleration->Text = L"" + acceleration;
		}

		void SaveBtnHandle() {

			IDS_Cam.Load_image();
			return;
			Is_RecoverParams(NULL, 1010);
			return;
			IDS_Cam.Save_Scan();

		}
		void Setup() {
			this->box_Arm_Length->Text = L"" + arm_length;
			this->box_Pixel_Size->Text = L"" + pixel_size;
			this->box_RotationSpeed->Text = L"" + rotation_speed;
			this->box_Orthogonal_Distance->Text = L"" + orthogonal_distance;	
			this->MotorAccInp->Text = L"" + acceleration;
			this->MotorMoveFPS->Text = L"" + FPS;
			this->box_MaximumStepSpeed->Text = L"" + stepspeed;
			if (par_ControlMode == 1) {//1£ºSortware Mode
				this->Text_ControlMode->Text = L"Sortware Mode";
			}
			else {//2£ºHardware Mode
				this->Text_ControlMode->Text = L"Hardware Mode";
			}
		}
		/// <summary>
		/// Zhaokang part
		/// </summary>
	private: System::Void movemotor_3_button_Click(System::Object^ sender, System::EventArgs^ e) {
		//double speed = FPS * pixel_size / orthogonal_distance / 2 / 3.1415926;//R=FPS*P/L/2pi
		ph_motor.setStepperSpeed(stepspeed);//set the speed
		double time = Number_of_Frames / FPS;//time=number of frames / FPS
		double position = rotation_speed * time;//position = speed * time
		double defPos = ph_motor.getPosition();//set the position
		double res = ph_motor.moveTo(defPos + position);//set the position moving to
		//ph_motor.setStepperAcc(0);//set the acceleration
		///richTextBox1, only used in debug
		this->richTextBox1->Text = L"stepspeed:" + stepspeed + L"\n" + L"MovePosition:" + position + L"\n" + L"Time:" + time;
	}

	private: System::Void Number_of_Frames_button_Click(System::Object^ sender, System::EventArgs^ e) {
		Number_of_Frames = Convert::ToDouble(MotorMoveFrames->Text);//Number of Frames
	}

	private: System::Void groupBox1_Enter(System::Object^ sender, System::EventArgs^ e) {
	}

	private: System::Void ParameterButton_Click(System::Object^ sender, System::EventArgs^ e) {
		pixel_size = Convert::ToDouble(box_Pixel_Size->Text);//Number of Frames
		orthogonal_distance = Convert::ToDouble(box_Orthogonal_Distance->Text);
		
		
		
		
		
	}
	private: System::Void MotorSelectComboBox_TextUpdate(System::Object^ sender, System::EventArgs^ e) {
		//if(this->MotorSelectComboBox->Text == motor1) {}
	}
	private: System::Void armlength_button_Click(System::Object^ sender, System::EventArgs^ e) {
		arm_length = Convert::ToDouble(box_Arm_Length->Text);
		//this->label_armlength->Text = L"" + arm_length;
	}
private: System::Void rotationspeed_button_Click(System::Object^ sender, System::EventArgs^ e) {
	rotation_speed = Convert::ToDouble(box_RotationSpeed->Text);
	maximum_step_speed = 2 * 3.1415926 * rotation_speed * arm_length / pixel_size;
	this->box_MaximumStepSpeed->Text = L""+ maximum_step_speed;
	//this->label_stepspeed->Text = L"" + maximum_step_speed;
	//this->label_rorationspeed->Text = L"" + rotation_speed;
}
private: System::Void stepspeed_button_Click(System::Object^ sender, System::EventArgs^ e) {
	maximum_step_speed = Convert::ToDouble(box_MaximumStepSpeed->Text);
	rotation_speed = maximum_step_speed * pixel_size / 2 / 3.1415926 / arm_length;
	this->box_RotationSpeed->Text = L"" + rotation_speed;
	//this->label_stepspeed->Text = L"" + maximum_step_speed;
	//this->label_rorationspeed->Text = L"" + rotation_speed;
}

private: System::Void calculateall_button_Click(System::Object^ sender, System::EventArgs^ e) {
	if (this->box_Arm_Length->Text == L"") {
		this->richTextBox1->Text += L"Arm Length uses default value;\n";
		this->box_Arm_Length->Text = L"" + arm_length;
	}
	else {
		arm_length = Convert::ToDouble(box_Arm_Length->Text);
	}
	if (this->box_Pixel_Size->Text == L"") {
		this->richTextBox1->Text += L"Pixel Size uses default value;\n";
		this->box_Pixel_Size->Text = L"" + pixel_size;
	}
	else {
		pixel_size = Convert::ToDouble(box_Pixel_Size->Text);
	}
	if (this->box_Orthogonal_Distance->Text == L"") {
		this->richTextBox1->Text += L"Orthogonal Distance uses default value;\n";
		this->box_Orthogonal_Distance->Text = L"" + orthogonal_distance;
	}
	else {
		orthogonal_distance = Convert::ToDouble(box_Pixel_Size->Text);
	}
	if (this->MotorAccInp->Text == L"") {
		this->richTextBox1->Text += L"Motor Acceleration uses default value;\n";
		this->MotorAccInp->Text = L"" + acceleration;
	}
	else {
		acceleration = Convert::ToDouble(MotorAccInp->Text);
	}




	if ((this->MotorMoveFPS->Text == L"") && (this->box_MaximumStepSpeed->Text == L"")
		&& (this->box_RotationSpeed->Text == L"")) {
		parameter_speed = 0;
	}
	else {
		parameter_speed = 1;
		if (this->MotorMoveFPS->Text != L"") {
			FPS = Convert::ToDouble(this->MotorMoveFPS->Text);
			rotation_speed = FPS * pixel_size / orthogonal_distance / 2 / 3.1415926 * 360;
			line_speed = rotation_speed * orthogonal_distance;

			stepspeed = rotation_speed / Rescale_factor;

			this->box_MaximumStepSpeed->Text = L"" + stepspeed;
			this->box_RotationSpeed->Text = L"" + rotation_speed;
		}
		else if (this->box_MaximumStepSpeed->Text != L"") {
			stepspeed = Convert::ToDouble(this->box_MaximumStepSpeed->Text);
			
			rotation_speed = stepspeed * Rescale_factor;
			FPS = rotation_speed / pixel_size * orthogonal_distance * 2 * 3.1415926 / 360;
			line_speed = rotation_speed * orthogonal_distance;
			this->MotorMoveFPS->Text = L"" + FPS;
			this->box_RotationSpeed->Text = L"" + rotation_speed;
		}
		else if (this->box_RotationSpeed->Text != L"") {
			rotation_speed = Convert::ToDouble(this->box_RotationSpeed->Text);
			stepspeed = rotation_speed / Rescale_factor;
			FPS = rotation_speed / pixel_size * orthogonal_distance * 2 * 3.1415926 / 360;
			line_speed = rotation_speed * orthogonal_distance;
			this->box_MaximumStepSpeed->Text = L"" + stepspeed;
			this->MotorMoveFPS->Text = L"" + FPS;
		}
	}


	if (this->ExposureTimeInp->Text == L"") {
		parameter_time = 0;
	}
	else {
		parameter_time = 1;
		exposuretime = Convert::ToDouble(this->ExposureTimeInp->Text);
	}


	if (this->MotorMoveFrames->Text == L"") {
		parameter_distance = 0;
	}
	else {
		parameter_distance = 1;
		Number_of_Frames= Convert::ToDouble(this->MotorMoveFrames->Text);
	}


	if (parameter_speed) {
		if (parameter_distance && parameter_time) {
			double time = Number_of_Frames / FPS;
			if (exposuretime != time) {
				this->richTextBox1->Text += L"Cannot get corract value!\n";
				goto end;
			}

		}
		else {
			if (parameter_distance || parameter_time) {
				if (parameter_distance) {
					exposuretime = Number_of_Frames * pixel_size / line_speed * 1000;
					this->ExposureTimeInp->Text = L"" + exposuretime;
				}
				else {
					Number_of_Frames = exposuretime * line_speed / pixel_size / 1000;
					double a = exposuretime * line_speed;
					double b = a / pixel_size;
					Number_of_Frames = b / 1000;
					this->MotorMoveFrames->Text = L"" + Number_of_Frames;
				}
			}
			else {
				this->richTextBox1->Text += L"Give Exposure Time or Number of Frames or Scan Degree;\n";

			}
		}
	}
	else {
		if (parameter_distance && parameter_time) {
			line_speed = Number_of_Frames * pixel_size / exposuretime * 1000;
			rotation_speed = line_speed / orthogonal_distance;
			stepspeed = rotation_speed / Rescale_factor;
			FPS = rotation_speed / pixel_size * orthogonal_distance * 2 * 3.1415926 / 360;
			this->box_MaximumStepSpeed->Text = L"" + stepspeed;
			this->box_RotationSpeed->Text = L"" + rotation_speed;
			this->MotorMoveFPS->Text = L"" + FPS;
		}
		else {
			this->richTextBox1->Text += L"Give one among Rotation Speed, Maximum Step Speed and FPS;\n";
		}
	}
	scanDegree = Number_of_Frames * pixel_size / arm_length / 2 / 3.1415926 * 360;
	this->box_scandegree->Text = L"" + scanDegree;
	end:;
}
	  
private: System::Void button_clear_Click(System::Object^ sender, System::EventArgs^ e) {

	this->MotorMoveFrames->Text = L"";
	this->ExposureTimeInp->Text = L"";
	this->box_MaximumStepSpeed->Text = L"";
	this->box_RotationSpeed->Text = L"";
	this->MotorMoveFPS->Text = L"";
}
private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	arm_length = Convert::ToDouble(this->box_Arm_Length->Text);
	pixel_size = Convert::ToDouble(this->box_Pixel_Size->Text);
	rotation_speed = Convert::ToDouble(this->box_RotationSpeed->Text);
	orthogonal_distance = Convert::ToDouble(this->box_Orthogonal_Distance->Text);
}
private: System::Void button_ControlMode_Click(System::Object^ sender, System::EventArgs^ e) {
	if (par_ControlMode == 1) {//1£ºSortware Mode
		this->Text_ControlMode->Text = L"Hardware Mode";
		par_ControlMode = 2;
	}
	else {//2£ºHardware Mode
		this->Text_ControlMode->Text = L"Software Mode";
		par_ControlMode = 1;
	}
}

};

}
