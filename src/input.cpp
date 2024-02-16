// INCLUDES DEFINITION FILE //
#include <input.h>

// FUNCTION IMPLEMENTATIONS //
InputState * inputManagement :: createInputState()
{
	return new InputState;
}

void inputManagement :: resetInput(InputState * input)
{
	input -> mouseX = 0;
	input -> mouseY = 0;
	input -> scroll = 0;
}

void inputManagement :: processInput(EngineCore * core, entityID cameraID)
{
	// VARIABLE INITIALIZATION //
	auto keys = core -> inputState -> pressedKeys;
	std :: vector<float> deltaPos = { 0.0f, 0.0f, 0.0f };
	std :: vector<float> deltaRot = { 0.0f, 0.0f, 0.0f };
	
	// SCALARS FOR THE DIFFERENT OPERATIONS //
	static float panScalar = 0.1f;
	static float rotScalar = 2.0f;
	static float zoomScalar = 0.05f;
	static float keyMoveSpeed = 10.0f;

	// IF BOTH LMB AND RMB ARE PRESSED: NO VALID INPUT COMBINATION, RETURNS //
	if(core -> inputState -> rmb && core -> inputState -> lmb)
		return;
	
	// PANS THE CAMERA/MOVES IT FORWARDS AND BACK //
	if(core -> inputState -> lmb == true)
	{
		if(core -> inputState -> shiftPressed)
			deltaPos[2] -= (core -> inputState -> mouseY) * panScalar;

		else
		{
			deltaPos[0] += (core -> inputState -> mouseX) * panScalar;
			deltaPos[1] += (core -> inputState -> mouseY) * panScalar;
		}
	}

	// ROTATES THE CAMERA //
	if(core -> inputState -> rmb == true)
	{
		deltaRot[1] += (core -> inputState -> mouseX) * rotScalar;
		deltaRot[0] -= (core -> inputState -> mouseY) * rotScalar;
	}

	// ZOOMS THE CAMERA IN //
	cameraHandler :: zoom
	(
		(Camera *) core -> curSceneRef -> components[CAMERA_COMP_ID][cameraID],
		(core -> inputState -> scroll) * zoomScalar
	);

	// MOVES THE CAMERA VIA KEY INPUT //
	float speedVal = keyMoveSpeed * (core -> deltaTime);
	if(keys['w'])
		deltaPos[2] += speedVal;
	if(keys['s'])
		deltaPos[2] -= speedVal;
	if(keys['a'])
		deltaPos[0] += speedVal;
	if(keys['d'])
		deltaPos[0] -= speedVal;
	if(keys[' '])
		deltaPos[1] += speedVal;
	if(core -> inputState -> cntrlPressed)
		deltaPos[1] -= speedVal;

	// APPLIES TRANSFORMATION //
	transformHandler :: translate
	(
		(Transform *) core -> curSceneRef -> components[TRANS_COMP_ID][cameraID],
		deltaPos
	);

	transformHandler :: rotate
	(
		(Transform *) core -> curSceneRef -> components[TRANS_COMP_ID][cameraID],
		deltaRot
	);
}
