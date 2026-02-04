#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

void hideCursor();

// player movements
void movePlayerRight(int &player_x, int player_y, int &score, int &lives);
void movePlayerLeft(int &player_x, int player_y, int &score, int &lives);
void handleJumpInput(int player_x, int player_y, int &jumpSteps);
void applyJump(int &player_x, int &player_y, int &jumpSteps, int &score, int &lives);
void applyGravity(int &player_x, int &player_y, int &score, int &lives, int jumpSteps);

// enemy movements
void moveEnemies(int &enemy1_x, int enemy1_y, int &d_enemy1, int &enemy2_x, int enemy2_y, int &d_enemy2, int &enemy3_x, int enemy3_y, int &d_enemy3);
// collision
void checkPassiveHit(int &player_x, int &player_y, int &lives);
void handleReset(int &player_x, int &player_y, int &lives);

// functionalities
void gotoxy(int x, int y);
char getCharAtxy(int x, int y);
void print_title();
void printMaze();
void draw_player(int x, int y);
void remove_player(int x, int y);
void draw_enemy1(int x, int y);
void draw_enemy2(int x, int y);
void draw_enemy3(int x, int y);
void remove_enemy_1(int x, int y);
void remove_enemy_2(int x, int y);
void remove_enemy_3(int x, int y);
void print_bonus();
void display_instructions();
bool collision(char nextHead, char nextFeet);

bool checkGameStatus(int score, int lives);

int main()
{
    // 1. Initial System Setup
    hideCursor(); // Hides the blinking cursor

    int menuOption = 0;

    // 2. The Main Program Loop (Returns here after every game)
    while (menuOption != 4)
    {
        system("cls");
        print_title();

        // Menu Display
        gotoxy(50, 16);
        cout << "1. Play Game";
        gotoxy(50, 17);
        cout << "2. Instructions";
        gotoxy(50, 18);
        cout << "3. Options";
        gotoxy(50, 19);
        cout << "4. Exit";
        gotoxy(50, 21);
        cout << "Select Option: ";
        cin >> menuOption;

        if (menuOption == 1)
        {
            // --- 3. DECLARE & INITIALIZE VARIABLES ---
            // These reset to default every time a new game starts
            int player_x = 5, player_y = 30;
            int lives = 3, score = 0, jumpSteps = 0;

            int enemy1_x = 5, enemy1_y = 4, d_enemy1 = 1;
            int enemy2_x = 80, enemy2_y = 12, d_enemy2 = -1;
            int enemy3_x = 5, enemy3_y = 20, d_enemy3 = 1;

            // --- 4. GAME INITIALIZATION ---
            system("cls");
            printMaze();
            print_bonus();
            draw_player(player_x, player_y);

            // --- 5. THE GAMEPLAY LOOP ---
            while (true)
            {
                // UI: Display Score/Lives on the side
                gotoxy(95, 5);
                cout << "Score: " << score << "  ";
                gotoxy(95, 6);
                cout << "Lives: " << lives << "  ";

                // Player Input & Movement
                movePlayerRight(player_x, player_y, score, lives);
                movePlayerLeft(player_x, player_y, score, lives);
                handleJumpInput(player_x, player_y, jumpSteps);

                // Physics
                applyJump(player_x, player_y, jumpSteps, score, lives);
                applyGravity(player_x, player_y, score, lives, jumpSteps);

                // Enemies & Hits
                moveEnemies(enemy1_x, enemy1_y, d_enemy1, enemy2_x, enemy2_y, d_enemy2, enemy3_x, enemy3_y, d_enemy3);
                checkPassiveHit(player_x, player_y, lives);

                // Check Win/Loss (Returns true to break loop)
                if (checkGameStatus(score, lives))
                {
                    break;
                }

                Sleep(30); // Controls game speed
            }
        }
        else if (menuOption == 2)
        {
            // Call your instructions function here
            display_instructions();
        }
        else if (menuOption == 3)
        {
            // Placeholder for your options function
            system("cls");
            cout << "Options Menu coming soon! Press any key...";
            _getch();
        }
    }

    return 0;
}
void movePlayerRight(int &player_x, int player_y, int &score, int &lives)
{
    if (GetAsyncKeyState(VK_RIGHT))
    {
        char nextHead = getCharAtxy(player_x + 3, player_y);
        char nextFeet = getCharAtxy(player_x + 3, player_y + 1);

        // 1. COLLECTION: If either part of the player hits 'X'
        if (nextHead == 'X' || nextFeet == 'X')
        {
            score++;
            // Erase the X from the screen
            if (nextHead == 'X')
            {
                gotoxy(player_x + 3, player_y);
                cout << " ";
            }
            else
            {
                gotoxy(player_x + 3, player_y + 1);
                cout << " ";
            }
        }

        // 2. MOVEMENT: Move if it's space or an 'X' (already erased)
        if (!collision(nextHead, nextFeet))
        {
            remove_player(player_x, player_y);
            player_x++;
            draw_player(player_x, player_y);
        }
        // 3. HAZARD: If it's not a wall, space, or item, it's a hazard (Enemy)
        else if (player_x >= 84 || (nextHead != '#' && nextHead != ' ' && nextHead != 'X'))
        {
            handleReset(player_x, player_y, lives);
        }
    }
}

