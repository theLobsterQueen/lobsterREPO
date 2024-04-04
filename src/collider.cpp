// INCLUDES DECLARATION FILE //
#include <collider.h>

// DEFINES FUNCTIONS //
Collider * colliderHandler :: createCollider
	(float lIn, float rIn, float uIn, float dIn, float fIn, float bIn)
{
	Collider * newCollider = new Collider;
	newCollider -> lWidth = lIn;
	newCollider -> rWidth = rIn;
	newCollider -> uHeight = uIn;
	newCollider -> dHeight = dIn;
	newCollider -> fDepth = fIn;
	newCollider -> bDepth = bIn;
	return newCollider;
}

bool colliderHandler :: checkCollision(entityID ent1, entityID ent2)
{
	// VARIABLE INITIALIZATION //
	bool xCollide = false;
	bool yCollide = false;
	bool zCollide = false;

	// GRABS NECESARRY COMPONENT DATA FROM ENTITIES //
	Transform * trans1 = ((Transform *) (globals :: curSceneRef -> components
			[TRANS_COMP_ID][ent1]));
	Collider * col1 = ((Collider *) (globals :: curSceneRef -> components
			[COLLIDE_COMP_ID][ent1]));

	Transform * trans2 = ((Transform *) (globals :: curSceneRef -> components
			[TRANS_COMP_ID][ent2]));
	Collider * col2 = ((Collider *) (globals :: curSceneRef -> components
			[COLLIDE_COMP_ID][ent2]));

	// BEGINS ACTUAL COLLISION CALCULATIONS //
	float xLow1 = trans1 -> position[0] - col1 -> lWidth;
	float xHigh1 = trans1 -> position[0] + col1 -> rWidth;

	float xLow2 = trans2 -> position[0] - col2 -> lWidth;
	float xHigh2 = trans2 -> position[0] + col2 -> rWidth;

	// CALCULATES X-COLLISIONS //
	if(xHigh1 >= xLow2 && xLow1 <= xHigh2)
		xCollide = true;

	return xCollide;
}
