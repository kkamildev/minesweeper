
#include "raylib.h"
#include "iostream"
#include "stdlib.h"
#include "time.h"


const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 800;

const int MINEFIELD_WIDTH = 18;
const int MINEFIELD_HEIGHT = 14;

const int CELL_SIZE = 40;
const int BOMBS_COUNT = 40;

char mineField[MINEFIELD_HEIGHT][MINEFIELD_WIDTH];
bool checkedFields[MINEFIELD_HEIGHT][MINEFIELD_WIDTH];
bool markFields[MINEFIELD_HEIGHT][MINEFIELD_WIDTH];


// 1 - game in progress 2 - win 3 - game over
int gameStatus;
int marksCount;
bool gameRunning = true;
bool started;

Rectangle exitButtonRect;
Rectangle retryButtonRect;
Font font;

void PrepareFieldForGame(char field[MINEFIELD_HEIGHT][MINEFIELD_WIDTH], bool checkedField[MINEFIELD_HEIGHT][MINEFIELD_WIDTH]) {
    started = false;
    gameStatus = 1;
    marksCount = BOMBS_COUNT;
    for(int i = 0;i<MINEFIELD_HEIGHT;i++) {
        for(int j = 0;j<MINEFIELD_WIDTH;j++) {
            field[i][j] = '0';
            checkedField[i][j] = false;
            markFields[i][j] = false;
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
                if(field[i][j] >= '1' && field[i][j] != 'B') {
                    std::string temp(1, field[i][j]);
                    DrawTextEx(font, temp.c_str(), {CELL_SIZE * j + primaryPosition.x + 10, CELL_SIZE * i + primaryPosition.y}, 45, 2, BLACK);
                }
            }
            // drawing marked places
            if(markFields[i][j]) {
                DrawRectangle(CELL_SIZE * j + primaryPosition.x, CELL_SIZE * i + primaryPosition.y, CELL_SIZE, CELL_SIZE, ORANGE);
            }
            // drawing final bombs places (win/lose)
            if(gameStatus != 1 && field[i][j] == 'B') {
                if(gameStatus == 2) {
                    DrawRectangle(CELL_SIZE * j + primaryPosition.x, CELL_SIZE * i + primaryPosition.y, CELL_SIZE, CELL_SIZE, GREEN);
                } else {
                    DrawRectangle(CELL_SIZE * j + primaryPosition.x, CELL_SIZE * i + primaryPosition.y, CELL_SIZE, CELL_SIZE, RED);
                }
            }
            DrawRectangleLines(CELL_SIZE * j + primaryPosition.x, CELL_SIZE * i + primaryPosition.y, CELL_SIZE, CELL_SIZE, BLACK);
        }
    }
    DrawRectangleLinesEx((Rectangle){primaryPosition.x, primaryPosition.y, CELL_SIZE * MINEFIELD_WIDTH, CELL_SIZE * MINEFIELD_HEIGHT}, 4, BLACK);
}

void DrawGamePanel(Vector2 primaryPosition, Vector2 mousePosition) {
    switch(gameStatus) {
        case 1:
            DrawTextEx(font, "GAME STATUS", primaryPosition, 60, 2, DARKGREEN);
        break;
        case 2:
            DrawTextEx(font, "YOU WIN!", primaryPosition, 60, 2, GREEN);
        break;
        case 3:
            DrawTextEx(font, "GAME OVER!", primaryPosition, 60, 2, RED);
        break;
    }
    if(gameStatus != 1) {
        exitButtonRect = {primaryPosition.x, primaryPosition.y + 140, 200, 50};
        retryButtonRect = {primaryPosition.x, primaryPosition.y + 80, 200, 50};
        // retry button
        if(CheckCollisionPointRec(mousePosition, retryButtonRect)) {
            DrawRectangle(retryButtonRect.x, retryButtonRect.y, retryButtonRect.width, retryButtonRect.height, LIGHTGRAY);
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                PrepareFieldForGame(mineField, checkedFields);
            }
        }
        DrawRectangleLinesEx(retryButtonRect, 4, BLACK);
        DrawTextEx(font, "RETRY", (Vector2){retryButtonRect.x + 60, retryButtonRect.y}, 50, 2, BLACK);
        // exit button
        if(CheckCollisionPointRec(mousePosition, exitButtonRect)) {
            DrawRectangle(exitButtonRect.x, exitButtonRect.y, exitButtonRect.width, exitButtonRect.height, LIGHTGRAY);
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                gameRunning = false;
            }
        }
        DrawRectangleLinesEx(exitButtonRect, 4, BLACK);
        DrawTextEx(font, "EXIT", (Vector2){exitButtonRect.x + 70, exitButtonRect.y}, 50, 2, BLACK);
    } else {
        exitButtonRect = {primaryPosition.x, primaryPosition.y + 200, 200, 50};
        DrawTextEx(font, ("Marks left:" + std::to_string(marksCount)).c_str(), (Vector2){primaryPosition.x, primaryPosition.y + 80}, 50, 2, BLACK);
        DrawTextEx(font, ("Bombs:" + std::to_string(BOMBS_COUNT)).c_str(), (Vector2){primaryPosition.x, primaryPosition.y + 140}, 50, 2, BLACK);
        // exit button
        if(CheckCollisionPointRec(mousePosition, exitButtonRect)) {
            DrawRectangle(exitButtonRect.x, exitButtonRect.y, exitButtonRect.width, exitButtonRect.height, LIGHTGRAY);
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                gameRunning = false;
            }
        }
        DrawRectangleLinesEx(exitButtonRect, 4, BLACK);
        DrawTextEx(font, "EXIT", (Vector2){exitButtonRect.x + 70, exitButtonRect.y}, 50, 2, BLACK);
    }
}


