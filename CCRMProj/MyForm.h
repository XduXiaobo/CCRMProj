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
	double speed = 30;
	double acc = 100000;
	/// <summary>
	/// Zhaokang part
	/// </summary>
	double pixel_size = 0.00005;//pixel_size
	double orthogonal_distance = 0.2;//orthogonal_distance
	double Number_of_Frames;
	double FPS;
	double arm_length=0.1;
	double rotation_speed;
	double maximum_step_speed = 250000;
	//wchar_t motor1[] = L"Phidgets";
	//double size_motor1 = size(motor1);
	//wchar_t motor2[] = L"Zaber";
	//double size_motor2 = size(motor2);


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
	private: System::Windows::Forms::Button^ SetMotorAccBtn;
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
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::TextBox^ scanDegreeInp;
	private: System::Windows::Forms::Button^ CalibBtn;
	private: System::Windows::Forms::Button^ SaveBtn;
	private: System::Windows::Forms::GroupBox^ groupBox3;
	private: System::Windows::Forms::TextBox^ MotorMoveFPS;



	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::Button^ MoveFPS_Btn;

	private: System::Windows::Forms::Button^ FPS_button;
	private: System::Windows::Forms::Button^ Number_of_Frames_button;
	private: System::Windows::Forms::TextBox^ MotorMoveFrames;





	private: System::Windows::Forms::Label^ label13;
	private: System::Windows::Forms::RichTextBox^ richTextBox1;
	private: System::Windows::Forms::Button^ IMG_Show;
	private: System::Windows::Forms::TextBox^ ExposureTimeInp;
	private: System::Windows::Forms::Label^ label14;
	private: System::Windows::Forms::Button^ ParameterButton;
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
private: System::Windows::Forms::Button^ armlength_button;
private: System::Windows::Forms::Button^ rotationspeed_button;
private: System::Windows::Forms::Button^ stepspeed_button;
private: System::Windows::Forms::Button^ movemotor_3_button;






private: System::Windows::Forms::Label^ label_numberofframes;

