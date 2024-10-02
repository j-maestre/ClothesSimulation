#include <stdio.h>
#include <raylib.h>
#include "raymath.h"
#include "public/rope_raylib.h"


int main(int argc, char** argv){

    // Inicializar la ventana
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    //const int screenWidth = GetMonitorWidth(0);
    //const int screenHeight = GetMonitorHeight(0);
    InitWindow(screenWidth, screenHeight, "Hello Cube");
    //SetWindowState(FLAG_FULLSCREEN_MODE);
    //HideCursor();
    //ShowCursor();
    
    // Definir la c�mara
    Camera camera = { 0 };
    camera.position = { 0.0f, 5.0f, -10.0f };
    camera.target = { 0.0f, 1.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 90.0f;
    camera.projection = CameraProjection::CAMERA_PERSPECTIVE;


    Vector3 cubePosition = { 0.0f, 1.0f, 0.0f };
    float cubeSize = 2.0f;

    //SetTargetFPS(60);

    Color black = { 0,0,0,0 };


    RopeRaylib rope(5.0f, 10);
    rope.InitRope();
    printf("Rope created correctly!\n");

    printf("Lenght-> %f\n Num particles %d", rope.m_lenght, rope.m_numParticles);

    while (!WindowShouldClose()){

        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

     
        BeginDrawing();
        ClearBackground(black);

        BeginMode3D(camera);

        DrawCube(cubePosition, cubeSize, cubeSize, cubeSize, BLUE);
        DrawCubeWires(cubePosition, cubeSize, cubeSize, cubeSize, DARKGRAY);


        EndMode3D();
        
        //int fps = GetFPS();
        //DrawText("Hello Cube!", 10, 10, 20, DARKGRAY);
        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
