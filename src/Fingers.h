#pragma once

#include <flext.h>

class Fingers{
public:
	Fingers(const FingerList &fingerList);
	std::vector<t_atom> getAtoms(int handIndex, int fingerIndex, const std::string &name);
	int count();
private:
 	std::vector<t_atom>makeFingerAtoms(int handIndex, int fingerIndex, const std::string &name, float value);
 	std::vector<t_atom>makeFingerAtoms(int handIndex, int fingerIndex, const std::string &name, float width, float length);
 	std::vector<t_atom>makeFingerAtoms(int handIndex, int fingerIndex, const std::string &name, float x, float y, float z);
 	std::vector<t_atom>makeFingerAtoms(int handIndex, int fingerIndex, const std::string &name, Finger::Type type);
 	const FingerList &fingers;
};

inline Fingers::Fingers(const FingerList &fingers):
fingers(fingers){}

inline std::vector<t_atom> Fingers::getAtoms(int handIndex, int fingerIndex, const std::string &name){
	auto finger = fingers[fingerIndex];

	if(name == "hands_finger_direction"){
		return makeFingerAtoms(handIndex, fingerIndex, name, finger.direction().x, finger.direction().y, finger.direction().z);
	}else if(name == "hands_finger_position"){
		return makeFingerAtoms(handIndex, fingerIndex, name, finger.tipPosition().x, finger.tipPosition().y, finger.tipPosition().z);
	}else if(name == "hands_finger_velocity"){
		return makeFingerAtoms(handIndex, fingerIndex, name, finger.tipVelocity().x, finger.tipVelocity().y, finger.tipVelocity().z);
	}else if(name == "hands_finger_size"){
		return makeFingerAtoms(handIndex, fingerIndex, name, finger.width(), finger.length());
	}else if(name == "hands_finger_type"){
		return makeFingerAtoms(handIndex, fingerIndex, name, finger.type());
	}else{
		post("unknown type");
		return std::vector<t_atom>();
	}
}	


inline std::vector<t_atom> Fingers::makeFingerAtoms(int handIndex, int fingerIndex, const std::string &name, float value){
	auto vector = allocateVector(name, 4);
	SETFLOAT(&vector[0], handIndex);
   	SETSYMBOL(&vector[1], gensym("finger"));
   	SETFLOAT(&vector[2], fingerIndex);

	int index = setDataNames(vector, getReturnType(name));
	SETFLOAT(&vector[index], value);
	return vector;

}


inline std::vector<t_atom> Fingers::makeFingerAtoms(int handIndex, int fingerIndex, const std::string &name, Finger::Type type){
	auto vector = allocateVector(name, 4);
    SETFLOAT(&vector[0], handIndex);
   	SETSYMBOL(&vector[1], gensym("finger"));
   	SETFLOAT(&vector[2], fingerIndex);

	int index = setDataNames(vector, getReturnType(name), 3);
	std::string fingerName;
	switch(type){
		case Finger::TYPE_THUMB:{
			fingerName = "thumb";
			break;
		}
		case Finger::TYPE_INDEX:{
			fingerName = "index";
			break;
		}
		case Finger::TYPE_MIDDLE:{
			fingerName = "middle";
			break;
		}
		case Finger::TYPE_RING:{
			fingerName = "ring";
			break;
		}
		case Finger::TYPE_PINKY:{
			fingerName = "pinky";
			break;
		}
	}
	SETSYMBOL(&vector[index], gensym(fingerName.c_str()));

	return vector;
}

inline std::vector<t_atom> Fingers::makeFingerAtoms(int handIndex, int fingerIndex, const std::string &name, float width, float length){
	auto vector = allocateVector(name, 5);
    SETFLOAT(&vector[0], handIndex);
   	SETSYMBOL(&vector[1], gensym("finger"));
   	SETFLOAT(&vector[2], fingerIndex);

	int index = setDataNames(vector, getReturnType(name),3);
	SETFLOAT(&vector[index], width);
	SETFLOAT(&vector[index+1], length);
	return vector;
}

inline std::vector<t_atom> Fingers::makeFingerAtoms(int handIndex, int fingerIndex, const std::string &name, float x, float y, float z){
	auto vector = allocateVector(name, 6);
    SETFLOAT(&vector[0], handIndex);
   	SETSYMBOL(&vector[1], gensym("finger"));
   	SETFLOAT(&vector[2], fingerIndex);

	int index = setDataNames(vector, getReturnType(name),3);
	SETFLOAT(&vector[index], x);
	SETFLOAT(&vector[index+1], y);
	SETFLOAT(&vector[index+2], z);
	return vector;
}

inline int Fingers::count(){
	return fingers.count();
}