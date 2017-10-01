#include <flext.h>
#include "Leap.h"
#include "LeapMath.h"
#include "Dispatcher.h"
#include "FlagHolder.h"
#include "Utils.h"

// check for appropriate flext version
#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif


class leapmotion: public flext_base
{
    FLEXT_HEADER(leapmotion,flext_base)
    
    // Create a sample listener and controller
    Dispatcher dispatcher;
    Controller controller;

	FlagHolder flagHolder;
 
    
public:
    leapmotion(){
        AddInAnything(); 
        AddOutList();  // general info
        AddOutList();  // hands, fingers, tools
        AddOutList();  // gesture info 

        // register methods
        FLEXT_ADDMETHOD(0,m_bang);
        FLEXT_ADDMETHOD_(0, "flag", m_flag);
        FLEXT_ADDMETHOD_(0, "info", m_info);

        // Set policy flag so that patch can receive events from
        // the controller even when the patch window is in background
        controller.setPolicyFlags(Controller::POLICY_BACKGROUND_FRAMES);
        
        // Have the sample listener receive events from the controller
        controller.addListener(dispatcher);
          
    }

    ~leapmotion(){
        // Remove the sample listener when done
        controller.removeListener(dispatcher);
    }
    void out_general(const Frame &frame){
        if (flagHolder.get("general")) {
            t_atom generalInfo[6];

            SETFLOAT(&generalInfo[0], (float)frame.id());
            SETFLOAT(&generalInfo[1], (float)frame.timestamp());
            SETFLOAT(&generalInfo[2], (float)frame.hands().count());
            SETFLOAT(&generalInfo[3], (float)frame.fingers().count());
            SETFLOAT(&generalInfo[4], (float)frame.tools().count());
            SETFLOAT(&generalInfo[5], (float)frame.gestures().count());
            ToOutList(OUTLET_GENERAL, 6, generalInfo);        
        }
    }

