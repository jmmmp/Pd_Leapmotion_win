#pragma once

#include <unordered_map>
#include <flext.h>

constexpr int OUTLET_GENERAL = 0;
constexpr int OUTLET_DATA = 1;
constexpr int OUTLET_GESTURE = 2;


const std::vector<std::string> flagNameList = {
	"general",

	"hands_sphere_radius",
	"hands_sphere_center",
	"hands_direction",
	"hands_palm_normal",
	"hands_palm_position",
	"hands_palm_velocity",
	"hands_tool_count",
	"hands_finger_count",

	"tools_direction",
	"tools_position",
	"tools_velocity",
	"tools_size",

	"fingers_direction",
	"fingers_position",
	"fingers_velocity",
	"fingers_size",
	"gestures"
};


struct ReturnType{
	std::vector<std::string> directoryNames;

	ReturnType(const std::string &first, const std::string &second){
		if(first != "") directoryNames.push_back(first);
		if(second != "") directoryNames.push_back(second);
	}
};

std::unordered_map<std::string, ReturnType> returnTypes({

 	std::make_pair<std::string, ReturnType>("hands_direction", ReturnType("direction", "")),
 	std::make_pair<std::string, ReturnType>("hands_palm_position", ReturnType("palm", "position")),
  	std::make_pair<std::string, ReturnType>("hands_palm_velocity", ReturnType("palm", "velocity")),
 	std::make_pair<std::string, ReturnType>("hands_palm_normal", ReturnType("palm", "normal")),

 	std::make_pair<std::string, ReturnType>("hands_sphere_radius", ReturnType("sphere", "radius")),
 	std::make_pair<std::string, ReturnType>("hands_sphere_center", ReturnType("sphere", "center")),

 	std::make_pair<std::string, ReturnType>("hands_tool_count", ReturnType("tool", "count")),
 	std::make_pair<std::string, ReturnType>("hands_finger_count", ReturnType("finger", "count")),

 	std::make_pair<std::string, ReturnType>("hands_finger_direction", ReturnType("finger", "direction")),
 	std::make_pair<std::string, ReturnType>("hands_finger_position", ReturnType("finger", "position")),
 	std::make_pair<std::string, ReturnType>("hands_finger_position", ReturnType("finger", "position")),
 	std::make_pair<std::string, ReturnType>("hands_finger_position", ReturnType("finger", "position")),

 	std::make_pair<std::string, ReturnType>("tools_direction", ReturnType("direction", "")),
 	std::make_pair<std::string, ReturnType>("tools_position", ReturnType("position", "")),
 	std::make_pair<std::string, ReturnType>("tools_velocity", ReturnType("velocity", "")),
 	std::make_pair<std::string, ReturnType>("tools_size", ReturnType("size", ""))
});

inline std::vector<t_atom> allocateVector(const std::string &name, int baseSize){
	auto returnType = returnTypes.at(name);
	std::vector<t_atom> vector;
	vector.resize(baseSize+returnType.directoryNames.size());
	return std::move(vector);
}


inline int setDataNames(std::vector<t_atom> &vector, ReturnType &returnType){
	int index = 1;
	for(int i = 0; i < returnType.directoryNames.size(); i++){
    	SETSYMBOL(&vector[index], gensym(returnType.directoryNames[i].c_str()));
    	index++;
    }
    return index;
}

inline std::vector<t_atom>makeAtoms(int handIndex, const std::string &name, float value){
	auto vector = allocateVector(name, 2);
	SETFLOAT(&vector[0], handIndex);
	int index = setDataNames(vector, returnTypes.at(name));
	SETFLOAT(&vector[index], value);
	return vector;
}

inline std::vector<t_atom>makeAtoms(int handIndex, const std::string &name, float x, float y, float z){
	auto vector = allocateVector(name, 4);
    SETFLOAT(&vector[0], handIndex);
	int index = setDataNames(vector, returnTypes.at(name));
	SETFLOAT(&vector[index], x);
	SETFLOAT(&vector[index+1], y);
	SETFLOAT(&vector[index+2], z);
	return vector;
}

inline std::vector<t_atom>makeFingerAtoms(int handIndex, int fingerIndex const std::string &name, float x, float y, float z){
	auto vector = allocateVector(name, 6);
    SETFLOAT(&vector[0], handIndex);
   	SETSYMBOL(&vector[1], handIndex);

	int index = setDataNames(vector, returnTypes.at(name));
	SETFLOAT(&vector[index], x);
	SETFLOAT(&vector[index+1], y);
	SETFLOAT(&vector[index+2], z);
	return vector;
}
