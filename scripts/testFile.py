# IMPORTS AND INITIALIZATIONS #

# LOBSTER API IMPORTS #
import coremodule

class testFile(coremodule.BaseScript) :
    # INTRINSIC METHODS #
    def _start(self) :
        self._set_data("parent_name", "Bingus Ascendant")
        self.transRef = self.get_component("Transform")

    def _update(self, delta_time) :
        self.transRef.rotate([ 0, 90 * delta_time, 0 ])

    # ATTRIBUTES #
    transRef = None
