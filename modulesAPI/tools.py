# IMPORTS AND INIITALIZATIONS #
from datetime import datetime

# ENITY OBJECT DEFINITION #
    # THIS CLASS IS USED AS A WRAPPER OBJECT FOR TARGET ID'S, BECAUSE AN ID BY ITSELF CANNOT #
    # BE RETURNED TO A CLASS IMMEDIATELY AFTER CALLING A "GET_ID" LIKE FUNCTION, BUT A WRAPPER 
    # CAN HOLD A REFERENCE TO THE ID, WHICH WILL EVENTUALLY BE CONFIGURED CORRECTLY BEFORE THE #
    # FUNCTIONS THAT USE THE ID FOR THEIR OPERATIONS TAKE EFFECT #
class Entity() :
    def __init__(self) :
        self.id = 0
        self.ready = False
    def __repr__(self) :
        return f"Entity wrapper with ID: {self.id}"
    def delete(self) :
        orders.append(("entity_deleteEntity", self))
        del(self)

# UTILITY FUNCTIONS #
orders_ref = [ ]
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
def get_entity_by_name(input_name) :
    new_ent = Entity()
    orders_ref.append(("entity_getEntityByName", new_ent, input_name, new_ent))
    return new_ent

def add_entity(input_name) :
    new_ent = Entity()
    orders_ref.append(("entity_addEntity", new_ent, input_name, new_ent))
    return new_ent

def debug_log(input_message) :
    if type(input_message) != str :
        self.debug_error("ERROR! ATTEMPTED TO OUTPUT A NON-STRING DEBUG_LOG MESSAGE!")
        return
    orders_ref.append(("DEBUG", get_timestamp() + input_message))

def debug_error(input_message) :
    if type(input_message) != str :
        self.debug_error("ERROR! ATTEMPTED TO OUTPUT A NON-STRING DEBUG_ERROR MESSAGE!")
    orders_ref.append(("ERROR", get_timestamp() + input_message))
