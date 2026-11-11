# 🚦 2D Animated Traffic Management System

A 2D animated traffic simulation project developed using **C++ and OpenGL/FreeGLUT** as part of the Computer Graphics and Multimedia Lab.

The project simulates a traffic management environment with animated vehicles, traffic signals, roads, buildings, and emergency ambulance movement. Users can interact with the simulation through keyboard controls to manage traffic flow, spawn vehicles, adjust simulation speed, and switch between day and night modes.

## 📌 Project Overview

The 2D Animated Traffic Management System demonstrates fundamental computer graphics concepts through an interactive traffic simulation.

It features a road intersection with multiple traffic directions, animated vehicles, traffic light signals, and an emergency ambulance system.

## ✨ Key Features

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
| Code::Blocks | Development environment |

## 📂 Project Structure

```text
2D-Traffic-Management-System/
│
├── main.cpp
├── README.md
└── ...
```

> You generally do not need to upload the compiled `traffic.exe` file to GitHub.

## ⚙️ Requirements

Before running the project, make sure you have:

- Windows 10 or later
- C++ compiler (GCC/G++)
- OpenGL
- FreeGLUT
- Code::Blocks or VS Code
- MSYS2 (recommended for installing GCC and FreeGLUT)

## 🚀 Installation and Setup

### Method 1: Using Code::Blocks

**Step 1: Install the required libraries**

Open the MSYS2 UCRT64 terminal and execute:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-freeglut
```

**Step 2: Configure the compiler**

Make sure Code::Blocks is configured to use the MSYS2 UCRT64 GCC compiler.

Compiler directory:

```text
C:\msys64\ucrt64
```

**Step 3: Create a new project**

1. Open Code::Blocks.
2. Select **File → New → Project**.
3. Choose **Console Application**.
4. Select **C++**.
5. Enter your project name and finish the setup.

**Step 4: Add the source file**

Add your `main.cpp` file to the project.

**Step 5: Configure linker settings**

1. Open **Project → Build options**.
2. Select your project name (not just an individual Debug or Release target).
3. Open the **Linker settings** tab.
4. Under **Link libraries**, add the following libraries:

```text
freeglut
opengl32
glu32
winmm
gdi32
```

**Step 6: Build and run**

- Press `Ctrl + F9` to build the project.
- Press `F9` to build and run the project.

Your traffic simulation window should open.

### Method 2: Using VS Code

**Step 1: Install FreeGLUT**

Open the MSYS2 UCRT64 terminal:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-freeglut
```

**Step 2: Navigate to your project directory**

```bash
cd /c/Users/YourUsername/OneDrive/Documents/YourProject
```

Replace `YourUsername` and `YourProject` with your actual Windows username and project directory.

**Step 3: Compile the project**

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o traffic.exe -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32
```

**Step 4: Run the application**

```bash
./traffic.exe
```

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
