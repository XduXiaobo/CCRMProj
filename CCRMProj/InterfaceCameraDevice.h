#pragma once
class InterfaceCameraDevice
{
public:
	  InterfaceCameraDevice() {}
	  virtual ~InterfaceCameraDevice() {}


	  virtual void method1() = 0;    // "= 0" part makes this method pure virtual, also makes this class abstract.
	  virtual void method2() = 0;
};