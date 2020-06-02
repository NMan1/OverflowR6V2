#include <Windows.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <shlobj.h>
#include "../helpers/settings.h"

using namespace settings;

class Config
{

public:
	Config(std::string name)
	{
		TCHAR path[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_DOCUMENTS, NULL, 0, path)))
		{
			std::string PathToFile = std::string(path) + "\\" + name;
			this->path = PathToFile; //Set path to cfg file
			this->input = std::ifstream(this->path); //open input
		}
	}

	void open()
	{
		this->outputstream = std::ofstream(this->path); //open output for config writing
	}

	void close()
	{
		this->outputstream.close(); //close output after writing config
	}

	template <typename tp>
	tp set(std::string name, tp value) {
		outputstream << name << " " << value << std::endl; //Write to config
		return true;
	}

	template <class tg>
	tg get(std::string name) {
		this->input.close(); //Close Input
		this->input = std::ifstream(this->path); //Jump back to idx 0 of txt file
		std::vector<std::string>stack;
		std::string buffer;
		while (this->input >> buffer) {
			stack.push_back(buffer); //push all options into vector
		}
		for (int i = 0; i < stack.size(); i++) {
			if (stack.at(i) == name) {

				return atof(stack.at(i + 1).c_str()); //if option found get value from idx + 1
			}
		}
	}
private:
	std::string path;
	std::ifstream input;
	std::ofstream outputstream;
};

void static SetConfig(Config* config)
{
	config->open();
	config->set<bool>("bOutline", outline);
	config->set<bool>("bTeam", team);
	config->set<bool>("bChams", chams);
	config->set<bool>("bCavEsp", cav_esp);

	config->set<int>("iAimSelection", aim_selection);
	config->set<int>("iAimBone", aim_bone);
	config->set<int>("iAimKey2", aim_key_2);
	config->set<int>("iAimKey", aim_key);
	config->set<int>("iSilentKey", silent_key);

	config->set<bool>("bAimbot", aimbot);
	config->set<int>("iAimFov", aim_fov);
	config->set<bool>("bSmoothing", smoothing);
	config->set<bool>("bSilent", silent);
	config->set<float>("iSmoothing", smoothing_value);
	config->set<bool>("bRunShoot", run_shoot);
	config->set<bool>("bDamage", damage);
	config->set<bool>("bNoRecoil", no_recoil);
	config->set<bool>("bSpread", no_spread);

	config->set<bool>("bUnlockAll", unlock_all);
	config->set<bool>("bColor", color);
	config->set<int>("iNoClip", no_clip_key);
	config->set<bool>("bNoClip", no_clip);
	config->set<bool>("bWepFov", wep_fov);
	config->set<bool>("bPlayerFov", player_fov);
	config->set<bool>("bSpeed", speed);
	config->set<bool>("bNoFlash", no_flash);

	config->set<float>("fSpread", spread_value);
	config->set<float>("fWepFov", wep_fov_value);
	config->set<float>("fPlayerFov", player_fov_value);
	config->set<float>("fValue", speed_value_f);
	config->set<int>("iSpeed", speed_value);
	config->set<int>("iBullet", bullet_value);
	config->set<float>("fBullet", bullet_value_f);

	config->set<float>("fChamsR", chams_r);
	config->set<float>("fChamsG", chams_g);
	config->set<float>("fChamsB", chams_b);
	config->set<float>("fChamsOpp", chams_oppacity);
	config->set<float>("fOpp", chams_oppacity_f);
	config->set<bool>("bno_animations;", no_animations);
	config->set<bool>("brapid_fire;", rapid_fire);

	config->close();
}

void static ReadConfig(Config* config)
{

	outline = config->get<bool>("bOutline");
	team = config->get<bool>("bTeam");
	chams = config->get<bool>("bChams");
	cav_esp = config->get<bool>("bCavEsp");

	aim_selection = config->get<int>("iAimSelection");
	aim_bone = config->get<int>("iAimBone");
	aim_key_2 = config->get<int>("iAimKey2");
	aim_key = config->get<int>("iAimKey");
	silent_key = config->get<int>("iSilentKey");

	aimbot = config->get<bool>("bAimbot");
	aim_fov = config->get<int>("iAimFov");
	smoothing = config->get<bool>("bSmoothing");
	silent = config->get<bool>("bSilent");
	smoothing_value = config->get<float>("iSmoothing");
	run_shoot = config->get<bool>("bRunShoot");
	damage = config->get<bool>("bDamage");
	no_recoil = config->get<bool>("bNoRecoil");
	no_spread = config->get<bool>("bSpread");

	unlock_all = config->get<bool>("bUnlockAll");
	color = config->get<bool>("bColor");
	no_clip_key = config->get<int>("iNoClip");
	no_clip = config->get<bool>("bNoClip");
	wep_fov = config->get<bool>("bWepFov");
	player_fov = config->get<bool>("bPlayerFov");
	speed = config->get<bool>("bSpeed");
	no_flash = config->get<bool>("bNoFlash");
	rapid_fire = config->get<bool>("brapid_fire");

	spread_value = config->get<float>("fSpread");
	wep_fov_value = config->get<float>("fWepFov");
	player_fov_value = config->get<float>("fPlayerFov");
	speed_value_f = config->get<int>("fValue");
	speed_value = config->get<int>("iSpeed");
	bullet_value = config->get<int>("iBullet");
	bullet_value_f = config->get<float>("fBullet");

	chams_r = config->get<float>("fChamsR");
	chams_g = config->get<float>("fChamsG");
	chams_b = config->get<float>("fChamsB");
	chams_oppacity = config->get<float>("fChamsOpp");
	chams_oppacity_f = config->get<float>("fOpp");
	no_animations = config->get<float>("bno_animations");
}

extern Config* DefualtCfg;
extern Config* LegitCfg;
extern Config* RageCfg;