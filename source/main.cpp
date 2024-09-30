#include <stdio.h>
#include <raylib.h>

int main(int argc, char** argv)
{
    // Inicializar la ventana
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Hello Triangle");

    // Definir los vértices del triángulo
    Vector2 point1 = { screenWidth / 2.0f, screenHeight / 4.0f };
    Vector2 point2 = { screenWidth / 4.0f, screenHeight * 3.0f / 4.0f };
    Vector2 point3 = { screenWidth * 3.0f / 4.0f, screenHeight * 3.0f / 4.0f };

    // Establecer el color del triángulo
    Color triangleColor = RED;

    // Bucle principal
    while (!WindowShouldClose()) // Detecta si se cierra la ventana
    {
        // Comenzar a dibujar
        BeginDrawing();
        ClearBackground(RAYWHITE); // Limpiar el fondo

        // Dibujar el triángulo
        DrawTriangle(point1, point2, point3, triangleColor);

        // Mostrar texto
        DrawText("Hello Triangle!", 10, 10, 20, DARKGRAY);

        EndDrawing(); // Finalizar el dibujado
    }

    // Cerrar ventana y liberar recursos
    CloseWindow();

    return 0;
}