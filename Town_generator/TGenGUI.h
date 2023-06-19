#pragma once
#include"TownMap.h"
#include <raygui.h>


class TGenGUI {
private:
	TownMap* TMap = nullptr;

	Color GUI_color = { 0,0,0,0 };

	int gui_width = -1;
	int screen_height = -1;
	int screen_width = -1;

	
	//zmienne pomocnicze:
	bool valueBoxPlotCountEditMode = false;
	bool valueBoxSeedEditMode = false;

public:
	TGenGUI();
	TGenGUI(TownMap* TMap_, int gui_width_, int screen_height_, int screen_width_, Color GUI_color_);
	void drawGUI();
	void drawDefault();
	void drawEdit();
	void drawManual();
	void resize(int screen_height_, int screen_width_);

private:
	void drawGUIbackground();

	void drawGenerateButton(bool enabled);
	void drawManualModeButton(bool enabled);
	void drawEditMapButton(bool enabled);
	void drawRefreshButton(bool enabled);

	void drawDebugToggle(bool enabled);
	void drawBasicControls(bool enabled);


};
