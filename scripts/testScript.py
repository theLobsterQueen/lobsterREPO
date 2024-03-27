# IMPORTS API MODULES #
import coremodule

class testScript(coremodule.BaseScript) :
    # INTRINSIC MODULES #
    def _awake(self) :
        self.meshRef = self.get_component("Mesh")

    def _start(self) :
        self.meshRef.change_tex("rock.png")
