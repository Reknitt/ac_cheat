#include "Hack.h"
#include "glDraw.h"
#include "mem.h"

bool Hack::WorldToScreen(Vec3 pos, Vec2& screen) {
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	Vec4 clipCoords;
	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[4] + pos.z * viewMatrix[8] + viewMatrix[12];
	clipCoords.y = pos.x * viewMatrix[1] + pos.y * viewMatrix[5] + pos.z * viewMatrix[9] + viewMatrix[13];
	clipCoords.z = pos.x * viewMatrix[2] + pos.y * viewMatrix[6] + pos.z * viewMatrix[10] + viewMatrix[14];
	clipCoords.w = pos.x * viewMatrix[3] + pos.y * viewMatrix[7] + pos.z * viewMatrix[11] + viewMatrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (viewport[2] / 2 * NDC.x) + (NDC.x + viewport[2] / 2);
	screen.y = -(viewport[3] / 2 * NDC.y) + (NDC.y + viewport[3] / 2);
	return true;
}

void Hack::Update() {
	memcpy(&viewMatrix, oViewMatrix, sizeof(viewMatrix));
}

Hack::Hack(uintptr_t* oViewMatrix, float WindowWidth, float WindowHeight) {
	this->oViewMatrix = oViewMatrix;
	this->WindowWidth = WindowWidth;
	this->WindowHeight = WindowHeight;
}

EntityList::EntityList(uintptr_t startEntList, int entCount) {
	this->startEntList = startEntList;
	this->entCount = entCount;

	for (int i = 0; i < entCount; i++) {
		entList[i] = (Entity**)(startEntList + 4 * i);
	}
}

float Hack::VectorDistance(Vec3 a, Vec3 b) {
	return sqrtf(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
}

bool Hack::IsEnemy(Entity* player, Entity* ent)
{
	if (player->Team == ent->Team)
		return false;
	else return true;
}

void Hack::DrawEspBox(Vec2 screenPos, Vec3 player, Vec3 bot) {
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	float dist = Hack::VectorDistance(player, bot);
	float entWidth = 2.0f;
	float entHeight = 5.2f;
	float width = (entWidth * viewport[2]) / (dist * 3);
	float height = (entHeight * viewport[3]) / (dist * 3);
	GL::DrawBox(screenPos.x, screenPos.y, width, height, 2, rgb::white);
}

float PI = 3.14159265358;
void Hack::AimAt(Entity* player, Entity* target) {
	Vec3 pl = player->posHead;
	Vec3 tar = target->posHead;
	Vec3 deltaVec;  
	deltaVec.x = tar.x - pl.x;
	deltaVec.y = tar.y - pl.y;
	deltaVec.z = tar.z - pl.z;
	float deltaLength = sqrtf(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);
	float pitch = asinf(deltaVec.z / deltaLength) * (180 / PI);
	float old_yaw = atan2f(deltaVec.y, deltaVec.x) * (180 / PI);
	float yaw = old_yaw + 90;
	player->angle.x = yaw;
	player->angle.y = pitch;

}


Entity* Hack::GetClosestEnemy(Entity* player, EntityList entList)
{
	Entity* curEnt = *entList.entList[1];
	Entity* closestEnemy = curEnt;
	Vec2 screen;
	float distance = VectorDistance(player->posHead, curEnt->posHead);
	for (int i = 2; i < entList.entCount; i++) {
		if (Hack::WorldToScreen(curEnt->posHead, screen)) {
			curEnt = *entList.entList[i];
			float newDist = VectorDistance(player->posHead, curEnt->posHead);
			if (distance > newDist) {
				distance = newDist;
				closestEnemy = curEnt;
			}
		}
	}
	return closestEnemy;
}

bool Hack::GetEnemyInCircle(EntityList entList, Entity& target, float radius)
{
	int viewport[4];
	Vec2 center;
	glGetIntegerv(GL_VIEWPORT, viewport);
	center.x = viewport[2] / 2;
	center.y = viewport[3] / 2;
	Entity* curEnt = *entList.entList[1];
	Vec2 pos2D;
	for (int i = 2; i < entList.entCount; i++) {
		Hack::WorldToScreen(curEnt->posHead, pos2D);
		float point = sqrtf(powf(pos2D.x - center.x, 2) + powf(pos2D.y - center.y, 2));
		if (point <= radius) {
			target = *curEnt;
			return true;
		}
		curEnt = *entList.entList[i];
	}
	return false;
}

void Hack::DrawAimCircle(float radius, float thickness)
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GL::DrawCircle(viewport[2] / 2, viewport[3] / 2, radius, thickness, rgb::purple);
}

bool Hack::EnemyInCircle(Entity* ent, float radius)
{
	int viewport[4];
	Vec2 center;
	glGetIntegerv(GL_VIEWPORT, viewport);
	center.x = viewport[2] / 2;
	center.y = viewport[3] / 2;
	Vec2 pos2D;
	Hack::WorldToScreen(ent->posHead, pos2D);
	float point = sqrtf(powf(pos2D.x - center.x, 2) + powf(pos2D.y - center.y, 2));
	if (point <= radius) {
		return true;
	}
	else return false;
}

bool Hack::EntIsDead(Entity* ent)
{
	if ((ent->Health <= 100) && (ent->Health >= 0))
		return false;
	else return true;
}

void Hack::SetNoRecoil(Entity* player) {
	uintptr_t* oVTableRecoil;
	uintptr_t* bPlayer = (uintptr_t*)player;
	uintptr_t* offset1 = (uintptr_t*)*(bPlayer + 0x364 / sizeof(uintptr_t*));
	uintptr_t* offset2 = (uintptr_t*)*(offset1 + 0x0);
	uintptr_t* offset3 = (uintptr_t*)*(offset2 + 0x14 / sizeof(uintptr_t*));
	// ret 8 opcode
	DWORD curProtection;
	oVTableRecoil = offset3;
	rsize_t size = sizeof(uintptr_t*);
	VirtualProtect(oVTableRecoil, size, PAGE_EXECUTE_READWRITE, &curProtection);
	memcpy_s(originalBytes, size, oVTableRecoil, size);
	*(oVTableRecoil) = 0x000008C2;
	VirtualProtect(oVTableRecoil, size, curProtection, &curProtection);
} 

void Hack::RestoreRecoil(Entity* player) {
	uintptr_t* oVTableRecoil;
	uintptr_t* bPlayer = (uintptr_t*)player;
	uintptr_t* offset1 = (uintptr_t*)*(bPlayer + 0x364 / sizeof(uintptr_t*));
	uintptr_t* offset2 = (uintptr_t*)*(offset1 + 0x0);
	uintptr_t* offset3 = (uintptr_t*)*(offset2 + 0x14 / sizeof(uintptr_t*));
	DWORD curProtection;
	oVTableRecoil = offset3;
	rsize_t size = sizeof(uintptr_t*);
	VirtualProtect(oVTableRecoil, size, PAGE_EXECUTE_READWRITE, &curProtection);
	memcpy_s(oVTableRecoil, size, originalBytes, size);
	VirtualProtect(oVTableRecoil, size, curProtection, &curProtection);
}
