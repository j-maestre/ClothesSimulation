#include <stdio.h>
#include <raylib.h>
#include "raymath.h"
#include <rlImGui.h>
#include "imgui_impl_raylib.h"
#include "imgui.h"

#include "public/jamon_physics.h"
#include "public/rope_raylib.h"
#include "public/clothe_raylib.h"
#include "public/job_system.h"

#include <chrono>
#include <iostream>

#define MULTITHREAD
#define SHOW_ROPES
#define IMGUI_ENABLED

int main(int argc, char** argv){

    //Shader shader = LoadShader("vertex_shader","fragment_shader");


    std::mutex mtx;

    //JobSystem job_sys;


    // Inicializar la ventana
    const int screenWidth = 1600;
    const int screenHeight = 900;
    
    //const int screenWidth = GetMonitorWidth(0);
    //const int screenHeight = GetMonitorHeight(0);
#ifdef MULTITHREAD
    InitWindow(screenWidth, screenHeight, "Multi thread cloth");
#endif
 
#ifndef MULTITHREAD
    InitWindow(screenWidth, screenHeight, "Single thread cloth");
#endif

    //SetWindowState(FLAG_FULLSCREEN_MODE);
    //HideCursor();
    //ShowCursor();
    //SetConfigFlags(FLAG_VSYNC_HINT);
    //SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    int refresh_rate = GetMonitorPhysicalHeight(0);

    const char* monitor_name = GetMonitorName(0);
    printf("%s Refresh Rate: %d\n", monitor_name, refresh_rate);

    SetTraceLogLevel(LOG_NONE);

    //const char* glVersion =  GetGraphicsDevice().glVersion;
    //printf("Versión de OpenGL: %s\n", glVersion);

    
    Camera camera = { 0 };
    camera.position = { 0.0f, 5.0f, -10.0f };
    camera.target = { 0.0f, 5.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 90.0f;
    camera.projection = CameraProjection::CAMERA_PERSPECTIVE;


    Vector3 cubePosition = { 0.0f, 0.0f, 10.0f };
    float cubeSize = 2.0f;


    unsigned int rows = 64;
    unsigned int cols = 64;
    //SetTargetFPS(120);

    Color black = { 0,0,0,0 };
    Color red = {255, 10, 10, 255};


    JE::RopeRaylib rope(0.25f, 100);
    JE::RopeRaylib rope2(0.25f, 100);
    JE::RopeRaylib rope3(0.25f, 100);
    
    float rope1_friction = 0.1f;
    float rope2_friction = 0.05f;
    float rope3_friction = 1.0f;

    float rope1_mass = 1.0f;
    float rope2_mass = 3.0f;
    float rope3_mass = 3.0f;

    float ropes_offset = -10.0f;

    rope.InitRope(JE::Vec3{ ropes_offset, 5.0f, 0.0f }, JE::Vec3{ 0.0f, -5.0f, 0.0f }, 1.0f, rope1_friction);
    rope2.InitRope(JE::Vec3{ ropes_offset, 5.0f, 0.0f }, JE::Vec3{ 0.0f, -5.0f, 0.0f }, 3.0f, rope2_friction);

    float rope_3_x_offset = -15.0f;
    rope3.InitRope(JE::Vec3{ rope_3_x_offset , 5.0f, 0.0f }, JE::Vec3{ 0.0f , -5.0f, 0.0f }, 3.0f, rope3_friction);


    printf("Ropes created correctly!\n");

    rope.SetFixed(0);
    rope2.SetFixed(0);
    rope3.SetFixed(0);
    rope3.SetFixed(99);

    rope.SetColor(BLUE);
    rope2.SetColor(RED);
    rope3.SetColor(GREEN);


    float speed = 1.5f;
    float amplitude = 2.0f;

    //Vec3 position, Vec3 direction, float strength, float spread_angle, float max_distance, bool enabled = true
    //JE::WindTurbine turbine(JE::Vec3{ 0.1f, 0.1f, 0.1f }, JE::Vec3{1.0f, 1.0f, 0.1f}, 50.0f, 50.0f, 10.0f);

    rlImGuiSetup(true);

    JE::ClotheRaylib courtain1(rows, cols, 10.0f, 10.0f);
    JE::ClotheRaylib courtain2(rows, cols, 10.0f, 10.0f);

   
    courtain1.InitClothe(JE::Vec3{ 10.0f, 5.0f, 10.0f }, 10.0f, 0.1f);
    courtain2.InitClothe(JE::Vec3{ 20.0f, 5.0f, 10.0f }, 10.0f, 0.1f);


    courtain1.SetColor(BLUE);

    
    courtain1.SetFixed(0,0);
    courtain1.SetFixed(cols - 1,0);

    courtain2.SetFixed(0,0);
    courtain2.SetFixed(cols - 1,0);




    float x_offset = 0.0f, y_offset = 0.0f, z_offset = 0.0f;
    //cloth.GetPosition(0, 11, x_offset, y_offset, z_offset);
    
    bool opening = false;
    bool closing = false;
    float courtain1_desired_x = 12.5f;
    float courtain2_desired_x = 27.5f;

    float courtain1_original_x = 20.0f;
    float courtain2_original_x = 20.0f;
    float courtains_speed = 7.5f;


    const float smoothing_factor = 0.1f;
    

    //courtain1.SetShader("assets/fragment_shader.fs", "assets/vertex_shader.vs");
    //courtain1.SetShader("assets/fragment_shader.fs", "assets/vertex_shader.vs");
    //courtain1.SetTexture("assets/texture_0.png");
    
    //courtain2.SetShader("assets/fragment_shader.fs", "assets/vertex_shader.vs");
    //courtain2.SetTexture("assets/texture_0.png");
    

    std::string fps_counter = "FPS: \n";

    JE::JamonPhysics::get_instance()->add_clothe(&courtain1);
    JE::JamonPhysics::get_instance()->add_clothe(&courtain2);

    JE::JamonPhysics::get_instance()->add_rope(&rope);
    JE::JamonPhysics::get_instance()->add_rope(&rope2);
    JE::JamonPhysics::get_instance()->add_rope(&rope3);

    JE::JamonPhysics::get_instance()->init_clothe_thread();
    JE::JamonPhysics::get_instance()->init_rope_thread();

    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_LEFT_CONTROL)){
            UpdateCamera(&camera, CAMERA_FIRST_PERSON); 
        }


        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        float x, y, z;
