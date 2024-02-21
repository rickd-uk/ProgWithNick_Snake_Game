#include <raylib.h>

#include <iostream>
#define FrameRate 60
#define Transparency 255
#define grid cellSize* cellCount

Color green = {173, 204, 96, Transparency};
Color darkGreen = {43, 51, 24, Transparency};

int cellSize = 30;
int cellCount = 25;

class Food {
 public:
  Vector2 pos = {5, 6};

  void Draw() {
    DrawRectangle(pos.x * cellSize, pos.y * cellSize, cellSize, cellSize,
                  darkGreen);
  }
};

int main() {
  std::cout << "Hello" << std::endl;
  InitWindow(grid, grid, "Snake");
  SetTargetFPS(FrameRate);

  Food food = Food();

  while (WindowShouldClose() == false) {
    BeginDrawing();

    ClearBackground(green);

    food.Draw();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

//  g++ -o main main.cpp -lraylib -lstdc++
