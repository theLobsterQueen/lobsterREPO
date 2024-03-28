# IMPORTS API MODULES #
import coremodule
import inputmodule

# SCRIPT ATTRIBUTES #
input_ref = inputmodule.input_ref

class testScript(coremodule.BaseScript) :
    # INTRINSIC MODULES #
    def _awake(self) :
        self.player = coremodule.Transform(0)
        new_mesh = coremodule.Mesh(1, "portrait.ply", "jinx.png")
        self.trans = coremodule.Transform(1, [ 0.0, 0.0, -10.0 ], [ 0.0, -90.0, 0.0 ])
        self.fire = 0.0
        self.fire_max = 0.5
        self.bullet_id = 2
        self.bullets = []

    def _update(self, delta_time) :
        self.fire -= delta_time
        if input_ref.is_key_pressed("w") :
            self.player.translate([0.0, 0.0, -20.0 * delta_time])
        if input_ref.is_key_pressed("s") :
            self.player.translate([0.0, 0.0, 20.0 * delta_time])
        if input_ref.is_key_pressed("a") :
            self.player.translate([-20.0 * delta_time, 0.0, 0.0])
        if input_ref.is_key_pressed("d") :
            self.player.translate([20.0 * delta_time, 0.0, 0.0])
        if input_ref.is_key_pressed("q") :
            self.player.rotate([0.0, -90.0 * delta_time, 0.0])
        if input_ref.is_key_pressed("e") :
            self.player.rotate([0.0, 90.0 * delta_time, 0.0])
        if input_ref.is_key_pressed(" ") and self.fire <= 0.0 :
            bullet = coremodule.Transform(self.bullet_id, self.player.get_position(), 
                    self.player.get_rotation(), [0.1, 0.01, 0.1])
            coremodule.Mesh(self.bullet_id, "portrait.ply", "rock.png")
            self.bullet_id += 1
            if self.bullet_id > 32 :
                self.bullet_id = 2
            self.bullets.append(bullet)
            self.fire = self.fire_max

        self.trans.translate([ 0.0, 0.0, -10.0 * delta_time ], True)
        self.trans.rotate([ 0.0, 30.0 * delta_time, 0.0 ])
        for bullet in self.bullets :
            bullet.translate([0.0, 0.0, -0.5 * delta_time])
        