#ifdef SHOW_ROPES
        rope.GetPosition(0,x,y,z);
        rope.SetPointPosition(0,cosf(GetTime() * speed) * 2.0f + ropes_offset,y,z);
        
        rope2.GetPosition(0,x,y,z);
        rope2.SetPointPosition(0,cosf(GetTime() * speed) * 2.0f + ropes_offset,y,z);
        
        rope3.GetPosition(0,x,y,z);
        rope3.SetPointPosition(0, rope_3_x_offset + cosf(GetTime() * speed) * 2.0f,y,z);
#endif

        // --- Wind turbine ---
        
        //DrawCube(Vector3{-5.0f, 0.0f ,0.0f}, 1.0f, 1.0f, 1.0f, WHITE);
        //DrawLine3D(Vector3{ -5.0f, 0.0f ,0.0f }, Vector3{ 10.0f, 10.0f ,0.0f }, WHITE);
        //rope.ApplyWindTurbine(turbine, GetFrameTime());

        // --------------------

        if (IsKeyPressed(KEY_SPACE)) {
            opening = true;
            closing = false;
        }
        if (IsKeyPressed(KEY_C)) {
            //printf("CLOSING\n");
            opening = false;
            closing = true;
        }


        if (opening) {
            courtain1.GetPosition(cols - 1,0,x,y,z);
            //x = Lerp(x,courtain1_desired_x, 0.1f);
            if (x >= courtain1_desired_x) x -= courtains_speed * GetFrameTime();
            //printf("Opening Courtain1 moving from posx: %f to %f\n",x, courtain1_desired_x);
            courtain1.SetPosition(cols - 1, 0,x,y,z); 

            courtain2.GetPosition(0,0,x,y,z);
            //x = Lerp(x,courtain2_desired_x, 0.1f);
            if (x <= courtain2_desired_x) x += courtains_speed * GetFrameTime();
            //printf("Opening Courtain2 posx: %f\n",x);
            courtain2.SetPosition(0,0,x,y,z);
        }

        if (closing) {
            courtain1.GetPosition(cols - 1, 0, x, y, z);
            //x = Lerp(x,courtain1_desired_x, 0.1f);
            if (x <= courtain1_original_x) x += courtains_speed * GetFrameTime();
            //printf("Closing Courtain1 from posx: %f to %f\n",x, courtain1_original_x);
            courtain1.SetPosition(cols - 1, 0, x, y, z);

            courtain2.GetPosition(0, 0, x, y, z);
            //x = Lerp(x,courtain2_desired_x, 0.1f);
            //printf("Closing Courtain2 posx: %f\n", x);
            if (x >= courtain2_original_x) x -= courtains_speed * GetFrameTime();
         
            courtain2.SetPosition(0, 0, x, y, z);
        }

        if (IsKeyPressedRepeat(KEY_J)) {
            courtain1.GetPosition(cols - 1, 0, x, y, z);
            courtain1.SetPosition(cols - 1, 0, x, y + 0.2f, z);
            
            courtain2.GetPosition(0, 0, x, y, z);
            courtain2.SetPosition(0, 0, x, y + 0.2f, z);
        }

        if (IsKeyPressedRepeat(KEY_K)) {
            courtain1.GetPosition(cols - 1, 0, x, y, z);
            courtain1.SetPosition(cols - 1, 0, x, y - 0.2f, z);

            courtain2.GetPosition(0, 0, x, y, z);
            courtain2.SetPosition(0, 0, x, y - 0.2f, z);
        }

        if (IsKeyPressed(KEY_H)) {
            courtain1.GetPosition(cols - 1, 0, x, y, z);
            courtain1.SetPosition(cols - 1, 0, x, y + 5.0f, z);
            courtain1.GetPosition(0, 0, x, y, z);
            courtain1.SetPosition(0, 0, x, y + 5.0f, z);

            courtain2.GetPosition(0, 0, x, y, z);
            courtain2.SetPosition(0, 0, x, y + 5.0f, z);
            courtain2.GetPosition(cols - 1, 0, x, y, z);
            courtain2.SetPosition(cols - 1, 0, x, y + 5.0f, z);
        }
        if (IsKeyPressed(KEY_G)) {
            courtain1.GetPosition(cols - 1, 0, x, y, z);
            courtain1.SetPosition(cols - 1, 0, x, y - 5.0f, z);
            courtain1.GetPosition(0, 0, x, y, z);
            courtain1.SetPosition(0, 0, x, y - 5.0f, z);

            courtain2.GetPosition(0, 0, x, y, z);
            courtain2.SetPosition(0, 0, x, y - 5.0f, z);
            courtain2.GetPosition(cols - 1, 0, x, y, z);
            courtain2.SetPosition(cols - 1, 0, x, y - 5.0f, z);
        }

