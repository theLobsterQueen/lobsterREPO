# THIS FILE CONTROLS SCENE MANAGEMENT, BY CHANGING THE CURRENT SCENE, RESETTING SCENES, ETC. #

# IMPORTS VARIABLES FROM THE CORE MODULE #
from coremodule import orders
from coremodule import Entity

def change_scene(input_scene) :
    orders.append(("scene_changeScene", Entity(), input_scene))

