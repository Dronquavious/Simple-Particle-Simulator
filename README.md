# Sand Particle Simulator

## Overview
The Sand Particle Simulator is a simple physics-based simulation built using the Raylib library. It allows users to place and interact with sand, stone, and water particles on a 2D grid, simulating their natural movement and behavior.

## Preview

https://github.com/user-attachments/assets/0ebb5892-11b1-4db0-bddb-5acd09b3ee06


## Features
- **Sand Simulation**: Sand falls and spreads diagonally when obstructed.
- **Water Simulation**: Water flows downward and spreads horizontally when needed.
- **Stone Placement**: Stones remain static and can be used to block the movement of sand and water.
- **Dynamic Particle Movement**: Sand and water particles move each frame with a natural randomized behavior.
- **User Interaction**: Users can place particles dynamically using mouse clicks.
- **Grid-Based Simulation**: The screen is divided into a grid where each cell represents a particle or empty space.

## Controls
- **Press 'Q'** to select sand placement mode.
- **Press 'W'** to select stone placement mode.
- **Press 'E'** to select water placement mode.
- **Left Mouse Click** to place the selected material at the mouse position.
- **Press 'H'** to hide or show the cursor.

## How It Works
- The simulation updates at **60 FPS**, processing each particle's movement.
- Particles move **from bottom to top** each frame to prevent re-processing moved particles.
- **Sand falls down** if there is space, otherwise moves diagonally if possible.
- **Water flows downward first**, then attempts diagonal movement, and finally moves horizontally if needed.
- **Stone remains static**, acting as an obstacle.


## Future Improvements
- **More Particle Types**: Adding oil, fire, or other materials.
- **Better Physics**: Introducing more realistic interactions.
- **UI Enhancements**: Improving visualization and adding a menu.
- **Performance Optimization**: Efficient data structures for handling large-scale particle simulations.

## License
This project is open-source and free to use. Feel free to contribute and improve the simulation!

## Credits
Developed using [Raylib](https://www.raylib.com/) for rendering and input handling.

