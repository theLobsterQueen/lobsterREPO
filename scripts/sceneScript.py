# IMPORTS AND INITIALIZATIONS #
import coremodule

# SCRIPT DEFINITION #
class sceneScript(coremodule.BaseScript) :
    def _awake(self) :
        self.ent_ref = self.get_entity_by_name("Bingus")
        self.new_ent = self.add_entity("Pingus")

        coremodule.Light(self.ent_ref)
        coremodule.Mesh(self.new_ent, "portrait.ply", "jinx.png")

    def _start(self) :
        self.trans_ref = self.new_ent.get_component("Transform")
        self.trans_ref.set_position((0.0, 0.0, 20.0))
        self.ent_ref.delete()
        bingus = self.ent_ref.get_component("Transform")
    def _update(self, delta_time) :
        pass
