#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

enum {
  BUTTON_SIZE = 40,
  BUTTON_SPACING = 5,
  BUTTON_BORDER = 1,
};

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

enum Tool {
  LINE_TOOL,
  RECT_TOOL,
  NUM_OF_TOOLS,
};
typedef enum Tool Tool;

void handleMouseClick(SDL_Renderer* renderer,
                      SDL_MouseButtonEvent button_event,
                      bool* shape_started,
                      Line* line,
                      Tool* tool,
                      Color palette[],
                      size_t palette_length);
void drawLine(SDL_Renderer* renderer,
              SDL_MouseButtonEvent button_event,
              bool* shape_started,
              Line* line);
void drawRect(SDL_Renderer* renderer,
              SDL_MouseButtonEvent button_event,
              bool* shape_started,
              Line* line);

void renderPalette(SDL_Renderer* renderer,
                   Color palette[],
                   size_t num_of_colors);
bool isInPalette(int x, int y, size_t num_of_colors);
int paletteIndex(int x);

void renderToolbox(SDL_Renderer* renderer, size_t palette_length);
bool isInToolbox(int x, int y, size_t palette_length);
Tool toolboxTool(int x, Tool current_tool, size_t palette_length);

void setDrawColor(SDL_Renderer* renderer, Color color);

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

  Color palette[] = {{0, 0, 0},
                     {255, 255, 255},
                     {243, 243, 243},
                     {255, 158, 236},
                     {0, 191, 254},
                     {0, 253, 149},
                     {253, 229, 0}};
  size_t palette_length = sizeof(palette) / sizeof(Color);
  renderPalette(renderer, palette, palette_length);

  renderToolbox(renderer, palette_length);

  SDL_Event event = {0};
  bool need_run = true;
  bool shape_started = false;
  Line line = {
    .startX = 0, .startY = 0, .endX = 0, .endY = 0, .color = {0, 0, 0}};
  Tool tool = LINE_TOOL;
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
          handleMouseClick(renderer,
                           event.button,
                           &shape_started,
                           &line,
                           &tool,
                           palette,
                           palette_length);
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

void handleMouseClick(SDL_Renderer* renderer,
                      SDL_MouseButtonEvent button_event,
                      bool* shape_started,
                      Line* line,
                      Tool* tool,
                      Color palette[],
                      size_t palette_length) {
  printf(
    "Mouse clicked at position (%d, %d)\n", button_event.x, button_event.y);

  if (isInPalette(button_event.x, button_event.y, palette_length)) {
    puts("Click was in palette");
    int color_index = paletteIndex(button_event.x);
    if (color_index != -1) {
      Color color = palette[color_index];
      line->color = color;
      printf("Color is now (%d, %d, %d)\n", color.red, color.green, color.blue);
    }
    *shape_started = false;
  } else if (isInToolbox(button_event.x, button_event.y, palette_length)) {
    puts("Click was in toolbox");
    *tool = toolboxTool(button_event.x, *tool, palette_length);
    printf("Tool is now Tool %d", *tool);
    *shape_started = false;
  } else {
    switch (*tool) {
      case LINE_TOOL:
        drawLine(renderer, button_event, shape_started, line);
        break;
      case RECT_TOOL:
        drawRect(renderer, button_event, shape_started, line);
        break;
      default:
        break;
    }
    renderPalette(renderer, palette, palette_length);
    renderToolbox(renderer, palette_length);
  }
}

void drawLine(SDL_Renderer* renderer,
              SDL_MouseButtonEvent button_event,
              bool* shape_started,
              Line* line) {
  if (!(*shape_started)) {
    *shape_started = true;
    line->startX = button_event.x;
    line->startY = button_event.y;
  } else {
    *shape_started = false;
    line->endX = button_event.x;
    line->endY = button_event.y;
    setDrawColor(renderer, line->color);
    SDL_RenderDrawLine(
      renderer, line->startX, line->startY, line->endX, line->endY);
    SDL_RenderPresent(renderer);
    printf("Line drawn from (%d, %d) to (%d, %d)\n",
           line->startX,
           line->startY,
           line->endX,
           line->endY);
  }
}

void drawRect(SDL_Renderer* renderer,
              SDL_MouseButtonEvent button_event,
              bool* shape_started,
              Line* line) {
  if (!(*shape_started)) {
    *shape_started = true;
    line->startX = button_event.x;
    line->startY = button_event.y;
  } else {
    *shape_started = false;
    line->endX = button_event.x;
    line->endY = button_event.y;
    setDrawColor(renderer, line->color);
    SDL_Rect rect = {line->startX,
                     line->startY,
                     line->endX - line->startX,
                     line->endY - line->startY};
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
    printf("Line drawn from (%d, %d) to (%d, %d)\n",
           line->startX,
           line->startY,
           line->endX,
           line->endY);
  }
}