    void out_tools(const Frame &frame){
        int num_tools = frame.tools().count();
        for(int i = 0; i<num_tools; i++){
            Tool tool= frame.tools()[i];

            t_atom toolInfo[5];
            if (flagHolder.get("tools_position")) {
                auto atoms = makeAtoms(i, "tools_position", tool.direction().x, tool.direction().y, tool.direction().z);
                ToOutAnything(OUTLET_DATA, gensym("tool"), atoms.size(), &atoms[0]);
            }
            if(flagHolder.get("tools_position")) {
                auto atoms = makeAtoms(i, "tools_position", tool.direction().x, tool.direction().y, tool.direction().z);
                ToOutAnything(OUTLET_DATA, gensym("tool"), atoms.size(), &atoms[0]);
            }
            if(flagHolder.get("tools_velocity")){
                auto atoms = makeAtoms(i, "tools_velocity", tool.direction().x, tool.direction().y, tool.direction().z);
                ToOutAnything(OUTLET_DATA, gensym("tool"), atoms.size(), &atoms[0]);
            }
            if(flagHolder.get("tools_size")){
                auto atoms = makeAtoms(i, "tools_size", tool.direction().x, tool.direction().y, tool.direction().z);
                ToOutAnything(OUTLET_DATA, gensym("tool"), atoms.size(), &atoms[0]);
            }
        }
    }
    void out_hands(const Frame &frame){
        int num_hands = frame.hands().count();
        for(int i = 0; i<num_hands; i++){
            Hand hand = frame.hands()[i];
            int num_fingers = hand.fingers().count();
            int num_tools = hand.tools().count();

            if(flagHolder.get("hands_direction")){
                auto atoms = makeAtoms(i, "hands_direction", hand.direction().x, hand.direction().y, hand.direction().z);
                ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
            }
            if(flagHolder.get("hands_palm_position")){
                auto atoms = makeAtoms(i, "hands_palm_position", hand.palmPosition().x, hand.palmPosition().y, hand.palmPosition().z);
                ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
            }
            if(flagHolder.get("hands_palm_velocity")){
                auto atoms = makeAtoms(i, "hands_palm_velocity", hand.palmVelocity().x, hand.palmVelocity().y, hand.palmVelocity().z);
                ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
            }
            if(flagHolder.get("hands_palm_normal")) {
                auto atoms = makeAtoms(i, "hands_palm_normal", hand.palmNormal().x, hand.palmNormal().y, hand.palmNormal().z);
                ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
            }
            if(flagHolder.get("hands_sphere_radius")){
                auto atoms = makeAtoms(i, "hands_sphere_radius", hand.sphereRadius());
                ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
            }
            if(flagHolder.get("hands_sphere_center")){
                auto atoms = makeAtoms(i, "hands_sphere_center", hand.sphereCenter().x, hand.sphereCenter().y, hand.sphereCenter().z);
                ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
            }
            if(flagHolder.get("hands_finger_count")){
                auto atoms = makeAtoms(i, "hands_finger_count", num_fingers);
                ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
            }
            if(flagHolder.get("hands_tool_count")){
                auto atoms = makeAtoms(i, "hands_tool_count", num_tools);
                ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
            }
            out_fingers(i, hand);
        }

    }
    void out_fingers(int handIndex, const Hand &hand){
        int num_fingers = hand.fingers().count();
        for(int j = 0; j<num_fingers; j++){
            Finger finger;
            finger = hand.fingers()[j];                    
            t_atom fingerInfo[7];
            if(flagHolder.get("fingers_direction")){
                SETFLOAT(&fingerInfo[0], handIndex); // index
                SETSYMBOL(&fingerInfo[1], gensym("fingers"));
                SETFLOAT(&fingerInfo[2], j);
                SETSYMBOL(&fingerInfo[3], gensym("direction"));
                SETFLOAT(&fingerInfo[4], finger.direction().x);
                SETFLOAT(&fingerInfo[5], finger.direction().y);
                SETFLOAT(&fingerInfo[6], finger.direction().z);
                ToOutAnything(OUTLET_DATA, gensym("hand"), 7, fingerInfo);
            }
            if(flagHolder.get("fingers_position")){
                SETFLOAT(&fingerInfo[0], handIndex); // index
                SETSYMBOL(&fingerInfo[1], gensym("fingers"));
                SETFLOAT(&fingerInfo[2], j);
                SETSYMBOL(&fingerInfo[3], gensym("position"));
                SETFLOAT(&fingerInfo[4], finger.tipPosition().x);
                SETFLOAT(&fingerInfo[5], finger.tipPosition().y);
                SETFLOAT(&fingerInfo[6], finger.tipPosition().z);
                ToOutAnything(OUTLET_DATA, gensym("hand"), 7, fingerInfo);
            }
            if(flagHolder.get("fingers_velocity")){
                SETFLOAT(&fingerInfo[0], handIndex); // index
                SETSYMBOL(&fingerInfo[1], gensym("fingers"));
                SETFLOAT(&fingerInfo[2], j);
                SETSYMBOL(&fingerInfo[3], gensym("velocity"));
                SETFLOAT(&fingerInfo[4], finger.tipVelocity().x);
                SETFLOAT(&fingerInfo[5], finger.tipVelocity().y);
                SETFLOAT(&fingerInfo[6], finger.tipVelocity().z);
                ToOutAnything(OUTLET_DATA, gensym("hand"), 7, fingerInfo);
            }
            if(flagHolder.get("fingers_size")){
                SETFLOAT(&fingerInfo[0], handIndex); // index
                SETSYMBOL(&fingerInfo[1], gensym("fingers"));
                SETFLOAT(&fingerInfo[2], j);
                SETSYMBOL(&fingerInfo[3], gensym("size"));
                SETFLOAT(&fingerInfo[4], finger.width());
                SETFLOAT(&fingerInfo[5], finger.length());
                ToOutAnything(OUTLET_DATA, gensym("hand"), 6, fingerInfo);
            }
        }
    }

