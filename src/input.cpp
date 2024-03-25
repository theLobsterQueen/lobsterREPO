// INCLUDES DEFINITION FILE //
#include <input.h>

// FUNCTION IMPLEMENTATIONS //
InputState * inputManagement :: createInputState()
{
	return new InputState;
}

void inputManagement :: resetInput(InputState * input)
{
	input -> mouseDeltaX = 0;
	input -> mouseDeltaY = 0;
	input -> scroll = 0;
}

void inputManagement :: processInput(entityID cameraID)
{
	// VARIABLE INITIALIZATION //
	auto keys = globals :: inputState -> pressedKeys;
	std :: vector<float> deltaPos = { 0.0f, 0.0f, 0.0f };
	std :: vector<float> deltaRot = { 0.0f, 0.0f, 0.0f };
	int mouseDeltaX = globals :: inputState -> mouseDeltaX;
	int mouseDeltaY = globals :: inputState -> mouseDeltaY;
	int mousePosX = globals :: inputState -> mouseX;
	int mousePosY = globals :: inputState -> mouseY;

	// SCALARS FOR THE DIFFERENT OPERATIONS //
	static float panScalar = 0.1f;
	static float rotScalar = 2.0f;
	static float zoomScalar = 0.05f;
	static float keyMoveSpeed = 10.0f;
	static float keyRotSpeed = 30.0f;

	// MOVES THE CAMERA VIA KEY INPUT //
	if(!(editorGlobals :: keyInput))
	{
		float speedVal = keyMoveSpeed * (globals :: deltaTime);
		float rotVal = keyRotSpeed * (globals :: deltaTime);
		if(keys['w'])
			deltaPos[2] -= speedVal;
		if(keys['s'])
			deltaPos[2] += speedVal;
		if(keys['q'])
			deltaRot[1] -= rotVal;
		if(keys['e'])
			deltaRot[1] += rotVal;
		if(keys['r'])
			deltaRot[0] += rotVal;
		if(keys['t'])
			deltaRot[0] -= rotVal;
		if(keys['a'])
			deltaPos[0] -= speedVal;
		if(keys['d'])
			deltaPos[0] += speedVal;
		if(keys['['])
			deltaPos[1] -= speedVal;
		if(keys[']'])
			deltaPos[1] += speedVal;
	}

	// CAMERA INPUT: CHECKS TO ENSURE THAT CAMERA IS ACTUALLY WITHIN VIEWPORT //
	if(!ImGui :: GetIO().WantCaptureMouse)
	{
		// IF BOTH LMB AND RMB ARE PRESSED: NO VALID INPUT COMBINATION, RETURNS //
		if(globals :: inputState -> rmb && globals :: inputState -> lmb)
			return;
		
		// PANS THE CAMERA/MOVES IT FORWARDS AND BACK //
		globals :: inputState -> mouseInput = false;
		if(globals :: inputState -> lmb == true)
		{
			globals :: inputState -> mouseInput = true;
			if(globals :: inputState -> shiftPressed)
				deltaPos[2] += mouseDeltaY * panScalar;

			else
			{
				deltaPos[0] -= mouseDeltaX * panScalar;
				deltaPos[1] += mouseDeltaY * panScalar;
			}
		}

		// ROTATES THE CAMERA //
		if(globals :: inputState -> rmb == true || 
				(globals :: inputState -> cntrlPressed && globals :: inputState -> lmb == true))
		{
			globals :: inputState -> mouseInput = true;
			deltaRot[1] -= mouseDeltaX * rotScalar;
			deltaRot[0] -= mouseDeltaY * rotScalar;
		}

		// ZOOMS THE CAMERA IN //
		cameraHandler :: zoom
		(
			(Camera *) globals :: curSceneRef -> components[CAMERA_COMP_ID][cameraID],
			(globals :: inputState -> scroll) * zoomScalar
		);
	}

	// APPLIES TRANSFORMATION //
	transformHandler :: translate
	(
		(Transform *) globals :: curSceneRef -> components[TRANS_COMP_ID][cameraID],
		deltaPos
	);

	transformHandler :: rotate
	(
		(Transform *) globals :: curSceneRef -> components[TRANS_COMP_ID][cameraID],
		deltaRot
	);
}
