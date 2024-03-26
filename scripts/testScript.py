# IMPORTS API MODULES #
import coremodule

class testScript(coremodule.BaseScript) :
    # INTRINSIC MODULES #
    def _start(self) :
        bingusID = self.add_entity("Bingus")
        self.add_component("Light", bingusID)

