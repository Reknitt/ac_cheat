#include "stdafx.h" 

struct Vec2 {
	float x, y;
};

struct Vec3 {
	float x, y, z;
};

struct Vec4 {
	float x, y, z, w;
};

struct Entity 
{
	void* vTable; //0x0000
	Vec3 posHead; //0x0004
	Vec3 velocity; //0x0010
	Vec3 deltapos; //0x001C
	Vec3 newPos; //0x0028
	Vec3 angle; //0x0034
	char pad_0040[16]; //0x0040
	float eyeheight; //0x0050
	float maxEyeHeight; //0x0054
	float aboveEye; //0x0058
	char pad_005C[144]; //0x005C
	uint32_t Health; //0x00EC
	uint32_t Armor; //0x00F0
	int32_t N0000008A; //0x00F4
	int32_t N0000008B; //0x00F8
	int32_t N0000008C; //0x00FC
	bool N0000008D; //0x0100
	char pad_0101[3]; //0x0101
	int32_t Ammo[9]; //0x0104
	int32_t MagAmmo[9]; //0x0128
	int32_t ReloadingDelay[9]; //0x014C
	int32_t StateShots[9]; //0x0170
	int32_t StateDamage[9]; //0x0194
	int32_t CurSkin; //0x01B8
	int32_t nextSkin[2]; //0x01BC
	int32_t ClientNum; //0x01C4
	char pad_01C8[20]; //0x01C8
	int32_t Frags; //0x01DC
	int32_t FlagsScore; //0x01E0
	int32_t Deaths; //0x01E4
	int32_t Tks; //0x01E8
	char pad_01EC[24]; //0x01EC
	uint8_t N000001D8; //0x0204
	char Name[16]; //0x0205
	int32_t trash; //0x0215
	char pad_0219[239]; //0x0219
	int32_t Team; //0x030C
	char pad_0310[49]; //0x0310
}; //Size: 0x0341

struct Entity_old2
{
	void* vTable; //0x0000
	Vec3 posHead; //0x0004
	Vec3 velocity; //0x0010
	Vec3 deltapos; //0x001C
	Vec3 newPos; //0x0028
	Vec3 angle; //0x0034
	char pad_0040[16]; //0x0040
	float eyeheight; //0x0050
	float maxEyeHeight; //0x0054
	float aboveEye; //0x0058
	char pad_005C[119]; //0x005C
	int16_t N000000E9; //0x00D3
	char pad_00D5[23]; //0x00D5
	int32_t Health; //0x00EC
	int32_t Armor; //0x00F0
	int32_t N0000008A; //0x00F4
	int32_t N0000008B; //0x00F8
	int32_t N0000008C; //0x00FC
	bool N0000008D; //0x0100
	char pad_0101[3]; //0x0101
	int32_t Ammo[9]; //0x0104
	int32_t MagAmmo[9]; //0x0128
	int32_t ReloadingDelay[9]; //0x014C
	int32_t StateShots[9]; //0x0170
	int32_t StateDamage[9]; //0x0194
	int32_t CurSkin; //0x01B8
	int32_t nextSkin[2]; //0x01BC
	int32_t ClientNum; //0x01C4
	char pad_01C8[20]; //0x01C8
	int32_t Frags; //0x01DC
	int32_t FlagsScore; //0x01E0
	int32_t Deaths; //0x01E4
	int32_t Tks; //0x01E8
	char pad_01EC[24]; //0x01EC
	uint8_t N000001D8; //0x0204
	char Name[16]; //0x0205
	int32_t team; //0x0215
	char pad_0219[38]; //0x0219
};

struct EntityList {
	/*Переделать двойной указатель на обычный*/
	Entity** entList[32]{ 0 };
	int entCount{ 0 };
	uintptr_t startEntList{ 0 };
	EntityList(uintptr_t startEntList, int entCount);
};

struct Hack {
	float WindowWidth, WindowHeight;
	float viewMatrix[16]{ 0 };
	byte originalBytes[10];
	uintptr_t* oViewMatrix;
	Hack(uintptr_t* oViewMatrix, float WindowWidth, float WindowHeight);
	void Update();
	bool WorldToScreen(Vec3 pos, Vec2& screen);
	void DrawEspBox(Vec2 screenPos, Vec3 player, Vec3 bot);
	float static VectorDistance(Vec3 a, Vec3 b);
	bool IsEnemy(Entity* player, Entity* ent);
	Entity* GetClosestEnemy(Entity* player, EntityList entList);
	bool GetEnemyInCircle(EntityList entList, Entity& target, float radius);
	void DrawAimCircle(float radius, float thickness);
	void AimAt(Entity* player, Entity* target);
	bool EnemyInCircle(Entity* ent, float radius);
	bool EntIsDead(Entity* ent);
	void SetNoRecoil(Entity* player);
	void RestoreRecoil(Entity* player);
};

struct PlayerEnt {
	char padding1[0xEC];
	int health;
	char padding2[0x53];
	int ammo;
	char padding3[0x448];
};
/*Переделать структуры. Сделать, чтобы можно было иницилизировать новое поле по оффсету*/
struct Entity_old {
	int* pointer;
	float x;
	float y;
	float z;
	char padding1[0xE0];
	int health;
	char padding2[0x53];
	int ammo;
	char padding3[0x448];
};


