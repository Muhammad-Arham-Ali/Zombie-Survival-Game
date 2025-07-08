#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

//-----------------Variables-------------------//

// Map
const int WIDTH = 40;
const int HEIGHT = 20;
char Map[HEIGHT][WIDTH];

// Player
int PlayerX = WIDTH / 2;
int PlayerY = HEIGHT / 2;
char bulletDirection = 'd';
int ammo = 20;
int lives = 3;

// Zombie
struct Zombie
{
    int x;
    int y;
};
const int MAX_ZOMBIES = 5;
Zombie zombies[MAX_ZOMBIES];
int zombieCount = 0;
int ZOMBIE_SPAWN_RATE = 5;
int zombieMoveCounter = 0;
int zombieMoveDelay = 10;

// Ammo
int ammospawnTimer = 0;
const int ammoDelay = 1;
const int AMMO_SPAWN_INTERVAL = 30000;
DWORD lastAmmoSpawnTime = GetTickCount();

// Game state
bool gameRunning = true;
int score = 0;
int level = 1;

//-----------------Function prototypes-----------------//

void MainMenu();
void LevelMenu();
void gotoxy(int x, int y);
char getCharAtxy(short int x, short int y);
void clearScreen();
void hideCursor();
void LoadLevel(int level);
void drawMap();
void printPlayer();
void erasePlayer();
void movePlayer(char dir);
void removeZombie(int x, int y);
void shoot();
void spawnZombies(int count);
void printZombies();
void eraseZombies();
void moveZombies();
bool isZombieAt(int x, int y);
bool playerCaught();
void checkGameOver();
void gameloop();
void spawnAmmo();
void collectAmmo();
void startGame();
void ExitGame();

//----------------Function Definitions-----------------//

// Console functions
void hideCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void gotoxy(int x, int y)
{
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearScreen()
{
    system("cls");
}

char getCharAtxy(short int x, short int y)

{
    CHAR_INFO ci;
    COORD xy = {0, 0};
    SMALL_RECT rect = {x, y, x, y};
    COORD coordBufSize;
    coordBufSize.X = 1;
    coordBufSize.Y = 1;
    return ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), &ci, coordBufSize, xy, &rect) ? ci.Char.AsciiChar : ' ';
}

// Menu Functions
void MainMenu()
{
    clearScreen();
    int choice;
    do
    {
        cout << "====== Zombie Survival ======" << endl
             << "1. Start Game" << endl
             << "2. Exit" << endl
             << "Select Option: ";
        cin >> choice;
    } while (choice < 1 || choice > 2);

    switch (choice)
    {
    case 1:
        LevelMenu();
        break;
    case 2:
        ExitGame();
        break;
    }
}

void LevelMenu()
{
    clearScreen();
    cout << "====== Select Level ======" << endl
         << "Level 1 - Easy" << endl
         << "Level 2 - Medium" << endl
         << "Level 3 - Hard" << endl
         << ""
         << "Select Option: ";
    int choice;
    cin >> choice;
    switch (choice)
    {
    case 1:
        level = 1;
        zombieMoveDelay = 5;
        ZOMBIE_SPAWN_RATE = 10;
        break;
    case 2:
        level = 2;
        zombieMoveDelay = 5;
        ZOMBIE_SPAWN_RATE = 10;
        break;
    case 3:
        level = 3;
        zombieMoveDelay = 4;
        ZOMBIE_SPAWN_RATE = 10;
        break;
    }
    startGame();
}

void GameOver()
{
    PlaySound(NULL, 0, 0);
    clearScreen();
    cout << "====== Game Over ======" << endl;
    cout << "Final Score: " << score << endl;
    if (score >= 500)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        cout << "Congratulations! You've completed the level" << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
    else
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        cout << "Better luck next time!" << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
    cout << "Press any key to return to the main menu..." << endl;
    _getch();
    Sleep(2000);
    clearScreen();
    MainMenu();
}

// level function
void LoadLevel(int level)
{
    string filename = "level" + to_string(level) + ".txt";
    ifstream fin(filename);
    if (!fin)
    {
        cout << "Error: Could not load " << filename << endl;
        gameRunning = false;
        return;
    }
    string line;
    for (int y = 0; y < HEIGHT; y++)
    {
        if (getline(fin, line) && line.length() >= WIDTH)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                Map[y][x] = line[x];
            }
        }
    }
    fin.close();
}

