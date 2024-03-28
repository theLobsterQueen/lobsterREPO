# CLASS DEFINITIONS #
class InputState :
    # CONSTRUCTOR/DESTRUCTOR ARGUMENT #
    def __init__(self) :
        self.pressed_keys = { }
        
    # METHOD DEFINITIONS #
    def press_key(self, input_key) :
        self.pressed_keys[input_key.lower()] = True
    def unpress_key(self, input_key) :
        self.pressed_keys[input_key.lower()] = False

    def is_key_pressed(self, input_key) :
        key = input_key.lower()
        if not self.pressed_keys.get(key) :
            self.pressed_keys[key] = False
        return self.pressed_keys[key]

# MODULE ATTRIBUTES #
input_ref = InputState()
