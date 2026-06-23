# Space Shooter Game

A 2D Space Shooter game developed using C++ and SFML. Players control a spaceship, shoot incoming alien enemies, earn points, and survive as long as possible. The game features sprite-based rendering, collision detection, explosion effects, score tracking, lives management, and multiple game states.

## Features

* Player-controlled spaceship movement
* Bullet shooting system
* Random enemy spawning
* Collision detection between bullets and enemies
* Explosion effects on enemy destruction
* Score tracking system
* Lives system
* Main Menu and Game Over screens
* Custom sprites and fonts
* SFML-based graphics rendering

## Technologies Used

* C++
* SFML 2.6
* Object-Oriented Programming
* Game Loop Architecture
* Event Handling
* Collision Detection

## Controls

| Key             | Action       |
| --------------- | ------------ |
| A / Left Arrow  | Move Left    |
| D / Right Arrow | Move Right   |
| Space           | Shoot        |
| Enter           | Start Game   |
| R               | Restart Game |

## Project Structure

assets/

* player.png
* enemy1.png
* bullet.png
* explosion.png
* background.png
* font.ttf

main.cpp

## How to Run

1. Install SFML 2.6
2. Place all assets in the project directory.
3. Compile the project:

g++ main.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system

4. Run:

./game

## Learning Outcomes

* Sprite management and rendering
* Real-time game loop implementation
* Collision detection techniques
* Resource loading and management
* Game state management
* Event-driven programming in C++
