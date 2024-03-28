# IMPORTS AND INITIALIZATIONS #
import coremodule
import inputmodule

class exampleBingusScript(coremodule.BaseScript) :
    def _start(self) :
        self.transRef = self.get_component("Transform")
        self.speed = 15
        self.facing = "north"
        self.fire_max = 0.5
        self.fire = 0.0
        self.bullets = [ ]

    def _update(self, delta_time) :
        deltaVec = [ 0.0, 0.0, 0.0 ]

        if(inputmodule.is_key_pressed("w")) :
            deltaVec[1] += self.speed * delta_time
            self.facing = "north"

        if(inputmodule.is_key_pressed("s")) :
            deltaVec[1] -= self.speed * delta_time
            self.facing = "south"

        if(inputmodule.is_key_pressed("d")) :
            deltaVec[0] += self.speed * delta_time
            self.facing = "east"

        if(inputmodule.is_key_pressed("a")) :
            deltaVec[0] -= self.speed * delta_time
            self.facing = "west"

        # APPLIES DELTA TO TRANSFORM #
        self.transRef.translate(deltaVec)
        self.update_bullets(delta_time)
    
        # IF SPACE IS PRESSED, SHOOTS BULLETS #
        if self.fire > 0 :
            self.fire -= delta_time
        if self.fire < 0 :
            self.fire = 0
        if(inputmodule.is_key_pressed(" ") and self.fire == 0) :
            self.shoot()
            self.fire = self.fire_max

    def shoot(self) :
        bulletID = self.add_entity("Bullet")
        transRef = self.get_component("transform", bulletID)
        transRef.set_position(self.get_component("transform").get_position())
        self.add_component("light", bulletID)

        self.bullets.append([ bulletID, self.facing, 0.5, False ])

    def update_bullets(self, delta_time) :
        for bullet_tuple in self.bullets :
            bulletID = bullet_tuple[0]
            bullet_facing = bullet_tuple[1]
            life = bullet_tuple[2]
            bullet_speed = 20 * delta_time
            
            # REDUCES LIFETIME OF BULLET #
            life -= delta_time
            if life <= 0.0 :
                self.bullets.remove(bullet_tuple)
                self.remove_entity(bulletID)
                return

            bullet_tuple[2] = life
            if bullet_tuple[3] is False :
                self.get_component("light", bulletID)
                bullet_tuple[3] = True

            transRef = self.get_component("transform", bulletID)
            if bullet_facing == "south" or bullet_facing == "west" :
                bullet_speed *= -1

            if bullet_facing == "north" or bullet_facing == "south" :
                transRef.translate([0.0, bullet_speed, 0.0])
            if bullet_facing == "west" or bullet_facing == "east" :
                transRef.translate([bullet_speed, 0.0, 0.0])

