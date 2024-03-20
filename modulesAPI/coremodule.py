# IMPORTS PYBIND11 CODE AND TOOLS #
from _coremodule import *

# IMPORTS OTHER MODULES TAHT SHOULD BE PACKAGED INTO THIS ONE #
from _entityapi import *
from _sceneapi import *
from _transformapi import *

class BaseScript :
    """
        This class is the base-line script that all Python classes, which intend to interact
            with the Lobster Engine, must derive from.
    """

    # CONSTRUCTOR/DESTRUCTOR ARGUMENTS #
    def __init__(self, entity_id) :
        self.name = self.__class__.__name__
        self.data = { }
        self.entity_id = entity_id

    # INTRINSIC METHODS #
    def _push_data(self) :
        for item in dir(self) :
            itemRef = getattr(self, item)
            if type(itemRef) == Transform :
                self.data["transform"] = itemRef
        dataCopy = dict(self.data)
        self.data = { }
        return dataCopy

    # UTILITY METHODS FOR CHILDREN #
    def _set_data(self, name, item) :
        self.data[name] = item

    def get_component(self, comp) :
        if comp == "Transform" :
            return scene_ref.get_trans_comp(self.entity_id)
        raise ValueError("Invalid comp name!")
