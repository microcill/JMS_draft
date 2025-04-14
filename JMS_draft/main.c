#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_BLOCKS 36

// 게임 진행 상태 정의
typedef enum GameScreen { TITLE, GAMEPLAY, GAMEOVER, WIN } GameScreen;


int main() {
    InitWindow(800, 600, "Block");
    SetTargetFPS(60);

    // 패들과 공
    Rectangle paddle = { 350, 550, 100, 20 };
    Rectangle colis = { 350, 550, 100, 3 };
    Rectangle retry = { 200, 400, 150, 100 };
    Rectangle end = { 450, 400, 150, 100 };
    Vector2 ball = { 400, 300 };
    Vector2 speed = { 5, -5 };
    float radius = 10;

    // 게임 상태 초기화: 시작 화면 (TITLE)
    GameScreen currentScreen = TITLE;

    //기타 함수
    int timer = 0;
    int set = 0;
    int block_num = 36;
    int heart = 3;

    // 블록들
    Rectangle blocks[MAX_BLOCKS];
    Color colors[6] = { RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE };
    bool active[MAX_BLOCKS] = { 0 };
    for (int i = 0; i < MAX_BLOCKS; i++) {
        blocks[i] = (Rectangle){ 20 + (i % 12) * 63, 50 + (i / 12) * 30, 60, 20 };
        active[i] = true;
    }


    while (!WindowShouldClose()) {

        // 패들 이동
        if (IsKeyDown(KEY_LEFT)) paddle.x -= 8;
        if (IsKeyDown(KEY_RIGHT)) paddle.x += 8;
        if (paddle.x > 700) paddle.x = 700;
        if (paddle.x < -5) paddle.x = -5;

        //인식부 이동
        if (IsKeyDown(KEY_LEFT)) colis.x -= 8;
        if (IsKeyDown(KEY_RIGHT)) colis.x += 8;
        if (colis.x > 700) colis.x = 700;
        if (colis.x < -5) colis.x = -5;

        // 공 이동
        ball.x += speed.x;
        ball.y += speed.y;

        // 벽 충돌
        if (ball.x < radius || ball.x > 800 - radius) speed.x *= -1;
        if (ball.y < radius) speed.y *= -1;

        // 바닥
        if (ball.y > 600) {
            ball = (Vector2){ 400, 300 };
            heart--;
        }

        // 패들 충돌
        if (CheckCollisionCircleRec(ball, radius, colis)) {
            speed.y *= -1;
            ball.y = 540;
        }

        // 블록 충돌
        for (int i = 0; i < MAX_BLOCKS; i++) {
            if (active[i] && CheckCollisionCircleRec(ball, radius, blocks[i])) {
                active[i] = false;
                speed.y *= -1;
                block_num--;
                break;
            }
        }

        // 그리기
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangleRec(paddle, WHITE);
        DrawRectangleRec(colis, WHITE);
        DrawCircleV(ball, radius, WHITE);
        for (int i = 0; i < MAX_BLOCKS; i++)
            if (active[i]) DrawRectangleRec(blocks[i], colors[i / 12]);
        DrawText(TextFormat("%d", timer), 385, 10, 20, WHITE);
        DrawText(TextFormat("heart: %d", heart), 600, 10, 20, WHITE);
        EndDrawing();

        //타이머
        if (block_num > 0 && heart != 0) set++;
        if (set == 60) {
            set = 0;
            timer++;
        }

        //확인용
        //if (IsKeyDown(KEY_A)) block_num = 0;


        //게임 오버
        if (block_num == 0) {
            BeginDrawing();
            DrawText(TextFormat("CLEAR"), 240, 200, 100, WHITE);
            DrawText(TextFormat("SCORE: %d", timer), 130, 300, 100, WHITE);
            ball.x = -100;
            ball.y = -100;
            BeginDrawing();
            DrawRectangleRec(retry, WHITE);
            DrawRectangleRec(end, WHITE);
            DrawText(TextFormat("retry"), 220, 425, 40, RED);
            DrawText(TextFormat("quit"), 490, 425, 40, RED);
        }
        if (heart == 0) {
            BeginDrawing();
            DrawText(TextFormat("GAME OVER"), 100, 200, 100, WHITE);
            ball.x = -100;
            ball.y = -100;
            BeginDrawing();
            DrawRectangleRec(retry, WHITE);
            DrawRectangleRec(end, WHITE);
            DrawText(TextFormat("retry"), 220, 425, 40, RED);
            DrawText(TextFormat("quit"), 490, 425, 40, RED);
        }
        //종료
        if (block_num == 0 || heart == 0) {
            if (CheckCollisionPointRec(GetMousePosition(), end)) {
                DrawRectangleLinesEx(end, 3, BLUE);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    break;
                }
            }
        }
        //재시작
        if (block_num == 0 || heart == 0) {
            if (CheckCollisionPointRec(GetMousePosition(), retry)) {
                DrawRectangleLinesEx(retry, 3, BLUE);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    CloseWindow();
                    return main();
                }
            }
        }
    }

    CloseWindow();
    return 0;
}