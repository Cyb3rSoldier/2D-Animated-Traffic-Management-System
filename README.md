# 🚦 2D Animated Traffic Management System

A 2D animated traffic simulation project developed using **C++ and OpenGL/FreeGLUT** as part of the Computer Graphics and Multimedia Lab.

The project simulates a traffic management environment with animated vehicles, traffic signals, roads, buildings, and emergency ambulance movement. Users can interact with the simulation through keyboard controls to manage traffic flow, spawn vehicles, adjust simulation speed, and switch between day and night modes.

## 📌 Project Overview

The 2D Animated Traffic Management System demonstrates fundamental computer graphics concepts through an interactive traffic simulation.

It features a road intersection with multiple traffic directions, animated vehicles, traffic light signals, and an emergency ambulance system.

### ✨ Key Features

- **Animated Vehicles:** Cars, buses, trucks, and ambulances move through the road network.
- **Traffic Signal System:** Simulates traffic lights with red, yellow, and green phases.
- **Emergency Ambulance:** Ambulances receive simplified traffic priority.
- **Vehicle Spawning:** Add different types of vehicles during simulation.
- **Traffic Directions:** Select different vehicle entry directions.
- **Day/Night Mode:** Switch between daytime and nighttime environments.
- **Simulation Speed Control:** Increase or decrease vehicle movement speed.
- **Pause and Resume:** Pause or continue the simulation whenever needed.
- **Traffic Environment:** Includes roads, buildings, trees, and other visual elements.
- **Collision Avoidance:** Vehicles maintain distance from other vehicles and respond to traffic signals.

## 🛠️ Technologies Used

| Technology | Purpose |
|---|---|
| C++ | Core programming language |
| OpenGL | 2D graphics rendering |
| FreeGLUT | Window management, keyboard input, and rendering loop |
| GCC / G++ | C++ compiler |
| VS Code / Code::Blocks | Development environment |

## 📂 Project Structure

```text
2D-Traffic-Management-System/
│
├── main.cpp
├── traffic.exe
├── README.md
└── ...
```

> Keep only the source files and necessary project resources in your repository. You generally do not need to upload the compiled `traffic.exe` file.

## ⚙️ Requirements

Before running the project, make sure you have:

- Windows 10 or later
- C++ compiler (GCC/G++)
- OpenGL
- FreeGLUT
- VS Code or Code::Blocks

## 🚀 Installation and Setup

### 1. Clone the Repository

```bash
git clone https://github.com/YOUR-USERNAME/2D-Traffic-Management-System.git
```

### 2. Navigate to the Project Directory

```bash
cd 2D-Traffic-Management-System
```

### 3. Install FreeGLUT (MSYS2 UCRT64)

Open the MSYS2 UCRT64 terminal and execute:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-freeglut
```

### 4. Compile the Project

Run the following command from the project directory:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o traffic.exe -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32
```

### 5. Run the Application

```bash
./traffic.exe
```

The traffic simulation window should open.

## 🎮 Keyboard Controls

| Key | Function |
|---|---|
| `SPACE` | Pause / Resume simulation |
| `+` | Increase simulation speed |
| `-` | Decrease simulation speed |
| `A` | Spawn a random vehicle |
| `C` | Spawn a car |
| `B` | Spawn a bus |
| `T` | Spawn a truck |
| `E` | Spawn an ambulance |
| `1` | Select left-side entry |
| `2` | Select right-side entry |
| `3` | Select bottom-side entry |
| `4` | Select top-side entry |
| `Z` | Select a random entry direction |
| `N` | Toggle day/night mode |
| `R` | Reset vehicles |
| `ESC` | Exit application |

## 🧠 Concepts Demonstrated

This project demonstrates several fundamental concepts of computer graphics and programming:

- 2D coordinate systems
- OpenGL primitive-based drawing
- Object-oriented data organization using structures and enumerations
- Animation using timer callbacks
- Keyboard event handling
- Traffic signal state management
- Vehicle movement and direction handling
- Basic collision avoidance
- Dynamic object creation and removal
- Double buffering for smoother rendering

## 🎯 Project Objectives

1. Develop an interactive 2D traffic simulation using C++ and OpenGL.
2. Demonstrate vehicle animation and movement in multiple directions.
3. Implement a basic traffic signal management system.
4. Simulate emergency vehicle priority.
5. Apply fundamental computer graphics concepts in a practical project.


⭐ If you find this project interesting, consider giving the repository a star!
