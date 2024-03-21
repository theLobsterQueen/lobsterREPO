# IMPORTS AND INITIALIZATIONS #

# LOBSTER API IMPORTS #
import coremodule

class testFile(coremodule.BaseScript) :
    # INTRINSIC METHODS #
    def _start(self) :
        # READS AND SETS DATA #
        self._set_data("parent_name", "Bingus Ascendant")
        self.transRef = self.get_component("Transform")

        # SCALES THE TRANSFORM #
        self.transRef.scale([ 0.5, 1.1, 0.5 ])

    def _update(self, delta_time) :
        # MOVES THE TRANSFORM UP AND ROTATES IT #
        self.transRef.rotate([ 0, 90 * delta_time, 0 ])

    # ATTRIBUTES #
    transRef = None
