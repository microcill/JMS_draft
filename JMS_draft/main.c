#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_BLOCKS 36

// ���� ���� ���� ����
typedef enum GameScreen { TITLE, GAMEPLAY, GAMEOVER, WIN } GameScreen;


int main() {
    InitWindow(800, 600, "Block");
    SetTargetFPS(60);

    // �е�� ��
    Rectangle paddle = { 350, 550, 100, 20 };
    Rectangle colis = { 350, 550, 100, 3 };
    Rectangle retry = { 200, 400, 150, 100 };
    Rectangle end = { 450, 400, 150, 100 };
    Vector2 ball = { 400, 300 };
    Vector2 speed = { 5, -5 };
    float radius = 10;

    // ���� ���� �ʱ�ȭ: ���� ȭ�� (TITLE)
    GameScreen currentScreen = TITLE;

    //��Ÿ �Լ�
    int timer = 0;
    int set = 0;
    int block_num = 36;
    int heart = 3;

    // ��ϵ�
    Rectangle blocks[MAX_BLOCKS];
    Color colors[6] = { RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE };
    bool active[MAX_BLOCKS] = { 0 };
    for (int i = 0; i < MAX_BLOCKS; i++) {
        blocks[i] = (Rectangle){ 20 + (i % 12) * 63, 50 + (i / 12) * 30, 60, 20 };
        active[i] = true;
    }


    while (!WindowShouldClose()) {

        // �е� �̵�
        if (IsKeyDown(KEY_LEFT)) paddle.x -= 8;
        if (IsKeyDown(KEY_RIGHT)) paddle.x += 8;
        if (paddle.x > 700) paddle.x = 700;
        if (paddle.x < -5) paddle.x = -5;

        //�νĺ� �̵�
        if (IsKeyDown(KEY_LEFT)) colis.x -= 8;
        if (IsKeyDown(KEY_RIGHT)) colis.x += 8;
        if (colis.x > 700) colis.x = 700;
        if (colis.x < -5) colis.x = -5;

        // �� �̵�
        ball.x += speed.x;
        ball.y += speed.y;

        // �� �浹
        if (ball.x < radius || ball.x > 800 - radius) speed.x *= -1;
        if (ball.y < radius) speed.y *= -1;

        // �ٴ�
        if (ball.y > 600) {
            ball = (Vector2){ 400, 300 };
            heart--;
        }

        // �е� �浹
        if (CheckCollisionCircleRec(ball, radius, colis)) {
            speed.y *= -1;
            ball.y = 540;
        }

        // ��� �浹
        for (int i = 0; i < MAX_BLOCKS; i++) {
            if (active[i] && CheckCollisionCircleRec(ball, radius, blocks[i])) {
                active[i] = false;
                speed.y *= -1;
                block_num--;
                break;
            }
        }

        // �׸���
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

        //Ÿ�̸�
        if (block_num > 0 && heart != 0) set++;
        if (set == 60) {
            set = 0;
            timer++;
        }

        //Ȯ�ο�
        //if (IsKeyDown(KEY_A)) block_num = 0;


        //���� ����
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
        //����
        if (block_num == 0 || heart == 0) {
            if (CheckCollisionPointRec(GetMousePosition(), end)) {
                DrawRectangleLinesEx(end, 3, BLUE);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    break;
                }
            }
        }
        //�����
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