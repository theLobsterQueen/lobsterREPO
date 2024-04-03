# IMPORTS PYTHON MODULES #
import inspect
from datetime import datetime

# IMPORTS LOBSTER MODULES #
import inputmodule

# CORE MODULE ATTRIBUTES #
comps = [ ]
orders = [ ]
script_refs = { }

# CORE MODULE FUNCTIONS #
def clear_orders() :
    orders.clear()
def clear_comps() :
    comps.clear()

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
    globals()[comp_name](entID)

def change_data(comp_name, entID, attribute_name, new_value) :
    comp_name = comp_name.lower()
    attrib = getattr(get_component(comp_name, entID), attribute_name)
    attrib = new_value
    
# UTILITY FUNCTIONS #
def add_vecs(vec_1, vec_2) :
    if len(vec_1) != len(vec_2) :
        raise ValueError("ERROR, ATTEMPTING TO ADD TWO VECS OF DIFFERENT DIMENSIONS!")
    i = 0
    ret_vec = [ ]
    while i < len(vec_1) :
        ret_vec.append(vec_1[i] + vec_2[i])
        i += 1
    return ret_vec

def assign_vec(vec_1, vec_2) :
    if len(vec_1) != len(vec_2) :
        raise ValueError("ERROR, ATTEMPTING TO ADD TWO VECS OF DIFFERENT DIMENSIONS!")
    i = 0
    while i < len(vec_1) :
        vec_1[i] = vec_2[i]
        i += 1
    return vec_1

def get_timestamp() :
    now = datetime.now()
    return (now.strftime("<%H:%M:%S>: "))

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
    def __init__(self, input_ent, input_color = []) :
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

# ENTITY OBJECT DEFINITION #
    # THIS CLASS IS USED AS A WRAPPER OBJECT FOR TARGET ID'S, BECAUSE AN ID BY ITSELF CANNOT #
    # BE RETURNED TO A CLASS IMMEDIATELY AFTER CALLING A "GET_ID" LIKE FUNCTION, BUT A WRAPPER 
    # CAN HOLD A REFERENCE TO THE ID, WHICH WILL EVENTUALLY BE CONFIGURED CORRECTLY BEFORE THE #
    # FUNCTIONS THAT USE THE ID FOR THEIR OPERATIONS TAKE EFFECT #
class Entity() :
    def __init__(self) :
        self.id = 0
        self.deleted = False
    def __repr__(self) :
        return f"Entity wrapper with ID: {self.id}"
    def delete(self) :
        orders.append(("entity_deleteEntity", self))
        del(self)
    def get_component(self, comp_name) :
        return get_component(comp_name, self.id)

# BASE SCRIPT #
class BaseScript :
    """
        This class is the base-line script that all Python classes, which intend to interact
            with the Lobster Engine, must derive from.
    """

    # UTILITY METHODS FOR PARENT CLASS #
    def push_data(self) :
        temp = list(globals()["orders"])
        globals()["orders"].clear()
        return temp

    # UTILITY METHODS FOR CHILD SCRIPTS #
    def get_entity_by_name(self, input_name) :
        new_ent = Entity()
        orders.append(("entity_getEntityByName", new_ent, input_name, new_ent))
        return new_ent

    def add_entity(self, input_name) :
        new_ent = Entity()
        orders.append(("entity_addEntity", new_ent, input_name, new_ent))
        return new_ent

    def debug_log(self, input_message) :
        if type(input_message) != str :
            self.debug_error("ERROR! ATTEMPTED TO OUTPUT A NON-STRING DEBUG_LOG MESSAGE!")
            return
        orders.append(("DEBUG", get_timestamp() + input_message))

    def debug_error(self, input_message) :
        if type(input_message) != str :
            self.debug_error("ERROR! ATTEMPTED TO OUTPUT A NON-STRING DEBUG_ERROR MESSAGE!")
        orders.append(("ERROR", get_timestamp() + input_message))

    # PLACEHOLDER DEFINITIONS FOR CHILD SCRIPT METHODS #
    def _awake(self) :
        pass
    def _start(self) :
        pass
    def _update(self, delta_time) :
        pass


