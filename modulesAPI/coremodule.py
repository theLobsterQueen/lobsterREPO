# IMPORTS PYBIND11 CODE AND TOOLS #
from _coremodule import *

# IMPORTS OTHER MODULES TAHT SHOULD BE PACKAGED INTO THIS ONE #
from _entityapi import *
from _transformapi import *

class BaseScript :
    """
        This class is the base-line script that all Python classes, which intend to interact
            with the Lobster Engine, must derive from.
    """
    # INTRINSIC METHODS #
    def __init__(self) :
        self.name = self.__class__.__name__
        self.data = { }

    def _push_data(self) :
        dataCopy = dict(self.data)
        self.data = { }
        return dataCopy
