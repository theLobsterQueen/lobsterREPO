# THIS MODULE CONTROLS THE GENERAL BEHAVIOR OF THE APP. FOR EXAMPLE, QUITTING THE GAME, #
    # LOADING SCENES, SETTING OPTIONS, ETC. #

# IMPORTS AND INITIALIZATIONS #
import coremodule

def change_scene(input_scene) :
    coremodule.orders.append("scene_changeScene", input_scene)