Vector2 GetField(Vector2 mousePosition, Vector2 primaryPosition) {
    return {(mousePosition.x - primaryPosition.x) / CELL_SIZE, (mousePosition.y - primaryPosition.y) / CELL_SIZE};
}

void PlantBombs(char field[MINEFIELD_HEIGHT][MINEFIELD_WIDTH], int selectedX, int selectedY) {
    srand(time(0));

    int bombs = BOMBS_COUNT;
    int bombX;
    int bombY;
    while(bombs > 0) {
        bombX = (int)(rand() % MINEFIELD_WIDTH);
        bombY = (int)(rand() % MINEFIELD_HEIGHT);
        if(field[bombY][bombX] == 'B') {
            continue;
        }
        if(std::abs(bombX - selectedX) < 3 && std::abs(bombY - selectedY) < 3) {
            continue;
        }
        // place bomb
        field[bombY][bombX] = 'B';
        for(int i = 0;i<3;i++) {
            for(int j = 0;j<3;j++) {
                if(bombY + i - 1 < 0 || bombX + j - 1 < 0 || bombY + i - 1 >= MINEFIELD_HEIGHT || bombX + j - 1 >= MINEFIELD_WIDTH) {
                    continue;
                }
                if(field[bombY + i - 1][bombX + j - 1] != 'B') {
                    field[bombY + i - 1][bombX + j - 1] += 1;
                }
            }
        }
        bombs--;
    }
}

void CheckField(int x, int y, char field[MINEFIELD_HEIGHT][MINEFIELD_WIDTH], bool checkedFields[MINEFIELD_HEIGHT][MINEFIELD_WIDTH]) {
    if(markFields[y][x]) {
        markFields[y][x] = false;
        marksCount++;
    }
    if(!checkedFields[y][x] && !markFields[y][x]) {
        checkedFields[y][x] = true;
        if(field[y][x] == 'B') {
            checkedFields[y][x] = false;
            // game over
            gameStatus = 3;
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

void MarkField(int x, int y) {
    if(!checkedFields[y][x]) {
        if(markFields[y][x]) {
            markFields[y][x] = false;
            marksCount++;
        } else if(marksCount > 0) {
            markFields[y][x] = true;
            marksCount--;
        }
    }
}

bool CheckWin() {
    for(int i = 0;i<MINEFIELD_HEIGHT;i++) {
        for(int j = 0;j<MINEFIELD_WIDTH;j++) {
            if(mineField[i][j] == 'B') {
                continue;
            }
            if(!checkedFields[i][j]) {
                return false;
            }
        }
    }
    return true;
}



int main() {
    Vector2 mousePosition;
    Vector2 primaryGridPosition = {50, 100};
    Vector2 primaryLeftPanelPosition = {primaryGridPosition.x + MINEFIELD_WIDTH * CELL_SIZE + 30, 100};
    Vector2 selectedField;
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MINESWEEPER");
    SetTargetFPS(60);
    
    // loading assets
    font = LoadFontEx("assets/fonts/Oswald-Bold.ttf", 64, 0, 0);

    Vector2 titlePosition = {WINDOW_WIDTH / 2 - MeasureTextEx(font, "MINESWEEPER", 80, 2).x / 2, 0};
    int x, y;

    PrepareFieldForGame(mineField, checkedFields);
    while(!WindowShouldClose() && gameRunning) {
        // mouse logic
        mousePosition = GetMousePosition();
        selectedField = {-1, -1};
        if(gameStatus == 1) {
            if(mousePosition.x >= primaryGridPosition.x && mousePosition.y >= primaryGridPosition.y) {
                if(mousePosition.x <= primaryGridPosition.x + CELL_SIZE * MINEFIELD_WIDTH && mousePosition.y <= primaryGridPosition.y + CELL_SIZE * MINEFIELD_HEIGHT) {
                    selectedField = GetField(mousePosition, primaryGridPosition); 
                }
            }
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if(selectedField.x > -1) {
                    x = (int)selectedField.x;
                    y = (int)selectedField.y;
                    if(!markFields[y][x]) {
                        if(!started) {
                            // setting all bombs
                            PlantBombs(mineField, x, y);
                            started = true;
                        }
                        CheckField(x, y, mineField, checkedFields);
                        if(CheckWin()) {
                            gameStatus = 2;
                        }
                    }
                }
            }
            if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                if(selectedField.x > -1) {
                    x = (int)selectedField.x;
                    y = (int)selectedField.y;
                    MarkField(x, y);
                }
            }
        } 
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextEx(font, "MINESWEEPER", titlePosition, 80, 2, BLACK);
        DrawBoard(mineField, checkedFields, primaryGridPosition, selectedField);
        DrawGamePanel(primaryLeftPanelPosition, mousePosition);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}