void drawMap()
{
    clearScreen();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            cout << Map[y][x] << " ";
        }
        cout << endl;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    printPlayer();
}

void HUB()
{
    int hudX = (WIDTH * 2) + 5;
    int hudY = 2;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    gotoxy(hudX - 2, hudY - 1);
    cout << "=====================";
    gotoxy(hudX - 2, hudY);
    cout << "     GAME STATUS     ";
    gotoxy(hudX - 2, hudY + 1);
    cout << "=====================";

    gotoxy(hudX, hudY + 3);
    cout << "Level: " << level << "    ";
    gotoxy(hudX, hudY + 5);
    cout << "Score: " << score << "    ";
    gotoxy(hudX, hudY + 7);
    cout << "Lives: " << lives << "    ";
    gotoxy(hudX, hudY + 9);
    cout << "Ammo:  " << ammo << "    ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

bool spacefree(int x, int y)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return false;
    if (Map[y][x] == '#')
        return false;
    return true;
}

// Player functions
void printPlayer()
{
    gotoxy(PlayerX * 2, PlayerY);
    cout << "😎";
}

void erasePlayer()
{
    gotoxy(PlayerX * 2, PlayerY);
    cout << "  ";
}

void movePlayer(char dir)
{
    erasePlayer();
    int x = PlayerX;
    int y = PlayerY;

    switch (dir)
    {
    case 'w':
        y--;
        break;
    case 's':
        y++;
        break;
    case 'a':
        x--;
        break;
    case 'd':
        x++;
        break;
    }

    if (spacefree(x, y))
    {
        PlayerX = x;
        PlayerY = y;
        collectAmmo();
    }
    printPlayer();
}

