#pragma once

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
struct Entity 
{
	void* N00000050; //0x0000
	Vec3 posHead; //0x0004
	char pad_0010[24]; //0x0010
	Vec3 posBody; //0x0028
	Vec3 angle; //0x0034
	char pad_0040[172]; //0x0040
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
	char pad_0215[3]; //0x0215
	int32_t Team; //0x0218
	char pad_021C[24]; //0x021C
}; //Size: 0x0234

struct EntityList {
	/*Переделать двойной указатель на обычный*/
	Entity** entList[32]{ 0 };
	int entCount{ 0 };
	uintptr_t startEntList{ 0 };
	EntityList(uintptr_t startEntList, int entCount);
};
