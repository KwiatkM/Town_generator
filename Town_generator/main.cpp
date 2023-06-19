#include <raylib.h>
#include <math.h>
#include <iostream>
#include <vector>

#define RAYGUI_IMPLEMENTATION
//#include "raygui.h" 

#include "TownMap.h"
#include "TGenGUI.h"
#include "TGenControl.h"


#define CANVAS_WIDTH 1000
#define CANVAS_HEIGHT 800

#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 800

//#define GUI_START           CANVAS_WIDTH
#define GUI_WIDTH           300
//#define GUI_BUTTONS_POS_X   GUI_START + 100


#define LOG(x) std::cout << x << '\n'

#define GUI_COLOR {0x8e, 0xa8, 0xc3, 0xff}
#define BCK_COLOR {0xdd, 0xb8, 0x8b, 0xff}
#define PLOT_COLOR {0xE3, 0xB2, 0x3C, 0xff}


int main(void)
{   
    //srand(time(NULL));
    bool debug = false;
    int n_of_plots = 50;
    int seed = -1;
    bool manual_mode = false;

    
    
    TownMap tMap(n_of_plots, CANVAS_WIDTH, CANVAS_HEIGHT);
    //tMap.generate(n_of_plots, seed);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Town generator");

    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();

    //TGenGUI gui(&tMap, GUI_WIDTH, screen_height, screen_width, GUI_COLOR);
    TGenControl controler(&tMap, screen_height, screen_width, GUI_WIDTH);

    while (!WindowShouldClose())
    {
        if (IsWindowResized()) {
            screen_width = GetScreenWidth();
            screen_height = GetScreenHeight();
            controler.resize(screen_height, screen_width);
        }

        BeginDrawing();
        ClearBackground(BCK_COLOR);

        controler.drawMap();



        EndDrawing();
        
    }
    
    CloseWindow();

    return 0;
}