void shoot()
{
    if (ammo <= 0)
        return;

    int bx = PlayerX;
    int by = PlayerY;
    ammo--;

    while (true)
    {
        switch (bulletDirection)
        {
        case 'w':
            by--;
            break;
        case 's':
            by++;
            break;
        case 'a':
            bx--;
            break;
        case 'd':
            bx++;
            break;
        }

        if (!spacefree(bx, by) || Map[by][bx] == 'O')
        {
            break;
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        gotoxy(bx * 2, by);
        cout << "*";
        Sleep(50);
        gotoxy(bx * 2, by);
        cout << " ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

        // Check for zombie hits
        if (isZombieAt(bx, by))
        {
            gotoxy(bx * 2, by);
            cout << "💥";
            Sleep(1000);
            gotoxy(bx * 2, by);
            cout << "  ";
            removeZombie(bx, by);
            score += 10;
            return;
        }
    }
}

void printZombies()
{
    for (int i = 0; i < zombieCount; i++)
    {
        gotoxy(zombies[i].x * 2, zombies[i].y);
        cout << "🧟";
    }
}

void eraseZombies()
{
    for (int i = 0; i < zombieCount; i++)
    {
        gotoxy(zombies[i].x * 2, zombies[i].y);
        cout << "  ";
    }
}

bool isZombieAt(int x, int y)
{
    for (int i = 0; i < zombieCount; i++)
    {
        if (zombies[i].x == x && zombies[i].y == y)
            return true;
    }
    return false;
}

void spawnZombies(int count)
{
    for (int i = 0; i < count && zombieCount < MAX_ZOMBIES; i++)
    {
        Zombie z;
        bool spawned = false;
        int attempts = 0;

        do
        {
            int edge = rand() % 4;
            switch (edge)
            {
            case 0:
                z.x = rand() % WIDTH;
                z.y = 1;
                break;
            case 1:
                z.x = WIDTH - 1;
                z.y = rand() % HEIGHT;
                break;
            case 2:
                z.x = rand() % WIDTH;
                z.y = HEIGHT - 1;
                break;
            case 3:
                z.x = 1;
                z.y = rand() % HEIGHT;
                break;
            }

            if (spacefree(z.x, z.y) && !isZombieAt(z.x, z.y) && Map[z.y][z.x] != 'O')
            {
                zombies[zombieCount] = z;
                zombieCount++;
                spawned = true;
            }
            attempts++;
        } while (!spawned && attempts < 100);
    }
}

void moveZombies()
{
    if (++zombieMoveCounter < zombieMoveDelay)
        return;

    zombieMoveCounter = 0;
    eraseZombies();

    for (int i = 0; i < zombieCount; i++)
    {
        int x = zombies[i].x;
        int y = zombies[i].y;

        if (zombies[i].x < PlayerX)
            x++;
        else if (zombies[i].x > PlayerX)
            x--;

        if (zombies[i].y < PlayerY)
            y++;
        else if (zombies[i].y > PlayerY)
            y--;

        if (spacefree(x, y) && !isZombieAt(x, y) && Map[y][x] != 'O')
        {
            zombies[i].x = x;
            zombies[i].y = y;
        }
    }
    printZombies();
}

void removeZombie(int x, int y)
{
    for (int i = 0; i < zombieCount; i++)
    {
        if (zombies[i].x == x && zombies[i].y == y)
        {
            zombies[i] = zombies[zombieCount - 1];
            zombieCount--;
            return;
        }
    }
}

// ammo functions
void spawnAmmo()
{
    int x, y, attempts = 0;
    bool found = false;
    do
    {
        x = rand() % WIDTH;
        y = rand() % HEIGHT;
        if (spacefree(x, y))
        {
            found = true;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            Map[y][x] = 'O';
            gotoxy(x * 2, y);
            cout << "O ";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        }
        attempts++;
    } while (!found && attempts < 100);
}

void collectAmmo()
{
    if (Map[PlayerY][PlayerX] == 'O')
    {
        ammo += 10;
        Map[PlayerY][PlayerX] = ' ';
    }
}

// game logics
bool playerCaught()
{
    for (int i = 0; i < zombieCount; i++)
    {
        if (zombies[i].x == PlayerX && zombies[i].y == PlayerY)
        {
            gotoxy(PlayerX * 2, PlayerY);
            cout << "💀";
            Sleep(1000);
            erasePlayer();
            lives--;

            PlayerX = WIDTH / 2;
            PlayerY = HEIGHT / 2;
            ammo = 10;

            eraseZombies();
            zombieCount = 0;

            drawMap();
            printPlayer();
            HUB();

            return true;
        }
    }
    return false;
}

void checkGameOver()
{
    if (lives <= 0 || score >= 500)
    {
        gameRunning = false;
        GameOver();
    }
}

void gameloop()
{
    PlaySound(TEXT("zombie.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    while (gameRunning)
    {

        if (_kbhit())
        {
            char key = _getch();
            if (key == 'w' || key == 'a' || key == 's' || key == 'd') // Player movement input
            {
                movePlayer(key);
            }
            else if (key == -32 || key == 224) // bullet direction input
            {
                int input = _getch();
                switch (input)
                {
                case 72:
                    bulletDirection = 'w';
                    break;
                case 80:
                    bulletDirection = 's';
                    break;
                case 75:
                    bulletDirection = 'a';
                    break;
                case 77:
                    bulletDirection = 'd';
                    break;
                }
            }
            else if (key == 27)
            {
                gameRunning = false;
                PlaySound(NULL, 0, 0);
                clearScreen();
                MainMenu();
            }
            else if (key == ' ')
            {
                shoot();
            }
        }
        eraseZombies();
        moveZombies();
        printZombies();
        HUB();
        checkGameOver();
        if (playerCaught())
        {
            checkGameOver();
        }

        if (rand() % 100 < ZOMBIE_SPAWN_RATE)
        {
            spawnZombies(1);
        }

        DWORD currentTime = GetTickCount();
        if (currentTime - lastAmmoSpawnTime >= AMMO_SPAWN_INTERVAL)
        {
            spawnAmmo();
            lastAmmoSpawnTime = currentTime;
        }

        Sleep(50);
    }
}

void startGame()
{
    clearScreen();
    gameRunning = true;
    score = 0;
    lives = 3;
    ammo = 20;
    PlayerX = WIDTH / 2;
    PlayerY = HEIGHT / 2;
    zombieCount = 0;
    bulletDirection = 'd';
    LoadLevel(level);
    drawMap();
    HUB();
    gameloop();
}

void ExitGame()
{
    exit(0);
}

int main()
{
    system("chcp 65001 > nul");
    SetConsoleOutputCP(CP_UTF8);
    srand(static_cast<unsigned>(time(0)));
    hideCursor();

    MainMenu();
    return 0;
}
