# Frogger Game - OOP Project

## Overview

This is a modern C++ implementation of the classic Frogger arcade game using the SFML (Simple and Fast Multimedia Library). The game demonstrates object-oriented programming principles through encapsulation, abstraction, and proper class design.

## Game Description

In this game, players control frogs that must navigate through dangerous roads filled with cars and rivers with floating logs to reach the safety of the upper area. The game features:

- Single player or two-player competitive mode
- Score tracking based on survival time
- Dynamic obstacles (cars and logs) moving in alternating directions
- Collision detection for realistic gameplay
- Visual feedback for player status (winning, dying)
- Persistent score storage for successful runs

## Controls

### Player 1:

- W: Move Up
- S: Move Down
- A: Move Left
- D: Move Right

### Player 2:

- Up Arrow: Move Up
- Down Arrow: Move Down
- Left Arrow: Move Left
- Right Arrow: Move Right

## Gameplay Features

- **Dynamic Difficulty**: The game spawns cars and logs at intervals, creating a challenging environment that evolves as you play
- **Score System**: Players earn points based on how long they survive, but only when they're in the dangerous areas (not in the safe spawn zone)
- **Death System**: When a player dies, a message appears for a few seconds before removing their frog from the game
- **Multiplayer Competition**: In two-player mode, both players compete to reach the top with the highest score
- **Winner Determination**: If both players complete the level, the player with the highest score wins

## Game Elements

- **Cars**: Moving obstacles on the road that cause death on collision
- **Logs**: Floating platforms on the river that the frog must use to cross (falling in water causes death)
- **Safe Areas**: The starting area and the goal area are safe zones

## Technical Implementation

The game is built using:

- C++ with SFML for graphics, input handling, and audio
- Object-oriented design with classes for game entities
- Game state management for menus, gameplay, and game over screens
- File I/O for score persistence
- Sound effects for actions and events

## OOP Principles Demonstrated

- **Encapsulation**: Game objects encapsulate their state and behavior
- **Abstraction**: Complex game mechanics are abstracted into simple interfaces
- **Class Design**: Well-defined classes with single responsibilities
- **Resource Management**: Proper handling of game resources like textures and sounds

## How to Build and Run

1. Ensure you have SFML installed
2. Compile with:
   g++ OOP_Project.cpp Frog.cpp Log.cpp Car.cpp PlayerManager.cpp -I/usr/include/SFML -L/usr/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o OOP_Project
3. Run the executable:
   ./OOP_Project

## Copyright

© 2025 Sanaullah. All rights reserved.

This project is for educational purposes only. The Frogger game concept is © Konami.

SFML is distributed under the zlib/png license.

This implementation is not for commercial use and is meant to demonstrate object-oriented programming principles as part of a university project.
