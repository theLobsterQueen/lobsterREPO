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

void inputManagement :: processInput(EngineCore * core, entityID cameraID)
{
	// VARIABLE INITIALIZATION //
	auto keys = core -> inputState -> pressedKeys;
	std :: vector<float> deltaPos = { 0.0f, 0.0f, 0.0f };
	std :: vector<float> deltaRot = { 0.0f, 0.0f, 0.0f };
	int mouseDeltaX = core -> inputState -> mouseDeltaX;
	int mouseDeltaY = core -> inputState -> mouseDeltaY;
	int mousePosX = core -> inputState -> mouseX;
	int mousePosY = core -> inputState -> mouseY;

	// SCALARS FOR THE DIFFERENT OPERATIONS //
	static float panScalar = 0.1f;
	static float rotScalar = 2.0f;
	static float zoomScalar = 0.05f;
	static float keyMoveSpeed = 10.0f;

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
	if(keys['['])
		deltaPos[1] += speedVal;
	if(keys[']'])
		deltaPos[1] -= speedVal;

	// CAMERA INPUT: CHECKS TO ENSURE THAT CAMERA IS ACTUALLY WITHIN VIEWPORT //
	bool processMouse = true;
	if(!(core -> inputState -> mouseInput))
	{
		processMouse = false;
		if(mousePosX >= core -> editorDataRef -> sidePanelWidth
			&& mousePosX <= (core -> winWidth) - (core -> editorDataRef -> sidePanelWidth)
			&& mousePosY <= (core -> winHeight) - (core -> editorDataRef -> bottomPanelHeight))
		{
			processMouse = true;
		}
	}

	if(processMouse)
	{
		// IF BOTH LMB AND RMB ARE PRESSED: NO VALID INPUT COMBINATION, RETURNS //
		if(core -> inputState -> rmb && core -> inputState -> lmb)
			return;
		
		// PANS THE CAMERA/MOVES IT FORWARDS AND BACK //
		core -> inputState -> mouseInput = false;
		if(core -> inputState -> lmb == true)
		{
			core -> inputState -> mouseInput = true;
			if(core -> inputState -> shiftPressed)
				deltaPos[2] -= mouseDeltaY * panScalar;

			else
			{
				deltaPos[0] += mouseDeltaX * panScalar;
				deltaPos[1] += mouseDeltaY * panScalar;
			}
		}

		// ROTATES THE CAMERA //
		if(core -> inputState -> rmb == true || 
				(core -> inputState -> cntrlPressed && core -> inputState -> lmb == true))
		{
			core -> inputState -> mouseInput = true;
			deltaRot[1] += mouseDeltaX * rotScalar;
			deltaRot[0] -= mouseDeltaY * rotScalar;
		}

		// ZOOMS THE CAMERA IN //
		cameraHandler :: zoom
		(
			(Camera *) core -> curSceneRef -> components[CAMERA_COMP_ID][cameraID],
			(core -> inputState -> scroll) * zoomScalar
		);
	}

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