#ifdef SHOW_ROPES
        rope.DrawRope();
        rope2.DrawRope();
        rope3.DrawRope();
#endif
            
        //courtain1.SetCamera(camera);
        //courtain2.SetCamera(camera);
        //courtain1.DrawClothe();
        //courtain2.DrawClothe();
        courtain1.DrawWireFrame();
        courtain2.DrawWireFrame();

        //DrawSphere(Vector3{0.0f, 5.0f, 0.0f}, 0.5f, red);

        DrawCube(cubePosition, cubeSize, cubeSize, cubeSize, WHITE);
        DrawCubeWires(cubePosition, cubeSize, cubeSize, cubeSize, DARKGRAY);


        EndMode3D();
#ifdef IMGUI_ENABLED

        rlImGuiBegin();

        ImGui::Text("Delta Time: %f", GetFrameTime());

        if (ImGui::CollapsingHeader("General values")) {
            ImGui::DragFloat("Speed", &speed, 0.01f, 0.0f, 10.0f);
            ImGui::DragFloat("Amplitude", &amplitude, 0.01f, 0.0f, 10.0f);
        }

        if (ImGui::CollapsingHeader("Rope 1 (Blue)")) {
            ImGui::DragFloat("Friction 1", &rope1_friction, 0.001f, 0.0f, 2.0f);
            ImGui::DragFloat("Mass 1", &rope1_mass, 0.01f, 0.0f, 10.0f);
            
            rope.SetAllFriction(rope1_friction);
            rope.SetAllMass(rope1_mass);
        }

        if (ImGui::CollapsingHeader("Rope 2 (Red)")) {
            ImGui::DragFloat("Friction 2", &rope2_friction, 0.001f, 0.0f, 2.0f);
            ImGui::DragFloat("Mass 2", &rope2_mass, 0.01f, 0.0f, 10.0f);
            rope2.SetAllFriction(rope2_friction);
            rope2.SetAllMass(rope2_mass);
        }

        if (ImGui::CollapsingHeader("Rope 3 (Green)")) {
            ImGui::DragFloat("Friction 3", &rope3_friction, 0.001f, 0.0f, 2.0f);
            ImGui::DragFloat("Mass 3", &rope3_mass, 0.01f, 0.0f, 10.0f);
            rope3.SetAllFriction(rope3_friction);
            rope3.SetAllMass(rope3_mass);
        }

        rlImGuiEnd();
#endif

        DrawFPS(10, 10);
        EndDrawing();
    }

    JE::JamonPhysics::get_instance()->close_threads();

    printf("%s", fps_counter.c_str());

    rlImGuiShutdown();
    CloseWindow();


    return 0;
}
