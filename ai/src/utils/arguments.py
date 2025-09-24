import sys

def parse_arguments():
    if len(sys.argv) < 7:
        print("Usage: python3 main.py -p PORT -n TEAM -h HOST [-f FREQUENCY]")
        sys.exit(84)
    port = int(sys.argv[2])
    team = sys.argv[4]
    host = sys.argv[6]
    frequency = 100

    if "-f" in sys.argv:
        try:
            f_index = sys.argv.index("-f")
            if f_index + 1 < len(sys.argv):
                frequency = int(sys.argv[f_index + 1])
                print(f"[AI] Using server frequency: {frequency}")
        except (ValueError, IndexError):
            print("[AI] Invalid frequency, using default 100")
    return port, team, host, frequency
