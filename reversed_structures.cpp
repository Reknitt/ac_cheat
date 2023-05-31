#include "stdafx.h"
#include "reversed_structures.h"


EntityList::EntityList(uintptr_t startEntList, int entCount) {
	this->startEntList = startEntList;
	this->entCount = entCount;

	for (int i = 0; i < entCount; i++) {
		entList[i] = (Entity**)(startEntList + 4 * i);
	}
}

