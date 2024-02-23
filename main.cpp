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
int offset = 75;

double lastUpdateTime = 0;

bool ElemInDeque(Vector2 element, std::deque<Vector2> deque) {
  for (unsigned int i = 0; i < deque.size(); i++) {
    if (Vector2Equals(deque[i], element)) {
      return true;
    }
  }
  return false;
}

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
  bool addSegment = false;

  void Draw() {
    for (unsigned int i = 0; i < body.size(); i++) {
      float x = body[i].x;
      float y = body[i].y;
      Rectangle segment =
          Rectangle{offset + x * cellSize, offset + y * cellSize,
                    (float)cellSize, (float)cellSize};
      DrawRectangleRounded(segment, 0.5, 6, darkGreen);
    }
  }

  void Update() {
    body.push_front(Vector2Add(body[0], dir));
    if (addSegment == true) {
      addSegment = false;
    } else {
      body.pop_back();
    }
  }

  void Reset() {
    body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    dir = {1, 0};
  }
};

class Food {
 public:
  Vector2 pos;
  Texture2D texture;

  Food(std::deque<Vector2> snakeBody) {
    Image image = LoadImage("images/food.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    pos = GenerateRandPos(snakeBody);
  }
  ~Food() { UnloadTexture(texture); }

  void Draw() {
    /* DrawRectangle(pos.x * cellSize, pos.y * cellSize, cellSize, cellSize,
     * darkGreen); */
    DrawTexture(texture, offset + pos.x * cellSize, offset + pos.y * cellSize,
                WHITE);
  }

  Vector2 GenerateRandomCell() {
    float x = GetRandomValue(0, cellCount - 1);
    float y = GetRandomValue(0, cellCount - 1);
    Vector2 pos = {x, y};
    return pos;
  }

  Vector2 GenerateRandPos(std::deque<Vector2> snakeBody) {
    Vector2 pos = GenerateRandomCell();
    while (ElemInDeque(pos, snakeBody)) {
      pos = GenerateRandomCell();
    }
    return pos;
  }
};

class Game {
 public:
  Snake snake = Snake();
  Food food = Food(snake.body);
  bool running = true;

  void Draw() {
    food.Draw();
    snake.Draw();
  }

  void Update() {
    if (running) {
      snake.Update();
      CheckCollisionWithFood();
      CheckCollisionWithBorder();
      CheckCollisionWithTail();
    }
  }

  void CheckCollisionWithFood() {
    if (Vector2Equals(snake.body[0], food.pos)) {
      food.pos = food.GenerateRandPos(snake.body);
      snake.addSegment = true;
    }
  }

  void CheckCollisionWithBorder() {
    if (snake.body[0].x == cellCount || snake.body[0].x == -1 ||
        snake.body[0].y == cellCount || snake.body[0].y == -1) {
      GameOver();
    }
  }

  void GameOver() {
    snake.Reset();
    food.pos = food.GenerateRandPos(snake.body);
    running = false;
  }

  void CheckCollisionWithTail() {
    std::deque<Vector2> headlessBody = snake.body;
    headlessBody.pop_front();
    if (ElemInDeque(snake.body[0], headlessBody)) {
      GameOver();
    }
  }
};

int main() {
  InitWindow(2 * offset + grid, 2 * offset + grid, "Snake");
  SetTargetFPS(FrameRate);

  Game game = Game();

  while (WindowShouldClose() == false) {
    BeginDrawing();

    if (eventTriggered(0.2)) {
      game.Update();
    }

    if (IsKeyPressed(KEY_UP) && game.snake.dir.y != 1) {
      game.snake.dir = {0, -1};
      game.running = true;
    }
    if (IsKeyPressed(KEY_DOWN) && game.snake.dir.y != -1) {
      game.snake.dir = {0, 1};
      game.running = true;
    }
    if (IsKeyPressed(KEY_LEFT) && game.snake.dir.x != 1) {
      game.snake.dir = {-1, 0};
      game.running = true;
    }
    if (IsKeyPressed(KEY_RIGHT) && game.snake.dir.x != -1) {
      game.snake.dir = {1, 0};
      game.running = true;
    }

    ClearBackground(green);

    DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5,
                                   (float)grid + 10, (float)grid + 10},
                         5, darkGreen);
    game.Draw();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

//  alias build_snake='g++ -o snake main.cpp -lraylib && ./snake'
