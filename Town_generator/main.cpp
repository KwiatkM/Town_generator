#include <raylib.h>
#include <math.h>
#include <iostream>
#include <vector>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h" 

#include "TownMap.h"


#define CANVAS_WIDTH 1000
#define CANVAS_HEIGHT 800

#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 800

#define GUI_START           CANVAS_WIDTH
#define GUI_WIDTH           WINDOW_WIDTH - CANVAS_WIDTH
#define GUI_BUTTONS_POS_X   GUI_START + 100

#define LOG(x) std::cout << x << '\n'

#define GUI_COLOR {0x8e, 0xa8, 0xc3, 0xff}
#define BCK_COLOR LIGHTGRAY
#define PLOT_COLOR {0xE3, 0xB2, 0x3C, 0xff}


int main(void)
{   
    srand(time(NULL));
    bool debug = false;
    int n_of_plots = 50;
    int seed = -1;
    bool manual_mode = false;
    
    /*
    Image buf_img = {
        buf,
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        1,
        PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };
    UnloadImage(buf_img);*/

    TownMap tMap(n_of_plots, CANVAS_WIDTH, CANVAS_HEIGHT);
    tMap.generate(n_of_plots, seed);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Town generator");

    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BCK_COLOR);

        if (manual_mode) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if(GetMouseX() < CANVAS_WIDTH)
                    tMap.plot_centers.push_back( { GetMouseX() ,GetMouseY()});
            }
            for (int i = 0; i < tMap.plot_centers.size(); i++) {
                //DrawCircle(gr.GetPoint(i).x, gr.GetPoint(i).y, 4, BLACK);
                DrawCircle(tMap.plot_centers[i].x,
                    tMap.plot_centers[i].y,
                    4,
                    BLACK);
            }

        }else{
			for (int i = 0; i < tMap.plot_centers.size(); i++) { //wype�nianie dzia�ek 
				if (tMap.plots[i].type == VALID) {
					for (int j = 1; j < tMap.plots[i].adj_intersection_id.size(); j++) {
						for (int k = 2; k < tMap.plots[i].adj_intersection_id.size(); k++) {
							Vector2 tr1 = { (float)tMap.plots[i].actual_border[0].x, (float)tMap.plots[i].actual_border[0].y };
							Vector2 tr2 = { (float)tMap.plots[i].actual_border[j].x, (float)tMap.plots[i].actual_border[j].y };
							Vector2 tr3 = { (float)tMap.plots[i].actual_border[k].x, (float)tMap.plots[i].actual_border[k].y };

							DrawTriangle(tr1, tr2, tr3, PLOT_COLOR);
							DrawTriangle(tr1, tr3, tr2, PLOT_COLOR);
						}
					}
				}
			}


        if (debug) {

            //rysowanie punkt�w
            for (int i = 0; i < tMap.plot_centers.size(); i++) {
                //DrawCircle(gr.GetPoint(i).x, gr.GetPoint(i).y, 4, BLACK);
                DrawCircle(tMap.plot_centers[i].x,
                    tMap.plot_centers[i].y,
                    4,
                    BLACK);
                /*for (int j : tMap.plots[i].adj_intersection_id) {
                    DrawLine(tMap.plot_centers[i].x,
                        tMap.plot_centers[i].y,
                        tMap.intersections[j].x,
                        tMap.intersections[j].y,
                        BLACK);
                }*/
            }

            //rysowanie tr�jk�t�w
            for (Triangle& t : tMap.triangles) {
                DrawLine(tMap.plot_centers[t.p1].x, tMap.plot_centers[t.p1].y,
                    tMap.plot_centers[t.p2].x, tMap.plot_centers[t.p2].y,
                    BLACK);
                DrawLine(tMap.plot_centers[t.p2].x, tMap.plot_centers[t.p2].y,
                    tMap.plot_centers[t.p3].x, tMap.plot_centers[t.p3].y,
                    BLACK);
                DrawLine(tMap.plot_centers[t.p1].x, tMap.plot_centers[t.p1].y,
                    tMap.plot_centers[t.p3].x, tMap.plot_centers[t.p3].y,
                    BLACK);
                //rysowanie skrzy�owa�
                DrawCircle(t.circumcircle.center.x,
                    t.circumcircle.center.y,
                    4,
                    RED);
            }
            //rysowanie skrzy�owana� z 2 przyleg�ymi drogami
            for (int i = 0; i < tMap.intersections.size(); i++) {
                if (tMap.intersections[i].connected_intersections_id.size() < 3) {
                    DrawCircle(tMap.intersections[i].x,
                        tMap.intersections[i].y,
                        6,
                        ORANGE);
                }
            }
            //rysowanie dr�g
            /*for (int i = 0; i < tMap.roads.size(); i++) {
                DrawLine(tMap.intersections[tMap.roads[i].inter_id1].x,
                    tMap.intersections[tMap.roads[i].inter_id1].y,
                    tMap.intersections[tMap.roads[i].inter_id2].x,
                    tMap.intersections[tMap.roads[i].inter_id2].y,
                    BLUE);
            }*/
            for (int i = 0; i < tMap.intersections.size(); i++) {
                for (int j = 0; j < tMap.intersections[i].connected_intersections_id.size(); j++) {
                    
                    DrawLine(tMap.intersections[i].x,
                        tMap.intersections[i].y,
                        tMap.intersections[tMap.intersections[i].connected_intersections_id[j]].x,
                        tMap.intersections[tMap.intersections[i].connected_intersections_id[j]].y,
                        BLUE);
                    
                }
            }

            //rysowanie skrzy�owa� granicznych
            for (int i = 0; i < tMap.intersections.size(); i++) {
                if (tMap.intersections[i].type == ON_BORDER) {
                    DrawCircle(tMap.intersections[i].x,
                        tMap.intersections[i].y,
                        6,
                        GREEN);
                }
            }
            
        }
        }


        

        //---------------------------------------------------------------------------------------------
        //                                             GUI
        //---------------------------------------------------------------------------------------------


        DrawRectangle(GUI_START, 0, GUI_WIDTH, WINDOW_HEIGHT, GUI_COLOR);

        GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
        
        debug = GuiCheckBox({ GUI_BUTTONS_POS_X,50,40,40 }, "Debug", debug);
        GuiValueBox({ GUI_BUTTONS_POS_X,100,GUI_WIDTH - 120,40 }, "Plot count", &n_of_plots, 3, 100, true);
        

        // generowanie miasta
        if (GuiButton({ GUI_START + 20,WINDOW_HEIGHT - 50,GUI_WIDTH - 40, 40 }, "Generate")) {
            if (manual_mode) {
                tMap.generate();
                manual_mode = false;
            }
            else {
                tMap.generate(n_of_plots, seed);
                std::cout << "wygenerowano " << tMap.plot_centers.size() << " nowych punktow" << '\n';
            }
            
        }
        if (IsKeyPressed(KEY_ENTER)) {
            if (manual_mode) {
                tMap.generate();
                manual_mode = false;
            }
            else {
                tMap.generate(n_of_plots, seed);
                std::cout << "wygenerowano " << tMap.plot_centers.size() << " nowych punktow" << '\n';
            }
        }


        if (GuiButton({ GUI_START + 20,WINDOW_HEIGHT - 100,GUI_WIDTH - 40, 40 }, "Manual mode")) {
            manual_mode = true;
            tMap.plot_centers.clear();
            std::cout << "tryb manualny wlaczony" << '\n';
        }
        EndDrawing();
        
    }
    
    CloseWindow();

    return 0;
}



