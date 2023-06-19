#include "TGenControl.h"


TGenControl::TGenControl(TownMap* townMap_, int screenHeigth_, int screenWidth_, int guiWidth_) {
	townMap = townMap_;
	screenHeight = screenHeigth_;
	screenWidth = screenWidth_;
	guiWidth = guiWidth_;

	gui = TGenGUI(townMap_, guiWidth, screenHeight, screenWidth, guiColor);
	canvas = TGenCanvas(townMap_);
}

void TGenControl::resize(int screenHeigth_, int screenWidth_) {
	screenHeight = screenHeigth_;
	screenWidth = screenWidth_;
	townMap->resizeCanvas(screenWidth - guiWidth, screenHeight);
	gui.resize(screenHeight, screenWidth);
	
}

void TGenControl::drawMap() {
	
	switch (townMap->options.state)
	{
	case S_DEFAULT:
		canvas.drawDefault();
		if (townMap->options.debug) {
			canvas.drawDebug();
		}
		gui.drawDefault();
		break;
	case S_EDIT_MODE:
		canvas.drawEdit();
		if (townMap->options.debug) {
			canvas.drawDebug();
		}
		gui.drawEdit();
		break;
	case S_MANUAL_INPUT:
		canvas.drawManual();
		if (townMap->options.debug) {
			canvas.drawDebug();
		}
		gui.drawManual();
		break;
	default:
		break;
	}

	
}