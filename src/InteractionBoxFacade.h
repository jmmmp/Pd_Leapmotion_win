#pragma once

class InteractionBoxFacade{

public:
	InteractionBoxFacade(const Frame& frame);
	std::vector<t_atom> getAtoms(const std::string &name);

private:
	std::vector<t_atom> makeIBAtoms(const std::string &name, float value);
	std::vector<t_atom> makeIBAtoms(const std::string &name, float x, float y, float z);
	const Frame &frame;
};

inline InteractionBoxFacade::InteractionBoxFacade(const Frame &frame):
frame(frame){}

inline std::vector<t_atom> InteractionBoxFacade::getAtoms(const std::string &name){
	auto interactionBox = frame.interactionBox();

	if(name == "interaction_box_center"){
		auto vector = interactionBox.center();
		return makeIBAtoms("interaction_box_center", vector.x, vector.y, vector.z);
	}else if(name == "interaction_box_width"){		
		return makeIBAtoms("interaction_box_width", interactionBox.width());
	}else if(name == "interaction_box_height"){
		return makeIBAtoms("interaction_box_height", interactionBox.height());
	}else if(name == "interaction_box_depth"){
		return makeIBAtoms("interaction_box_depth", interactionBox.depth());
	}else{
		post("unknown parameter for interaction box");
		return std::vector<t_atom>();
	}
}

inline std::vector<t_atom> InteractionBoxFacade::makeIBAtoms(const std::string &name, float value){
	auto vector = allocateVector(name, 1);
	int index = setDataNames(vector, getReturnType(name),0);
	SETFLOAT(&vector[index], value);
	return vector;
}

inline std::vector<t_atom> InteractionBoxFacade::makeIBAtoms(const std::string &name, float x, float y, float z){
	auto vector = allocateVector(name, 3);
	int index = setDataNames(vector, getReturnType(name),0);
	SETFLOAT(&vector[index], x);
	SETFLOAT(&vector[index+1], y);
	SETFLOAT(&vector[index+2], z);
	return vector;
}