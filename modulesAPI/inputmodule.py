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

# MODULE ATTRIBUTES #
input_ref = InputState()

# FUNCTION DEFINITIONS #
def is_key_pressed(input_key) :
    key = input_key.lower()
    if not input_ref.pressed_keys.get(key) :
        input_ref.pressed_keys[key] = False
    return input_ref.pressed_keys[key]
