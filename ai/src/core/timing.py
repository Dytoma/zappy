import time

class ProtocolTiming:
    def __init__(self, frequency=100):
        self.frequency = frequency
        self.action_times = {
            'Forward': 7, 'Right': 7, 'Left': 7, 'Look': 7,
            'Inventory': 1, 'Take': 7, 'Set': 7, 'Broadcast': 7,
            'Incantation': 300, 'Fork': 42, 'Eject': 7, 'Connect_nbr': 0
        }

    def get_execution_time(self, action):
        base_time = self.action_times.get(action, 7)
        return base_time / self.frequency

    def wait_after_action(self, action):
        wait_time = self.get_execution_time(action)
        time.sleep(wait_time)
        print(f"[TIMING] Waited {wait_time:.3f}s for {action}")
