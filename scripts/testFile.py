# IMPORTS AND INITIALIZATIONS #

# LOBSTER API IMPORTS #
import coremodule

class testFile(coremodule.BaseScript) :
    # INTRINSIC METHODS #
    def _start(self) :
        pass

    def _update(self, delta_time) :
        self.counter += delta_time
        if self.counter >= 1.0 :
            self.counter = 0.0
            if self.ticked :
                print("Tick")
            else :
                print("Tock")
            self.ticked = not self.ticked

    # UTILITY METHODS #
    def utility(self) :
        pass

    # ATTRIBUTES #
    counter = 0.0
    ticked = False
testFile()
