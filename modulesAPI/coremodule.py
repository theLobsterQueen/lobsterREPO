# IMPORTS API MODULES #
from _coremodule import *
from _entityapi import *
from _sceneapi import *
from _transformapi import *
from _lightapi import *

class BaseScript :
    """
        This class is the base-line script that all Python classes, which intend to interact
            with the Lobster Engine, must derive from.
    """

    # CONSTRUCTOR/DESTRUCTOR ARGUMENTS #
    def __init__(self) :
        self.name = self.__class__.__name__
        self.data = { }
        self.comp_dict = { } # LIST OF COMPONENTS TO BE EVALUATED WHEN NEXT REVIEWED BY BACKEND #
        self.comps_to_add = [ ] # LIST OF COMPONENTS TO ADD WHEN NEXT REVIEWED BY BACKEND #

    def _initialize(self, entity_id) :
        # LISTS ALL COMPONENTS THAT THE SCRIPT WILL SEARCH FOR #
        componentList = [ "transform", "light" ]

        # SEARCHES FOR THEM #
        for comp in componentList :
            func = getattr(scene_ref, f"get_{comp}_comp")
            temp = globals()[comp.title()]()
            valid = func(entity_id, temp)
            if valid is True :
                self.comp_dict[comp] = temp

    # INTRINSIC METHODS #
    def _push_data(self) :
        for item in dir(self) :
            comp = type(getattr(self, item)).__name__.lower()
            if comp in self.comp_dict :
                self.data[comp] = getattr(self, item)
        dataCopy = dict(self.data)
        self.data = { }
        return dataCopy

    # UTILITY METHODS FOR CHILDREN #
    def _set_data(self, name, item) :
        self.data[name] = item

    # METHODS FOR HANDLING COMPONENTS #
    def get_component(self, comp) :
        if not self.comp_dict.get(comp) :
            raise ValueError(f"Entity does not have component of type {comp}!")
        return self.comp_dict[comp]

    def add_component(self, comp) :
        # IF ALREADY HAS COMPONENT, DOES NOTHING #
        if self.comp_dict.get(comp) :
            return
        self.comps_to_add.append(comp)
