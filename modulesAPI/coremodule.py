# IMPORTS PYTHON MODULES #
import inspect

# IMPORTS API MODULES #
from _coremodule import *
from _entityapi import *
from _sceneapi import *
from _meshapi import *
from _transformapi import *
from _lightapi import *

# CORE MODULE ATTRIBUTES #
script_refs = { }

class BaseScript :
    """
        This class is the base-line script that all Python classes, which intend to interact
            with the Lobster Engine, must derive from.
    """

    # UTILITY FUNCTIONS FOR THE BASE CLASS ONLY #
    def __init__(self) :
        self.name = self.__class__.__name__
        self.comps = [] # LIST OF COMPONENTS TO BE EVALUATED WHEN NEXT REVIEWED BY BACKEND #
            # INCLUDES AN ENTITY ID REFERENCE AS THE SECOND PART OF EACH ENTRY #
        self.comps_to_add = [] # LIST OF COMPONENTS TO ADD WHEN NEXT REVIEWED BY BACKEND #

        self.id = 0

    def _initialize(self, entity_id) :
        # LISTS ALL COMPONENTS THAT THE SCRIPT WILL SEARCH FOR #
        componentList = [ "transform", "light", "mesh" ]
        self.id = entity_id

        # SEARCHES FOR THEM #
        for comp in componentList :
            func = getattr(scene_ref, f"get_{comp}_comp")
            temp = globals()[comp.title()]()
            valid = func(entity_id, temp)
            if valid is True :
                self.comps.append((comp, temp, self.id))

    def _push_data(self) :
        return self.comps
    def _reset_values(self) :
        for item in self.comps :
            if item[0] == "mesh" :
                item[1].disable_reload()

    # PLACEHOLDER DEFINITIONS FOR CHILD SCRIPT METHODS #
    def _awake(self) :
        pass
    def _start(self) :
        pass
    def _update(self, delta_time) :
        pass

    # METHODS FOR HANDLING COMPONENTS #
    def get_component(self, comp, entID = -1) :
        # RETURNS OWN COMPONENTS #
        comp = comp.lower()
        if entID == -1 :
            entID = self.id
            ret_comp = None
            for comp_tuple in self.comps :
                if comp == comp_tuple[0] and self.id == comp_tuple[2] :
                   ret_comp = comp_tuple[1] 

            if ret_comp == None :
                raise ValueError(f"Entity does not have component of type {comp}!")
            return ret_comp

        # OTHERWISE, ATTEMPTS TO GET COMPONENT FROM ID #
        func = getattr(scene_ref, f"get_{comp.lower()}_comp")
        temp = globals()[comp.title()]() 
        valid = func(entID, temp)
        if valid is True :
            self.comps.append((comp, temp, entID))
            return temp
        else :
            raise ValueError(f"ERROR! COULD NOT FIND {comp} IN ENTITY ID {entID}!")

    def add_component(self, comp, entID = -1) :
        if entID == -1 :
            entID = self.id
        self.comps_to_add.append((comp.lower(), entID))
    
    def push_to_add(self) :
        temp = list(self.comps_to_add)
        self.comps_to_add = []
        return temp

    # METHODS FOR HANDLING ENTITIES #
    def add_entity(self, entName) :
        val = scene_ref.add_entity(entName)
        if(val != -1) :
            return val
        else :
            print("ERROR! ATTEMPTED TO ADD ENTITY TO SCENE, BUT SCENE IS ALREADY AT " +\
                    "MAX ENTITIES!")
            raise ValueError
