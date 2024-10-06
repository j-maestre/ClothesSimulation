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
    camera.target = { 0.0f, 5.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 90.0f;
    camera.projection = CameraProjection::CAMERA_PERSPECTIVE;


    Vector3 cubePosition = { 0.0f, 0.0f, 10.0f };
    float cubeSize = 2.0f;

    SetTargetFPS(100);

    Color black = { 0,0,0,0 };
    Color red = {255, 10, 10, 255};


    JE::RopeRaylib rope(0.25f, 100);
    JE::RopeRaylib rope2(0.25f, 100);
    JE::RopeRaylib rope3(0.25f, 100);

    rope.InitRope(JE::Vec3{ 0.0f, 5.0f, 0.0f }, JE::Vec3{ 0.0f, 0.0f, 0.0f }, 1.0f);
    rope2.InitRope(JE::Vec3{ 0.0f, 5.0f, 0.0f }, JE::Vec3{ 0.0f, 0.0f, 0.0f }, 3.0f);

    float rope_3_x_offset = 5.0f;
    rope3.InitRope(JE::Vec3{ rope_3_x_offset , 5.0f, 0.0f }, JE::Vec3{ rope_3_x_offset , 0.0f, 0.0f }, 3.0f);
    printf("Ropes created correctly!\n");

    rope3.SetFixed(99);

    rope.SetColor(BLUE);
    rope2.SetColor(RED);
    rope3.SetColor(GREEN);

    //printf("Lenght-> %f\n Num particles %d", rope.m_lenght, rope.m_numParticles);

    const float speed = 2.0f;

    while (!WindowShouldClose()) {

        UpdateCamera(&camera, CAMERA_FIRST_PERSON);


        BeginDrawing();
        ClearBackground(black);
        BeginMode3D(camera);

        float x, y, z;
        rope.GetPosition(0,x,y,z);
        rope.SetPointPosition(0,cosf(GetTime() * speed) * 2.0f,y,z);
        
        rope2.GetPosition(0,x,y,z);
        rope2.SetPointPosition(0,cosf(GetTime() * speed) * 2.0f,y,z);
        
        rope3.GetPosition(0,x,y,z);
        rope3.SetPointPosition(0, rope_3_x_offset + cosf(GetTime() * speed) * 2.0f,y,z);


        //if (IsKeyDown(KEY_SPACE)) {
            rope.Update(GetFrameTime());
            rope2.Update(GetFrameTime());
            rope3.Update(GetFrameTime());
        //}
            rope.DrawRope();
            rope2.DrawRope();
            rope3.DrawRope();
            


        //DrawSphere(Vector3{0.0f, 5.0f, 0.0f}, 0.5f, red);

        DrawCube(cubePosition, cubeSize, cubeSize, cubeSize, WHITE);
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
