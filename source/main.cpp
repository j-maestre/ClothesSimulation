#include <stdio.h>
#include <raylib.h>
#include "raymath.h"
#include <rlImGui.h>
#include "imgui_impl_raylib.h"
#include "imgui.h"

#include "public/rope_physics.h"
#include "public/rope_raylib.h"
#include "public/clothe_raylib.h"
#include "public/job_system.h"

#include <chrono>
#include <iostream>

#define MULTITHREAD
//#define SHOW_ROPES

std::atomic<bool> keep_running(true); 
std::atomic<float> dt(0.01f);

int main(int argc, char** argv){

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


    unsigned int rows = 32;
    unsigned int cols = 32;
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

    rope.InitRope(JE::Vec3{ 0.0f, 5.0f, 0.0f }, JE::Vec3{ 0.0f, -5.0f, 0.0f }, 1.0f, rope1_friction);
    rope2.InitRope(JE::Vec3{ 0.0f, 5.0f, 0.0f }, JE::Vec3{ 0.0f, -5.0f, 0.0f }, 3.0f, rope2_friction);

    float rope_3_x_offset = 5.0f;
    rope3.InitRope(JE::Vec3{ rope_3_x_offset , 5.0f, 0.0f }, JE::Vec3{ 0.0f , -5.0f, 0.0f }, 3.0f, rope3_friction);


    printf("Ropes created correctly!\n");

    rope3.SetFixed(99);

    rope.SetColor(BLUE);
    rope2.SetColor(RED);
    rope3.SetColor(GREEN);

    //printf("Lenght-> %f\n Num particles %d", rope.m_lenght, rope.m_numParticles);

    float speed = 10.0f;
    float amplitude = 2.0f;

    //Vec3 position, Vec3 direction, float strength, float spread_angle, float max_distance, bool enabled = true
    //JE::WindTurbine turbine(JE::Vec3{ 0.1f, 0.1f, 0.1f }, JE::Vec3{1.0f, 1.0f, 0.1f}, 50.0f, 50.0f, 10.0f);

    rlImGuiSetup(true);

    JE::ClotheRaylib cloth(rows, cols, 5.0f, 5.0f);
    JE::ClotheRaylib courtain1(rows, cols, 10.0f, 10.0f);
    JE::ClotheRaylib courtain2(rows, cols, 10.0f, 10.0f);

    JE::ClotheRaylib courtain3(rows, cols, 10.0f, 10.0f);
    JE::ClotheRaylib courtain4(rows, cols, 10.0f, 10.0f);
    JE::ClotheRaylib courtain5(rows, cols, 10.0f, 10.0f);

    cloth.InitClothe(JE::Vec3{ 0.0f, 5.0f, 0.0f }, 10.0f, 0.1f);
    courtain1.InitClothe(JE::Vec3{ 10.0f, 5.0f, 10.0f }, 10.0f, 0.1f);
    courtain2.InitClothe(JE::Vec3{ 20.0f, 5.0f, 10.0f }, 10.0f, 0.1f);

    courtain3.InitClothe(JE::Vec3{ 20.0f, -5.0f, 10.0f }, 10.0f, 0.1f);
    courtain4.InitClothe(JE::Vec3{ 20.0f, -10.0f, 10.0f }, 10.0f, 0.1f);
    courtain5.InitClothe(JE::Vec3{ 20.0f, -15.0f, 10.0f }, 10.0f, 0.1f);

    courtain1.SetColor(BLUE);



    cloth.SetFixed(0,0);
    cloth.SetFixed(cols - 1,0);
    
    courtain1.SetFixed(0,0);
    courtain1.SetFixed(cols - 1,0);

    courtain2.SetFixed(0,0);
    courtain2.SetFixed(cols - 1,0);
    
    courtain3.SetFixed(0,0);
    courtain4.SetFixed(0,0);
    courtain5.SetFixed(0,0);



    float x_offset, y_offset, z_offset;
    cloth.GetPosition(0, 11, x_offset, y_offset, z_offset);
    
    bool opening = false;
    bool closing = false;
    float courtain1_desired_x = 12.5f;
    float courtain2_desired_x = 27.5f;

    float courtain1_original_x = 20.0f;
    float courtain2_original_x = 20.0f;
    float courtains_speed = 7.5f;


    cloth.SetTexture("assets/texture_0.png");

    const float smoothing_factor = 0.1f;
    
    
#ifdef MULTITHREAD
    // ----- Physiscs thread ----- 
    std::thread physics([&courtain1, &courtain2, &mtx]() {
        
        const float fixed_time_step = 1.0f / 60.0f;
        auto previous_time = std::chrono::high_resolution_clock::now();


        while (keep_running) {
            std::lock_guard<std::mutex> lock(mtx);
            auto current_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration elapsed = current_time - previous_time;

            float delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() / 1000.0f;
            float acum = delta_time;

            //static float acum = dt.load();


            while (acum < fixed_time_step) {
                //printf("wait... DT: %f/%f \n", acum, fixed_time_step);
                auto loop_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float> loop_elapsed = loop_time - current_time;

                // Incrementar el acumulador con el tiempo transcurrido en cada iteración
                acum += loop_elapsed.count();

                // Actualizar current_time para la siguiente iteración
                current_time = loop_time;

            }

            //printf("GO\n");
            courtain1.Update(dt.load());
            courtain2.Update(dt.load());
            

            previous_time = current_time;
        }
        printf("Custom Thread closed\n");
    });
    // ---------------------------
#endif
    std::string fps_counter = "FPS: \n";

    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_LEFT_CONTROL)){
            UpdateCamera(&camera, CAMERA_FIRST_PERSON); 
        }


        BeginDrawing();
        ClearBackground(black);
        BeginMode3D(camera);

        float x, y, z;
