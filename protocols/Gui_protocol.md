
# 📡 Server–Client Communication Protocol

This document outlines the communication protocol between the **Server** and the **Graphical Interface (Client GUI)**. All messages are string-based and end with a newline character `\n`.

## 1. GUI Authentication
When the server prompts for a team name, the graphical interface (GUI) must authenticate itself by sending the following:

```nginx
GRAPHIC
```  

This indicates to the server that the connecting client is a graphical monitor and not a player.

---

## 2. Symbol Definitions

| Symbol | Meaning                     |
|--------|-----------------------------|
| `X`    | Width or horizontal position |
| `Y`    | Height or vertical position |
| `q0`   | Resource 0 (food) quantity  |
| `q1`   | Resource 1 (linemate) quantity |
| `q2`   | Resource 2 (deraumere) quantity |
| `q3`   | Resource 3 (sibur) quantity |
| `q4`   | Resource 4 (mendiane) quantity |
| `q5`   | Resource 5 (phiras) quantity |
| `q6`   | Resource 6 (thystame) quantity |
| `n`    | Player number               |
| `O`    | Orientation: `1` (N), `2` (E), `3` (S), `4` (W) |
| `L`    | Player or incantation level |
| `e`    | Egg number                  |
| `T`    | Time unit                   |
| `N`    | Name of the team            |
| `R`    | Incantation result          |
| `M`    | Message                     |
| `i`    | Resource number             |

---

## 3. Server ↔ Client Commands

### 3.1 Map Information

| Client command | Direction      | Server response                  | Description |
|----------------|----------------|----------------------------------|-------------|
| `msz\n`        | Client → Server | `msz X Y\n`                      | Map size (width and height) |
| `bct X Y\n`    | Client → Server | `bct X Y q0 q1 q2 q3 q4 q5 q6\n` | Tile content at position (X, Y) |
| `mct\n`        | Client → Server | Series of `bct` lines            | Content of the whole map (all tiles) |

### 3.2 Team Management

| Client command | Direction       | Server response                    | Description |
|----------------|-----------------|------------------------------------|-------------|
| `tna\n`        | Client → Server | `tna N\n` (repeated for each team) | Name(s) of all teams |

### 3.3 Player State

| Client command | Direction       | Server response                     | Description |
|----------------|-----------------|-------------------------------------|-------------|
|                | Server → Client | `pnw #n X Y O L N\n`                | New player connected |
| `ppo #n\n`     | Client → Server | `ppo #n X Y O\n`                    | Player position |
| `plv #n\n`     | Client → Server | `plv #n L\n`                        | Player level |
| `pin #n\n`     | Client → Server | `pin #n X Y q0 q1 q2 q3 q4 q5 q6\n` | Player inventory |
|           | Server → Client | `pex #n\n`                          | Player expulsion |
|           | Server → Client | `pbc #n M\n`                        | Broadcast message |
|           | Server → Client | `pfk #n\n`                          | Player lays an egg |
|           | Server → Client | `pdr #n i\n`                        | Player drops a resource |
|           | Server → Client | `pgt #n i\n`                        | Player picks up a resource |
|           | Server → Client | `pdi #n\n`                          | Player dies |

### 3.4 Incantation System

| Client command | Direction | Server response         | Description |
|---------------|-----------|-------------------------|-------------|
|          | Server → Client | `pic X Y L #n #n ...\n` | Start of an incantation at (X, Y) by listed players |
|          | Server → Client | `pie X Y R\n`           | End of an incantation at (X, Y) with result |

### 3.5 Egg Events

| Client command | Direction | Server response   | Description |
|---------------|-----------|-------------------|-------------|
|          | Server → Client | `enw #e #n X Y\n` | An egg is laid by player `#n` at (X, Y) |
|          | Server → Client | `ebo #e\n`        | Egg hatches and connects |
|          | Server → Client | `edi #e\n`        | Egg dies |

### 3.6 Time Management

| Client command | Direction       | Server response | Description |
|----------------|-----------------|-----------------|-------------|
| `sgt\n`        | Client → Client | `sgt T\n`       | Current time unit |
| `sst T\n`      | Client → Server | `sst T\n`       | Set the time unit |

### 3.7 Game Status

| Client command | Direction | Server response | Description |
|---------------|-----------|-----------------|-------------|
|          | Server → Client | `seg N\n`       | Game ends, team `N` wins |
|          | Server → Client | `smg M\n`       | Server message |

### 3.8 Error Handling

| Client command | Direction | Server response | Description |
|---------------|-----------|-----------------|-------------|
|          | Server → Client | `suc\n`         | Unknown command received |
|          | Server → Client | `sbp\n`         | Command with bad parameters |

---

## 4. Notes

- All commands end with a newline `\n`.
- Identifiers such as `#n` and `#e` represent IDs prefixed with `#`.
- Commands may be repeated in sequence (e.g., `tna` for each team).
- Coordinate and resource values are integers.
- This protocol is case-sensitive and the server handles only the specified commands above.
