#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

typedef struct Color Color;
struct Color {
  int red;
  int green;
  int blue;
};

typedef struct Line Line;
struct Line {
  int startX;
  int startY;
  int endX;
  int endY;
  Color color;
};

void handleMouseClick(SDL_Renderer* renderer, SDL_MouseButtonEvent button_event,
  bool* line_started, Line* line);
void setDrawColor(SDL_Renderer* renderer, Color color);
void renderPalette(
  SDL_Renderer* renderer, Color palette[], size_t num_of_colors);

int main(int argc, char** argv) {
  int error_code = SDL_Init(SDL_INIT_EVERYTHING);
  if (error_code != 0) {
    printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
    return error_code;
  }

  SDL_Window* window = SDL_CreateWindow(
    "Draw lines", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

  SDL_Renderer* renderer =
    SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  Color bg_color = {255, 255, 255};
  setDrawColor(renderer, bg_color);
  SDL_RenderClear(renderer);

  Color palette[] = {{0, 0, 0}, {255, 255, 255}, {46, 53, 50}, {126, 145, 129},
    {199, 206, 219}, {160, 170, 178}, {148, 132, 155}};
  renderPalette(renderer, palette, sizeof(palette) / sizeof(Color));

  SDL_Event event = {0};
  bool need_run = true;
  bool line_started = false;
  Line line = {
    .startX = 0, .startY = 0, .endX = 0, .endY = 0, .color = {0, 0, 0}};
  while (need_run) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_q:
              need_run = false;
              break;
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
          handleMouseClick(renderer, event.button, &line_started, &line);
          break;
        case SDL_QUIT:
          need_run = false;
          break;
      }
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}

void handleMouseClick(SDL_Renderer* renderer, SDL_MouseButtonEvent button_event,
  bool* line_started, Line* line) {
  printf(
    "Mouse clicked at position (%d, %d)\n", button_event.x, button_event.y);
  if (!(*line_started)) {
    *line_started = true;
    line->startX = button_event.x;
    line->startY = button_event.y;
  } else {
    *line_started = false;
    line->endX = button_event.x;
    line->endY = button_event.y;
    setDrawColor(renderer, line->color);
    SDL_RenderDrawLine(
      renderer, line->startX, line->startY, line->endX, line->endY);
    SDL_RenderPresent(renderer);
    printf("Line drawn from (%d, %d) to (%d, %d)\n", line->startX, line->startY,
      line->endX, line->endY);
  }
}

void setDrawColor(SDL_Renderer* renderer, Color color) {
  SDL_SetRenderDrawColor(
    renderer, color.red, color.green, color.blue, SDL_ALPHA_OPAQUE);
}

void renderPalette(
  SDL_Renderer* renderer, Color palette[], size_t num_of_colors) {
  int const SIZE = 40;
  int const SPACING = 5;

  int x = SPACING;
  int y = SPACING;

  for (size_t i = 0; i < num_of_colors; i++) {
    SDL_Rect button = {x, y, SIZE, SIZE};
    setDrawColor(renderer, palette[i]);
    SDL_RenderFillRect(renderer, &button);
    x += SIZE + SPACING;
  }

  SDL_RenderPresent(renderer);
}
