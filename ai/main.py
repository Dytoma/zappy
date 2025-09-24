#!/usr/bin/env python3

from ai.src.utils.arguments import parse_arguments
from ai.src.core.ai import AdvancedZappyAI
from ai.src.client.socket.socket import create_socket, perform_handshake

def main():
    port, team, host, frequency = parse_arguments()
    sock = None
    try:
        sock = create_socket(host, port)
        game_info = perform_handshake(sock, team)

        print(f"[AI] Game initialized:")
        print(f"  - Available slots: {game_info['available_slots']}")
        print(f"  - World size: {game_info['world_width']}x{game_info['world_height']}")
        print(f"  - Server frequency: {frequency}")

        ai = AdvancedZappyAI(sock, team, frequency, port, host)
        ai.run()
    finally:
        if sock:
            try:
                sock.close()
            except:
                pass

if __name__ == "__main__":
    main()
