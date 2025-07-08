# 🧟 Zombie Survival Game

A complete C++ console-based survival shooter game where the player fights off zombies on a grid map. The player can move, shoot, collect ammo, and try to survive as long as possible while scoring points. The game features emoji-based visuals, background sound, level selection, and real-time zombie movement.

________________________________________

🚀 Features

• 🎮 Emoji-based player and zombies on a grid  
• 🧠 Basic zombie AI: Zombies chase the player  
• 🔫 Arrow-key-based bullet direction  
• 🧱 Level system with map loading from files  
• 💾 Ammo item spawning & collection  
• 🧍 Player lives and scoring system  
• 🔉 Background music using Windows sound API  
• 📋 Simple main menu and level selector  
• 🧠 Uses: 

    o 1D arrays and structs  
    o Console cursor control (gotoxy)  
    o Sleep, SetConsoleTextAttribute  
    o Keyboard input using `_kbhit()` and `_getch()`  
________________________________________

🧑‍💻 Technologies Used

• Language: C++  
• Compiler: g++ (GCC / MinGW)  
• Platform: Windows Console  
• APIs Used: `<windows.h>`, `<conio.h>`, `<mmsystem.h>`  

> ❗Note: Uses Windows-only functions, so not compatible with Linux/Mac.

________________________________________

📂 File Structure

📦 ZombieSurvivalGame  
 ┣ 📄 main.cpp  
 ┣ 📄 level1.txt  
 ┣ 📄 level2.txt  
 ┣ 📄 level3.txt  
 ┣ 📄 zombie.wav  
 ┣ 📄 README.md  

________________________________________

🔧 How to Run

    1. Compile with: g++ main.cpp -o ZombieGame -lwinmm
    2. Run Or use a C++ IDE like Code::Blocks or Visual Studio.  
> ❗Note: Make sure `zombie.wav` and level files are in the same folder as the `.exe` file.

________________________________________

📚 What You’ll Learn

• How to use Windows console functions for UI  
• Handling real-time user input in games  
• Managing entities (zombies, bullets, items) using arrays  
• Implementing basic enemy AI  
• Using sound in console apps  
• Structuring a game loop in C++  

________________________________________

🌱 Future Improvements

• Smarter zombie pathfinding (e.g., A* algorithm)  
• Different types of zombies (fast, exploding, etc.)  
• Add health kits and multiple weapons  
• Multiplayer mode (local or networked)  
• GUI version using SFML or SDL  
• Save/load game support  
• Leaderboard/high-score system  

________________________________________

🙌 Contributing

This game was created for learning purposes. Contributions for refactoring (e.g., OOP, classes, or game engine migration) are welcome!