void setDrawColor(SDL_Renderer* renderer, Color color) {
  SDL_SetRenderDrawColor(
    renderer, color.red, color.green, color.blue, SDL_ALPHA_OPAQUE);
}

void renderPalette(SDL_Renderer* renderer,
                   Color palette[],
                   size_t num_of_colors) {
  int section_w =
    num_of_colors * BUTTON_SIZE + (num_of_colors + 1) * BUTTON_SPACING;
  int section_h = BUTTON_SIZE + 2 * BUTTON_SPACING;
  SDL_Rect palette_section = {0, 0, section_w, section_h};
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawRect(renderer, &palette_section);

  int x = BUTTON_SPACING;
  int y = BUTTON_SPACING;

  for (size_t i = 0; i < num_of_colors; i++) {
    SDL_Rect border = {x, y, BUTTON_SIZE, BUTTON_SIZE};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &border);

    SDL_Rect button = {x + BUTTON_BORDER,
                       y + BUTTON_BORDER,
                       BUTTON_SIZE - 2 * BUTTON_BORDER,
                       BUTTON_SIZE - 2 * BUTTON_BORDER};
    setDrawColor(renderer, palette[i]);
    SDL_RenderFillRect(renderer, &button);

    x += BUTTON_SIZE + BUTTON_SPACING;
  }

  SDL_RenderPresent(renderer);
}

bool isInPalette(int x, int y, size_t num_of_colors) {
  return x < (num_of_colors * BUTTON_SIZE +
              (num_of_colors + 1) * BUTTON_SPACING) &&
         y < (BUTTON_SIZE + 2 * BUTTON_SPACING);
}

int paletteIndex(int x) {
  int full_width = BUTTON_SPACING + BUTTON_SIZE;
  if (x % full_width > BUTTON_SPACING) {
    return x / full_width;
  } else {
    return -1;
  }
}

void renderToolbox(SDL_Renderer* renderer, size_t palette_length) {
  int section_x = palette_length * BUTTON_SIZE +
                  (palette_length + 1) * BUTTON_SPACING + BUTTON_SPACING;
  int section_y = 0;
  int section_w =
    NUM_OF_TOOLS * BUTTON_SIZE + (NUM_OF_TOOLS + 1) * BUTTON_SPACING;
  int section_h = BUTTON_SIZE + 2 * BUTTON_SPACING;
  SDL_Rect toolbox_section = {section_x, section_y, section_w, section_h};
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawRect(renderer, &toolbox_section);

  int x = section_x + BUTTON_SPACING;
  int y = section_y + BUTTON_SPACING;

  for (size_t i = 0; i < NUM_OF_TOOLS; i++) {
    SDL_Rect border = {x, y, BUTTON_SIZE, BUTTON_SIZE};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &border);

    SDL_Rect button = {x + BUTTON_BORDER,
                       y + BUTTON_BORDER,
                       BUTTON_SIZE - 2 * BUTTON_BORDER,
                       BUTTON_SIZE - 2 * BUTTON_BORDER};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &button);

    x += BUTTON_SIZE + BUTTON_SPACING;
  }

  x = section_x + BUTTON_SPACING;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

  // Line tool

  SDL_RenderDrawLine(
    renderer, x + 5, y + 5, x + BUTTON_SIZE - 5, y + BUTTON_SIZE - 5);

  // ---------

  x += BUTTON_SIZE + BUTTON_SPACING;

  // Rectangle tool

  SDL_Rect rect_tool = {
    x + 5, y + 10, BUTTON_SIZE - 2 * 5, BUTTON_SIZE - 2 * 10};
  SDL_RenderFillRect(renderer, &rect_tool);

  // --------------

  SDL_RenderPresent(renderer);
}

bool isInToolbox(int x, int y, size_t palette_length) {
  int section_x = palette_length * BUTTON_SIZE +
                  (palette_length + 1) * BUTTON_SPACING + BUTTON_SPACING;
  int section_w =
    NUM_OF_TOOLS * BUTTON_SIZE + (NUM_OF_TOOLS + 1) * BUTTON_SPACING;
  int section_h = BUTTON_SIZE + 2 * BUTTON_SPACING;

  return x > section_x && x < section_x + section_w && y < section_h;
}

Tool toolboxTool(int x, Tool current_tool, size_t palette_length) {
  int full_width = BUTTON_SPACING + BUTTON_SIZE;

  int section_x = palette_length * BUTTON_SIZE +
                  (palette_length + 1) * BUTTON_SPACING + BUTTON_SPACING;
  int relativeX = x - section_x;

  if (relativeX % full_width > BUTTON_SPACING) {
    return (Tool)(relativeX / full_width);
  } else {
    return current_tool;
  }
}
