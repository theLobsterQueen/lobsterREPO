# IMPORTS AND INITIALIZATIONS #
import coremodule
import inputmodule

# CONSTANTS #
c_bullet_life = 10.0
c_bullet_speed = 30.0

class playerScript(coremodule.BaseScript) :
    def _awake(self) :
        self.player_ref = self.get_entity_by_name("Player")
        self.bullets = []
        self.id = 0
        self.fire_timer = 0.0
        self.fire_max = 0.5

    def _start(self) :
        self.trans_ref = coremodule.get_component("Transform", self.player_ref)
        self.move_speed = 20

    def _update(self, delta_time) :
        delta_move = [ 0.0, 0.0, 0.0 ]

        # MOVES THE PLAYER FORWARD/BACKWARDS #
        if inputmodule.input_ref.is_key_pressed("w") :
            delta_move[1] += self.move_speed * delta_time
        if inputmodule.input_ref.is_key_pressed("s") :
            delta_move[1] -= self.move_speed * delta_time

        # ROTATES THE PLAYER #
        if inputmodule.input_ref.is_key_pressed("a") :
            delta_move[0] -= self.move_speed * delta_time
        if inputmodule.input_ref.is_key_pressed("d") :
            delta_move[0] += self.move_speed * delta_time 

        # APPLIES VALUES #
        self.trans_ref.translate(delta_move, True)

        # SHOOTS BULLETS #
        self.fire_timer -= delta_time
        if inputmodule.input_ref.is_key_pressed(" ") and self.fire_timer <= 0.0 :
            self.fire_timer = self.fire_max
            bullet_ent = self.add_entity(f"Bullet{self.id}")
            self.bullets.append([bullet_ent, c_bullet_life, False])
            self.id += 1

        # UPDATES BULLETS #
        self.update_bullets(delta_time)

    def update_bullets(self, delta_time) :
        for bullet in self.bullets :
            if bullet[2] is False :
                bullet[2] = True
                continue

            if bullet[2] is True :
                self.debug_log(str(self.trans_ref))
                self.debug_log(str(bullet[0]))
                bullet[2] = coremodule.get_component("Transform", bullet[0])
                bullet[2].set_position(self.trans_ref.get_position())
                bullet[2].set_rotation([ 0.0, -90.0, 0.0 ]) 
                bullet[2].set_scale([ 0.10, 0.10, 0.10 ])
                coremodule.Mesh(bullet[0], "portrait.ply", "rock.png")

            # MOVES BULLET FORWARD #
            bullet[2].translate([0.0, c_bullet_speed * delta_time, 0.0], True)
