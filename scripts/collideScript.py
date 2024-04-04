import coremodule
class collideScript(coremodule.BaseScript) :
    def _on_collision(self, other_id) :
        coremodule.debug_log(f"COLLIDED WITH {other_id}!")
