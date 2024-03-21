# IMPORTS AND INITIALIZATIONS #

# LOBSTER API IMPORTS #
import coremodule

class lightScript(coremodule.BaseScript) :
    # INTRINSIC METHODS #
    def _start(self) :
        # READS AND SETS DATA #
        self._set_data("parent_name", "Leet Light")
        self.lightRef = self.get_component("light")
        self.value = 1.0
        self.value_scalar = -1.0

    def _update(self, delta_time) :
        self.value += (delta_time * self.value_scalar)
        if self.value < 0 or self.value > 1 :
            self.value_scalar = self.value_scalar * -1.0
        self.lightRef.set_color([1.0, 1.0, 1.0, self.value])
