import socket
import sys

def create_socket(host, port):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((host, port))
        print(f"[CLIENT] Connected to {host}:{port}")
        return s
    except Exception as e:
        print(f"[CLIENT] Connection failed: {e}")
        sys.exit(84)

def receive_line(sock):
    message = ""
    while True:
        char = sock.recv(1).decode()
        if not char:
            raise ConnectionResetError()
        if char == '\n':
            break
        message += char
    return message

def perform_handshake(sock, team_name):
    try:

        welcome = receive_line(sock)
        print(f"[SERVER] {welcome}")
        
      
        sock.sendall((team_name + "\n").encode())


        slots_response = receive_line(sock)
        print(f"[SERVER] Available slots: {slots_response}")
        available_slots = int(slots_response)


        dimensions_response = receive_line(sock)
        print(f"[SERVER] World dimensions: {dimensions_response}")
        width, height = map(int, dimensions_response.split())
        
        return {
            'available_slots': available_slots,
            'world_width': width,
            'world_height': height
        }
        
    except Exception as e:
        print(f"[CLIENT] Handshake error: {e}")
        sock.close()
        sys.exit(84)