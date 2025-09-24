import socket

def receive_complete_response(sock):
    message = ""
    while True:
        try:
            char = sock.recv(1).decode()
            if not char:
                raise ConnectionResetError()
            if char == '\n':
                break
            message += char
        except socket.error:
            raise ConnectionResetError()
        except UnicodeDecodeError:
            raise ConnectionResetError()
    return message

def send_command(sock, command):
    try:
        sock.sendall((command + "\n").encode())
        response = receive_complete_response(sock)
        return response
    except (ConnectionResetError, ConnectionAbortedError, BrokenPipeError):
        raise
    except socket.error:
        raise ConnectionResetError()
    except Exception as e:
        print(f"[CLIENT] Command error: {e}")
        return None

def send_forward(sock):
    return send_command(sock, "Forward")

def send_right(sock):
    return send_command(sock, "Right")

def send_left(sock):
    return send_command(sock, "Left")

def send_look(sock):
    return send_command(sock, "Look")

def send_inventory(sock):
    return send_command(sock, "Inventory")

def send_take(sock, object_name):
    return send_command(sock, f"Take {object_name}")

def send_set(sock, object_name):
    return send_command(sock, f"Set {object_name}")

def send_broadcast(sock, message):
    return send_command(sock, f"Broadcast {message}")

def send_connect_nbr(sock):
    return send_command(sock, "Connect_nbr")

def send_fork(sock):
    return send_command(sock, "Fork")

def send_eject(sock):
    return send_command(sock, "Eject")

def send_incantation(sock):
    return send_command(sock, "Incantation")