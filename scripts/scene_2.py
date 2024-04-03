import coremodule
import scenemodule

class scene_2(coremodule.BaseScript) :
    def _start(self) :
        self.counter = 0.0
        self.step = 0.0

    def _update(self, delta_time) :
        self.counter += delta_time
        self.step += delta_time
        if self.step >= 0.1 :
            coremodule.debug_log(str(round(self.counter, 2)))
            self.step = 0.0

        if self.counter >= 3.0 :
            scenemodule.change_scene("Scene_1")
