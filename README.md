# RL Engine

A high-performance, deterministic C++ game engine designed from the ground up for Reinforcement Learning (RL) and Machine Learning (ML) Agents. 

The engine strips away bloated architectures found in standard game engines, providing a pure, headless-capable simulation loop that can run at millions of frames per second to train AI rapidly. It functions similarly to OpenAI Gym or Unity ML-Agents but runs fully natively in C++.

## 🚀 Features

- **Plug-and-Play Architecture**: The engine core is completely decoupled from game logic. You can plug in any custom game (Combat, Racing, Flappy Bird) as long as it implements the `Environment` interface.
- **Strict Determinism**: Uses a fixed timestep (`FIXED_DT`) and integer/fixed-point math friendly structures to guarantee identical outcomes for identical inputs.
- **Built-in Replay System**: Automatically records integer actions that can be precisely replayed later for debugging or validating ML models.
- **Headless Capable**: Simulation logic (`step()`) is separated from drawing (`render()`), allowing lightning-fast background training.

---

## 🧠 How the RL Integration Works

The RL Engine operates on a standard Reinforcement Learning **Agent-Environment Loop**. 

In Reinforcement Learning, an **Agent** learns to make decisions by performing **actions** in an **environment**, which yields **observations** and **rewards**. 

Here is how the RL engine interacts with your game natively:

1. **Initialization:** The Engine initializes your custom game via the `Environment` interface.
2. **Start of Episode:** The engine calls `reset()` to put the game in its starting state.
3. **The Simulation Loop:** (Runs at fixed intervals or as fast as possible in headless mode)
   - The Agent looks at the game state via `get_observation(player_id)`. This returns a normalized vector of floats representing exactly what the AI can "see" (e.g., enemy positions, player health, distances).
   - The Agent makes a decision and passes it to the engine as an integer action.
   - The engine calls `step(action_p1, action_p2)` to deterministically advance the game physics/logic by precisely one frame based on those actions.
   - The Agent looks at the outcome by calling `get_reward(player_id)`. It learns to maximize positive rewards (e.g., landing a hit) and minimize negative rewards (e.g., taking damage or losing).
4. **End of Episode:** When `is_done()` returns true (e.g., a player dies or time runs out), the loop stops and goes back to `reset()` for the next training episode.

---

## 🧩 How to Build Your Own Game

Because the engine's main loop only interacts with the abstract `Environment` interface, you can build any game without touching the core engine files.

### 1. Create your Environment Class
Create a new class that inherits from `Environment` (found in `src/core/environment.h`).

### 2. Implement the Core AI Methods
You must define how the AI sees, acts, and gets rewarded in your specific game:

```cpp
#include "environment.h"

class MyCustomEnv : public Environment {
public:
    // 1. Reset the game to the starting state (called at the start of each episode)
    void reset() override { /* ... */ }
    
    // 2. Advance the game by exactly 1 frame based on integer actions. 
    // Example: 0 = idle, 1 = move left, 2 = move right, 3 = jump.
    void step(int action_p1, int action_p2) override { /* ... */ }
    
    // 3. Return a flattened vector of floats representing the AI's "eyes".
    // Example: { player_x, player_y, enemy_x, enemy_y, distance_to_target }
    std::vector<float> get_observation(int player_id) const override { /* ... */ }
    
    // 4. Return the reward for this specific frame. 
    // Example: +1.0 for dealing damage, -1.0 for dying, 0.0 otherwise.
    float get_reward(int player_id) const override { /* ... */ }
    
    // 5. Return true if the episode is over (e.g., someone won or died).
    bool is_done() const override { /* ... */ }

    // 6. (Optional) Draw the game using Raylib for human viewing.
    void render() const override { /* ... */ }
    void render_debug(bool isPaused, bool isRecording, bool isPlaying) const override { /* ... */ }
};
```

### 3. Plug it into the Engine
Open `src/game_entry.cpp` and swap out the environment pointer. **This is the ONLY file you need to touch to integrate your game!** You never need to modify the core engine or `main.cpp`.

```cpp
#include "environment.h"
#include "games/my_custom_game/my_custom_env.h"

Environment* CreateGameEnvironment() {
    // Return your custom environment here
    return new MyCustomEnv(); 
}
```

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

### Running the Engine (Human Mode)
Execute the built binary from the `build/` directory:
```bash
./RLEngine
```

### 🤖 Training the Agent (Headless Mode)
To run the engine without visual rendering and execute a lightning-fast RL Q-Learning training loop simulating thousands of episodes instantly:
```bash
./RLEngine --train
```
*Note: This will train the agent and save its "brain" to `agent.bin`.*

### 📺 Watching the Agent Play
After the agent has finished training, you can watch it play the game using its learned policy:
```bash
./RLEngine --play
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
