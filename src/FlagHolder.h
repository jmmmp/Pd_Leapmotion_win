#pragma once

#include <unordered_map>
#include "DataProperties.h"
#include "Utils.h"

class FlagHolder{
public:
	FlagHolder();
	void print();
	bool get(const std::string &flagName);
	void set(const std::string &fragName, bool flag);
private:
	const char* flag_to_string(const std::string &flagName);
	
	std::unordered_map<std::string, bool> flags;
};

inline FlagHolder::FlagHolder() {
	// all disabled by default
	for (const std::string &name : flagNameList) {
		flags.emplace(name, false);
	}

}

inline bool FlagHolder::get(const std::string &flagName) {
	return flags.at(flagName);
}

inline void FlagHolder::set(const std::string &flagName, bool flag) {
	flags[flagName] = flag;
}


inline void FlagHolder::print() {
	post("General Info:%s", flag_to_string("general"));

	post("-Hands-");
	post("direction:%s", flag_to_string("hands_direction"));
	post("palm position:%s", flag_to_string("hands_palm_position"));
	post("palm velocity:%s", flag_to_string("hands_palm_velocity"));
	post("palm normal:%s", flag_to_string("hands_palm_normal"));

	post("finger count:%s", flag_to_string("hands_finger_count"));
	post("tool count:%s", flag_to_string("hands_tool_count"));
	post("sphere radius:%s", flag_to_string("hands_sphere_radius"));
	post("sphere center:%s", flag_to_string("hands_sphere_center"));

	post("-Tools-");
	post("direction:%s", flag_to_string("tools_direction"));
	post("position:%s", flag_to_string("tools_position"));
	post("velocity:%s", flag_to_string("tools_velocity"));
	post("size:%s", flag_to_string("tools_size"));

	post("-Fingers-");
	post("direction:%s", flag_to_string("fingers_direction"));
	post("position:%s", flag_to_string("fingers_position"));
	post("velocity:%s", flag_to_string("fingers_velocity"));
	post("size:%s", flag_to_string("fingers_size"));

}

inline const char* FlagHolder::flag_to_string(const std::string &flagName) {
	return bool_to_string(flags.at(flagName));
}