void movePlayerLeft(int &player_x, int player_y, int &score, int &lives)
{
    if (GetAsyncKeyState(VK_LEFT))
    {
        char nextHead = getCharAtxy(player_x - 1, player_y);
        char nextFeet = getCharAtxy(player_x - 1, player_y + 1);

        // 1. COLLECTION: If either part of the player hits 'X'
        if (nextHead == 'X' || nextFeet == 'X')
        {
            score++;
            // Erase the X from the screen
            if (nextHead == 'X')
            {
                gotoxy(player_x - 1, player_y);
                cout << " ";
            }
            else
            {
                gotoxy(player_x - 1, player_y + 1);
                cout << " ";
            }
        }

        // 2. MOVEMENT: Move if it's space or an 'X'
        if (!collision(nextHead, nextFeet))
        {
            remove_player(player_x, player_y);
            player_x--;
            draw_player(player_x, player_y);
        }
        // 3. HAZARD: If it's not a wall, space, or item, it's a hazard (Enemy)
        else if (player_x <= 1 || (nextHead != '#' && nextHead != ' ' && nextHead != 'X'))
        {
            handleReset(player_x, player_y, lives);
        }
    }
}
void handleJumpInput(int player_x, int player_y, int &jumpSteps)
{
    if (GetAsyncKeyState(VK_SPACE))
    {
        char groundL = getCharAtxy(player_x, player_y + 2);
        char groundR = getCharAtxy(player_x + 2, player_y + 2);

        // Only jump if standing on a base (#)
        if ((groundL == '#' || groundR == '#') && jumpSteps == 0)
        {
            // INCREASED jumpSteps to 10 to make sure you can reach the platforms
            jumpSteps = 10;
        }
    }
}

void applyJump(int &player_x, int &player_y, int &jumpSteps, int &score, int &lives)
{
    if (jumpSteps > 0)
    {
        char nextHead = getCharAtxy(player_x, player_y - 1);
        char nextFeet = getCharAtxy(player_x + 2, player_y - 1);

        // --- COLLECTION LOGIC ---
        if (nextHead == 'X' || nextFeet == 'X')
        {
            score++;
            // Remove the X from the screen so it's not counted twice
            if (nextHead == 'X')
            {
                gotoxy(player_x, player_y - 1);
                cout << " ";
            }
            else
            {
                gotoxy(player_x + 2, player_y - 1);
                cout << " ";
            }
        }

        if ((nextHead == ' ' || nextHead == 'X') && (nextFeet == ' ' || nextFeet == 'X'))
        {
            remove_player(player_x, player_y);
            player_y--;
            draw_player(player_x, player_y);
            jumpSteps--;
        }
        else
        {
            jumpSteps = 0;
            if (nextHead == '[' || nextHead == 'O' || nextHead == '|' ||
                nextFeet == '[' || nextFeet == 'O' || nextFeet == '|')
            {
                handleReset(player_x, player_y, lives);
            }
        }
    }
}

