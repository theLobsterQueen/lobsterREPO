# IMPORTS AND INITIALIZATIONS #

# LOBSTER API IMPORTS #
import coremodule

class testFile(coremodule.BaseScript) :
    # INTRINSIC METHODS #
    def _start(self) :
        # READS AND SETS DATA #
        self._set_data("parent_name", "Bingus Ascendant")
        self.transRef = self.get_component("transform")
        self.transRef.rotate([0, -90, 0])

    def _update(self, delta_time) :
        # MOVES THE TRANSFORM UP AND ROTATES IT #
        self.transRef.rotate([ 0, 20 * delta_time, 0 ])
