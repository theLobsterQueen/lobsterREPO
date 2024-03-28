# IMPORTS AND INITIALIZATIONS #
import coremodule
import inputmodule

class exampleBingusScript(coremodule.BaseScript) :
    def _start(self) :
        self.transRef = coremodule.get_component("transform", self.id)
        self.light = coremodule.Light(self.id)
        self.speed = 15
        self.facing = "north"
        self.fire_max = 0.5
        self.fire = 0.0
        self.bullets = [ ]
        self.bulletID = 3

    def _update(self, delta_time) :
        deltaVec = [ 0.0, 0.0, 0.0 ]
        if(inputmodule.input_ref.is_key_pressed("w")) :
            deltaVec[1] += self.speed * delta_time
            self.facing = "north"

        if(inputmodule.input_ref.is_key_pressed("s")) :
            deltaVec[1] -= self.speed * delta_time
            self.facing = "south"

        if(inputmodule.input_ref.is_key_pressed("d")) :
            deltaVec[0] += self.speed * delta_time
            self.facing = "east"

        if(inputmodule.input_ref.is_key_pressed("a")) :
            deltaVec[0] -= self.speed * delta_time
            self.facing = "west"

        # APPLIES DELTA TO TRANSFORM #
        self.transRef.translate(deltaVec, True)
        self.update_bullets(delta_time)
    
        # IF SPACE IS PRESSED, SHOOTS BULLETS #
        if self.fire > 0 :
            self.fire -= delta_time
        if self.fire < 0 :
            self.fire = 0
        if(inputmodule.input_ref.is_key_pressed(" ") and self.fire == 0) :
            self.shoot()
            self.fire = self.fire_max

    def shoot(self) :
        bullet_trans = coremodule.Transform(self.bulletID, self.transRef.get_position(),
                [ 0.0, -90.0, 0.0 ], [ 0.10, 0.10, 0.10 ])
        coremodule.Mesh(self.bulletID, "portrait.ply", "rock.png")
        coremodule.Light(self.bulletID, [ 0.8, 0.4, 0.4, 0.3 ])
        self.bulletID += 1
        self.bullets.append([ bullet_trans, self.facing, 0.5 ])

    def update_bullets(self, delta_time) :
        for bullet_tuple in self.bullets :
            bullet_trans = bullet_tuple[0]
            bullet_facing = bullet_tuple[1]
            life = bullet_tuple[2]
            bullet_speed = 20 * delta_time
            
            # REDUCES LIFETIME OF BULLET #
            life -= delta_time
            if life <= 0.0 :
                self.bullets.remove(bullet_tuple)
                return
            bullet_tuple[2] = life

            if bullet_facing == "south" or bullet_facing == "west" :
                bullet_speed *= -1
            if bullet_facing == "north" or bullet_facing == "south" :
                bullet_trans.translate([0.0, bullet_speed, 0.0], True)
            if bullet_facing == "west" or bullet_facing == "east" :
                bullet_trans.translate([bullet_speed, 0.0, 0.0], True)