void applyGravity(int &player_x, int &player_y, int &score, int &lives, int jumpSteps)
{
    if (jumpSteps == 0)
    {
        char belowL = getCharAtxy(player_x, player_y + 2);
        char belowR = getCharAtxy(player_x + 2, player_y + 2);

        // --- COLLECTION LOGIC ---
        if (belowL == 'X' || belowR == 'X')
        {
            score++;
            if (belowL == 'X')
            {
                gotoxy(player_x, player_y + 2);
                cout << " ";
            }
            else
            {
                gotoxy(player_x + 2, player_y + 2);
                cout << " ";
            }
        }

        if ((belowL == ' ' || belowL == 'X') && (belowR == ' ' || belowR == 'X'))
        {
            remove_player(player_x, player_y);
            player_y++;
            draw_player(player_x, player_y);
        }
        else if (belowL != '#' && belowR != '#' && belowL != ' ' && belowR != ' ')
        {
            handleReset(player_x, player_y, lives);
        }
    }
}

void gotoxy(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
char getCharAtxy(int x, int y)
{
    CHAR_INFO ci;
    COORD xy = {0, 0};
    SMALL_RECT rect = {(short)x, (short)y, (short)x, (short)y};
    COORD coordBufSize;
    coordBufSize.X = 1;
    coordBufSize.Y = 1;
    return ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), &ci, coordBufSize, xy, &rect) ? ci.Char.AsciiChar
                                                                                            : ' ';
}
void print_title()
{

    gotoxy(50, 10);
    cout << "\e[0;31m" << " ____        _ _      _     _   _      _ _" << "\e[0m" << endl;
    gotoxy(50, 11);
    cout << "\e[0;31m" << "| __ ) _   _| | | ___| |_  | | | | ___| | |" << "\e[0m" << endl;
    gotoxy(50, 12);
    cout << "\e[0;31m" << "|  _ \\| | | | | |/ _ \\ __| | |_| |/ _ \\ | |" << "\e[0m" << endl;
    gotoxy(50, 13);
    cout << "\e[0;31m" << "| |_) | |_| | | |  __/ |_  |  _  |  __/ | |" << "\e[0m" << endl;
    gotoxy(50, 14);
    cout << "\e[0;31m" << "|____/ \\__,_|_|_|\\___|\\__| |_| |_|\\___|_|_|" << "\e[0m" << endl;
}
void printMaze()
{
    string red = "\e[0;31m";
    string reset = "\e[0m";
    cout << "#########################################################################################" << endl; // 88
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "#################################################################################      " << red << "#" << reset << endl; // 8 //4 space after
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "     ################################################################################# " << red << "#" << reset << endl; // 8
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "#################################################################################      " << red << "#" << reset << endl; // 8
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << red << "#" << reset << "                                                                                       " << red << "#" << reset << endl;
    cout << "#########################################################################################" << endl; // 8
}
void draw_enemy1(int x, int y)
{
    gotoxy(x, y);
    cout << "\e[0;31m" << "[OO]" << "\e[0m";
    gotoxy(x, y + 1);
    cout << "\e[0;31m" << "|##|" << "\e[0m";
    gotoxy(x, y + 2);
    cout << "\e[0;31m" << "|##|" << "\e[0m";
    gotoxy(x, y + 3);
    cout << "\e[0;31m" << "/--\\" << "\e[0m";
}
void draw_enemy2(int x, int y)
{
    gotoxy(x, y);
    cout << "\e[0;31m" << "[OO]" << "\e[0m";
    gotoxy(x, y + 1);
    cout << "\e[0;31m" << "|##|" << "\e[0m";
    gotoxy(x, y + 2);
    cout << "\e[0;31m" << "|##|" << "\e[0m";
    gotoxy(x, y + 3);
    cout << "\e[0;31m" << "/--\\" << "\e[0m";
}
void draw_enemy3(int x, int y)
{
    gotoxy(x, y);
    cout << "\e[0;31m" << "[OO]" << "\e[0m";
    gotoxy(x, y + 1);
    cout << "\e[0;31m" << "|##|" << "\e[0m";
    gotoxy(x, y + 2);
    cout << "\e[0;31m" << "|##|" << "\e[0m";
    gotoxy(x, y + 3);
    cout << "\e[0;31m" << "/--\\" << "\e[0m";
}
void remove_enemy_1(int x, int y)
{

    gotoxy(x, y);
    cout << "    ";
    gotoxy(x, y + 1);
    cout << "    ";
    gotoxy(x, y + 2);
    cout << "    ";
    gotoxy(x, y + 3);
    cout << "    ";
}
void remove_enemy_2(int x, int y)
{
    gotoxy(x, y);
    cout << "    ";
    gotoxy(x, y + 1);
    cout << "    ";
    gotoxy(x, y + 2);
    cout << "    ";
    gotoxy(x, y + 3);
    cout << "    ";
}
void remove_enemy_3(int x, int y)
{
    gotoxy(x, y);
    cout << "    ";
    gotoxy(x, y + 1);
    cout << "    ";
    gotoxy(x, y + 2);
    cout << "    ";
    gotoxy(x, y + 3);
    cout << "    ";
}
void draw_player(int x, int y)
{
    gotoxy(x, y);
    cout << "\e[0;32m" << "(0)" << "\e[0m";
    gotoxy(x, y + 1);
    cout << "\e[0;32m" << "/ \\" << "\e[0m";
}
void remove_player(int x, int y)
{
    gotoxy(x, y);
    cout << "   ";
    gotoxy(x, y + 1);
    cout << "   ";
}
bool collision(char nextHead, char nextFeet)
{
    if (nextHead == '#' || nextHead == '[' || nextHead == 'O' || nextHead == '|' || nextFeet == '#' || nextFeet == '[' || nextFeet == 'O' || nextFeet == '|')
        return true;
    return false;
}

