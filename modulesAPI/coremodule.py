# IMPORTS PYBIND11 CODE AND TOOLS #
from _coremodule import *

class BaseScript :
    """
        This class is the base-line script that all Python classes, which intend to interact
            with the Lobster Engine, must derive from.
    """

    # CONSTRUCTOR/DESTRUCTOR ARGUMENTS #
    def __init__(self) :
        self.name = self.__class__.__name__
        scripts.add_script(self.name)

    def __del__(self) :
        for attribute in dir(self) :
            if attribute in watched_methods :
                scripts.add_script_func(self.name, attribute, getattr(self, attribute))

    # ATTRIBUTE DEFINITIONS #
    name = ""
