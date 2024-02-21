#include <raylib.h>
#include <raymath.h>

#include <deque>
#include <iostream>

#define FrameRate 60
#define Transparency 255
#define grid cellSize* cellCount

Color green = {173, 204, 96, Transparency};
Color darkGreen = {43, 51, 24, Transparency};

int cellSize = 30;
int cellCount = 25;

double lastUpdateTime = 0;

bool eventTriggered(double interval) {
  double currentTime = GetTime();
  if (currentTime - lastUpdateTime >= interval) {
    lastUpdateTime = currentTime;
    return true;
  }
  return false;
}

class Snake {
 public:
  std::deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
  Vector2 dir = {1, 0};

  void Draw() {
    for (unsigned int i = 0; i < body.size(); i++) {
      float x = body[i].x;
      float y = body[i].y;
      Rectangle segment = Rectangle{x * cellSize, y * cellSize, (float)cellSize,
                                    (float)cellSize};
      DrawRectangleRounded(segment, 0.5, 6, darkGreen);
    }
  }
  void Update() {
    body.pop_back();
    body.push_front(Vector2Add(body[0], dir));
  }
};

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

class Game {
 public:
  Snake snake = Snake();
  Food food = Food();

  void Draw() {
    food.Draw();
    snake.Draw();
  }

  void Update() { snake.Update(); }
};

int main() {
  InitWindow(grid, grid, "Snake");
  SetTargetFPS(FrameRate);

  Game game = Game();

  while (WindowShouldClose() == false) {
    BeginDrawing();

    if (eventTriggered(0.2)) {
      game.Update();
    }

    if (IsKeyPressed(KEY_UP) && game.snake.dir.y != 1) {
      game.snake.dir = {0, -1};
    }
    if (IsKeyPressed(KEY_DOWN) && game.snake.dir.y != -1) {
      game.snake.dir = {0, 1};
    }
    if (IsKeyPressed(KEY_LEFT) && game.snake.dir.x != 1) {
      game.snake.dir = {-1, 0};
    }
    if (IsKeyPressed(KEY_RIGHT) && game.snake.dir.x != -1) {
      game.snake.dir = {1, 0};
    }

    ClearBackground(green);

    game.Draw();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

//  alias build_snake='g++ -o snake main.cpp -lraylib && ./snake'

