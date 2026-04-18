#include <nds.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAP_W 16
#define MAP_H 12
#define MAX_BOLTS 3
#define BOLT_LIFE 10

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int x;
    int y;
    int dx;
    int dy;
    int life;
    bool active;
} Bolt;

static const char *kMap[MAP_H] = {
    "################",
    "#M....#.....#..#",
    "#.##..#..##.#.##",
    "#....##.....#..#",
    "#.##....###....#",
    "#..####...##.#.#",
    "#......#.......#",
    "#.####.#.#####.#",
    "#....#.#.....#.#",
    "##.#.#.###.#.#.#",
    "#..#.....R.#..P#",
    "################"
};

static char mapData[MAP_H][MAP_W + 1];
static Point player;
static Point rune;
static Point portal;
static Bolt bolts[MAX_BOLTS];
static int hp = 3;
static int frameCounter = 0;
static bool won = false;
static bool lost = false;

static bool is_wall(int x, int y)
{
    if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H)
        return true;

    return mapData[y][x] == '#';
}

static bool is_bolt_at(int x, int y)
{
    for (int i = 0; i < MAX_BOLTS; i++)
    {
        if (bolts[i].active && bolts[i].x == x && bolts[i].y == y)
            return true;
    }

    return false;
}

static void reset_level(void)
{
    for (int y = 0; y < MAP_H; y++)
    {
        strcpy(mapData[y], kMap[y]);

        for (int x = 0; x < MAP_W; x++)
        {
            if (mapData[y][x] == 'M')
            {
                player.x = x;
                player.y = y;
                mapData[y][x] = '.';
            }
            else if (mapData[y][x] == 'R')
            {
                rune.x = x;
                rune.y = y;
                mapData[y][x] = '.';
            }
            else if (mapData[y][x] == 'P')
            {
                portal.x = x;
                portal.y = y;
                mapData[y][x] = '.';
            }
        }
    }

    for (int i = 0; i < MAX_BOLTS; i++)
        bolts[i].active = false;

    hp = 3;
    frameCounter = 0;
    won = false;
    lost = false;
}

static void cast_spell(int dx, int dy)
{
    for (int i = 0; i < MAX_BOLTS; i++)
    {
        if (!bolts[i].active)
        {
            bolts[i].active = true;
            bolts[i].x = player.x + dx;
            bolts[i].y = player.y + dy;
            bolts[i].dx = dx;
            bolts[i].dy = dy;
            bolts[i].life = BOLT_LIFE;

            if (is_wall(bolts[i].x, bolts[i].y))
                bolts[i].active = false;

            return;
        }
    }
}

static void update_bolts(void)
{
    for (int i = 0; i < MAX_BOLTS; i++)
    {
        if (!bolts[i].active)
            continue;

        bolts[i].x += bolts[i].dx;
        bolts[i].y += bolts[i].dy;
        bolts[i].life--;

        if (is_wall(bolts[i].x, bolts[i].y) || bolts[i].life <= 0)
        {
            bolts[i].active = false;
            continue;
        }

        if (bolts[i].x == rune.x && bolts[i].y == rune.y)
        {
            won = true;
            bolts[i].active = false;
        }
    }
}

static void update_rune(void)
{
    if ((frameCounter % 18) != 0 || won || lost)
        return;

    int dx = 0;
    int dy = 0;

    if (player.x < rune.x)
        dx = -1;
    else if (player.x > rune.x)
        dx = 1;
    else if (player.y < rune.y)
        dy = -1;
    else if (player.y > rune.y)
        dy = 1;

    if (!is_wall(rune.x + dx, rune.y + dy))
    {
        rune.x += dx;
        rune.y += dy;
    }

    if (rune.x == player.x && rune.y == player.y)
    {
        hp--;
        if (hp <= 0)
            lost = true;
    }
}

static void try_move_player(int dx, int dy)
{
    int nx = player.x + dx;
    int ny = player.y + dy;

    if (won || lost)
        return;

    if (!is_wall(nx, ny))
    {
        player.x = nx;
        player.y = ny;
    }

    if (player.x == portal.x && player.y == portal.y)
        won = true;

    if (player.x == rune.x && player.y == rune.y)
    {
        hp--;
        if (hp <= 0)
            lost = true;
    }
}

static void draw_scene(void)
{
    consoleClear();

    iprintf("Crash Mage Trial\n");
    iprintf("D-Pad move  A cast  B reset\n");
    iprintf("Defeat R or reach P   HP:%d\n\n", hp);

    for (int y = 0; y < MAP_H; y++)
    {
        for (int x = 0; x < MAP_W; x++)
        {
            char ch = mapData[y][x];

            if (x == portal.x && y == portal.y)
                ch = 'P';
            if (x == rune.x && y == rune.y && !won)
                ch = 'R';
            if (is_bolt_at(x, y))
                ch = '*';
            if (x == player.x && y == player.y)
                ch = 'M';

            iprintf("%c", ch);
        }
        iprintf("\n");
    }

    iprintf("\nLegend: M=mage R=rune P=portal *=spell\n");

    if (won)
        iprintf("\nVictory. Press B to restart.\n");
    else if (lost)
        iprintf("\nYou got folded by the rune. Press B.\n");
    else
        iprintf("\nTip: A shoots the last move direction.\n");
}

int main(int argc, char **argv)
{
    consoleDemoInit();
    resetLevel:
    reset_level();

    int facingX = 1;
    int facingY = 0;

    while (1)
    {
        swiWaitForVBlank();
        scanKeys();

        int keys_down = keysDown();

        if (keys_down & KEY_B)
            goto resetLevel;

        if (keys_down & KEY_LEFT)
        {
            facingX = -1;
            facingY = 0;
            try_move_player(-1, 0);
        }
        else if (keys_down & KEY_RIGHT)
        {
            facingX = 1;
            facingY = 0;
            try_move_player(1, 0);
        }
        else if (keys_down & KEY_UP)
        {
            facingX = 0;
            facingY = -1;
            try_move_player(0, -1);
        }
        else if (keys_down & KEY_DOWN)
        {
            facingX = 0;
            facingY = 1;
            try_move_player(0, 1);
        }

        if ((keys_down & KEY_A) && !won && !lost)
            cast_spell(facingX, facingY);

        update_bolts();
        update_rune();
        draw_scene();

        frameCounter++;
    }
}
