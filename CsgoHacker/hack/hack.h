enum BONE
{
	HEAD,		//Í·
	NECK,		//²±×Ó
	CHEST,		//ÐØ
	STOMACH,	//Î¸
	WAIST,		//Ñü
	NONE
};

enum LINE_STATUS_BAR
{
	LEFT,
	RIGHT,
	UPPER,
	BOTTOM
};

struct Vector {
	Vector() noexcept
		: x(), y(), z() {}

	Vector(float x, float y, float z) noexcept
		: x(x), y(y), z(z) {}

	Vector(float* p)
	{
		*this = p;
	};

	Vector(const Vector& in)
	{
		x = in.x;
		y = in.y;
		z = in.z;
	};

	~Vector()
	{

	}

	Vector& operator+(const Vector& v) noexcept {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector& operator-(const Vector& v) noexcept {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}



	operator float* () { return &x; }

	inline float LengthSqr(void)
	{
		return x * x + y * y + z * z;
	}

	inline float Length(void)
	{
		float Length = 0;

		Length = sqrt(LengthSqr());

		return Length;
	}

	inline float Distance(Vector& ToVector)
	{
		return (ToVector - *this).Length();
	}


	float x, y, z;
};

struct extra_players_data_s
{
	int m_Index;
	int m_iTeam;
	int m_flPlayerHealth;
	bool m_IsVisible;
	int m_lifeState;
	int m_glowIndex;
	Vector m_vOrigin;
	Vector m_vBoneOrigin[6];
};



//class Matrix4x4
//{
//public:
//	union
//	{
//		struct
//		{
//			float        _11, _12, _13, _14;
//			float        _21, _22, _23, _24;
//			float        _31, _32, _33, _34;
//			float        _41, _42, _43, _44;
//		};
//		float m[4][4];
//	};
//};
struct ViewMatrix {
	ViewMatrix() noexcept
		: m() {}

	float* operator[](int index) noexcept {
		return m[index];
	}

	const float* operator[](int index) const noexcept {
		return m[index];
	}

	float m[4][4];
};

class CHack
{
public:
	HANDLE m_hprocess = NULL;
	HANDLE m_hclient=NULL;
	HANDLE m_hengine= NULL;


	bool GetHandle();
	//Matrix4x4* GetViewMatrix();

	void Run();

	bool UpdateLocalPlayer();

	void UpdatePlayers();

	void ClearPlayersData();

	void Debug();

	void ScreenInfo();

	void PlayerESP();

	void GlowESP();

	void Aimbot();


	void DrawBox(float x, float y, float w, float h, const ImColor col);

	void DrawStatusLine(float x, float y, float w, float h, float status_value, float max_of_status_value, ImColor col, LINE_STATUS_BAR status_side = LINE_STATUS_BAR::LEFT);

	void DrawDistance(float x, float y, float w, float h, float distance);
	void Draw3DCircle(Vector vOrigin, ImColor col);

	ImColor PlayerColor(int iTeam);

	
private:
	
	extra_players_data_s g_Local;
	std::deque<extra_players_data_s>g_Players;
};
extern std::unique_ptr<CHack>m_pHack;