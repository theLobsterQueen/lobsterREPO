import coremodule
class testScript(coremodule.BaseScript) :
    def _awake(self) :
        self.trans_ref = coremodule.get_component("Transform", self.id)
        self.camera_ref = coremodule.get_component("Transform", coremodule.get_entity_by_name("Camera"))
        self.ent_ref = coremodule.add_entity("Bingus")
        self.scalar = 1.0
        self.col_cooldown = 0.0
        self.c_col_max = 0.1

    def _start(self) :
        self.ent_trans = coremodule.get_component("Transform", coremodule.get_entity_by_name("Bingus"))

    def _update(self, delta_time) :
        speed = 10.0
        self.col_cooldown -= delta_time
        self.trans_ref.translate( [ speed * self.scalar * delta_time, 0.0, 0.0 ], True)
        self.camera_ref.translate( [ speed * self.scalar * delta_time, 0.0, 0.0 ], True)

    def _on_collision(self, other_id) :
        if self.col_cooldown <= 0.0 :
            self.scalar *= -1
            self.col_cooldown = self.c_col_max
            coremodule.get_component("Transform", other_id).translate([0.0, 1.0, 0.0])
