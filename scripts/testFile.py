# IMPORTS AND INITIALIZATIONS #

# LOBSTER API IMPORTS #
import coremodule
import inputmodule

class testFile(coremodule.BaseScript) :
    # INTRINSIC METHODS #
    def _start(self) :
        # READS AND SETS DATA #
        self._set_data("parent_name", "Bingus Ascendant")
        self.transRef = self.get_component("transform")

    def _update(self, delta_time) :
        # MOVES THE TRANSFORM UP AND ROTATES IT #
        deltaPos = [ 0.0, 0.0, 0.0 ]
        speed = 20

        if inputmodule.is_key_pressed("w") :
            deltaPos[1] += speed * delta_time
        if inputmodule.is_key_pressed("s") :
            deltaPos[1] -= speed * delta_time
        if inputmodule.is_key_pressed("d") :
            deltaPos[0] += speed * delta_time
        if inputmodule.is_key_pressed("a") :
            deltaPos[0] -= speed * delta_time
        self.transRef.translate(deltaPos)