private: System::Windows::Forms::Label^ label_fps;




















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
			this->MotorAccInp = (gcnew System::Windows::Forms::TextBox());
			this->MotorSpdInp = (gcnew System::Windows::Forms::TextBox());
			this->movePosInp = (gcnew System::Windows::Forms::TextBox());
			this->Motor_curr_pos = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->MoveBtn = (gcnew System::Windows::Forms::Button());
			this->SetMotorAccBtn = (gcnew System::Windows::Forms::Button());
			this->SetMotorSpeedBtn = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->l = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->ExposureTimeInp = (gcnew System::Windows::Forms::TextBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
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
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->scanDegreeInp = (gcnew System::Windows::Forms::TextBox());
			this->CalibBtn = (gcnew System::Windows::Forms::Button());
			this->SaveBtn = (gcnew System::Windows::Forms::Button());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->label_numberofframes = (gcnew System::Windows::Forms::Label());
			this->label_fps = (gcnew System::Windows::Forms::Label());
			this->Number_of_Frames_button = (gcnew System::Windows::Forms::Button());
			this->MotorMoveFrames = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->FPS_button = (gcnew System::Windows::Forms::Button());
			this->MotorMoveFPS = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->MoveFPS_Btn = (gcnew System::Windows::Forms::Button());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->IMG_Show = (gcnew System::Windows::Forms::Button());
			this->ParameterButton = (gcnew System::Windows::Forms::Button());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->box_Orthogonal_Distance = (gcnew System::Windows::Forms::TextBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->box_Pixel_Size = (gcnew System::Windows::Forms::TextBox());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->box_MaximumStepSpeed = (gcnew System::Windows::Forms::TextBox());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->box_RotationSpeed = (gcnew System::Windows::Forms::TextBox());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->box_Arm_Length = (gcnew System::Windows::Forms::TextBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->armlength_button = (gcnew System::Windows::Forms::Button());
			this->rotationspeed_button = (gcnew System::Windows::Forms::Button());
			this->stepspeed_button = (gcnew System::Windows::Forms::Button());
			this->movemotor_3_button = (gcnew System::Windows::Forms::Button());
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
			this->groupBox1->Controls->Add(this->MotorAccInp);
			this->groupBox1->Controls->Add(this->MotorSpdInp);
			this->groupBox1->Controls->Add(this->movePosInp);
			this->groupBox1->Controls->Add(this->Motor_curr_pos);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->MoveBtn);
			this->groupBox1->Controls->Add(this->SetMotorAccBtn);
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
			// MotorAccInp
			// 
			this->MotorAccInp->Location = System::Drawing::Point(306, 186);
			this->MotorAccInp->Margin = System::Windows::Forms::Padding(4);
			this->MotorAccInp->Name = L"MotorAccInp";
			this->MotorAccInp->Size = System::Drawing::Size(148, 32);
			this->MotorAccInp->TabIndex = 2;
			// 
			// MotorSpdInp
			// 
			this->MotorSpdInp->Location = System::Drawing::Point(306, 137);
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
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label5->ForeColor = System::Drawing::Color::White;
			this->label5->Location = System::Drawing::Point(15, 191);
			this->label5->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(206, 25);
			this->label5->TabIndex = 0;
			this->label5->Text = L"Set motor acceleration";
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
			// SetMotorAccBtn
			// 
			this->SetMotorAccBtn->BackColor = System::Drawing::Color::DarkGreen;
			this->SetMotorAccBtn->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SetMotorAccBtn->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->SetMotorAccBtn->Location = System::Drawing::Point(263, 240);
			this->SetMotorAccBtn->Margin = System::Windows::Forms::Padding(4);
			this->SetMotorAccBtn->Name = L"SetMotorAccBtn";
			this->SetMotorAccBtn->Size = System::Drawing::Size(198, 39);
			this->SetMotorAccBtn->TabIndex = 0;
			this->SetMotorAccBtn->Text = L"Set acceleration";
			this->SetMotorAccBtn->UseVisualStyleBackColor = false;
			this->SetMotorAccBtn->Click += gcnew System::EventHandler(this, &MyForm::SetMotorAccBtn_Click);
			// 
			// SetMotorSpeedBtn
			// 
			this->SetMotorSpeedBtn->BackColor = System::Drawing::Color::DarkGreen;
			this->SetMotorSpeedBtn->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SetMotorSpeedBtn->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->SetMotorSpeedBtn->Location = System::Drawing::Point(32, 239);
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
			this->label4->Location = System::Drawing::Point(15, 140);
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
			// groupBox2
			// 
			this->groupBox2->BackColor = System::Drawing::Color::Transparent;
			this->groupBox2->Controls->Add(this->ExposureTimeInp);
			this->groupBox2->Controls->Add(this->label14);
			this->groupBox2->Controls->Add(this->gainBoost);
			this->groupBox2->Controls->Add(this->camGammaInp);
			this->groupBox2->Controls->Add(this->Cam_curr_gamma);
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
			this->groupBox2->Location = System::Drawing::Point(738, 478);
			this->groupBox2->Margin = System::Windows::Forms::Padding(4);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Padding = System::Windows::Forms::Padding(4);
			this->groupBox2->Size = System::Drawing::Size(623, 361);
			this->groupBox2->TabIndex = 2;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Camera Settings";
			// 
			// ExposureTimeInp
			// 
			this->ExposureTimeInp->Location = System::Drawing::Point(231, 277);
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
			this->label14->ForeColor = System::Drawing::Color::White;
			this->label14->Location = System::Drawing::Point(15, 276);
			this->label14->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(179, 25);
			this->label14->TabIndex = 4;
			this->label14->Text = L"Set Exposure Time";
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
			this->ScanBtn->Location = System::Drawing::Point(18, 832);
			this->ScanBtn->Margin = System::Windows::Forms::Padding(4);
			this->ScanBtn->Name = L"ScanBtn";
			this->ScanBtn->Size = System::Drawing::Size(189, 73);
			this->ScanBtn->TabIndex = 0;
			this->ScanBtn->Text = L"Scan";
			this->ScanBtn->UseVisualStyleBackColor = false;
			this->ScanBtn->Click += gcnew System::EventHandler(this, &MyForm::ScanBtn_Click);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label9->ForeColor = System::Drawing::Color::OliveDrab;
			this->label9->Location = System::Drawing::Point(222, 839);
			this->label9->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(124, 25);
			this->label9->TabIndex = 1;
			this->label9->Text = L"Scan degree";
			// 
			// scanDegreeInp
			// 
			this->scanDegreeInp->Location = System::Drawing::Point(216, 871);
			this->scanDegreeInp->Margin = System::Windows::Forms::Padding(4);
			this->scanDegreeInp->Name = L"scanDegreeInp";
			this->scanDegreeInp->Size = System::Drawing::Size(148, 28);
			this->scanDegreeInp->TabIndex = 2;
			this->scanDegreeInp->Text = L"0";
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
			this->groupBox3->Controls->Add(this->label_numberofframes);
			this->groupBox3->Controls->Add(this->label_fps);
			this->groupBox3->Controls->Add(this->Number_of_Frames_button);
			this->groupBox3->Controls->Add(this->MotorMoveFrames);
			this->groupBox3->Controls->Add(this->label13);
			this->groupBox3->Controls->Add(this->FPS_button);
			this->groupBox3->Controls->Add(this->MotorMoveFPS);
			this->groupBox3->Controls->Add(this->label12);
			this->groupBox3->Controls->Add(this->MoveFPS_Btn);
			this->groupBox3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11));
			this->groupBox3->ForeColor = System::Drawing::Color::GreenYellow;
			this->groupBox3->Location = System::Drawing::Point(1399, 137);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(384, 519);
			this->groupBox3->TabIndex = 3;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Simple Setting";
			// 
			// label_numberofframes
			// 
			this->label_numberofframes->AutoSize = true;
			this->label_numberofframes->ForeColor = System::Drawing::Color::Silver;
			this->label_numberofframes->Location = System::Drawing::Point(31, 340);
			this->label_numberofframes->Name = L"label_numberofframes";
			this->label_numberofframes->Size = System::Drawing::Size(82, 26);
			this->label_numberofframes->TabIndex = 14;
			this->label_numberofframes->Text = L"label22";
			// 
			// label_fps
			// 
			this->label_fps->AutoSize = true;
			this->label_fps->ForeColor = System::Drawing::Color::Silver;
			this->label_fps->Location = System::Drawing::Point(31, 160);
			this->label_fps->Name = L"label_fps";
			this->label_fps->Size = System::Drawing::Size(82, 26);
			this->label_fps->TabIndex = 13;
			this->label_fps->Text = L"label21";
			// 
			// Number_of_Frames_button
			// 
			this->Number_of_Frames_button->BackColor = System::Drawing::Color::DarkGreen;
			this->Number_of_Frames_button->Font = (gcnew System::Drawing::Font(L"Arial", 10));
			this->Number_of_Frames_button->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->Number_of_Frames_button->Location = System::Drawing::Point(221, 323);
			this->Number_of_Frames_button->Name = L"Number_of_Frames_button";
			this->Number_of_Frames_button->Size = System::Drawing::Size(148, 69);
			this->Number_of_Frames_button->TabIndex = 7;
			this->Number_of_Frames_button->Text = L"Set Number of Frames";
			this->Number_of_Frames_button->UseVisualStyleBackColor = false;
			this->Number_of_Frames_button->Click += gcnew System::EventHandler(this, &MyForm::Number_of_Frames_button_Click);
			// 
			// MotorMoveFrames
			// 
			this->MotorMoveFrames->Location = System::Drawing::Point(221, 272);
			this->MotorMoveFrames->Margin = System::Windows::Forms::Padding(4);
			this->MotorMoveFrames->Name = L"MotorMoveFrames";
			this->MotorMoveFrames->Size = System::Drawing::Size(148, 32);
			this->MotorMoveFrames->TabIndex = 6;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label13->ForeColor = System::Drawing::Color::White;
			this->label13->Location = System::Drawing::Point(22, 272);
			this->label13->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(178, 25);
			this->label13->TabIndex = 5;
			this->label13->Text = L"Number of  Frames";
			// 
			// FPS_button
			// 
			this->FPS_button->BackColor = System::Drawing::Color::DarkGreen;
			this->FPS_button->Font = (gcnew System::Drawing::Font(L"Arial", 10));
			this->FPS_button->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->FPS_button->Location = System::Drawing::Point(221, 142);
			this->FPS_button->Name = L"FPS_button";
			this->FPS_button->Size = System::Drawing::Size(148, 44);
			this->FPS_button->TabIndex = 4;
			this->FPS_button->Text = L"Set FPS";
			this->FPS_button->UseVisualStyleBackColor = false;
			this->FPS_button->Click += gcnew System::EventHandler(this, &MyForm::FPS_button_Click);
			// 
			// MotorMoveFPS
			// 
			this->MotorMoveFPS->Location = System::Drawing::Point(221, 81);
			this->MotorMoveFPS->Margin = System::Windows::Forms::Padding(4);
			this->MotorMoveFPS->Name = L"MotorMoveFPS";
			this->MotorMoveFPS->Size = System::Drawing::Size(148, 32);
			this->MotorMoveFPS->TabIndex = 3;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label12->ForeColor = System::Drawing::Color::White;
			this->label12->Location = System::Drawing::Point(31, 81);
			this->label12->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(51, 25);
			this->label12->TabIndex = 3;
			this->label12->Text = L"FPS";
			// 
			// MoveFPS_Btn
			// 
			this->MoveFPS_Btn->BackColor = System::Drawing::Color::DarkGreen;
			this->MoveFPS_Btn->Font = (gcnew System::Drawing::Font(L"Arial", 10));
			this->MoveFPS_Btn->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->MoveFPS_Btn->Location = System::Drawing::Point(221, 439);
			this->MoveFPS_Btn->Name = L"MoveFPS_Btn";
			this->MoveFPS_Btn->Size = System::Drawing::Size(148, 44);
			this->MoveFPS_Btn->TabIndex = 3;
			this->MoveFPS_Btn->Text = L"Move Motor";
			this->MoveFPS_Btn->UseVisualStyleBackColor = false;
			this->MoveFPS_Btn->Click += gcnew System::EventHandler(this, &MyForm::MoveFPS_Btn_Click);
			// 
			// richTextBox1
			// 
			this->richTextBox1->Enabled = false;
			this->richTextBox1->Location = System::Drawing::Point(227, 749);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(252, 87);
			this->richTextBox1->TabIndex = 4;
			this->richTextBox1->Text = L"";
			// 
			// IMG_Show
			// 
			this->IMG_Show->BackColor = System::Drawing::Color::DarkGreen;
			this->IMG_Show->Font = (gcnew System::Drawing::Font(L"Arial", 10));
			this->IMG_Show->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->IMG_Show->Location = System::Drawing::Point(738, 861);
			this->IMG_Show->Name = L"IMG_Show";
			this->IMG_Show->Size = System::Drawing::Size(148, 44);
			this->IMG_Show->TabIndex = 8;
			this->IMG_Show->Text = L"IMG_Show";
			this->IMG_Show->UseVisualStyleBackColor = false;
			this->IMG_Show->Click += gcnew System::EventHandler(this, &MyForm::IMG_Show_Click);
			// 
			// ParameterButton
			// 
			this->ParameterButton->BackColor = System::Drawing::Color::DarkGreen;
			this->ParameterButton->Font = (gcnew System::Drawing::Font(L"Arial", 10));
			this->ParameterButton->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->ParameterButton->Location = System::Drawing::Point(393, 173);
			this->ParameterButton->Name = L"ParameterButton";
			this->ParameterButton->Size = System::Drawing::Size(191, 44);
			this->ParameterButton->TabIndex = 10;
			this->ParameterButton->Text = L"Set Parameter ";
			this->ParameterButton->UseVisualStyleBackColor = false;
			this->ParameterButton->Click += gcnew System::EventHandler(this, &MyForm::ParameterButton_Click);
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->box_Orthogonal_Distance);
			this->groupBox4->Controls->Add(this->ParameterButton);
			this->groupBox4->Controls->Add(this->label15);
			this->groupBox4->Controls->Add(this->box_Pixel_Size);
			this->groupBox4->Controls->Add(this->label16);
			this->groupBox4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11));
			this->groupBox4->ForeColor = System::Drawing::Color::GreenYellow;
			this->groupBox4->Location = System::Drawing::Point(738, 137);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(613, 240);
			this->groupBox4->TabIndex = 8;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Basic Parameter Setting";
			// 
			// box_Orthogonal_Distance
			// 
			this->box_Orthogonal_Distance->Location = System::Drawing::Point(221, 128);
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
			this->box_Pixel_Size->Location = System::Drawing::Point(221, 81);
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
			// box_MaximumStepSpeed
			// 
			this->box_MaximumStepSpeed->Location = System::Drawing::Point(248, 563);
			this->box_MaximumStepSpeed->Margin = System::Windows::Forms::Padding(4);
			this->box_MaximumStepSpeed->Name = L"box_MaximumStepSpeed";
			this->box_MaximumStepSpeed->Size = System::Drawing::Size(148, 28);
			this->box_MaximumStepSpeed->TabIndex = 16;
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label19->ForeColor = System::Drawing::Color::White;
			this->label19->Location = System::Drawing::Point(22, 570);
			this->label19->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(206, 25);
			this->label19->TabIndex = 15;
			this->label19->Text = L"Maximum Step Speed";
			// 
			// box_RotationSpeed
			// 
			this->box_RotationSpeed->Location = System::Drawing::Point(248, 513);
			this->box_RotationSpeed->Margin = System::Windows::Forms::Padding(4);
			this->box_RotationSpeed->Name = L"box_RotationSpeed";
			this->box_RotationSpeed->Size = System::Drawing::Size(148, 28);
			this->box_RotationSpeed->TabIndex = 14;
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label18->ForeColor = System::Drawing::Color::White;
			this->label18->Location = System::Drawing::Point(22, 519);
			this->label18->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(146, 25);
			this->label18->TabIndex = 13;
			this->label18->Text = L"Rotation Speed";
			// 
			// box_Arm_Length
			// 
			this->box_Arm_Length->Location = System::Drawing::Point(248, 460);
			this->box_Arm_Length->Margin = System::Windows::Forms::Padding(4);
			this->box_Arm_Length->Name = L"box_Arm_Length";
			this->box_Arm_Length->Size = System::Drawing::Size(148, 28);
			this->box_Arm_Length->TabIndex = 12;
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label17->ForeColor = System::Drawing::Color::White;
			this->label17->Location = System::Drawing::Point(22, 460);
			this->label17->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(113, 25);
			this->label17->TabIndex = 11;
			this->label17->Text = L"Arm Length";
			// 
			// armlength_button
			// 
			this->armlength_button->BackColor = System::Drawing::Color::DarkGreen;
			this->armlength_button->Font = (gcnew System::Drawing::Font(L"Arial", 10));
			this->armlength_button->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->armlength_button->Location = System::Drawing::Point(530, 448);
			this->armlength_button->Name = L"armlength_button";
			this->armlength_button->Size = System::Drawing::Size(173, 44);
			this->armlength_button->TabIndex = 11;
			this->armlength_button->Text = L"Set Arm Length";
			this->armlength_button->UseVisualStyleBackColor = false;
			this->armlength_button->Click += gcnew System::EventHandler(this, &MyForm::armlength_button_Click);
			// 
			// rotationspeed_button
			// 
			this->rotationspeed_button->BackColor = System::Drawing::Color::DarkGreen;
			this->rotationspeed_button->Font = (gcnew System::Drawing::Font(L"Arial", 10));
			this->rotationspeed_button->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->rotationspeed_button->Location = System::Drawing::Point(530, 501);
			this->rotationspeed_button->Name = L"rotationspeed_button";
			this->rotationspeed_button->Size = System::Drawing::Size(191, 44);
			this->rotationspeed_button->TabIndex = 17;
			this->rotationspeed_button->Text = L"Set Rotation Speed";
			this->rotationspeed_button->UseVisualStyleBackColor = false;
			this->rotationspeed_button->Click += gcnew System::EventHandler(this, &MyForm::rotationspeed_button_Click);
			// 
			// stepspeed_button
			// 
			this->stepspeed_button->BackColor = System::Drawing::Color::DarkGreen;
			this->stepspeed_button->Font = (gcnew System::Drawing::Font(L"Arial", 10));
			this->stepspeed_button->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->stepspeed_button->Location = System::Drawing::Point(530, 559);
			this->stepspeed_button->Name = L"stepspeed_button";
			this->stepspeed_button->Size = System::Drawing::Size(191, 44);
			this->stepspeed_button->TabIndex = 18;
			this->stepspeed_button->Text = L"Set Step Speed";
			this->stepspeed_button->UseVisualStyleBackColor = false;
			this->stepspeed_button->Click += gcnew System::EventHandler(this, &MyForm::stepspeed_button_Click);
			// 
			// movemotor_3_button
			// 
			this->movemotor_3_button->BackColor = System::Drawing::Color::DarkGreen;
			this->movemotor_3_button->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->movemotor_3_button->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->movemotor_3_button->Location = System::Drawing::Point(530, 625);
			this->movemotor_3_button->Margin = System::Windows::Forms::Padding(4);
			this->movemotor_3_button->Name = L"movemotor_3_button";
			this->movemotor_3_button->Size = System::Drawing::Size(188, 44);
			this->movemotor_3_button->TabIndex = 3;
			this->movemotor_3_button->Text = L"Move motor";
			this->movemotor_3_button->UseVisualStyleBackColor = false;
			this->movemotor_3_button->Click += gcnew System::EventHandler(this, &MyForm::movemotor_3_button_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 18);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::WindowFrame;
			this->ClientSize = System::Drawing::Size(1841, 924);
			this->Controls->Add(this->movemotor_3_button);
			this->Controls->Add(this->stepspeed_button);
			this->Controls->Add(this->rotationspeed_button);
			this->Controls->Add(this->armlength_button);
			this->Controls->Add(this->box_MaximumStepSpeed);
			this->Controls->Add(this->label19);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->box_RotationSpeed);
			this->Controls->Add(this->IMG_Show);
			this->Controls->Add(this->label18);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->box_Arm_Length);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->label17);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->scanDegreeInp);
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
				res = ph_motor.setStepperSpeed(speed);
				res = ph_motor.setStepperAcc(acc);

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

		void CheckArduino() {

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

		};

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


			double NewscanDegree = Convert::ToDouble(scanDegreeInp->Text);
			//IDS_Cam.setupLUT("Scan");
			if (NewscanDegree != 0) {
				scanDegree = NewscanDegree;
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

			double acceleration = Convert::ToDouble(MotorAccInp->Text);
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
		/// <summary>
		/// Zhaokang part
		/// </summary>
	private: System::Void MoveFPS_Btn_Click(System::Object^ sender, System::EventArgs^ e) {
		double speed = FPS * pixel_size / orthogonal_distance / 2 / 3.1415926;//R=FPS*P/L/2pi
		ph_motor.setStepperSpeed(speed);//set the speed
		double time = Number_of_Frames / FPS;//time=number of frames / FPS
		double position = speed * time;//position = speed * time
		double defPos = ph_motor.getPosition();//set the position
		double res = ph_motor.moveTo(defPos + position);//set the position moving to
		//ph_motor.setStepperAcc(0);//set the acceleration
		///richTextBox1, only used in debug
		this->richTextBox1->Text = L"Speed:" + speed + L"\n" + L"MovePosition:" + position + L"\n" + L"Time:" + time;
	}
	private: System::Void FPS_button_Click(System::Object^ sender, System::EventArgs^ e) {
		FPS = Convert::ToDouble(MotorMoveFPS->Text);//FPS
		this->label_fps->Text = L"" + FPS;
	}
	private: System::Void Number_of_Frames_button_Click(System::Object^ sender, System::EventArgs^ e) {
		Number_of_Frames = Convert::ToDouble(MotorMoveFrames->Text);//Number of Frames
		this->label_numberofframes->Text = L"" + Number_of_Frames;
	}
	private: System::Void IMG_Show_Click(System::Object^ sender, System::EventArgs^ e) {
		//IplImage *img1 = cvLoadImage("pic1.png");
		Mat image = imread("pic1.png");
		namedWindow("test", WINDOW_AUTOSIZE);
		//imshow("test", image );
		waitKey(0);
		destroyWindow("test");
		//releaseImage(&image);

		///this->pictureBox1->Image = image;
		//System::Drawing::Bitmap^ MyImage;
		//pictureBox1->SizeMode = PictureBoxSizeMode::StretchImage;
		//MyImage = gcnew System::Drawing::Bitmap("pic1.png");
		//pictureBox1->ClientSize = System::Drawing::Size(500, 500);
		//pictureBox1->Image = dynamic_cast<System::Drawing::Image^>(MyImage);
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
	this->box_RotationSpeed->Text = L"" + maximum_step_speed;
	//this->label_stepspeed->Text = L"" + maximum_step_speed;
	//this->label_rorationspeed->Text = L"" + rotation_speed;
}
private: System::Void movemotor_3_button_Click(System::Object^ sender, System::EventArgs^ e) {
	ph_motor.setStepperSpeed(maximum_step_speed);
}
};
	
}
