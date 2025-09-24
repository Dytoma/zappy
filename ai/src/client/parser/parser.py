import argparse

def parse_args():
    parser = argparse.ArgumentParser(description="Zappy AI Client", add_help=False)
    parser.add_argument("-p", "--port", type=int, required=True, help="Port number")
    parser.add_argument("-n", "--name", required=True, help="Team name")
    parser.add_argument("-h", "--host", default="127.0.0.1", help="Server address")
    parser.add_argument("--help", action="help", help="Show this help message and exit")
    return parser.parse_args()
