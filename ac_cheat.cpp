#include "stdafx.h"
#include "hook.h"
#include "glDraw.h"
#include "Hack.h"

uintptr_t gamebase;
uintptr_t* playerBase;
uintptr_t startEntList;
EntityList entList{ 0, 0 };
Entity* playerEnt;
uintptr_t dwViewMatrix;
uintptr_t* oViewMatrix;
int* entCount;
bool isHooked = false;
HANDLE hConsole;
Hack esp{ 0,0,0 };
float WindowWidth = 0;
float WindowHeight = 0;
float Radius = 90;
enum { GUN_KNIFE = 0, GUN_PISTOL, GUN_CARBINE, GUN_SHOTGUN, GUN_SUBGUN, GUN_SNIPER, GUN_ASSAULT, GUN_GRENADE, GUN_AKIMBO, NUMGUNS };

void CreateConsole() {
	AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

HWND window;
BOOL CALLBACK enumWind(HWND handle, LPARAM lp) {
	DWORD procId;
	GetWindowThreadProcessId(handle, &procId);

	if (GetCurrentProcessId() != procId)
		return true;
	window = handle;
	return false;
}

HWND GetProccessWindow() {
	window = NULL;
	EnumWindows(enumWind, NULL);
	if (window == NULL)
		return NULL;

	RECT size;
	GetWindowRect(window, &size);

	WindowWidth = size.right - size.left;
	WindowHeight = size.bottom - size.top;

	return window;
}

void CheatMenu() {
	system("cls");
	if (isHooked)
		std::cout << "Hooked" << std::endl;
	for (int i = 1; i < *entCount; i++) {
		Entity* curEnt = *entList.entList[i];
		//std::cout << "ent[" << i << "].health= " << curEnt->health << std::endl;
	} 
}

void Draw() {
	HDC hdc = wglGetCurrentDC();
	GL::SetupOrtho();
	GL::DrawRect(30, 30, 200, 200, rgb::aqua);
	GL::DrawOutLine(40, 40, 100, 100, 2, rgb::lime);
	GL::RestoreGL();
}

typedef BOOL(__stdcall* twglSwapBuffers) (HDC hdc);
twglSwapBuffers owglSwapBuffers;

BOOL hkwglSwapBuffers(HDC hdc) {
	esp.Update();
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	for (int i = 1; i < entList.entCount; i++) {
		Entity* curEnt = *entList.entList[i];
		Vec3 center = curEnt->posHead;
		center.z = center.z - 4.5f + 5.2f / 2;
		Vec2 entPos2D;
		GL::SetupOrtho();
		esp.DrawAimCircle(Radius, 10);
		if (esp.WorldToScreen(center, entPos2D)) {
			if (!esp.EntIsDead(curEnt)) {
				if (esp.IsEnemy(playerEnt, curEnt)) {
					esp.DrawEspBox(entPos2D, playerEnt->posHead, curEnt->posHead);
					if (esp.EnemyInCircle(curEnt, Radius)) {
						GL::DrawOutLine(entPos2D.x, entPos2D.y, viewport[2] / 2, viewport[3], 1, rgb::green);
						if (GetAsyncKeyState(VK_LBUTTON)) {
							esp.AimAt(playerEnt, curEnt);
						}
					}
					else {
						GL::DrawOutLine(entPos2D.x, entPos2D.y, viewport[2] / 2, viewport[3], 1, rgb::red);
					}
				}
			}
		}
		GL::RestoreGL();
	}
	//Entity* closestEnemy = esp.GetClosestEnemy(playerEnt, entList);
	/*Entity target;
	if (esp.GetEnemyInCircle(entList, target, Radius)) {
		if (GetAsyncKeyState(VK_LBUTTON))
			esp.AimAt(playerEnt, &target);
	}
	*/
	
	return owglSwapBuffers(hdc);
}

void WINAPI onDllDetach() {
	fclose((FILE*)stdin);
	fclose((FILE*)stdout);
	FreeConsole();
}

DWORD WINAPI HackThread(PVOID base) {
	CreateConsole();
	GetProccessWindow();
	std::cout << "[Injected]" << std::endl;
	gamebase = (uintptr_t)GetModuleHandle("ac_client.exe");
	std::cout << gamebase << std::endl; 
	uintptr_t* oPlayerBase = (uintptr_t*)(gamebase + 0x0018AC00);
	playerEnt = (Entity*)(*oPlayerBase);
	int* health = (int*)(&playerEnt->Health);
	
	startEntList = *(uintptr_t*)(gamebase + 0x18AC04);
	entCount = (int*)(gamebase + 0x18AC0C);
	entList = EntityList(startEntList, *entCount);
	//dwViewMatrix = 0x17DFDC;
	//dwViewMatrix = 0x192068;
	dwViewMatrix = 0x17DFCC;
	dwViewMatrix = 0x17DFD0;
	oViewMatrix = (uintptr_t*)(gamebase + dwViewMatrix);
	esp = Hack(oViewMatrix, WindowWidth, WindowHeight);

	Hook wglSwapBuffers("opengl32.dll", "wglSwapBuffers", (BYTE*)hkwglSwapBuffers, (BYTE*)&owglSwapBuffers, 5);
	bool stop = false;
	while (!stop) {


		if (GetAsyncKeyState(VK_DELETE) & 1) { 
			stop = true;
			wglSwapBuffers.Disable();
			isHooked = false;
		}

		if (GetAsyncKeyState(VK_F1) & 1) {
			wglSwapBuffers.Enable();
			esp.SetNoRecoil(playerEnt);
			isHooked = true;
		}

		if (GetAsyncKeyState(VK_F2) & 1) {
			wglSwapBuffers.Disable();
			esp.RestoreRecoil(playerEnt);
			isHooked = false;
		}

		if (GetAsyncKeyState(VK_F3) & 1) {
			playerEnt->Health = 4444;
			playerEnt->Armor = 4444;
			playerEnt->MagAmmo[GUN_ASSAULT] = 4444;
			
		}

		CheatMenu();
	}
	
	FreeLibraryAndExitThread(static_cast<HMODULE>(base), 0);
	return 0;
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  
    DWORD fdwReason,    
    LPVOID lpvReserved) 
{
    switch (fdwReason)
    {
		case DLL_PROCESS_ATTACH:
			CreateThread(0, 0, HackThread, 0, 0, 0);
			break;

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			break;

		case DLL_PROCESS_DETACH:

			onDllDetach();

			break;
    }
    return TRUE; 
}