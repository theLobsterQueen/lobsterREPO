import coremodule

class demoScript(coremodule.BaseScript) :
    def _start(self) :
        self.trans_ref = coremodule.Transform(1)
        self.debug_log("Hewwo!")

    def _update(self, delta_time) :
        self.trans_ref.rotate([0.0, 140.0 * delta_time, 0.0])
