#include "TGenGUI.h"

TGenGUI::TGenGUI() {

}

TGenGUI::TGenGUI(TownMap* TMap_, int gui_width_, int screen_height_, int screen_width_, Color GUI_color_) {
	TMap = TMap_;
	gui_width = gui_width_;
	screen_height = screen_height_;
	screen_width = screen_width_;
	GUI_color = GUI_color_;

	GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
}

void TGenGUI::drawGUI() {
	DrawRectangle(screen_width - gui_width, 0, gui_width, screen_height, GUI_color);
	switch (TMap->options.state) {
	case S_DEFAULT:
		
		drawDebugToggle(true);
		drawBasicControls(true);
		drawEditMapButton(true);
		drawManualModeButton(true);
		drawGenerateButton(true);
		break;

	case S_MANUAL_INPUT:
		drawDebugToggle(true);
		drawBasicControls(false);
		drawEditMapButton(false);
		drawManualModeButton(true);
		drawGenerateButton(true);
		break;

	case S_EDIT_MODE:
		drawDebugToggle(true);
		//drawBasicControls(false);
		drawEditMapButton(true);
		drawManualModeButton(false);
		drawGenerateButton(false);
		drawRefreshButton(true);
		break;
	}

	
}



void TGenGUI::drawDefault() {
	drawGUIbackground();

	drawDebugToggle(true);
	drawBasicControls(true);
	drawEditMapButton(true);
	drawManualModeButton(true);
	drawGenerateButton(true);
}
void TGenGUI::drawEdit() {
	drawGUIbackground();

	drawDebugToggle(true);
	//drawBasicControls(false);
	drawEditMapButton(true);
	drawManualModeButton(false);
	drawGenerateButton(false);
	drawRefreshButton(true);
}
void TGenGUI::drawManual(){
	drawGUIbackground();

	drawDebugToggle(true);
	drawBasicControls(false);
	drawEditMapButton(false);
	drawManualModeButton(true);
	drawGenerateButton(true);
}

void TGenGUI::resize(int screen_height_, int screen_width_){
	screen_height = screen_height_;
	screen_width = screen_width_;
}




void TGenGUI::drawGUIbackground() {
	DrawRectangle(screen_width - gui_width, 0, gui_width, screen_height, GUI_color);
}

void TGenGUI::drawGenerateButton(bool enabled) {
	if (!enabled) GuiDisable();
	if (GuiButton({ (float)(screen_width - gui_width + 20), (float)(screen_height - 60), (float)(gui_width - 40), 40 }, "Generate")) {
		if (TMap->options.state == S_MANUAL_INPUT) TMap->generateFromExistingPoints();
		else TMap->generate();

		TMap->options.state = S_DEFAULT;
	}
	GuiEnable();
}

void TGenGUI::drawManualModeButton(bool enabled) {
	if (!enabled) GuiDisable();
	if (TMap->options.state == S_MANUAL_INPUT) {
		if (GuiButton({ (float)(screen_width - gui_width + 20), (float)(screen_height - 120), (float)(gui_width - 40), 40 }, "Exit manual mode")) {
			TMap->options.state = S_DEFAULT;
		}
	}
	else {
		if (GuiButton({ (float)(screen_width - gui_width + 20), (float)(screen_height - 120), (float)(gui_width - 40), 40 }, "Manual mode")) {
			TMap->options.state = S_MANUAL_INPUT;
			TMap->clear();
		}
	}
	
	GuiEnable();
}

void TGenGUI::drawEditMapButton(bool enabled) {
	if (!enabled) GuiDisable();
	if (TMap->options.state == S_EDIT_MODE) {
		if (GuiButton({ (float)(screen_width - gui_width + 20), (float)(screen_height - 180), (float)(gui_width - 40), 40 }, "Exit edit mode")) {
			TMap->options.state = S_DEFAULT;
		}
	}
	else {
		if (GuiButton({ (float)(screen_width - gui_width + 20), (float)(screen_height - 180), (float)(gui_width - 40), 40 }, "Edit map")) {
			TMap->options.state = S_EDIT_MODE;
		}
	}
	GuiEnable();
}

void TGenGUI::drawRefreshButton(bool enabled) {
	if (!enabled) GuiDisable();
	if (GuiButton({ (float)(screen_width - gui_width + 20), (float)(screen_height - 240), (float)(gui_width - 40), 40 }, "Refresh")) {
		//TODO
	}
	GuiEnable();
}



void TGenGUI::drawDebugToggle(bool enabled) {
	if (!enabled) GuiDisable();
	TMap->options.debug = GuiCheckBox({ (float)screen_width - gui_width + 100 ,20,40,40 }, "Debug", TMap->options.debug);
	GuiEnable();
}

void TGenGUI::drawBasicControls(bool enabled) {
	if (!enabled) GuiDisable();
	if(GuiValueBox({ (float)screen_width - gui_width + 100,80,(float)gui_width - 120,40 }, "Plot count", &(TMap->options.plot_number), 10, 150, valueBoxPlotCountEditMode))
		valueBoxPlotCountEditMode = !valueBoxPlotCountEditMode;
	if(GuiValueBox({ (float)screen_width - gui_width + 100,140,(float)gui_width - 120,40 }, "Seed", &(TMap->options.seed), INT32_MIN, INT32_MAX, valueBoxSeedEditMode))
		valueBoxSeedEditMode = !valueBoxSeedEditMode;
	TMap->options.coast = GuiCheckBox({ (float)screen_width - gui_width + 100 ,200,40,40 }, "Coast", TMap->options.coast);
	TMap->options.river = GuiCheckBox({ (float)screen_width - gui_width + 100 ,260,40,40 }, "River", TMap->options.river);
	TMap->options.town_square = GuiCheckBox({ (float)screen_width - gui_width + 100 ,320,40,40 }, "Town square", TMap->options.town_square);
	TMap->options.church = GuiCheckBox({ (float)screen_width - gui_width + 100 ,380,40,40 }, "Church", TMap->options.church);
	TMap->options.walls = GuiCheckBox({ (float)screen_width - gui_width + 100 ,440,40,40 }, "Walls", TMap->options.walls);
	GuiEnable();
}