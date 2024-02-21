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
  Vector2 pos;
  Texture2D texture;

  Food() {
    Image image = LoadImage("images/food.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    pos = GenerateRandPos();
  }
  ~Food() { UnloadTexture(texture); }

  void Draw() {
    /* DrawRectangle(pos.x * cellSize, pos.y * cellSize, cellSize, cellSize,
     * darkGreen); */
    DrawTexture(texture, pos.x * cellSize, pos.y * cellSize, WHITE);
  }
  Vector2 GenerateRandPos() {
    float x = GetRandomValue(0, cellCount - 1);
    float y = GetRandomValue(0, cellCount - 1);
    return Vector2{x, y};
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

//  alias build_snake='g++ -o snake main.cpp -lraylib && ./snake'

