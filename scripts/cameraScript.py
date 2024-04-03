import coremodule
import inputmodule

class cameraScript(coremodule.BaseScript) :
    def _awake(self) :
        self.ent_ref = coremodule.get_entity_by_name("Camera")

    def _start(self) :
        self.trans_ref = coremodule.get_component("Transform", self.ent_ref)

    def _update(self, delta_time) :
        # VARIABLE INITIALIZATION #
        delta_vec = [ 0.0, 0.0, 0.0 ]
        delta_rot = [ 0.0, 0.0, 0.0 ]
        camera_speed = 20.0 * delta_time
        rot_speed = 60.0 * delta_time
        max_angle = 70

        # APPLIES VALUES #
        rot = self.trans_ref.get_rotation()
        if rot[0] > max_angle :
            rot[0] = max_angle
        if rot[0] < -max_angle :
            rot[0] = -max_angle
        self.trans_ref.set_rotation(rot)

        # MOVES FORWARDS/BACKWARDS #
        if(inputmodule.input_ref.is_key_pressed("w")) :
            delta_vec[2] -= camera_speed
        if(inputmodule.input_ref.is_key_pressed("s")) :
            delta_vec[2] += camera_speed
        # STRAFES LEFT/RIGHT #
        if(inputmodule.input_ref.is_key_pressed("a")) :
            delta_vec[0] -= camera_speed
        if(inputmodule.input_ref.is_key_pressed("d")) :
            delta_vec[0] += camera_speed
        # MOVES UP/DOWN #
        if(inputmodule.input_ref.is_key_pressed("q")) :
            delta_vec[1] -= camera_speed
        if(inputmodule.input_ref.is_key_pressed("e")) :
            delta_vec[1] += camera_speed

        # ROTATES ABOUT THE Y AXIS #
        if(inputmodule.input_ref.is_key_pressed("u")) :
            delta_rot[1] -= rot_speed
        if(inputmodule.input_ref.is_key_pressed("i")) :
            delta_rot[1] += rot_speed
        # ROTATES ABOUT THE X AXIS #
        if(inputmodule.input_ref.is_key_pressed("j")) :
            delta_rot[0] -= rot_speed
        if(inputmodule.input_ref.is_key_pressed("k")) :
            delta_rot[0] += rot_speed
        # ROTATES ABOUT THE Z AXIS #
        if(inputmodule.input_ref.is_key_pressed("n")) :
            delta_rot[2] -= rot_speed
        if(inputmodule.input_ref.is_key_pressed("m")) :
            delta_rot[2] += rot_speed

        self.trans_ref.translate(delta_vec)
        self.trans_ref.rotate(delta_rot)
