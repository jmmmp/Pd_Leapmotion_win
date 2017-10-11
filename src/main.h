#pragma once

#include <flext.h>

#include "Leap.h"
#include "LeapMath.h"
#include "Dispatcher.h"
#include "FlagHolder.h"

#include "Fingers.h"
#include "InteractionBoxFacade.h"
#include "ConfigFacade.h"
#include "DeviceFacade.h"
#include "GestureFacade.h"
#include "Utils.h"

class leapmotion: public flext_base{

    FLEXT_HEADER(leapmotion,flext_base)
    
    Dispatcher dispatcher;
    Controller controller;
	FlagHolder flagHolder;
    ConfigFacade configFacade;
    DeviceFacade deviceFacade;
    GestureFacade gestureFacade;
    
public:
    leapmotion();
    ~leapmotion();

	void m_info();
    void m_bang();
    void m_flag(int argc, const t_atom *argv);
    void m_gestures(int argc, const t_atom *argv);
    void m_config(int argc, const t_atom *argv);

private:

    void out_general(const Frame &frame);
    void out_interaction_box(const Frame &frame);
    void out_tools(const Frame &frame);
    void out_hands(const Frame &frame);
    void out_arm(int handIndex, const Hand &hand);
    void out_fingers(int handIndex, const Hand &hand);
    void out_gesture_type(int gestureIndex, const Gesture &gesture);
    void out_gesture_state(int gestureIndex, const Gesture &gesture);
    void out_gesture_duration(int gestureIndex, const Gesture &gesture);
    void out_gesture_id(int gestureIndex, const Gesture &gesture);


    FLEXT_CALLBACK(m_info);
    FLEXT_CALLBACK(m_bang);
	FLEXT_CALLBACK_V(m_flag);
    FLEXT_CALLBACK_V(m_gestures);
    FLEXT_CALLBACK_V(m_config);


};
