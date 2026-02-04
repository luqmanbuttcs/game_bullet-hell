Bullet Hell - Console Platformer

A classic C++ console-based platformer game featuring physics, enemy patrolling, and collectible-based progression. This game utilizes the Windows API for high-performance console rendering and real-time input handling.

🚀 Features

Dynamic Physics: Realistic gravity and jump mechanics that require precise timing to navigate platforms.

Patrolling Enemy AI: Three automated guards ([OO]) with distinct patrol paths and collision detection.

Collectible System: Collect yellow 'X' icons to boost your score and reach the "Hero" status.

Lives & Reset Logic: A 3-life system that resets the player to the start of the level upon hitting hazards.

Colorized Console UI: Full use of ANSI escape codes for a modern look in an old-school environment.

Instructional Menu: Integrated mission briefing to help new players get started.

🎮 Controls

The game uses real-time keyboard input for a responsive feel:

Action

Key

Move Left

Left Arrow

Move Right

Right Arrow

Jump

Spacebar

Navigate Menu

1, 2, 3, 4

🛠️ Requirements

Operating System: Windows (The game relies on windows.h for cursor control and conio.h for input).

Compiler: A C++ compiler like MinGW (GCC) or MSVC (Visual Studio).

Terminal: Windows Terminal is recommended for the best color support.

📝 Gameplay Rules

Objective: Gather 50 points to win.

Enemies: Avoid contact with guards ([OO]).

Boundaries: Do not touch the walls (#) or you will lose a life.

Gravity: You cannot walk on air; ensure there is a platform beneath you.

🏗️ Technical Highlights

Flicker Reduction: Uses hideCursor() and gotoxy() to update only specific parts of the screen rather than clearing the whole buffer.

Collision Engine: Uses getCharAtxy() to query the console buffer directly, allowing for pixel-perfect (character-perfect) collision detection without complex data structures.

State Management: A clean main loop that separates input handling, physics updates, and rendering.

🔧 Troubleshooting

Colors not showing? Older versions of cmd.exe may not support ANSI colors by default. Try running the game in Windows Terminal or a modern IDE console.

Game running too fast/slow? Adjust the Sleep(30); value in the gameplay loop in main.cpp to match your hardware's performance.

🗺️ Roadmap

1) Add multiple level maps.

2) Implement projectile shooting for the player.

3) Add sound effects using Beep() or PlaySound().

4) High score persistence (saving to a .txt file).

Developed as a C++ Technical Project.
