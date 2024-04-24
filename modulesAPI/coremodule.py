# IMPORTS PYTHON MODULES #
import inspect

from tools import *

# CORE MODULE ATTRIBUTES #
comps = [ ]
orders = [ ]
orders = orders_ref # ASSIGNS THE PLACEHOLDER REF IN TOOLS TO THE ACTUAL ORDERS LIST #
script_refs = { }

# CORE MODULE FUNCTIONS #
def clear_orders() :
    orders.clear()

def clear_comps() :
    comps.clear()

def change_scene(input_scene) :
    orders.append(("scene_changeScene", Entity(), input_scene))

# METHODS FOR HANDLING COMPONENTS #
def get_component(comp_name, entID) :
    if type(entID) != int :
        if type(entID) == Entity :
            entID = entID.id

        else :
            raise TypeError("ERROR! Input \"ID\" value for get_component() was not of int type!")
    comp_name = comp_name.title()
    for comp in comps :
        if comp.__class__.__name__ == comp_name and comp.entity.id == entID :
            return comp

    # IF COULD NOT FIND COMPONENT REFERENCE, CREATES ONE AND RETURNS IT #
    try :
        globals()[comp_name]
    except KeyError :
        print(f"ERROR! COULD NOT FIND {comp_name} IN COMPONENT DIRECTORY!")

    return globals()[comp_name](entID)

def change_data(comp_name, entID, attribute_name, new_value) :
    comp_name = comp_name.lower()
    attrib = getattr(get_component(comp_name, entID), attribute_name)
    attrib = new_value
    
# COMPONENT DEFINITIONS #
class Component() :
    def __init__(self, comp_name, input_entity) :
        entity = input_entity
        self.type = comp_name
        if type(input_entity) != Entity :
            entity = Entity()
            entity.id = input_entity
        self.entity = entity

        # LOOKS FOR DUPLICATES IN THE COMP LIST. IF NONE EXIST, ADDS THIS COMP #
        comps.append(self)

    def __repr__(self) :
        return (f"COMPONENT OF TYPE {self.__class__.__name__.upper()} IN ENTITY {self.entity.id}"
        + f" AT {hex(id(self))}!")

class Transform(Component) :
    def __init__(self, input_entity, position = [], rotation = [], scale = []) :
        super().__init__(self.__class__.__name__, input_entity)

        # ATTRIBUTE DEFINITIONS #
        self.position = [ 0.0, 0.0, 0.0 ]
        self.rotation = [ 0.0, 0.0, 0.0 ]
        self.scale = [ 0.0, 0.0, 0.0 ]

        # CHECKS FOR ORDERS PACK INTO INIT #
        if len(position) != 0 :
            self.set_position(position)
        if len(rotation) != 0 :
            self.set_rotation(rotation)
        if len(scale) != 0 :
            self.set_scale(scale)

    # GETTER METHODS #
    def get_position(self) :
        return list(self.position)
    def get_rotation(self) :
        return list(self.rotation)
    def get_scale(self) :
        return list(self.scale)

    # SETTER METHODS #
    def sync_data(self, input_position, input_rotation, input_scale) :
        self.position = input_position
        self.rotation = input_rotation
        self.scale = input_scale

    # ORDER METHODS #
    def translate(self, delta_vec, globally = False) :
        orders.append(("transform_translate", self.entity, delta_vec, globally))

    def rotate(self, delta_vec) :
        orders.append(("transform_rotate", self.entity, delta_vec))

    def scale(self, delta_vec) :
        orders.append(("transform_scale", self.entity, delta_vec))

    def set_position(self, input_position) :
        self.position = input_position
        orders.append(("transform_setPosition", self.entity, input_position))
    def set_rotation(self, input_rotation) :
        self.rotation = input_rotation
        orders.append(("transform_setRotation", self.entity, input_rotation))
    def set_scale(self, input_scale) :
        self.scale = input_scale
        orders.append(("transform_setScale", self.entity, input_scale))

class Mesh(Component) :
    def __init__(self, input_entity, mesh_name = "", tex_name = "") :
        super().__init__(self.__class__.__name__, input_entity)

        # ATTRIBUTE DEFINITIONS #
        self.tex_name = ""
        self.mesh_name = ""
    
        # LOOKS FOR ODERS PACKED INTO THE INIT #
        if mesh_name != "" :
            self.set_mesh(mesh_name)
        if tex_name != "" :
            self.set_texture(tex_name)

    # GETTERS #
    def get_mesh(self) :
        return self.mesh_name
    def get_texture(self) :
        return self.tex_name

    # ORDER METHODS #
    def set_mesh(self, mesh_name) :
        self.mesh_name = mesh_name
        orders.append(("mesh_setMesh", self.entity, mesh_name))
    def set_texture(self, tex_name) :
        self.tex_name = tex_name
        orders.append(("mesh_setTexture", self.entity, tex_name))

class Light(Component) :
    def __init__(self, input_entity, input_color = []) :
        super().__init__(self.__class__.__name__, input_entity)
        self.color = [ 1.0, 1.0, 1.0, 0.40 ]
        if len(input_color) != 0 :
            self.color = input_color
        self.set_light(self.color)

    # ORDER METHODS #
    def get_light(self) :
        return list(self.color)

    def set_light(self, input_color) :
        self.color = input_color
        orders.append(("light_setLight", self.entity, input_color))

class Script(Component) :
    def __init__(self, input_entity, input_script = "") :
        super().__init__(self.__class__.__name__, input_entity)
        self.script_ref = ""
        orders.append(("script_addScript", self.entity))
        if input_script != "" :
            self.set_script(input_script)

    def set_script(self, input_script) :
        if type(input_script) != str :
            self.debug_log\
                    ("ERROR! Attempted to set script reference, but input_script was not a string!", True)
        if script_refs.get(input_script) :
            seld.debug_log(f"ERROR! '{input_script}' could not be found!", True)

        self.script_ref = input_script
        orders.append(("script_setScript", self.entity, input_script))

# BASE SCRIPT #
class BaseScript :
    """
        This class is the base-line script that all Python classes, which intend to interact
            with the Lobster Engine, must derive from.
    """

    # CONSTRUCTOR #
    def __init__(self, ent_ref) :
        self.id = ent_ref

    # UTILITY METHODS FOR PARENT CLASS #
    def push_data(self) :
        temp = list(globals()["orders"])
        globals()["orders"].clear()
        return temp

    # PLACEHOLDER DEFINITIONS FOR CHILD SCRIPT METHODS #
    def _awake(self) :
        pass
    def _start(self) :
        pass
    def _update(self, delta_time) :
        pass
    def _on_collision(self, other_id) :
        pass


