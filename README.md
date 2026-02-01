# Terminal-Angry-Birds
A console-based Angry Birds game implemented in C++ for a university Programming Basics final project.
🎮 Game Overview

Terminal Angry Birds is a text-based version of the popular physics-based puzzle game. Players shoot projectiles to destroy enemy structures using different types of shots with realistic physics simulation.
✨ Features

    Three Unique Levels

        Level 1: Tilted Towers (Easy)

        Level 2: Bowers Castle (Medium)

        Level 3: Twin Towers (Hard)

    Multiple Shot Types

        Normal Shot: Standard projectile

        Explosive Shot: Creates a 3x3 explosion on impact

        Bouncy Shot: Can bounce up to 3 times off walls

    Game Systems

        Realistic physics simulation with gravity

        Score tracking and star rating system

        Save/load game progress

        Level completion tracking

        Ammo limitation per level

🛠️ Installation & Compilation
Prerequisites

    Linux/Unix-based system

    C++ compiler

    Terminal

Setting up and running

```bash
git clone https://github.com/Rfannn/Terminal-Angry-Birds
cd Terminal-Angry-Birds
g++ -o game game.cpp
./game
```

💾 Save System

The game automatically saves progress to game_save_data.txt after completing each level. Saved data includes:

    Current level

    Total score

    Star ratings for each level

📁 Project Structure
```text
game.cpp              - Main game source code
game_save_data.txt    - Save file 
README.md            - This documentation
```
