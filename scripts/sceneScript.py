# IMPORTS AND INITIALIZATIONS #
import coremodule
import appmodule

# SCRIPT DEFINITION #
class sceneScript(coremodule.BaseScript) :
    def _start(self) :
        self.debug_log("The end is never the end...")
        self.counter = 0.25
        self.timer = 5

    def _update(self, delta_time) :
        self.counter -= delta_time
        if self.counter <= 0.0 :
            self.debug_log("is never the end...")
            self.counter = 0.25
            self.timer -= 1
            if self.timer == 0 :
                self.debug_error("This should be an error!")
        appmodule.change_scene("Bingus")

