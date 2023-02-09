namespace vars
{
	extern bool bMenuOpen;
	namespace global
	{
		extern bool enable;
	}

	namespace aimbot
	{

		extern bool enable;
		extern bool only_enemy;
		extern float fov;
		extern float downoffset;
		extern float transverseoffset;
		extern int bone;
		extern bool aiming_on_rmb;

	}

	namespace visuals
	{
		extern int player_type;
		extern int box_type;
		extern int statusbar_way;
		extern float col_teammate[3];
		extern float col_enemy[3];
		extern bool health;
		extern bool glow;
		extern bool distance;

	}
	namespace font
	{
		extern int style;
		extern float size;
	}
	namespace extra
	{
		extern bool debug;
	}
	void load_default_settings();
}
