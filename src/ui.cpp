// FILE DEFINITION INCLUDE //
#include <ui.h>

// FUNCTION IMPLEMENTATIONS //
void uiManagement :: drawEditorUI(EngineCore * core)
{
	// DRAWS SCENE TREE //
	if(ImGui :: Begin("Scene Tree", NULL, core -> editorDataRef -> windowFlags))
	{
		for(entityID i = 0; i < LOBSTER_MAX_ENTITIES; i++)
		{
			Entity tempEntity = core -> curSceneRef -> entities[i];
			if(tempEntity.mask == 0)
				continue;

			if(core -> editorDataRef -> entitySelected == false)
			{
				core -> editorDataRef -> entitySelected = true;
				core -> editorDataRef -> curActiveEntity = i;
			}

			ImGui :: TreeNodeEx
			(
				tempEntity.name.c_str(), 
				core -> editorDataRef -> sceneTreeFlags |= ImGuiTreeNodeFlags_Leaf
			);
			if(ImGui :: IsItemClicked())
				core -> editorDataRef -> curActiveEntity = i;
			ImGui :: TreePop();
		}
	} ImGui :: End();

	// DRAWS INSPECTOR //
	ImGui :: SetNextWindowSize
		(ImVec2((core -> winWidth) * 0.3f, (core -> winHeight) * 0.8f), ImGuiCond_FirstUseEver);
	if(ImGui :: Begin("Inspector"))
	{
		// GETS ENTITY CURRENTLY SELECTED IN SCENE TREE //
		Entity activeEntity = core -> curSceneRef -> entities[core -> editorDataRef -> curActiveEntity];

		// WRITES OUT ENTITY DATA //
		ImGui :: Text("Name: %s", activeEntity.name.c_str());

		// RELAYS TRANSFORM COMPONENT DATA //
		if((activeEntity.mask & (1 << TRANS_COMP_ID)) >= 1)
		{
			Transform * curTrans = (Transform *) core -> curSceneRef -> components[TRANS_COMP_ID]
				[core -> editorDataRef -> curActiveEntity];
			
			ImGui :: Text("Transform Data\n");
			if(ImGui :: BeginTable("TransformTable", 3))
			{
				for(int row = 0; row < 3; row++)
				{
					ImGui :: TableNextRow();
					std :: vector<float> curVec;
					switch(row)
					{
						case 0 :
							curVec = transformHandler :: getGlobalPosition(curTrans);
						break;

						case 1 :
							curVec = curTrans -> rotation;
						break;

						case 2 :
							curVec = curTrans -> scale;
						break;
					};

					for(int column = 0; column < 3; column++)
					{
						ImGui :: TableSetColumnIndex(column);
						if(row == 1)
						{
							float rotValue = curVec[column];
							while(rotValue > 180)
								rotValue -= 180.0f;
							while(rotValue < -180)
								rotValue += 180.0f;
							curVec[column] = rotValue;
						}
						ImGui :: Text("%f", curVec[column]);
					}
				}
				ImGui :: EndTable();			
			}
		}

		// RELAYS CAMERA DATA //
		if((activeEntity.mask & (1 << CAMERA_COMP_ID)) >= 1)
		{
			Camera * curCamera = (Camera *) core -> curSceneRef -> components[CAMERA_COMP_ID]
				[core -> editorDataRef -> curActiveEntity];
			
			ImGui :: Text
			(
			 	"Camera Data\n%f\t%f\n%f\n%s\n%s", 
				curCamera -> near, curCamera -> far, 
				curCamera -> FOV,
				curCamera -> curPipelineRef -> vertShaderName.c_str(),
				curCamera -> curPipelineRef -> fragShaderName.c_str()
			);
		}

		// RELAYS LIGHT DATA //
		if((activeEntity.mask & (1 << LIGHT_COMP_ID)) >= 1)
		{
			Light * curLight = (Light *) core -> curSceneRef -> components[LIGHT_COMP_ID]
				[core -> editorDataRef -> curActiveEntity];
			
			ImGui :: Text
			(
			 	"Light Data\n%f, %f, %f, %f", 
				curLight -> color[0], curLight -> color[1], curLight -> color[2], curLight -> color[3]
			);
		}

		// RELAYS MESH DATA //
		if((activeEntity.mask & (1 << MESH_COMP_ID)) >= 1)
		{
			Mesh * curMesh = (Mesh *) core -> curSceneRef -> components[MESH_COMP_ID]
				[core -> editorDataRef -> curActiveEntity];

			ImGui :: Text("Mesh Data\n%s", curMesh -> name.c_str());
		}
	} ImGui :: End();
}
