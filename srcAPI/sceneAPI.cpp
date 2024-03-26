// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <iostream>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <scene.h>
#include <transform.h>
#include <light.h>
#include <APIUtils.h>

// STATIC TEST FUNCTIONS //
static Scene sceneRef;

// FUNCTIONS FOR SCENE CLASS //
void sceneOut(Scene& self)
{
	for(unsigned i = 0; i < LOBSTER_MAX_ENTITIES; i++)
	{
		Entity curEntity = self.entities[i];
		if(curEntity.mask == 0)
			continue;

		std :: cout << "ENTITY NAME: " << curEntity.name << std :: endl;
		if((curEntity.mask & (1 << TRANS_COMP_ID)) >= 1)
			std :: cout << "\tHAS TRANSFORM!" << std :: endl;
		if((curEntity.mask & (1 << MESH_COMP_ID)) >= 1)
			std :: cout << "\tHAS MESH!" << std :: endl;
		if((curEntity.mask & (1 << LIGHT_COMP_ID)) >= 1)
			std :: cout << "\tHAS LIGHT!" << std :: endl;
		if((curEntity.mask & (1 << CAMERA_COMP_ID)) >= 1)
			std :: cout << "\tHAS CAMERA!" << std :: endl;
	}
}

pybind11 :: object getEntity(Scene& self, entityID id)
	{ return pybind11 :: cast(self.entities[id]); }

int addEntity(Scene& self, std :: string entityName)
{
	bool returnStat = false;
	for(entityID i = 0; i < LOBSTER_MAX_ENTITIES; i++)
		if(self.entities[i].mask == 0)
		{
			self.activeEntities++;
			self.entities[i].mask |= (1 << TRANS_COMP_ID);
			self.entities[i].name = entityName;
			self.components[TRANS_COMP_ID][i] = (compPtr) (new Transform);
			return i;
		}
	return -1;
}

// RETURNS COMPONENT REFERENCES //
bool getTransComp(Scene& self, entityID id, Transform& transOut)
{
	if((self.entities[id].mask & (1 << TRANS_COMP_ID)) == 0)
		return false;

	transOut = (*((Transform *) (self.components[TRANS_COMP_ID][id])));
	return true; 
}

bool getLightComp(Scene& self, entityID id, Light& lightOut)
{
	if((self.entities[id].mask & (1 << LIGHT_COMP_ID)) == 0)
		return false;

	lightOut = (*((Light *) (self.components[LIGHT_COMP_ID][id])));
	return true; 
}

// BINDS SCENE CLASS //
PYBIND11_MODULE(_sceneapi, m)
{
	pybind11 :: class_<Scene>(m, "Scene")
		.def("get_entity", &getEntity)
		.def("add_entity", &addEntity)
		.def("scene_out", &sceneOut)

		.def("get_transform_comp", &getTransComp)
		.def("get_light_comp", &getLightComp);
}
