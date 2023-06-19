#pragma once

#include "TGenCanvas.h"
#include "TGenGUI.h"
#include "TownMap.h"

class TGenControl {
private:
	TownMap* townMap;
	TGenGUI gui;
	TGenCanvas canvas;

	int screenHeight;
	int screenWidth;
	int guiWidth;

	Color guiColor = { 0x8e, 0xa8, 0xc3, 0xff };


public:
	TGenControl(TownMap* townMap_, int screenHeigth_, int screenWidth_, int guiWidth_);
	void drawMap();
	void resize(int screenHeigth_, int screenWidth_);
};