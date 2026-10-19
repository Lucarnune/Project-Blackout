#include<raylib.h>
#include<iostream>
class Player
{
    public:
    float x, y;
    int speed;
    int width_p, height_p;
    int health;
    bool dead;
    void Update()
    {
        if(!dead)
        {
            if(IsKeyDown(KEY_A))
            {
                x -= speed;
            }
            if(IsKeyDown(KEY_D))
            {
                x += speed;
            }
            if(IsKeyDown(KEY_W))
            {
                y -= speed;
            }
            if(IsKeyDown(KEY_S))
            {
                y += speed;
            }
        }
    }
    void Lose()
    {
        if(health <= 0)
        {
            health = 0;
            dead = true;
        }
    }
    Rectangle Playerhitbox()
    {
        return { x, y, width_p, height_p };
    }
    void LimitMovement()
    {
        if(y <= 0)
        {
            y = 0;
        }
        if(y + height_p >= GetScreenHeight())
        {
            y = GetScreenHeight() - height_p;
        }
        if(x <= 0)
        {
            x = 0;
        }
        if(x + width_p >= GetScreenWidth())
        {
            x = GetScreenWidth() - width_p;
        }
    }
    void Draw()
    {
        DrawRectangle(x, y, width_p, height_p, RAYWHITE);
    }
};
class CPU: public Player
{
public:
void findPlayer(Player target)
{
        if(target.x >= x) x += speed;
        if(target.x <= x) x -= speed;
        if(target.y >= y) y += speed;
        if(target.y <= y) y -= speed;
}
Rectangle CPUhitbox()
{
    return { x, y, width_p, height_p };
}
void Draw()
{
    DrawRectangle(x, y, width_p, height_p, RED);
}
};
Player player;
CPU cpu;
int main()
{
    const int width = 1280;
    const int height = 800;
    player.x = width/2 - 50;
    player.y = height/2;
    player.speed = 10;
    player.width_p = 45;
    player.height_p = 45;
    player.health = 100;
    player.dead = false;
    cpu.x = width/2 + 50;
    cpu.y = width/2;
    cpu.speed = 6;
    cpu.width_p = 45;
    cpu.height_p = 45;
    float survivalTimer = 0.0f;
    float damageTimer = 1.0f;
    InitWindow(width, height, "Blackout");
    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        //Updateing
        player.Update();
        player.LimitMovement();
        cpu.findPlayer(player);
        player.Lose();
        damageTimer -= GetFrameTime();
        survivalTimer += GetFrameTime();
        if(CheckCollisionRecs(cpu.CPUhitbox(), player.Playerhitbox()) && damageTimer <= 0)
        {
            player.health -= 15;
            damageTimer = 1.0f;
        }
        //Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("Health: %i", (int)player.health), width - 1230, 50, 50, RAYWHITE);
        DrawText(TextFormat("Time: %i", (int)survivalTimer), width - 500, 50, 50, RAYWHITE);
        if(player.dead)
        {
            DrawText("GAME OVER", width/2 - 500, height/2, 100, RAYWHITE);
            DrawText("Any Key To Reset", width/2 - 500, height/2 + 300, 100, RAYWHITE);
            survivalTimer = survivalTimer;
        }
        if(player.dead && GetKeyPressed() > 0)
        {
            player.dead = false;
            player.health = 100;
            cpu.x = width/2;
            player.x = width/2;
            cpu.y = height/2 + 150;
            player.y = height/2;
            survivalTimer = 0.0f;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}