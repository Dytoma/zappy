import sys
import random
import subprocess
import os
from ai.src.client.sender.sender import *
from ai.src.client.parser.parse_response import (
    parse_response, RESOURCE_NAMES, has_sufficient_resources,
    get_resource_count, parse_inventory
)
from .timing import ProtocolTiming
from .elevation import ELEVATION_REQUIREMENTS

class AdvancedZappyAI:
    def __init__(self, sock, team_name, frequency, port=None, host=None):
        self.sock = sock
        self.team_name = team_name
        self.timing = ProtocolTiming(frequency)
        self.level = 1
        self.inventory = {name: 0 for name in RESOURCE_NAMES}
        self.inventory['food'] = 10
        self.moves = 0
        self.vision_data = None
        self.last_broadcast = 0
        self.in_coop_mode = False
        self.target_direction = None
        self.coop_cooldown = 0
        self.steps_since_broadcast = 0
        self.port = port
        self.host = host


    def get_vision_range(self):
        return (self.level + 1) ** 2

    def move_towards_broadcast(self):
        direction = self.target_direction
        if direction is None:
            return self.random_movement()
        if direction in [1, 2, 8]:
            parsed = self.send_and_parse(send_forward, 'Forward')
        elif direction in [3, 6]:
            parsed = self.send_and_parse(send_right, 'Right')
            self.send_and_parse(send_forward, 'Forward')
        elif direction in [4, 7]:
            parsed = self.send_and_parse(send_left, 'Left')
            self.send_and_parse(send_forward, 'Forward')
        elif direction == 5:
            parsed = self.send_and_parse(send_right, 'Right')
            self.send_and_parse(send_right, 'Right')
            self.send_and_parse(send_forward, 'Forward')
        else:
            parsed = self.send_and_parse(send_forward, 'Forward')
        self.steps_since_broadcast += 1
        if self.steps_since_broadcast > 10:
            self.target_direction = None
            self.steps_since_broadcast = 0
        return parsed

    def drop_unneeded_resources(self):
        for res in RESOURCE_NAMES[1:]:
            if self.inventory.get(res, 0) > 2:
                parsed = self.send_and_parse(send_set, 'Set', res)
                if parsed and parsed.get("data") == "ok":
                    self.inventory[res] -= 1
        if self.inventory.get('food', 0) > 20:
            parsed = self.send_and_parse(send_set, 'Set', 'food')
            if parsed and parsed.get("data") == "ok":
                self.inventory['food'] -= 1

    def try_eject_if_blocked(self):
        if not self.vision_data:
            return
        parsed = parse_response(self.vision_data)
        if parsed.get("type") != "look":
            return
        current_tile = parsed.get("data", [[]])[0]
        player_count = current_tile.count("player")
        if player_count >= 2:
            parsed = self.send_and_parse(send_eject, 'Eject')
            if parsed and parsed.get("data") == "ok":
                print("[AI] Eject executed (players detected on same tile)")

    def check_coop_arrival(self):
        if self.vision_data:
            parsed_look = parse_response(self.vision_data)
            if parsed_look.get("type") == "look":
                current_tile = parsed_look.get("data", [])[0]
                player_count = current_tile.count("player")
                if player_count >= 2:
                    print("[AI] Arrived on cooperative tile with enough teammates. Launching incantation.")
                    self.attempt_elevation()
                    self.in_coop_mode = False
                    self.target_direction = None


    def respond_to_broadcast(self, text, direction):
        if text.startswith("READY_ELEV"):
            try:
                parts = text.split(":")
                level = int(parts[1])
                if level == self.level and not self.in_coop_mode:
                    self.in_coop_mode = True
                    self.target_direction = direction
                    self.coop_cooldown = 20
                    print(f"[AI] Joining incantation call from direction {direction} (READY_ELEV)")
            except Exception as e:
                print(f"[AI] Broadcast parse error: {e}")
        elif text.startswith("NEED:"):
            try:
                parts = text.split(":")
                resource_needed = parts[1]
                qty_needed = int(parts[2])
                lvl_str = parts[3] if len(parts) > 3 else "LVL" + str(self.level)
                lvl = int(lvl_str.replace("LVL", ""))
                if lvl == self.level and not self.in_coop_mode:
                    self.in_coop_mode = True
                    self.target_direction = direction
                    self.coop_cooldown = 20
                    print(f"[AI] Joining cooperative call for {resource_needed} from direction {direction} (NEED)")
            except Exception as e:
                print(f"[AI] Failed to parse NEED broadcast: {e}")

    def send_and_parse(self, command_func, action_name, *args):
        try:
            response = command_func(self.sock, *args) if args else command_func(self.sock)
            self.timing.wait_after_action(action_name)
            if response:
                parsed = parse_response(response)
                return parsed
            return None
        except (ConnectionResetError, ConnectionAbortedError, BrokenPipeError):
            self.cleanup_and_exit()
        except Exception as e:
            print(f"[AI] Command error: {e}")
            return None

    def cleanup_and_exit(self):
        try:
            if hasattr(self, 'sock') and self.sock:
                self.sock.close()
        except:
            pass
        sys.exit(0)

    def handle_server_message(self, parsed_response):
        if not parsed_response:
            return False

        response_type = parsed_response.get("type")
        data = parsed_response.get("data")

        if response_type == "broadcast":
            direction = data.get("direction", 0)
            text = data.get("text", "")
            print(f"[AI] Broadcast from direction {direction}: {text}")
            self.respond_to_broadcast(text, direction)
            return True
        elif response_type == "ejection":
            print(f"[AI] Ejected from direction {data}")
            return True
        elif response_type == "death":
            print("[AI] Player died!")
            sys.exit(0)
        return False

    def update_inventory(self, parsed_response):
        if parsed_response and parsed_response.get("type") == "inventory":
            inventory_data = parsed_response.get("data", {})
            for resource in RESOURCE_NAMES:
                self.inventory[resource] = get_resource_count(inventory_data, resource)

    def can_elevate(self):
        if self.level >= 8:
            return False
        requirements = ELEVATION_REQUIREMENTS.get(self.level, {})
        resource_requirements = {k: v for k, v in requirements.items() if k != 'players'}
        return has_sufficient_resources(self.inventory, resource_requirements)

    def attempt_elevation(self):
        if not self.can_elevate():
            return False
        parsed_look = parse_response(self.vision_data) if self.vision_data else None
        players_here = 0
        if parsed_look and parsed_look.get("type") == "look":
            current_tile = parsed_look.get("data", [])[0]
            players_here = current_tile.count("player")
        requirements = ELEVATION_REQUIREMENTS.get(self.level, {})
        needed_players = requirements.get('players', 1)
        if players_here < needed_players:
            return False
        resource_requirements = {k: v for k, v in requirements.items() if k != 'players'}
        print(f"[AI] Attempting elevation {self.level} → {self.level + 1}")
        print(f"[AI] Need {needed_players} players for this elevation")
        for resource, needed in resource_requirements.items():
            available = self.inventory.get(resource, 0)
            to_drop = min(available, needed)
            for _ in range(to_drop):
                parsed = self.send_and_parse(send_set, 'Set', resource)
                if parsed and parsed.get("data") == "ok":
                    self.inventory[resource] -= 1
        parsed = self.send_and_parse(send_incantation, 'Incantation')
        if parsed and parsed.get("type") == "incantation":
            incant_data = parsed.get("data", {})
            if incant_data.get("success"):
                self.level = incant_data.get("level", self.level + 1)
                print(f"[AI] Elevation successful! Now level {self.level}")
                return True
        print(f"[AI] Elevation failed")
        return False

    def collect_resources_intelligently(self):

        if not self.vision_data:
            parsed = self.send_and_parse(send_look, 'Look')
            if parsed and parsed.get("type") == "look":
                self.vision_data = parsed.get("raw")
            else:
                return False
        
        parsed_vision = parse_response(self.vision_data)
        if parsed_vision.get("type") != "look":
            return False
        
        tiles = parsed_vision.get("data", [])
        if not tiles:
            return False
    
        current_tile = tiles[0] if tiles else []
        current_food = get_resource_count(self.inventory, 'food')
        
        if self.in_coop_mode and current_food >= 3:
            return False

        collected_something = False
        

        if 'food' in current_tile:
            parsed = self.send_and_parse(send_take, 'Take', 'food')
            if parsed and parsed.get("data") == "ok":
                print(f"[AI] Collected food ({current_food + 1} total)")
                collected_something = True
        

        if current_food >= 3:
            requirements = ELEVATION_REQUIREMENTS.get(self.level, {})
            for resource in RESOURCE_NAMES[1:]:
                if resource in current_tile and resource in requirements:
                    current_count = get_resource_count(self.inventory, resource)
                    needed_count = requirements.get(resource, 0)
                    if current_count < needed_count:
                        parsed = self.send_and_parse(send_take, 'Take', resource)
                        if parsed and parsed.get("data") == "ok":
                            print(f"[AI] Collected {resource} for elevation ({current_count + 1}/{needed_count})")
                            collected_something = True

        
        if collected_something:
            return True

        for i, tile in enumerate(tiles[1:min(len(tiles), 6)]):
            if 'food' in tile:
                if i + 1 <= 2:
                    print(f"[AI] Moving toward food (tile {i + 1})")
                    return self.send_and_parse(send_forward, 'Forward')
                else:
                    print(f"[AI] Turning toward food (tile {i + 1})")
                    return self.send_and_parse(send_left if i % 2 == 0 else send_right, 
                                             'Left' if i % 2 == 0 else 'Right')
 
        if current_food >= 10:
            requirements = ELEVATION_REQUIREMENTS.get(self.level, {})
            for i, tile in enumerate(tiles[1:min(len(tiles), 6)]):
                for resource in RESOURCE_NAMES[1:]:
                    if resource in tile and resource in requirements:
                        current_count = get_resource_count(self.inventory, resource)
                        needed_count = requirements.get(resource, 0)
                        if current_count < needed_count:
                            if i + 1 <= 2:
                                return self.send_and_parse(send_forward, 'Forward')
                            else:
                                return self.send_and_parse(send_left if i % 2 == 0 else send_right,
                                                         'Left' if i % 2 == 0 else 'Right')
        
        return False

    def broadcast_status(self):
        if self.moves - self.last_broadcast < 20:
            return
        if self.can_elevate():
            requirements = ELEVATION_REQUIREMENTS.get(self.level, {})
            needed_players = requirements.get('players', 1)
            message = f"READY_ELEV:{self.level}:NEED_{needed_players}_PLAYERS"
        else:
            needed = []
            requirements = ELEVATION_REQUIREMENTS.get(self.level, {})
            for resource, needed_count in requirements.items():
                if resource != 'players':
                    current = get_resource_count(self.inventory, resource)
                    if current < needed_count:
                        needed.append(f"{resource}:{needed_count-current}")
            if needed:
                message = f"NEED:{needed[0]}:LVL{self.level}"
            else:
                return
        parsed = self.send_and_parse(send_broadcast, 'Broadcast', message)
        if parsed and parsed.get("data") == "ok":
            print(f"[AI] Broadcasted: {message}")
            self.last_broadcast = self.moves


    def can_help(self):
        food_count = get_resource_count(self.inventory, 'food')
        return food_count > 15

    def broadcast_offer_help(self):
        message = f"HELP_AVAILABLE:LVL{self.level}:FOOD_{get_resource_count(self.inventory, 'food')}"
        self.send_and_parse(send_broadcast, 'Broadcast', message)

    def smart_movement(self):
        if not self.vision_data:
            return self.random_movement()
        parsed_vision = parse_response(self.vision_data)
        if parsed_vision.get("type") != "look":
            return self.random_movement()
        tiles = parsed_vision.get("data", [])
        requirements = ELEVATION_REQUIREMENTS.get(self.level, {})
        for i, tile in enumerate(tiles[1:6], 1):
            if get_resource_count(self.inventory, 'food') < 8 and 'food' in tile:
                print(f"[AI] Moving toward food")
                return self.send_and_parse(send_forward, 'Forward')
            for resource in RESOURCE_NAMES[1:]:
                if resource in tile and resource in requirements:
                    current = get_resource_count(self.inventory, resource)
                    needed = requirements.get(resource, 0)
                    if current < needed:
                        print(f"[AI] Moving toward {resource}")
                        return self.send_and_parse(send_forward, 'Forward')
        return self.random_movement()

    def random_movement(self):
        actions = [
            (send_forward, 'Forward'),
            (send_forward, 'Forward'),
            (send_forward, 'Forward'),
            (send_right, 'Right'),
            (send_left, 'Left')
        ]
        action_func, action_name = random.choice(actions)
        return self.send_and_parse(action_func, action_name)

    def check_team_slots(self):
        if self.moves % 20 == 0:
                food = get_resource_count(self.inventory, 'food')
                if food < 100: 
                    print(f"[AI] Attempting fork (slots:, food: {food}, level: {self.level})...")
                    fork_parsed = self.send_and_parse(send_fork, 'Fork')
                    if fork_parsed and fork_parsed.get("data") == "ok":
                        print(f"[AI] Created an egg!")
                        try:
                            current_dir = os.path.dirname(os.path.abspath(__file__))
                            ai_dir = os.path.join(current_dir, '..', '..')
                            ai_dir = os.path.abspath(ai_dir)
                            ai_main_path = os.path.join(ai_dir, 'main.py')
                            base_dir = os.path.dirname(ai_dir)
                            
                            cmd = [
                                sys.executable,
                                ai_main_path,
                                '-p', str(self.port),
                                '-n', self.team_name,
                                '-h', self.host if self.host else 'localhost'
                            ]
                            env = os.environ.copy()
                            if 'PYTHONPATH' in env:
                                env['PYTHONPATH'] = f"{base_dir}:{env['PYTHONPATH']}"
                            else:
                                env['PYTHONPATH'] = base_dir
                            
                            subprocess.Popen(cmd, env=env, cwd=base_dir)
                        except Exception as e:
                            print(f"[AI] Failed to launch new AI: {e}")
                    else:
                        print(f"[AI] Fork failed: {fork_parsed}")


    def run(self):
        print(f"[AI] Level: {self.level}, Frequency: {self.timing.frequency}")
        try:
            while True:
                parsed = self.send_and_parse(send_look, 'Look')
                if self.handle_server_message(parsed):
                    continue
                elif parsed and parsed.get("type") == "look":
                    self.vision_data = parsed.get("raw")
                    tiles_count = len(parsed.get("data", []))
                    print(f"[AI] Vision: {tiles_count} tiles visible")
                if self.moves % 3 == 0:
                    parsed = self.send_and_parse(send_inventory, 'Inventory')
                    if self.handle_server_message(parsed):
                        continue
                    elif parsed:
                        self.update_inventory(parsed)
                        food = get_resource_count(self.inventory, 'food')
                        resources = sum(get_resource_count(self.inventory, r) for r in RESOURCE_NAMES[1:])
                        print(f"[AI] Food: {food}, Resources: {resources}")
                resource_action_taken = self.collect_resources_intelligently()
                if self.moves % 20 == 0:
                    self.attempt_elevation()
                self.check_team_slots()
                self.broadcast_status()
                if self.in_coop_mode:
                    move_parsed = self.move_towards_broadcast()
                    self.timing.wait_after_action('Forward')
                    look_resp = self.send_and_parse(send_look, 'Look')
                    if look_resp and look_resp.get("type") == "look":
                        self.vision_data = look_resp.get("raw")
                        self.check_coop_arrival()
                    self.moves += 1
                    continue
                if resource_action_taken:
                    self.moves += 1
                    continue
                
                if self.moves % 15 == 0:
                    self.drop_unneeded_resources()
                if self.moves % 12 == 0:
                    self.try_eject_if_blocked()
                move_parsed = self.smart_movement()
                if self.handle_server_message(move_parsed):
                    continue
                elif move_parsed:
                    result = move_parsed.get("data")
                    if result == "ok":
                        print(f"[AI] Movement successful")
                    elif result == "ko":
                        print(f"[AI] Movement blocked")
                
                self.moves += 1
                food_count = get_resource_count(self.inventory, 'food')
                if food_count < 3:
                    print("[AI] CRITICAL: Very low food!")
                elif food_count < 6:
                    print("[AI] Warning: Low food")
                
                if self.moves % 25 == 0:
                    print(f"[AI] Move {self.moves}, Level {self.level}, Food {food_count}")
        except Exception as e:
            print(f"[AI] Fatal error: {e}")
            import traceback
            traceback.print_exc()

