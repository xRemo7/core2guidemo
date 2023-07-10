class LightManager:
    def __init__(self):
        self.rooms = {
            "1": {"state": "off", "mode": "sleep", "color": "white"},
            "2": {"state": "off", "mode": "sleep", "color": "white"},
            "3": {"state": "off", "mode": "sleep", "color": "white"},
            "hallway": {"state": "off", "mode": "sleep", "color": "white"},
        }
        self.light_changed_callback = None

    def update_room_mode(self, room, mode):
        if room in self.rooms:
            self.rooms[room]["mode"] = mode
            print(f"Updated room {room}: Mode = {mode}")
            if self.light_changed_callback is not None:
                self.light_changed_callback(room, "mode", mode)
        else:
            print(f"Unknown room {room}")

    def update_room_color(self, room, color):
        if room in self.rooms:
            self.rooms[room]["color"] = color
            print(f"Updated room {room}: Color = {color}")
            if self.light_changed_callback is not None:
                self.light_changed_callback(room, "color", color)
        else:
            print(f"Unknown room {room}")

    def update_room_state(self, room, state):
        if room in self.rooms:
            self.rooms[room]["state"] = state
            print(f"Updated room {room}: State = {state}")
            if self.light_changed_callback is not None:
                self.light_changed_callback(room, "state", state)
        else:
            print(f"Unknown room {room}")

    def get_room_state(self, room):
        if room in self.rooms:
            return self.rooms[room]
        else:
            print(f"Unknown room {room}")
            return None


lightManager = LightManager()
