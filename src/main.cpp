
#include "raylib.h"
#include "iostream"


const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 800;

const int MINEFIELD_WIDTH = 18;
const int MINEFIELD_HEIGHT = 14;

const int CELL_SIZE = 40;

char mineField[MINEFIELD_HEIGHT][MINEFIELD_WIDTH];
bool checkedFields[MINEFIELD_HEIGHT][MINEFIELD_WIDTH];

Font font;

class Text {
    private:
        std::string content;
        int contentWidth;
    public:
        Vector2 position;
        int size;
        int spacing = 2;
        Color color = BLACK;
        Text(std::string content, Vector2 position, int size) {
            this->size = size;
            this->position = position;
            setContent(content);
        }
    void setContent(std::string content) {
        this->content = content;
        this->contentWidth = MeasureTextEx(font, content.c_str(), size, spacing).x;
    }
    void DrawPlain() {
        DrawTextEx(font, content.c_str(), position, size, 2, color);
    }
    void DrawCentered() {
        DrawTextEx(font, content.c_str(), (Vector2){position.x - contentWidth / 2, position.y}, size, 2, color);
    }
};

void PrepareFieldForGame(char field[MINEFIELD_HEIGHT][MINEFIELD_WIDTH], bool checkedField[MINEFIELD_HEIGHT][MINEFIELD_WIDTH]) {
    for(int i = 0;i<MINEFIELD_HEIGHT;i++) {
        for(int j = 0;j<MINEFIELD_WIDTH;j++) {
            field[i][j] = '0';
            checkedField[i][j] = false;
        }
    }
}

void DrawBoard(char field[MINEFIELD_HEIGHT][MINEFIELD_WIDTH], bool checkedField[MINEFIELD_HEIGHT][MINEFIELD_WIDTH], Vector2 primaryPosition) {
    for(int i = 0;i<MINEFIELD_HEIGHT;i++) {
        for(int j = 0;j<MINEFIELD_WIDTH;j++) {
            DrawRectangleLines(CELL_SIZE * j + primaryPosition.x, CELL_SIZE * i + primaryPosition.y, CELL_SIZE, CELL_SIZE, BLACK);
        }
    }
    DrawRectangleLinesEx((Rectangle){primaryPosition.x, primaryPosition.y, CELL_SIZE * MINEFIELD_WIDTH, CELL_SIZE * MINEFIELD_HEIGHT}, 4, BLACK);
}



int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MINESWEEPER");
    
    // loading assets
    font = LoadFontEx("assets/fonts/Oswald-Bold.ttf", 64, 0, 0);

    // loading content
    Text title("MINESWEEPER", {WINDOW_WIDTH / 2, 0}, 80);
    PrepareFieldForGame(mineField, checkedFields);
    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        title.DrawCentered();
        DrawBoard(mineField, checkedFields, (Vector2){50, 100});
        EndDrawing();
    }
    CloseWindow();
    return 0;
}