
#include "raylib.h"
#include "iostream"
#include "stdlib.h"
#include "time.h"


const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 800;

const int MINEFIELD_WIDTH = 18;
const int MINEFIELD_HEIGHT = 14;

const int CELL_SIZE = 40;
const int BOMB_COUNT = 40;

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

void DrawBoard(char field[MINEFIELD_HEIGHT][MINEFIELD_WIDTH], bool checkedFields[MINEFIELD_HEIGHT][MINEFIELD_WIDTH], Vector2 primaryPosition, Vector2 selectedField) {
    for(int i = 0;i<MINEFIELD_HEIGHT;i++) {
        for(int j = 0;j<MINEFIELD_WIDTH;j++) {
            if((int)selectedField.x == j && (int)selectedField.y == i) {
                DrawRectangle(CELL_SIZE * j + primaryPosition.x, CELL_SIZE * i + primaryPosition.y, CELL_SIZE, CELL_SIZE, GRAY);
            }
            if(checkedFields[i][j]) {
                DrawRectangle(CELL_SIZE * j + primaryPosition.x, CELL_SIZE * i + primaryPosition.y, CELL_SIZE, CELL_SIZE, LIGHTGRAY);
            } 
            DrawRectangleLines(CELL_SIZE * j + primaryPosition.x, CELL_SIZE * i + primaryPosition.y, CELL_SIZE, CELL_SIZE, BLACK);
        }
    }
    DrawRectangleLinesEx((Rectangle){primaryPosition.x, primaryPosition.y, CELL_SIZE * MINEFIELD_WIDTH, CELL_SIZE * MINEFIELD_HEIGHT}, 4, BLACK);
}

Vector2 GetField(Vector2 mousePosition, Vector2 primaryPosition) {
    return {(mousePosition.x - primaryPosition.x) / CELL_SIZE, (mousePosition.y - primaryPosition.y) / CELL_SIZE};
}

void PlantBombs(char field[MINEFIELD_HEIGHT][MINEFIELD_WIDTH], int bombs, Vector2 selectedField) {
    srand(time(0));
    Vector2 bombPosition;
    while(bombs > 0) {
        bombPosition = {(float)(rand() % MINEFIELD_WIDTH), (float)(rand() % MINEFIELD_HEIGHT)};
        if(selectedField.x == bombPosition.x && selectedField.y == bombPosition.y) {
            continue;
        } else {
            // place bomb
            field[(int)bombPosition.y][(int)bombPosition.x] = 'B';
            bombs--;
        }
    }
}

void CheckField(int x, int y, char field[MINEFIELD_HEIGHT][MINEFIELD_WIDTH], bool checkedFields[MINEFIELD_HEIGHT][MINEFIELD_WIDTH]) {
    if(!checkedFields[y][x]) {
        checkedFields[y][x] = true;
        if(field[y][x] == 'B') {
            checkedFields[y][x] = false;
        } else if(field[y][x] == '0') {
            for(int i = 0;i<3;i++) {
                for(int j = 0;j<3;j++) {
                    if(y + i - 1 < 0 || x + j - 1 < 0 || y + i - 1 >= MINEFIELD_HEIGHT || x + j - 1 >= MINEFIELD_WIDTH) {
                        continue;
                    }
                    if(i == 1 && j == 1) {
                        continue;
                    }
                    CheckField(x + j - 1, y + i - 1, field, checkedFields);
                }
            }
        }
    }
}



int main() {
    Vector2 mousePosition;
    Vector2 primaryGridPosition = {50, 100};
    Vector2 selectedField;
    bool started = false;
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MINESWEEPER");
    
    // loading assets
    font = LoadFontEx("assets/fonts/Oswald-Bold.ttf", 64, 0, 0);

    // loading content
    Text title("MINESWEEPER", {WINDOW_WIDTH / 2, 0}, 80);

    int bombCount = BOMB_COUNT;

    int x, y;

    PrepareFieldForGame(mineField, checkedFields);
    while(!WindowShouldClose()) {

        // mouse logic
        mousePosition = GetMousePosition();
        selectedField = {-1, -1};
        if(mousePosition.x >= primaryGridPosition.x && mousePosition.y >= primaryGridPosition.y) {
            if(mousePosition.x <= primaryGridPosition.x + CELL_SIZE * MINEFIELD_WIDTH && mousePosition.y <= primaryGridPosition.y + CELL_SIZE * MINEFIELD_HEIGHT) {
                selectedField = GetField(mousePosition, primaryGridPosition); 
            }
        }
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if(selectedField.x > -1) {
                x = (int)selectedField.x;
                y = (int)selectedField.y;
                if(!started) {
                    // setting all bombs
                    PlantBombs(mineField, bombCount, selectedField);
                    started = true;
                }
                CheckField(x, y, mineField, checkedFields);
            }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        title.DrawCentered();
        DrawBoard(mineField, checkedFields, primaryGridPosition, selectedField);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}