# Antigravity ML Engine

A high-performance, deterministic C++ game engine designed from the ground up for Reinforcement Learning (RL) and Machine Learning (ML) Agents. 

The engine strips away bloated architectures found in standard game engines, providing a pure, headless-capable simulation loop that can run at millions of frames per second to train AI rapidly. It functions similarly to OpenAI Gym or Unity ML-Agents but runs fully natively in C++.

## Features

- **Plug-and-Play Architecture**: The engine core is completely decoupled from game logic. You can plug in any custom game (Combat, Racing, Flappy Bird) as long as it implements the `Environment` interface.
- **Strict Determinism**: Uses a fixed timestep (`FIXED_DT`) and integer/fixed-point math friendly structures to guarantee identical outcomes for identical inputs.
- **Built-in Replay System**: Automatically records integer actions that can be precisely replayed later for debugging or validating ML models.
- **Headless Capable**: Simulation logic (`step()`) is separated from drawing (`render()`), allowing lightning-fast background training.

---

## 🛠️ How to Build and Run

### Prerequisites
- CMake (3.16+)
- A C++20 compatible compiler
- [Raylib](https://www.raylib.com/) installed on your system (for visual rendering)

### Compiling
1. Clone the repository and navigate to the root directory.
2. Create a build directory and compile:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

### Running the Engine
Execute the built binary from the `build/` directory:
```bash
./DeterministicCombatEngine
```

---

## 🎮 How to Play (Combat Demo)

The engine currently comes pre-packaged with a 2D `CombatEnvironment` demo.

- **Main Menu**: Press `ENTER` to start the simulation.
- **Player 1 Controls**: `A` (Left), `D` (Right), `SPACE` (Attack)
- **Player 2 Controls**: `Left Arrow`, `Right Arrow`, `ENTER` (Attack)
- **Pause**: Press `ESC` to bring up the pause menu overlay.

### Engine / Debug Controls
- **`TAB`**: Pause the internal simulation frame (freezes logic but keeps rendering).
- **`F`**: Step the simulation forward exactly one frame (only works while internal simulation is paused via `TAB`).
- **`R`**: Toggle Replay Recording. When enabled, every input is recorded.
- **`P`**: Toggle Replay Playback. Automatically drives the game using the recorded inputs.

---

## 🧩 How to Make a New Game

The engine's main loop only interacts with the abstract `Environment` interface. To build a new game, you don't need to touch `main.cpp`'s core logic. 

1. **Create your game environment class**:
   Create a new class (e.g., `FlappyBirdEnv`) that inherits from `Environment` (found in `src/core/environment.h`).

2. **Implement the 5 core AI methods**:
   ```cpp
   class FlappyBirdEnv : public Environment {
   public:
       // Reset the game to the starting state
       void reset() override;
       
       // Advance the game by 1 frame based on the AI's action (e.g., 1 = jump, 0 = do nothing)
       void step(int action_p1, int action_p2) override;
       
       // Return what the AI can see (e.g., player Y position, pipe X/Y positions)
       std::vector<float> get_observation(int player_id) const override;
       
       // Return +1 for passing a pipe, -100 for crashing
       float get_reward(int player_id) const override;
       
       // Return true if the bird crashed
       bool is_done() const override;

       // Optional: Draw the game using Raylib for human viewing
       void render() const override;
       void render_debug(bool isPaused, bool isRecording, bool isPlaying) const override;
   };
   ```

3. **Plug it into the Engine**:
   Open `src/main.cpp` and swap out the environment pointer:
   ```cpp
   // Instead of CombatEnvironment
   Environment* env = new FlappyBirdEnv(); 
   ```

4. **Compile and Train!**
   Your game is now fully integrated with the engine's deterministic loop, replay systems, and pause menus.