    void m_bang(){

        Frame frame = dispatcher.frame;
        int num_hands = frame.hands().count();
        int num_tools = frame.tools().count();
        int num_gestures = frame.gestures().count();

        out_general(frame);
        out_tools(frame);
        out_hands(frame);

        // hands and fingers
  


        t_atom gestureCountInfo[1];            
        for(int i = 0;i < num_gestures; i++){
            Gesture gesture;
            gesture = frame.gestures()[i];
            //type
            t_atom gestureTypeInfo[3];
            SETFLOAT(&gestureTypeInfo[0], i);
            SETSYMBOL(&gestureTypeInfo[1], gensym("type"));
            switch(gesture.type()){
                case Gesture::TYPE_INVALID:
                    SETSYMBOL(&gestureTypeInfo[2], gensym("TYPE_INVALID"));
                    break;
                case Gesture::TYPE_SWIPE:
                    SETSYMBOL(&gestureTypeInfo[2], gensym("TYPE_SWIPE"));
                    break;
                case Gesture::TYPE_CIRCLE:
                    SETSYMBOL(&gestureTypeInfo[2], gensym("TYPE_CIRCLE"));
                    break;
                case Gesture::TYPE_SCREEN_TAP:
                    SETSYMBOL(&gestureTypeInfo[2], gensym("TYPE_SCREEN_TAP"));
                    break;
                case Gesture::TYPE_KEY_TAP:
                    SETSYMBOL(&gestureTypeInfo[2], gensym("TYPE_KEY_TAP"));
                    break;
            }
            ToOutList(OUTLET_GESTURE, 3, gestureTypeInfo);

            //state
            t_atom gestureStateInfo[3];
            SETFLOAT(&gestureStateInfo[0], i);
            SETSYMBOL(&gestureStateInfo[1], gensym("state"));
            switch(gesture.state())
            {
                case Gesture::STATE_INVALID:
                    SETSYMBOL(&gestureStateInfo[2], gensym("STATE_INVALID"));
                    break;
                case Gesture::STATE_START:
                    SETSYMBOL(&gestureStateInfo[2], gensym("TYPE_START"));
                    break;
                case Gesture::STATE_UPDATE:
                    SETSYMBOL(&gestureStateInfo[2], gensym("STATE_UPDATE"));
                    break;
                case Gesture::STATE_STOP:
                    SETSYMBOL(&gestureStateInfo[2], gensym("TYPE_STOP"));
                    break;
            }
            ToOutList(OUTLET_GESTURE, 3, gestureStateInfo);

            t_atom gestureDurationInfo[3];
            SETFLOAT(&gestureDurationInfo[0], i);
            SETSYMBOL(&gestureDurationInfo[1], gensym("duration"));
            SETFLOAT(&gestureDurationInfo[2], gesture.duration());
            ToOutList(OUTLET_GESTURE, 3, gestureDurationInfo);

            t_atom gestureIdInfo[3];
            SETFLOAT(&gestureIdInfo[0], i);
            SETSYMBOL(&gestureIdInfo[1], gensym("id"));
            SETFLOAT(&gestureIdInfo[2], gesture.id());
            ToOutList(OUTLET_GESTURE, 3, gestureIdInfo);

        }
    }

	void m_flag(int argc, t_atom *argv){
		if (argc != 2) {
			post("flag message must contain two elements. flag name and 1 or 0");
			return;
		}

		std::string flagName = GetAString(argv[0]);
		bool flag = static_cast<bool>(GetAInt(argv[1]));

		flagHolder.set(flagName, flag);
    }

    void m_gestures(int argc, const t_atom *argv){
        if(argc > 2){
            post("leapmotion: gestures too many arguments");
        }
        else if(argc < 2){
            post("leapmotion: gestures too few arguments");
        }
        else{
            if((argv[0].a_type != A_SYMBOL) || (argv[1].a_type != A_FLOAT)){
                post("leapmotion: gestures wrong arguments");
                return;
            }
            char *type = argv[0].a_w.w_symbol->s_name;
            float s = argv[1].a_w.w_float;
            
            if(strcmp(type, "TYPE_CIRCLE") == 0){
                if(s == 0.0){
                    controller.enableGesture(Gesture::TYPE_CIRCLE, false);
                }else{
                    controller.enableGesture(Gesture::TYPE_CIRCLE, true);
                }
            }
            if(strcmp(type, "TYPE_SWIPE") == 0){
                if(s == 0.0){
                    controller.enableGesture(Gesture::TYPE_SWIPE, false);
                }else{
                    controller.enableGesture(Gesture::TYPE_SWIPE, true);
                }
            }
            if(strcmp(type, "TYPE_KEY_TAP") == 0){
                if(s == 0.0){
                    controller.enableGesture(Gesture::TYPE_KEY_TAP, false);
                }else{
                    controller.enableGesture(Gesture::TYPE_KEY_TAP, true);
                }
            }
            if(strcmp(type, "TYPE_SCREEN_TAP") == 0){
                if(s == 0.0){
                    controller.enableGesture(Gesture::TYPE_SCREEN_TAP, false);
                }else{
                    controller.enableGesture(Gesture::TYPE_SCREEN_TAP, true);
                }
            }
        }
    }
	void m_info() {
		flagHolder.print();
		post("-Gestures-");
		post("TYPE_CIRCLE:%s", bool_to_string(controller.isGestureEnabled(Gesture::TYPE_CIRCLE)));
		post("TYPE_SWIPE:%s", bool_to_string(controller.isGestureEnabled(Gesture::TYPE_SWIPE)));
		post("TYPE_KEY_TAP:%s", bool_to_string(controller.isGestureEnabled(Gesture::TYPE_KEY_TAP)));
		post("TYPE_SCREEN_TAP:%s", bool_to_string(controller.isGestureEnabled(Gesture::TYPE_SCREEN_TAP)));


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


private:

    FLEXT_CALLBACK(m_info);
    FLEXT_CALLBACK(m_bang);
	FLEXT_CALLBACK_V(m_flag);
};

FLEXT_NEW("leapmotion",leapmotion)
