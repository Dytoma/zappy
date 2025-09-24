# Zappy Server-AI Communication Protocol

## 1. Initial Connection

### Connection Sequence
1. **Client**: Opens TCP socket on server port
2. **Server**: `WELCOME\n`
3. **Client**: `TEAM-NAME\n` (team name)
4. **Server**: `CLIENT-NUM\n` (available slots count)
5. **Server**: `X Y\n` (world dimensions)

### Example
```
<-- WELCOME
--> GraphicTeam
<-- 5
<-- 10 10
```

## 2. Client to Server Commands

### 2.1 Movement and Orientation

| Command | Syntax | Execution Time | Response |
|---------|--------|----------------|----------|
| Move forward | `Forward\n` | 7/f | `ok\n` |
| Turn right 90° | `Right\n` | 7/f | `ok\n` |
| Turn left 90° | `Left\n` | 7/f | `ok\n` |

### 2.2 Observation

| Command | Syntax | Execution Time | Response |
|---------|--------|----------------|----------|
| Look around | `Look\n` | 7/f | `[tile1, tile2, ...]\n` |
| Check inventory | `Inventory\n` | 1/f | `[food n, linemate n, ...]\n` |

#### Look Response Example
```
<-- [player, , , thystame, , food, , , , , , thystame, , , , , , ]
```

#### Inventory Response Example
```
<-- [food 345, sibur 3, phiras 5, deraumere 0]
```

### 2.3 Object Manipulation

| Command | Syntax | Execution Time | Response |
|---------|--------|----------------|----------|
| Take object | `Take object\n` | 7/f | `ok\n` or `ko\n` |
| Drop object | `Set object\n` | 7/f | `ok\n` or `ko\n` |

#### Available Objects
- `food`
- `linemate`
- `deraumere`
- `sibur`
- `mendiane`
- `phiras`
- `thystame`

### 2.4 Communication and Special Actions

| Command | Syntax | Execution Time | Response |
|---------|--------|----------------|----------|
| Broadcast message | `Broadcast text\n` | 7/f | `ok\n` |
| Connection count | `Connect_nbr\n` | - | `value\n` |
| Fork player | `Fork\n` | 42/f | `ok\n` |
| Eject players | `Eject\n` | 7/f | `ok\n` or `ko\n` |
| Start incantation | `Incantation\n` | 300/f | `Elevation underway\nCurrent level: k\n` or `ko\n` |

## 3. Server to Client Messages

### 3.1 Broadcast Messages
When another player broadcasts a message:
```
message K, text\n
```
Where `K` is the tile number indicating the sound's origin direction.

### 3.2 Ejection Messages
When the player is ejected by another:
```
eject: K\n
```
Where `K` is the direction from which the ejection comes.

### 3.3 Player Death
```
dead\n
```

## 4. Time Management

### Execution Time Formula
```
execution_time = action_time / f
```

Where:
- `action_time`: base action time
- `f`: frequency (reciprocal of time unit, default = 100)

### Examples with f=100
- `Forward`: 7/100 = 0.07 seconds
- `Look`: 7/100 = 0.07 seconds
- `Incantation`: 300/100 = 3 seconds

## 5. Vision System

### Tile Numbering
Vision follows a diamond pattern centered on the player:

```
Level 1:    Level 2:      Level 3:
   0           0 1 2         0 1 2 3 4
  1 2         3 4 5 6       5 6 7 8 9 10
             7 8 9 10      11 12 13 14 15 16
                          17 18 19 20 21 22
                             23 24 25 26
```

### Vision Range by Level
- Level 1: 3 tiles (0, 1, 2)
- Level 2: 8 tiles (0-7)
- Level 3: 15 tiles (0-14)
- etc.

## 6. Elevation System

### Requirements by Level

| Elevation | Players | linemate | deraumere | sibur | mendiane | phiras | thystame |
|-----------|---------|----------|-----------|-------|----------|--------|----------|
| 1→2 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
| 2→3 | 2 | 1 | 1 | 1 | 0 | 0 | 0 |
| 3→4 | 2 | 2 | 0 | 1 | 0 | 2 | 0 |
| 4→5 | 4 | 1 | 1 | 2 | 0 | 1 | 0 |
| 5→6 | 4 | 1 | 2 | 1 | 3 | 0 | 0 |
| 6→7 | 6 | 1 | 2 | 3 | 0 | 1 | 0 |
| 7→8 | 6 | 2 | 2 | 2 | 2 | 2 | 1 |

## 7. Error Handling

### Invalid Commands
```
ko\n
```

### Failure Conditions
- Unknown command
- Non-existent object during `Take`/`Set`
- Incantation conditions not met
- No players to eject

## 8. Command Buffer

### Limitations
- Maximum 10 pending commands per client
- Beyond this limit, commands are ignored
- Execution in order of reception

### Asynchronous Handling
- Client can send commands without waiting for responses
- Responses arrive in command execution order
- Each command only blocks the concerned player

## 9. Session Examples

### Complete Connection Session
```
<-- WELCOME
--> TeamAlpha
<-- 3
<-- 20 15
--> Look
<-- [player, food, , linemate, , , ]
--> Take food
<-- ok
--> Inventory
<-- [food 11, linemate 0, deraumere 0, sibur 0, mendiane 0, phiras 0, thystame 0]
```

### Incantation Session
```
--> Look
<-- [player, linemate, , , ]
--> Take linemate
<-- ok
--> Incantation
<-- Elevation underway
<-- Current level: 2
```

## 10. Sound Transmission

### Direction Numbering
Sound direction is indicated by tile numbers around the player:

```
  6 1 2
  5 P 3  (P = Player position)
  4 7 8
```

Where:
- Tile 0: Same tile as player (self-broadcast)
- Tiles 1-8: Surrounding tiles in clockwise order starting from front

### Shortest Path Rule
When multiple paths are possible (spherical world), the shortest path is always chosen for sound transmission.

## 11. Resource Management

### Resource Density Formula
```
resource_quantity = map_width * map_height * density
```

### Density Values
| Resource | Density |
|----------|---------|
| food | 0.5 |
| linemate | 0.3 |
| deraumere | 0.15 |
| sibur | 0.1 |
| mendiane | 0.1 |
| phiras | 0.08 |
| thystame | 0.05 |

### Respawn Cycle
Resources respawn every 20 time units following the density rules.

## 12. Player Lifecycle

### Initial State
- 10 life units (1260 time units survival)
- Random starting position
- Random starting direction
- Level 1

### Survival
- 1 food unit = 126 time units of life
- Death occurs when life reaches 0
- Connection terminates on death

## 13. Team Management

### Egg System
- `Fork` command creates an egg
- Eggs add team slots
- New clients hatch random available eggs
- Ejection destroys eggs on the tile

### Team Communication
- Only through server (no external communication)
- Broadcast system for team coordination
- `Connect_nbr` shows available team slots

## 14. Important Notes

- All commands end with `\n`
- Protocol is case-sensitive
- `ko` responses indicate failure
- Connection closes automatically on player death
- World is spherical (edges wrap around)
- Maximum 6 players must reach level 8 to win