#ifdef SHOW_ROPES
        rope.GetPosition(0,x,y,z);
        rope.SetPointPosition(0,cosf(GetTime() * speed) * 2.0f,y,z);
        
        rope2.GetPosition(0,x,y,z);
        rope2.SetPointPosition(0,cosf(GetTime() * speed) * 2.0f,y,z);
        
        rope3.GetPosition(0,x,y,z);
        rope3.SetPointPosition(0, rope_3_x_offset + cosf(GetTime() * speed) * 2.0f,y,z);
#endif

        cloth.GetPosition(cols - 1, 0, x,y,z);
        cloth.SetPosition(cols - 1, 0, x_offset + (cosf(GetTime() * speed) * amplitude), y, z);

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


        
        dt.store(GetFrameTime());

        if (GetTime() > 2.0f) {

            //float dt = GetFrameTime(); 
            
            //fps_counter += std::to_string(dt) + "\n";
            //float smooth_delta_time = 0.0f;
            //smooth_delta_time = (1.0f - smoothing_factor) * smooth_delta_time + smoothing_factor * dt;

#ifdef SHOW_ROPES

        //if (IsKeyDown(KEY_SPACE)) {
            rope.Update(GetFrameTime());
            rope2.Update(GetFrameTime());
            rope3.Update(GetFrameTime());
        //}
            rope.DrawRope();
            rope2.DrawRope();
            rope3.DrawRope();
#endif

            //cloth.Update(GetFrameTime());
            //cloth.DrawClothe();

            

            //auto start = std::chrono::high_resolution_clock::now();
            
            
#ifndef MULTITHREAD
            courtain1.Update(dt);
            courtain2.Update(dt);
#endif
            /*
            courtain3.Update(GetFrameTime());
            courtain4.Update(GetFrameTime());
            courtain5.Update(GetFrameTime());
            */
            
            
            
            /*
            job_sys.add_task(std::bind(&JE::Cloth::Update, &courtain1, smooth_delta_time));
            job_sys.add_task(std::bind(&JE::Cloth::Update, &courtain2, smooth_delta_time));
            job_sys.add_task(std::bind(&JE::Cloth::Update, &courtain3, dt));
            job_sys.add_task(std::bind(&JE::Cloth::Update, &courtain4, dt));
            job_sys.add_task(std::bind(&JE::Cloth::Update, &courtain5, dt));
            
            job_sys.wait_until_finish();
            */
            

            /*auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float, std::milli> duration = end - start;
            std::cout << "Tiempo de ejecucion antes: " << duration.count() << " ms" << std::endl;
            
            
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            std::cout << "Tiempo de ejecucion despues: " << duration.count() << " ms" << std::endl;


            start = std::chrono::high_resolution_clock::now();

            courtain1.DrawClothe();
            courtain2.DrawClothe();
            */
            /*courtain3.DrawClothe();
            courtain4.DrawClothe();
            courtain5.DrawClothe();*/

           /* end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            std::cout << "Tiempo de ejecucion draw: " << duration.count() << " ms" << std::endl;
            */

        }

        /*float tmp_x, tmp_y, tmp_z;
        cloth.GetPosition(0, 0, tmp_x, tmp_y, tmp_z);
        printf("First x:%f y:%f, z:%f\n", tmp_x, tmp_y, tmp_z);
        cloth.GetPosition(1, 0, tmp_x, tmp_y, tmp_z);
        printf("Second x:%f y:%f, z:%f\n", tmp_x, tmp_y, tmp_z);*/
            
        courtain1.DrawClothe();
        courtain2.DrawClothe();

        //DrawSphere(Vector3{0.0f, 5.0f, 0.0f}, 0.5f, red);

        DrawCube(cubePosition, cubeSize, cubeSize, cubeSize, WHITE);
        DrawCubeWires(cubePosition, cubeSize, cubeSize, cubeSize, DARKGRAY);


        EndMode3D();
        
        rlImGuiBegin();

        ImGui::Text("Delta Time: %f", GetFrameTime());

        if (ImGui::CollapsingHeader("General values")) {
            ImGui::DragFloat("Speed", &speed, 0.01f, 0.0f, 10.0f);
            ImGui::DragFloat("Amplitude", &amplitude, 0.01f, 0.0f, 10.0f);
        }

        if (ImGui::CollapsingHeader("Rope 1")) {
            ImGui::DragFloat("Friction 1", &rope1_friction, 0.001f, 0.0f, 2.0f);
            ImGui::DragFloat("Mass 1", &rope1_mass, 0.01f, 0.0f, 10.0f);
            
            rope.SetAllFriction(rope1_friction);
            rope.SetAllMass(rope1_mass);
        }

        if (ImGui::CollapsingHeader("Rope 2")) {
            ImGui::DragFloat("Friction 2", &rope2_friction, 0.001f, 0.0f, 2.0f);
            ImGui::DragFloat("Mass 2", &rope2_mass, 0.01f, 0.0f, 10.0f);
            rope2.SetAllFriction(rope2_friction);
            rope2.SetAllMass(rope2_mass);
        }

        if (ImGui::CollapsingHeader("Rope 3")) {
            ImGui::DragFloat("Friction 3", &rope3_friction, 0.001f, 0.0f, 2.0f);
            ImGui::DragFloat("Mass 3", &rope3_mass, 0.01f, 0.0f, 10.0f);
            rope3.SetAllFriction(rope3_friction);
            rope3.SetAllMass(rope3_mass);
        }

        rlImGuiEnd();

        //int fps = GetFPS();
        //DrawText("Hello Cube!", 10, 10, 20, DARKGRAY);
        DrawFPS(10, 10);

        EndDrawing();
        
    }

#ifdef MULTITHREAD
    keep_running = false;
    if (physics.joinable()) {
        physics.join();
    }
#endif

    printf("%s", fps_counter.c_str());

    rlImGuiShutdown();
    CloseWindow();


    return 0;
}
