# IMPORTS PYTHON MODULES #
import inspect

# IMPORTS LOBSTER MODULES #
import inputmodule

# CORE MODULE ATTRIBUTES #
comps = [ ]
orders = [ ]
script_refs = { }

# CORE MODULE FUNCTIONS #
def clear_orders() :
    orders.clear()

# METHODS FOR HANDLING COMPONENTS #
def get_component(comp_name, entID) :
    for comp in comps :
        if comp.__class__.__name__ == comp_name.title() and comp.entity_id == entID :
            return comp
    raise ValueError(f"ERROR! COMP OF TYPE {comp_name} IN ENTITY {entID} COULD NOT BE FOUND!")

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

# COMPONENT DEFINITIONS #
class Component() :
    def __init__(self, comp_name, input_id) :
        self.entity_id = input_id
        comps.append(self)

    def __repr__(self) :
        return f"COMPONENT OF TYPE {self.__class__.__name__.upper()} IN ENTITY {self.entity_id}!"

class Transform(Component) :
    def __init__(self, input_id, position = [], rotation = [], scale = []) :
        super().__init__("Transform", input_id)

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
        orders.append(("transform_translate", self.entity_id, delta_vec, globally))
    def rotate(self, delta_vec) :
        orders.append(("transform_rotate", self.entity_id, delta_vec))
    def scale(self, delta_vec) :
        orders.append(("transform_scale", self.entity_id, delta_vec))

    def set_position(self, input_position) :
        self.position = input_position
        orders.append(("transform_setPosition", self.entity_id, input_position))
    def set_rotation(self, input_rotation) :
        self.rotation = input_rotation
        orders.append(("transform_setRotation", self.entity_id, input_rotation))
    def set_scale(self, input_scale) :
        self.scale = input_scale
        orders.append(("transform_setScale", self.entity_id, input_scale))

class Mesh(Component) :
    def __init__(self, input_id, mesh_name = "", tex_name = "") :
        super().__init__("Mesh", input_id)

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
        orders.append(("mesh_setMesh", self.entity_id, mesh_name))
    def set_texture(self, tex_name) :
        self.tex_name = tex_name
        orders.append(("mesh_setTexture", self.entity_id, tex_name))

class Light(Component) :
    def __init__(self, input_id, input_color = []) :
        super().__init__("Light", input_id)
        self.color = [ 1.0, 1.0, 1.0, 0.40 ]
        if len(input_color) != 0 :
            self.color = input_color
        self.set_light(self.color)

    # ORDER METHODS #
    def get_light(self) :
        return list(self.color)

    def set_light(self, input_color) :
        self.color = input_color
        orders.append(("light_setLight", self.entity_id, input_color))

# BASE SCRIPT #
class BaseScript :
    """
        This class is the base-line script that all Python classes, which intend to interact
            with the Lobster Engine, must derive from.
    """

    # UTILITY FUNCTIONS FOR THE BASE CLASS ONLY #
    def __init__(self, entity_id) :
        self.name = self.__class__.__name__
        self.id = entity_id

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