void print_bonus()
{
    gotoxy(5, 2);
    cout << "\e[0;33m" << "X" << "\e[0m";
    for (int i = 10; i < 80; i += 5)
    {
        gotoxy(i, 2);
        cout << "\e[0;33m" << "X" << "\e[0m";
    }
    for (int j = 5; j < 80; j += 5)
    {
        gotoxy(j, 10);
        cout << "\e[0;33m" << "X" << "\e[0m";
    }
    for (int j = 5; j < 80; j += 5)
    {
        gotoxy(j, 18);
        cout << "\e[0;33m" << "X" << "\e[0m";
    }
    for (int j = 5; j < 80; j += 5)
    {
        gotoxy(j, 26);
        cout << "\e[0;33m" << "X" << "\e[0m";
    }
}

void checkPassiveHit(int &player_x, int &player_y, int &lives)
{
    char currentHead = getCharAtxy(player_x, player_y);
    char currentFeet = getCharAtxy(player_x, player_y + 1);

    if (currentHead == '[' || currentHead == 'O' || currentHead == '|' ||
        currentFeet == '[' || currentFeet == 'O' || currentFeet == '|')
    {
        handleReset(player_x, player_y, lives);
    }
}

void moveEnemies(int &enemy1_x, int enemy1_y, int &d_enemy1, int &enemy2_x, int enemy2_y, int &d_enemy2, int &enemy3_x, int enemy3_y, int &d_enemy3)
{

    remove_enemy_1(enemy1_x, enemy1_y);
    // Look ahead: If moving right, check x+4. If moving left, check x-1.
    if (d_enemy1 == 1 && getCharAtxy(enemy1_x + 4, enemy1_y) == '#')
        d_enemy1 = -1;
    else if (d_enemy1 == -1 && getCharAtxy(enemy1_x - 1, enemy1_y) == '#')
        d_enemy1 = 1;
    enemy1_x += d_enemy1;
    draw_enemy1(enemy1_x, enemy1_y);

    // --- ENEMY 2 LOGIC ---
    remove_enemy_2(enemy2_x, enemy2_y);
    if (d_enemy2 == 1 && getCharAtxy(enemy2_x + 4, enemy2_y) == '#')
        d_enemy2 = -1;
    else if (d_enemy2 == -1 && getCharAtxy(enemy2_x - 1, enemy2_y) == '#')
        d_enemy2 = 1;
    enemy2_x += d_enemy2;
    draw_enemy2(enemy2_x, enemy2_y);

    // --- ENEMY 3 LOGIC ---
    remove_enemy_3(enemy3_x, enemy3_y);
    if (d_enemy3 == 1 && getCharAtxy(enemy3_x + 4, enemy3_y) == '#')
        d_enemy3 = -1;
    else if (d_enemy3 == -1 && getCharAtxy(enemy3_x - 1, enemy3_y) == '#')
        d_enemy3 = 1;
    enemy3_x += d_enemy3;
    draw_enemy3(enemy3_x, enemy3_y);
}
bool checkGameStatus(int score, int lives)
{
    if (score >= 50) // Adjust this number based on how many 'X' you have
    {
        system("cls");
        gotoxy(45, 12);
        cout << "\e[0;32m" << "***********************************" << "\e[0m";
        gotoxy(45, 13);
        cout << "\e[0;32m" << "* CONGRATULATIONS!        *" << "\e[0m";
        gotoxy(45, 14);
        cout << "\e[0;32m" << "* YOU ARE A HERO!        *" << "\e[0m";
        gotoxy(45, 15);
        cout << "\e[0;32m" << "***********************************" << "\e[0m";
        gotoxy(45, 17);
        cout << "Final Score: " << score;
        Sleep(5000); // Give them 5 seconds to celebrate
        return true;
    }
    if (lives <= 0)
    {
        system("cls");
        gotoxy(50, 15);
        cout << "\033[34m===========================\033[0m" << endl;
        gotoxy(50, 16);
        cout << "\033[31mGAME OVER! Final Score: \033[0m" << score << endl;
        gotoxy(50, 17);
        cout << "\033[34m===========================\033[0m" << endl;
        Sleep(3000);
        return true; // Exits the main function and closes the game
    }
    return false;
}
void handleReset(int &player_x, int &player_y, int &lives)
{
    lives--;
    remove_player(player_x, player_y);
    player_x = 5;
    player_y = 30;
    draw_player(player_x, player_y);
}

void hideCursor()
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // Set the cursor visibility to false
    SetConsoleCursorInfo(out, &cursorInfo);
}
void display_instructions()
{
    system("cls"); // Clear the menu screen
    print_title(); // Show the game title at the top

    gotoxy(45, 10);
    cout << "--- MISSION BRIEFING ---";

    gotoxy(40, 12);
    cout << "-> Use LEFT and RIGHT ARROWS to move.";
    gotoxy(40, 13);
    cout << "-> Press SPACEBAR to jump.";

    gotoxy(40, 15);
    cout << "-> Collect '\e[0;33mX\e[0m' to increase your SCORE.";
    gotoxy(40, 16);
    cout << "-> Avoid '\e[0;31m[OO]\e[0m' guards. Touching them costs a LIFE.";
    gotoxy(40, 17);
    cout << "-> Avoid '\e[0;31m#\e[0m' Boundaries. Touching them costs a LIFE.";

    gotoxy(45, 19);
    cout << "GOAL: Reach 50 points to WIN!";

    gotoxy(42, 23);
    cout << "Press any key to return to Main Menu...";

    _getch(); // Wait for user to read before going back
}