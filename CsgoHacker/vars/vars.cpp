#include "../includes.h"

namespace vars
{
	bool bMenuOpen = true;

	namespace global
	{
		bool enable;
	}

	namespace aimbot
	{
		bool enable = false;
		bool only_enemy = false;
		float fov = 0.f;
		float downoffset = 0.f;
		float transverseoffset = 0.f;
		int bone = 0;
		bool aiming_on_rmb = false;
	}
	namespace visuals
	{
		int player_type = 0;
		int box_type = 0;
		int statusbar_way = 0;
		float col_teammate[3];
		float col_enemy[3];
		bool health = false;
		bool glow = false;
		bool distance = false;
	}

	namespace font
	{
		int style = 0;
		float size = 0;
	}

	namespace extra
	{
		bool debug = false;
	}

	void load_default_settings() {
		global::enable = true;


		aimbot::enable = false;
		aimbot::only_enemy = true;
		aimbot::fov = 7.5f;
		aimbot::downoffset = 0.f; 
		aimbot::transverseoffset = 0.f;
		aimbot::aiming_on_rmb = true;

		visuals::player_type = 0;
		visuals::box_type = 4;
		visuals::statusbar_way = 0;
		visuals::col_enemy[0] = 1.f;
		visuals::col_enemy[1] = 1.f;
		visuals::col_enemy[2] = 0.f;

		visuals::col_teammate[0] = 0.f;
		visuals::col_teammate[1] = 1.f;
		visuals::col_teammate[2] = 1.f;
		visuals::health = true;
		visuals::glow = true;
		visuals::distance = true;


		font::style = 1;
		extra::debug = true;
	}
}
