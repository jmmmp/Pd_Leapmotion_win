#include "main.h"
// check for appropriate flext version
#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif

leapmotion::leapmotion():
configFacade(controller),
deviceFacade(controller),
gestureFacade(controller){
    AddInAnything(); 
    AddOutList();  // general info
    AddOutList();  // hands, fingers, tools
    AddOutList();  // gesture info 
    // register methods

    FLEXT_ADDMETHOD(0,m_bang);
    FLEXT_ADDMETHOD_(0, "flag", m_flag);
    FLEXT_ADDMETHOD_(0, "info", m_info);
    FLEXT_ADDMETHOD_(0, "config", m_config);
    FLEXT_ADDMETHOD_(0, "gestures", m_gestures);

    controller.setPolicyFlags(Controller::POLICY_BACKGROUND_FRAMES);
    controller.addListener(dispatcher);

}

leapmotion::~leapmotion(){
    controller.removeListener(dispatcher);
}

void leapmotion::m_info() {
    flagHolder.print();
    configFacade.print();
    deviceFacade.print();
    gestureFacade.print();
}

void leapmotion::m_bang(){

    Frame frame = dispatcher.frame;

    int num_hands = frame.hands().count();
    int num_tools = frame.tools().count();
    int num_gestures = frame.gestures().count();

    out_general(frame);
    out_interaction_box(frame);
    out_tools(frame);
    out_hands(frame);

    // hands and fingers
    t_atom gestureCountInfo[1];            
    for(int i = 0;i < num_gestures; i++){
        Gesture gesture = frame.gestures()[i];
        out_gesture_type(i, gesture);
        out_gesture_state(i, gesture);
        out_gesture_duration(i, gesture);
        out_gesture_id(i, gesture);
    }
}

void leapmotion::m_flag(int argc, const t_atom *argv){
    if (argc != 2) {
        post("flag message must contain two elements. flag name and 1 or 0");
        return;
    }
    std::string flagName = GetAString(argv[0]);
    bool flag = static_cast<bool>(GetAInt(argv[1]));
    flagHolder.set(flagName, flag);
}


void leapmotion::m_gestures(int argc, const t_atom *argv){
    if(argc != 2){
        post("leapmotion: gestures wrong number of arguments, must be 2");
        return;
    }

    if((argv[0].a_type != A_SYMBOL) || (argv[1].a_type != A_FLOAT)){
        post("leapmotion: gestures wrong arguments. must be symbol float.");
        return;
    }
    std::string type(argv[0].a_w.w_symbol->s_name);
    float flag = static_cast<bool>(argv[1].a_w.w_float);
    
    if(type == "TYPE_CIRCLE"){
        controller.enableGesture(Gesture::TYPE_CIRCLE, flag);
    }else if(type == "TYPE_SWIPE"){
        controller.enableGesture(Gesture::TYPE_SWIPE, flag);
    }else if(type == "TYPE_KEY_TAP"){
        controller.enableGesture(Gesture::TYPE_KEY_TAP, flag);
    }else if(type == "TYPE_SCREEN_TAP"){
        controller.enableGesture(Gesture::TYPE_SCREEN_TAP, flag);
    }else{
        post("unknown gesture");
    }
}

void leapmotion::m_config(int argc, const t_atom *argv){
    if(argc != 2){
        post("leapmotion: config wrong number of arguments, must be 2");
        return;
    }

    if((argv[0].a_type != A_SYMBOL) || (argv[1].a_type != A_FLOAT)){
        post("leapmotion: config wrong arguments. must be symbol float.");
        return;
    }
    std::string type(argv[0].a_w.w_symbol->s_name);
    float value = argv[1].a_w.w_float;
    configFacade.set(type, value);

}

// private functions
void leapmotion::out_general(const Frame &frame){
    if (flagHolder.get("general")) {
        t_atom generalInfo[8];
        auto fingers = frame.fingers();
        SETFLOAT(&generalInfo[0], frame.id());
        SETFLOAT(&generalInfo[1], frame.timestamp());
        SETFLOAT(&generalInfo[2], frame.hands().count());
        SETFLOAT(&generalInfo[3], flagHolder.get("fingers_extended") ? fingers.count() : fingers.extended().count());
        SETFLOAT(&generalInfo[4], frame.tools().count());
        SETFLOAT(&generalInfo[5], frame.gestures().count());
        SETFLOAT(&generalInfo[6], frame.isValid());
        SETFLOAT(&generalInfo[7], frame.currentFramesPerSecond());

        ToOutList(OUTLET_GENERAL, 8, generalInfo);        
    }
}

void leapmotion::out_interaction_box(const Frame& frame){
    InteractionBoxFacade ibFacade(frame);

    if(flagHolder.get("interaction_box_center")) {
        auto atoms = ibFacade.getAtoms("interaction_box_center");
        ToOutAnything(OUTLET_DATA, gensym("interaction_box"), atoms.size(), &atoms[0]);
    }
    if(flagHolder.get("interaction_box_width")){
        auto atoms = ibFacade.getAtoms("interaction_box_width");
        ToOutAnything(OUTLET_DATA, gensym("interaction_box"), atoms.size(), &atoms[0]);
    }
    if(flagHolder.get("interaction_box_height")){
        auto atoms = ibFacade.getAtoms("interaction_box_height");
        ToOutAnything(OUTLET_DATA, gensym("interaction_box"), atoms.size(), &atoms[0]);
    }
    if(flagHolder.get("interaction_box_depth")){
        auto atoms = ibFacade.getAtoms("interaction_box_depth");
        ToOutAnything(OUTLET_DATA, gensym("interaction_box"), atoms.size(), &atoms[0]);
    }
}

