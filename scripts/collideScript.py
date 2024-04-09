import coremodule
class collideScript(coremodule.BaseScript) :
    def _update(self, delta_time) :
        pass

    def _on_collision(self, other_id) :
        coremodule.debug_log(f"COLLIDED WITH {other_id}!")
