#pragma once

class ConfigFacade{
	
public:
	ConfigFacade( Controller &controller);
	void set( const std::string &param, float value);
	void print();
private:
	std::string messageToKey(const std::string &message);
	std::unordered_map<std::string, std::string> dict;
	Controller &controller;
};


inline ConfigFacade::ConfigFacade( Controller &controller):
controller(controller){

	dict["circle_min_radius"] = "Gesture.Circle.MinRadius";
	dict["circle_min_arc"] = "Gesture.Circle.MinArc";
	dict["swipe_min_length"] = "Gesture.Swipe.MinLength";
	dict["swipe_min_velocity"] = "Gesture.Swipe.MinVelocity";
	dict["key_tap_min_down_velocity"] = "Gesture.KeyTap.MinDownVelocity";
	dict["key_tap_history_seconds"] = "Gesture.KeyTap.HistorySeconds";
	dict["key_tap_min_distance"] = "Gesture.KeyTap.MinDistance";
	dict["screen_tap_min_forward_velocity"] = "Gesture.ScreenTap.MinForwardVelocity";
	dict["screen_tap_history_seconds"] = "Gesture.ScreenTap.HistorySeconds";
	dict["screen_tap_min_distance"] = "Gesture.ScreenTap.MinDistance";
}


inline void ConfigFacade::set(const std::string &param, float value){

	try{
		std::string key = dict.at(param);
		controller.config().setFloat(key, value);
		controller.config().save();
	}catch (std::out_of_range e){
		post("no such config parameter");
	}
}

inline void ConfigFacade::print(){
	Config config = controller.config();
    post("-Config-");
    post("Circle MinRadius:%f", config.getFloat("Gesture.Circle.MinRadius"));
    post("Circle MinArc:%f", config.getFloat("Gesture.Circle.MinArc"));

    post("Swipe MinLength:%f",  config.getFloat("Gesture.Swipe.MinLength"));
    post("Swipe MinVelocity:%f",  config.getFloat("Gesture.Swipe.MinVelocity"));

    post("KeyTap MinDownVelocity:%f",  config.getFloat("Gesture.KeyTap.MinDownVelocity"));
    post("KeyTap HistorySeconds:%f",  config.getFloat("Gesture.KeyTap.HistorySeconds"));
    post("KeyTap MinDistance:%f",  config.getFloat("Gesture.KeyTap.MinDistance"));

    post("ScreenTap MinForwardVelocity:%f",  config.getFloat("Gesture.ScreenTap.MinForwardVelocity"));
    post("ScreenTap HistorySeconds:%f",  config.getFloat("Gesture.ScreenTap.HistorySeconds"));
    post("ScreenTap MinDistance:%f",  config.getFloat("Gesture.ScreenTap.MinDistance"));
}