void leapmotion::out_tools(const Frame &frame){
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

void leapmotion::out_hands(const Frame &frame){
    int num_hands = frame.hands().count();
    for(int i = 0; i<num_hands; i++){
        Hand hand = frame.hands()[i];
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
        if(flagHolder.get("hands_is_left")){
            auto atoms = makeAtoms(i, "hands_is_left", hand.isLeft());
            ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
        }
        if(flagHolder.get("hands_is_right")){
            auto atoms = makeAtoms(i, "hands_is_right", hand.isRight());
            ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
        }
        if(flagHolder.get("hands_grab_strength")){
        	auto atoms = makeAtoms(i, "hands_grab_strength", hand.grabStrength());
        	ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
        }
        if(flagHolder.get("hands_pinch_strength")){
        	auto atoms = makeAtoms(i, "hands_pinch_strength", hand.pinchStrength());
        	ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
        }
        if(flagHolder.get("hands_time_visible")){
        	auto atoms = makeAtoms(i, "hands_time_visible", hand.timeVisible());
        	ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
        }
        if(flagHolder.get("hands_tool_count")){
            auto atoms = makeAtoms(i, "hands_tool_count", num_tools);
            ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
        }

        out_arm(i, hand);
        out_fingers(i, hand);
    }
}

void leapmotion::out_arm(int handIndex, const Hand &hand){
	auto arm = hand.arm();
	if(flagHolder.get("arms_center")){
        auto atoms = makeAtoms(handIndex, "hands_arms_center", arm.center());
        ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
    }
    if(flagHolder.get("arms_direction")){
        auto atoms = makeAtoms(handIndex, "hands_arms_direction", arm.direction());
        ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
    }
	if(flagHolder.get("arms_elbow_position")){
		auto ePos = arm.elbowPosition();
        auto atoms = makeAtoms(handIndex, "hands_arms_elbow_position", arm.elbowPosition());
        ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
    }
    if(flagHolder.get("arms_wrist_position")){
        auto atoms = makeAtoms(handIndex, "hands_arms_wrist_position", arm.wristPosition());
        ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
    }
    if(flagHolder.get("arms_width")){
        auto atoms = makeAtoms(handIndex, "hands_arms_width", arm.width());
        ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
    }
}

void leapmotion::out_fingers(int handIndex, const Hand &hand){
    FingerList fingerList = flagHolder.get("fingers_extended") ? hand.fingers().extended() : hand.fingers();

    Fingers fingers(fingerList);
    int num_fingers = fingerList.count();

    if(flagHolder.get("hands_finger_count")){
        auto atoms = makeAtoms(handIndex, "hands_finger_count", num_fingers);
        ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
    }

    for(int fingerIndex = 0; fingerIndex < fingerList.count(); fingerIndex++){

        if(flagHolder.get("fingers_direction")){
            auto atoms = fingers.getAtoms(handIndex, fingerIndex, "hands_finger_direction");
            ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
        }
        if(flagHolder.get("fingers_position")){
            auto atoms = fingers.getAtoms(handIndex, fingerIndex, "hands_finger_position");
            ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
        }
        if(flagHolder.get("fingers_velocity")){
            auto atoms = fingers.getAtoms(handIndex, fingerIndex, "hands_finger_velocity");
            ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
        }
        if(flagHolder.get("fingers_size")){
            auto atoms = fingers.getAtoms(handIndex, fingerIndex, "hands_finger_size");
            ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
        }
        if(flagHolder.get("fingers_type")){
            auto atoms = fingers.getAtoms(handIndex, fingerIndex, "hands_finger_type");
            ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
        }
        if(flagHolder.get("fingers_is_extended")){
            auto atoms = fingers.getAtoms(handIndex, fingerIndex, "hands_finger_is_extended");
            ToOutAnything(OUTLET_DATA, gensym("hand"), atoms.size(), &atoms[0]);
        }
    }
}

void leapmotion::out_gesture_type(int gestureIndex, const Gesture &gesture){
    t_atom gestureTypeInfo[3];
    SETFLOAT(&gestureTypeInfo[0], gestureIndex);
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
}

void leapmotion::out_gesture_state(int gestureIndex, const Gesture &gesture){

    t_atom gestureStateInfo[3];
    SETFLOAT(&gestureStateInfo[0], gestureIndex);
    SETSYMBOL(&gestureStateInfo[1], gensym("state"));
    switch(gesture.state()){
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
}

void leapmotion::out_gesture_duration(int gestureIndex, const Gesture &gesture){
    t_atom gestureDurationInfo[3];
    SETFLOAT(&gestureDurationInfo[0], gestureIndex);
    SETSYMBOL(&gestureDurationInfo[1], gensym("duration"));
    SETFLOAT(&gestureDurationInfo[2], gesture.duration());
    ToOutList(OUTLET_GESTURE, 3, gestureDurationInfo);
}

void leapmotion::out_gesture_id(int gestureIndex, const Gesture &gesture){
    t_atom gestureIdInfo[3];
    SETFLOAT(&gestureIdInfo[0], gestureIndex);
    SETSYMBOL(&gestureIdInfo[1], gensym("id"));
    SETFLOAT(&gestureIdInfo[2], gesture.id());
    ToOutList(OUTLET_GESTURE, 3, gestureIdInfo);
}

FLEXT_NEW("leapmotion",leapmotion)
