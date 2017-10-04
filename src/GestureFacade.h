#pragma once 

#include "GestureFacade.h"

class GestureFacade{

public:
	GestureFacade(Controller &controller);
	void print();
private:
	Controller &controller;
};

GestureFacade::GestureFacade(Controller &controller):
controller(controller){

}

void GestureFacade::print(){
    post("-Gestures-");
    post("TYPE_CIRCLE:%s", bool_to_string(controller.isGestureEnabled(Gesture::TYPE_CIRCLE)));
    post("TYPE_SWIPE:%s", bool_to_string(controller.isGestureEnabled(Gesture::TYPE_SWIPE)));
    post("TYPE_KEY_TAP:%s", bool_to_string(controller.isGestureEnabled(Gesture::TYPE_KEY_TAP)));
    post("TYPE_SCREEN_TAP:%s", bool_to_string(controller.isGestureEnabled(Gesture::TYPE_SCREEN_TAP)));
}