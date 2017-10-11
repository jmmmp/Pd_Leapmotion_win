
class Arm{

public:
	Arm(const Arm &arm);
	std::vector<t_atom> getAtoms(int handIndex, int fingerIndex, const std::string &name);
private:
 	std::vector<t_atom>makeFingerAtoms(int handIndex, int fingerIndex, const std::string &name, float value);
 	std::vector<t_atom>makeFingerAtoms(int handIndex, int fingerIndex, const std::string &name, float width, float length);
 	std::vector<t_atom>makeFingerAtoms(int handIndex, int fingerIndex, const std::string &name, float x, float y, float z);
 	std::vector<t_atom>makeFingerAtoms(int handIndex, int fingerIndex, const std::string &name, Finger::Type type);
 	const FingerList &fingers;
};

};
