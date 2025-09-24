import re
from typing import List, Dict, Optional, Union

def parse_simple_response(response: str) -> str:
    response = response.strip()
    if response in ["ok", "ko"]:
        return response
    return response

def parse_look(response: str) -> List[List[str]]:
    response = response.strip()

    if not (response.startswith('[') and response.endswith(']')):
        return []
    
    content = response[1:-1]
    tiles_raw = content.split(',')
    tiles = []
    
    for tile_raw in tiles_raw:
        tile_content = tile_raw.strip()
        if tile_content == "":
            tiles.append([])
        else:
            objects = [obj.strip() for obj in tile_content.split() if obj.strip()]
            tiles.append(objects)
    
    return tiles

def parse_inventory(response: str) -> Dict[str, int]:
    response = response.strip()
    
    if not (response.startswith('[') and response.endswith(']')):
        return {}
    
    content = response[1:-1]
    items = {}

    for item_raw in content.split(','):
        item = item_raw.strip()
        if not item:
            continue
        parts = item.split()
        if len(parts) >= 2:
            try:
                quantity = int(parts[-1])
                name = " ".join(parts[:-1])
                items[name] = quantity
            except ValueError:
                items[item] = 0
        else:
            items[item] = 0
    
    return items

def parse_broadcast_message(response: str) -> Optional[Dict[str, Union[int, str]]]:
    response = response.strip()
    
    match = re.match(r'message (\d+), (.+)', response)
    if match:
        direction = int(match.group(1))
        text = match.group(2)
        return {"direction": direction, "text": text}
    
    return None

def parse_ejection_message(response: str) -> Optional[int]:
    response = response.strip()
    
    match = re.match(r'eject: (\d+)', response)
    if match:
        return int(match.group(1))
    
    return None

def parse_incantation_response(response: str) -> Dict[str, Union[bool, int, str]]:
    response = response.strip()
    
    if response == "ko":
        return {"success": False, "level": 0, "message": "Incantation failed"}
    
    lines = response.split('\n')
    if len(lines) >= 2 and lines[0] == "Elevation underway":
        level_match = re.match(r'Current level: (\d+)', lines[1])
        if level_match:
            level = int(level_match.group(1))
            return {"success": True, "level": level, "message": response}
    
    return {"success": False, "level": 0, "message": response}

def parse_connect_nbr_response(response: str) -> Optional[int]:
    response = response.strip()
    
    try:
        return int(response)
    except ValueError:
        return None

def parse_death_message(response: str) -> bool:
    return response.strip() == "dead"

def identify_response_type(response: str) -> str:
    response = response.strip()
    
    if response in ["ok", "ko"]:
        return "simple"
    elif response == "dead":
        return "death"
    elif response.startswith("message "):
        return "broadcast"
    elif response.startswith("eject: "):
        return "ejection"
    elif response.startswith("Elevation underway"):
        return "incantation"
    elif response.startswith('[') and response.endswith(']'):
        if re.search(r'\w+ \d+', response):
            return "inventory"
        else:
            return "look"
    elif response.isdigit():
        return "connect_nbr"
    else:
        return "unknown"

def parse_response(response: str) -> Dict:
    response_type = identify_response_type(response)
    
    result = {
        "type": response_type,
        "raw": response,
        "data": None
    }
    
    if response_type == "simple":
        result["data"] = parse_simple_response(response)
    elif response_type == "look":
        result["data"] = parse_look(response)
    elif response_type == "inventory":
        result["data"] = parse_inventory(response)
    elif response_type == "broadcast":
        result["data"] = parse_broadcast_message(response)
    elif response_type == "ejection":
        result["data"] = parse_ejection_message(response)
    elif response_type == "incantation":
        result["data"] = parse_incantation_response(response)
    elif response_type == "connect_nbr":
        result["data"] = parse_connect_nbr_response(response)
    elif response_type == "death":
        result["data"] = True
    else:
        result["data"] = response
    
    return result

RESOURCE_NAMES = ["food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]

def is_valid_resource(name: str) -> bool:
    return name in RESOURCE_NAMES

def get_resource_count(inventory: Dict[str, int], resource: str) -> int:
    return inventory.get(resource, 0)

def has_sufficient_resources(inventory: Dict[str, int], required: Dict[str, int]) -> bool:
    for resource, needed in required.items():
        if get_resource_count(inventory, resource) < needed:
            return False
    return True
