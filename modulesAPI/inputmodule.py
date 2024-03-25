# IMPORTS API MODULES #
from _inputmodule import *

# FUNCTION DEFINITIONS #
def is_key_pressed(inputKey) :
    return input_ref.priv_is_key_pressed(inputKey.lower())
