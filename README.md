<h1 align="center">🚀 ZAPPY - A Tribute to Zaphod Beeblebrox</h1>

--------  

> A multiplayer network game where several teams compete on a tile map containing resources. The winning team is the first one to have at least 6 players reach the maximum elevation level.

## 📋 Table of Contents

- [Overview](#-overview)
- [Prerequisites](#-prerequisites)
- [Installation](#-installation)
- [Usage](#-usage)
- [Architecture](#️-architecture)
- [Game Rules](#-game-rules)
- [Protocols](#-protocols)
- [Testing](#-testing)
- [Contributing](#-contributing)

## 🎯 Overview

Zappy is a complex project consisting of three distinct programs:

- **🖥️ Server (`zappy_server`)** : Written in C, manages the virtual world and its inhabitants
- **🎮 Graphical Interface (`zappy_gui`)** : Written in C++, allows visualization of the world
- **🤖 AI Client (`zappy_ai`)** : Free language choice, controls an inhabitant through commands

### Game Objective

Teams compete on the planet Trantor to collect resources and perform elevation rituals. The first team with 6 players at the maximum level wins the game.

## 🔧 Prerequisites

- **C Compiler** (gcc/clang)
- **C++ Compiler** (g++/clang++)
- **Make**
- **Raylib** (for graphical interface - optional but recommended)
- **Operating System** : Linux/macOS/Windows (WSL)

### Installation on Fedora

```bash
# Essential development tools
sudo dnf groupinstall "Development Tools"
sudo dnf install gcc gcc-c++ make

# Raylib for graphical interface (optional)
bash scripts/install_raylib.sh

# Git if not already installed
sudo dnf install git

# For documentation (optional)
sudo dnf install nodejs npm
```  

Use `apt` if you are on Ubuntu or any other directive depending on your system specifications.

## 🚀 Installation

### 1. Clone the project
```bash
git clone git@github.com:EpitechPromo2028/B-YEP-400-TLS-4-1-zappy-dytoma.batogouma.git
cd zappy
```

### 2. Installing dependencies
For this to work properly, You'll need to have `sudo` permissions
- Installing `raylib`
  ```bash
  bash scripts/install_raylib.sh
  
  # Or do the following
  cd scripts
  chmod +x install_raylib.sh
  ./install_raylib.sh
  ```
- Installing libraries for `python`
  ```bash
  bash scripts/install_py_lib.sh
  
  # Or do the following
  cd scripts
  chmod +x install_py_lib.sh
  ./install_py_lib.sh
  ```

### 3. Compilation
```bash
# Compile all binaries
make

# Or compile individually
make zappy_server
make zappy_gui
make zappy_ai
```

### 4. Cleanup
```bash
# Clean compilation files
make clean

# Complete cleanup
make fclean

# Recompile from scratch
make re
```

## 🎮 Usage

### Server
```bash
./zappy_server -p port -x width -y height -n team1 team2 ... -c clientsNb -f freq
```

**Options:**
- `-p port` : Port number
- `-x width` : World width
- `-y height` : World height
- `-n team1 team2 ...` : Team names
- `-c clientsNb` : Number of authorized clients per team
- `-f freq` : Reciprocal of time unit for action execution

**Example:**
```bash
./zappy_server -p 4242 -x 10 -y 10 -n team1 team2 -c 3 -f 100
```

### Graphical Interface
```bash
./zappy_gui -p port -h machine
```

**Options:**
- `-p port` : Server port number
- `-h machine` : Server hostname

**Example:**
```bash
./zappy_gui -p 4242 -h localhost
```

### AI Client
```bash
./zappy_ai -p port -n name -h machine
```

**Options:**
- `-p port` : Server port number
- `-n name` : Team name
- `-h machine` : Machine name (localhost by default)

**Example:**
```bash
./zappy_ai -p 4242 -n team1 -h localhost
```

## 🏗️ Architecture

```
zappy/
├── server/              # Server code (C)
│   ├── src/
│   ├── include/
│   └── Makefile
├── GUI/                 # Graphical interface (C++)
│   ├── src/
│   ├── include/
│   └── Makefile
├── AI/                  # AI Client
│   ├── src/
│   └── Makefile
├── docs/                # Documentation
├── scripts/             # Installation scripts
└── README.md
```

## 🎲 Game Rules

### 🌍 Trantor World

- **Geography** : Spherical world with no relief (exiting one side = returning from the other)
- **Resources** : 6 types of stones + food
- **Survival** : 1 food unit = 126 time units

### 💎 Available Resources

| Resource | Density |
|----------|---------|
| Food | 0.5 |
| Linemate | 0.3 |
| Deraumere | 0.15 |
| Sibur | 0.1 |
| Mendiane | 0.1 |
| Phiras | 0.08 |
| Thystame | 0.05 |

### 📈 Elevation System

To advance to the next level, you need:
- A certain number of players at the same level
- Specific resources
- Perform an incantation ritual

| Level | Players | Linemate | Deraumere | Sibur | Mendiane | Phiras | Thystame |
|-------|---------|----------|-----------|-------|----------|--------|----------|
| 1→2 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
| 2→3 | 2 | 1 | 1 | 1 | 0 | 0 | 0 |
| 3→4 | 2 | 2 | 0 | 1 | 0 | 2 | 0 |
| 4→5 | 4 | 1 | 1 | 2 | 0 | 1 | 0 |
| 5→6 | 4 | 1 | 2 | 1 | 3 | 0 | 0 |
| 6→7 | 6 | 1 | 2 | 3 | 0 | 1 | 0 |
| 7→8 | 6 | 2 | 2 | 2 | 2 | 2 | 1 |

### 🔍 Vision and Perception

- **Vision** : Increases with level (triangular shape)
- **Sound** : Omnidirectional broadcast with direction indication
- **Communication** : Via broadcast only

## 📡 Protocols
For more information on the protocol, look at the `protocols/` folder at the root of the repository.  

### Client → Server Commands

| Action | Command | Time | Response |
|--------|---------|------|----------|
| Move forward | `Forward` | 7/f | `ok` |
| Turn right | `Right` | 7/f | `ok` |
| Turn left | `Left` | 7/f | `ok` |
| Look around | `Look` | 7/f | `[tile1, tile2, ...]` |
| Inventory | `Inventory` | 1/f | `[linemate n, sibur n, ...]` |
| Broadcast | `Broadcast text` | 7/f | `ok` |
| Free connections | `Connect_nbr` | - | `value` |
| Fork | `Fork` | 42/f | `ok` |
| Eject | `Eject` | 7/f | `ok/ko` |
| Take object | `Take object` | 7/f | `ok/ko` |
| Set object | `Set object` | 7/f | `ok/ko` |
| Incantation | `Incantation` | 300/f | `Elevation underway...` |

### Initial Connection
```
<-- WELCOME
--> TEAM-NAME
<-- CLIENT-NUM
<-- X Y
```

## 🤝 Contributing

1. **Fork** the project
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a **Pull Request**

### Code Standards

- **C** : C99 standard, respect Epitech conventions
- **C++** : C++17 minimum
- **Indentation** : 4 spaces
- **Documentation** : Comments for complex functions

## 📝 License

This project is developed as part of the Epitech curriculum.

## 👥 Contributors

- **Léon Haddad**
- **Dytoma Batogouma**
- **Eleejah Roudier**
- **Cedric Khambounheuang**
- **Melissa Hammouche**
- **Sofiane Majdoul**

