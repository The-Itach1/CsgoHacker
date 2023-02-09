#pragma once

namespace offsets {
    constexpr auto local_player = 0xDEA964;
    constexpr auto entity_list = 0x4DFFF14;
    constexpr auto view_matrix = 0x4DF0D44;
    constexpr auto dwClientState = 0x59F194;
    constexpr auto dwGlowObjectManager = 0x535A9D8;

    constexpr auto m_iFov = 0x31F4;
    constexpr auto m_iGlowIndex = 0x10488;
    constexpr auto bone_matrix = 0x26A8;
    constexpr auto team_num = 0xF4;
    constexpr auto life_state = 0x25F;
    constexpr auto origin = 0x138;
    constexpr auto dormant = 0xED;
    constexpr auto Health = 0x100;
    constexpr auto Crosshair = 0x11838;
    constexpr auto ForceAttack = 0x322AC7C;
    constexpr auto Scoped_1 = 0x9974;
    constexpr auto m_zoomLevel = 0x33E0;
    constexpr auto MyWeapons = 0x2E08;
    constexpr auto m_iItemDefinitionIndex = 0x2FBA;
    constexpr auto ActiveWeapon = 0x2F08;
    constexpr auto Flags = 0x104;
    constexpr auto Jump = 0x52B8BFC;
    constexpr auto dwClientState_ViewAngles = 0x4D90;
    constexpr auto m_aimPunchAngle = 0x303C;
}

/* Check if the ptr parameter is valid */
#define IsValidPtr(x) ( x != NULL && (DWORD_PTR)x >= 0x10000 && (DWORD_PTR)x < 0x00007FFFFFFEFFFF)

class CViewMatrix {
public:
    ViewMatrix view_matrix;
};

class BoneMatrix {
public:
    char pad_00C[0xC];
    float waist_x;
    char pad_01C[0xC];
    float waist_y;
    char pad_02C[0xC];
    float waist_z;

    char pad_0F0[0xC0];
    char pad_0FC[0xC];
    float stomach_x;
    char pad_10C[0xC];
    float stomach_y;
    char pad_11C[0xC];
    float stomach_z;

    char pad_12C[0xC];
    float chest_x;
    char pad_13C[0xC];
    float chest_y;
    char pad_14C[0xC];
    float chest_z;

    char pad_15C[0xC];
    float neck_x;
    char pad_16C[0xC];
    float neck_y;
    char pad_17C[0xC];
    float neck_z;

    char pad_18C[0xC];
    float head_x;
    char pad_19C[0xC];
    float head_y;
    char pad_1AC[0xC];
    float head_z;
    
};


struct GlowStruct
{
    char pad_0000[8]; //0x0000
    float Red; //0x0008
    float Green; //0x000C
    float Blue; //0x0010
    float Alpha; //0x0014
    char pad_0018[16]; //0x0018
    bool renderOccluded; //0x0028
    bool renderUnoccluded; //0x0029
};


class ClientPlayer {
public:
	char pad_00F4[0xF4]; 
	unsigned int teamId;
	char pad_00100[0x8];
	unsigned int Health;
    char pad_00138[0x34];
    Vector loction;
    char pad_0025C[0x118];
    unsigned int lifestate;
    char pad_026A8[0x2448];
    BoneMatrix* bone_matrix;

    bool IsValid() {
        return (this->Health > 1 && this->Health <= 100);
    }

    bool IsDead() {
        return !(this->Health > 1);
    }
    Vector GetHeadOrigin()
    {
        return Vector(bone_matrix->head_x, bone_matrix->head_y, bone_matrix->head_z);
    }
    Vector GetNeckOrigin()
    {
        return Vector(bone_matrix->neck_x, bone_matrix->neck_y, bone_matrix->neck_z);
    }
    Vector GetChestOrigin()
    {
        return Vector(bone_matrix->chest_x, bone_matrix->chest_y, bone_matrix->chest_z);
    }
    Vector GetStomachOrigin()
    {
        return Vector(bone_matrix->stomach_x, bone_matrix->stomach_y, bone_matrix->stomach_z);
    }
    Vector GetWaistOrigin()
    {
        return Vector(bone_matrix->waist_x, bone_matrix->waist_y, bone_matrix->waist_z);
    }
    
};

