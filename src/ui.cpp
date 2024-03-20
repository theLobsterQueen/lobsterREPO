// FILE DEFINITION INCLUDE //
#include <ui.h>

// REDUDANT INCLUDES TO APPEASE BILL GATES //
#include <filesystem>

// FUNCTION IMPLEMENTATIONS //
void uiManagement :: drawEditorUI(EngineCore * core)
{
	// DRAWS MENU BAR //
	if(ImGui :: BeginMainMenuBar())
	{
		if(ImGui :: BeginMenu("Scene"))
		{
			if(ImGui :: MenuItem("Save Scene"))
				sceneManagement :: saveScene(core -> curSceneRef);

			std :: filesystem :: directory_iterator dirIt;
			if(ImGui :: BeginMenu("Load Scene"))
			{
				try
				{ 
					dirIt = std :: filesystem :: begin
						(std :: filesystem :: directory_iterator("./scenes")); 
				}

				catch(std :: filesystem :: filesystem_error err)
				{
					dirIt = std :: filesystem :: begin
						(std :: filesystem :: directory_iterator("./../../scenes")); 
				}

				for(auto& p : dirIt)
				{
					std :: string fileName = p.path().stem().string();
					if(ImGui :: MenuItem(fileName.c_str()))
					{
						Scene * newScene = sceneManagement :: loadScene(std :: string(fileName + ".lscn"));
						sceneManagement :: sceneOut(newScene);
						sceneManagement :: changeScene(core, newScene);
					}
				}

				ImGui :: EndMenu();
			}

			ImGui :: EndMenu();
		}

		if(ImGui :: BeginMenu("Scripts"))
		{
			if(ImGui :: MenuItem("Compile Scene"))
				appManagement :: compileScripts(core);

			ImGui :: EndMenu();
		}
	} ImGui :: EndMainMenuBar();
	float topBarY = 20.0f;

	// DRAWS SCENE TREE //
	ImGui :: SetNextWindowPos(ImVec2(0, topBarY));
	ImGui :: SetNextWindowSize
	(ImVec2
	 	(
		 	core -> editorDataRef -> sidePanelWidth, 
			(core -> editorDataRef -> sidePanelHeight) - topBarY
		)
	);
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
	ImGui :: SetNextWindowPos
		(ImVec2((core -> winWidth) - (core -> editorDataRef -> sidePanelWidth), topBarY));
	ImGui :: SetNextWindowSize
	(ImVec2
	 	(
		 	core -> editorDataRef -> sidePanelWidth, 
		 	(core -> editorDataRef -> sidePanelHeight) - topBarY
		)
	);
	if(ImGui :: Begin("Inspector", NULL, core -> editorDataRef -> windowFlags))
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
								rotValue -= 360.0f;
							while(rotValue < -180)
								rotValue += 360.0f;
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
			 	"Camera Data\n%f\t%f\n%s\n%s", 
				curCamera -> near, curCamera -> far, 
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

		// RELAYS SCRIPT DATA //
		if((activeEntity.mask & (1 << SCRIPT_COMP_ID)) >= 1)
		{
			Script * curScript = (Script *) (core -> curSceneRef -> components[SCRIPT_COMP_ID]
				[core -> editorDataRef -> curActiveEntity]);
			ImGui :: Text("Script Data\n%s", curScript -> name.c_str());
		}
	} ImGui :: End();

	// DRAWS FILE SYSTEM //

}
