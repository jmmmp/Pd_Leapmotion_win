#pragma once
#include "Leap.h"

class DeviceFacade{
	
public:
	DeviceFacade(Controller &controller);
	void print();
	std::string boolToYesNo(bool value);

private:
	std::string typeToString(Device::Type type);
	Controller &controller;
};

inline DeviceFacade::DeviceFacade(Controller &controller):
controller(controller){}

inline void DeviceFacade::print(){
	auto devices = controller.devices();
	if(devices.isEmpty()){
		post("No leapmotion device found");
		return;
	}
	auto device = devices[0];

	post("baseline: %f", device.baseline());
	post("horizontal view angle: %f", device.horizontalViewAngle());
	post("vertical view angle: %f", device.verticalViewAngle());

	post("embedded: %s", boolToYesNo(device.isEmbedded()).c_str());
	post("flipped: %s", boolToYesNo(device.isFlipped()).c_str());
	post("streaming: %s", boolToYesNo(device.isStreaming()).c_str());
	post("valid: %s", boolToYesNo(device.isValid()).c_str());
	post("range: %f", device.range());
	post("serial number: %s", device.serialNumber().c_str());
	post("device type: %s", typeToString(device.type()).c_str());
	
}
inline std::string DeviceFacade::boolToYesNo(bool value){
	return value ? "yes" : "no";
}

inline std::string DeviceFacade::typeToString(Device::Type type){
	switch(type){
		case Device::Type::TYPE_PERIPHERAL:{
			return "peripheral";
		}
		case Device::Type::TYPE_KEYBOARD:{
			return "keyboard";
		}
		case Device::Type::TYPE_LAPTOP:{
			return "laptop";
		}
		default:{
			return "unknown";
		}
	}
}