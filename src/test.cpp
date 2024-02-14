// INCLUDES DEFINITION FILES //
#include <test.h>

void testFuncs :: processInput(EngineCore * core, entityID inputID)
{
	// MOVES THE TRANSFORM UP AND DOWN //
	auto keys = core -> inputState -> pressedKeys;
	std :: vector<float> deltaPos = { 0.0f, 0.0f, 0.0f };
	std :: vector<float> deltaRot = { 0.0f, 0.0f, 0.0f };
	float deltaScale = 0.0f;
	float moveSpeed = 5.0f;
	float rotSpeed = 60.0f;
	float scaleSpeed = 0.5f;

	Camera * camera = (Camera * ) 
		(core -> curSceneRef -> components[CAMERA_COMP_ID][inputID]);

	if(keys['w'])
		deltaPos[2] += moveSpeed * (core -> deltaTime);
	if(keys['s'])
		deltaPos[2] -= moveSpeed * (core -> deltaTime);
	if(keys['a'])
		deltaPos[0] += moveSpeed * (core -> deltaTime);
	if(keys['d'])
		deltaPos[0] -= moveSpeed * (core -> deltaTime);
	if(keys[','])
		deltaPos[1] += moveSpeed * (core -> deltaTime);
	if(keys['.'])
		deltaPos[1] -= moveSpeed * (core -> deltaTime);

	if(keys['f'])
		deltaRot[0] += rotSpeed * (core -> deltaTime);
	if(keys['g'])
		deltaRot[0] -= rotSpeed * (core -> deltaTime);

	if(keys['k'])
		deltaScale += scaleSpeed * (core -> deltaTime);
	if(keys['l'])
		deltaScale -= scaleSpeed * (core -> deltaTime);

	if(keys['z'])
		deltaRot[2] += rotSpeed * (core -> deltaTime);
	if(keys['x'])
		deltaRot[2] -= rotSpeed * (core -> deltaTime);

	if(keys['q'])
		deltaRot[1] += rotSpeed * (core -> deltaTime);
	if(keys['e'])
		deltaRot[1] -= rotSpeed * (core -> deltaTime);

	// APPLIES TRANSFORMATION //
	transformHandler :: translate
	(
		(Transform *) core -> curSceneRef -> components[TRANS_COMP_ID][inputID],
		deltaPos
	);

	transformHandler :: rotate
	(
		(Transform *) core -> curSceneRef -> components[TRANS_COMP_ID][inputID],
		deltaRot
	);

	std :: vector<float> scale = { 1.0f, 1.0f, 1.0f };
	for(int i = 0; i < 3; i++)
		scale[i] *= deltaScale;

	transformHandler :: scale
	(
		(Transform *) core -> curSceneRef -> components[TRANS_COMP_ID][inputID],
		scale
	);
}
