// FILE DEFINITION INCLUDE //
#include <ui.h>

// REDUDANT INCLUDES TO APPEASE BILL GATES //
#include <filesystem>

// FUNCTION IMPLEMENTATIONS //
void uiManagement :: drawEditorUI()
{
	// DRAWS MENU BAR //
	if(ImGui :: BeginMainMenuBar())
	{
		if(ImGui :: BeginMenu("File"))
		{
			if(ImGui :: MenuItem("Save Scene"))
				sceneManagement :: saveScene(globals :: curSceneRef);
			if(ImGui :: MenuItem("Save Scene As..."))
				editorGlobals :: savingScene = true;

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
					std :: string pathName = p.path().string();
					if(pathName.find(".lscn") == std :: string :: npos 
						|| pathName.find(".swp") != std :: string ::npos)
					{
						continue;
					}

					std :: string fileName = p.path().stem().string();
					if(ImGui :: MenuItem(fileName.c_str()))
						sceneManagement :: changeScene(sceneManagement :: loadScene
								(std :: string(fileName + ".lscn")));
				}
				ImGui :: EndMenu();
			}
			ImGui :: EndMenu();
		}

		if(ImGui :: BeginMenu("Scene"))
		{
			if(ImGui :: MenuItem("Compile Scene"))
			{
				appManagement :: compileScripts();
				appManagement :: startScripts(true);
			}
			if(ImGui :: MenuItem("Play Scene"))
				globals :: isPlaying = true;
			if(ImGui :: MenuItem("Stop Scene"))
				globals :: isPlaying = false;
			ImGui :: EndMenu();
		}

	} ImGui :: EndMainMenuBar();
	float topBarY = 20.0f;

	// DRAWS THE SAVE SCENE AS MODAL, IF IT'S ACTIVE //
	if(editorGlobals :: savingScene)
	{
		ImGui :: OpenPopup("Save Scene As...");
		ImGui :: SetNextWindowSize(ImVec2(globals :: winWidth * 0.4f, globals :: winHeight * 0.3f));
		if(ImGui :: BeginPopupModal("Save Scene As..."), NULL, editorGlobals :: windowFlags)
		{
			static char inputString[LOB_FILE_NAME_MAX] = "";
			if(ImGui :: InputText
					("File Name", inputString, LOB_FILE_NAME_MAX, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				sceneManagement :: saveScene(globals :: curSceneRef, std :: string(inputString));
				editorGlobals :: savingScene = false;
			}
		}
		ImGui :: EndPopup();
	}

	// DRAWS SCENE TREE //
	ImGui :: SetNextWindowPos(ImVec2(0, topBarY));
	ImGui :: SetNextWindowSize
	(ImVec2
	 	(
		 	editorGlobals :: sidePanelWidth, 
			(editorGlobals :: sidePanelHeight) - topBarY
		)
	);
	if(ImGui :: Begin("Scene Tree", NULL, editorGlobals :: windowFlags))
	{
		for(entityID i = 0; i < LOBSTER_MAX_ENTITIES; i++)
		{
			Entity tempEntity = globals :: curSceneRef -> entities[i];
			if(tempEntity.mask == 0)
				continue;

			if(editorGlobals :: entitySelected == false)
			{
				editorGlobals :: entitySelected = true;
				editorGlobals :: curActiveEntity = i;
			}

			ImGui :: TreeNodeEx
			(
				tempEntity.name.c_str(), 
				editorGlobals :: sceneTreeFlags |= ImGuiTreeNodeFlags_Leaf
			);
			if(ImGui :: IsItemClicked())
				editorGlobals :: curActiveEntity = i;
			ImGui :: TreePop();
		}
	} ImGui :: End();

	// DRAWS INSPECTOR //
	ImGui :: SetNextWindowPos
		(ImVec2((globals :: winWidth) - (editorGlobals :: sidePanelWidth), topBarY));
	ImGui :: SetNextWindowSize
	(ImVec2
	 	(
		 	editorGlobals :: sidePanelWidth, 
		 	(editorGlobals :: sidePanelHeight) - topBarY
		)
	);
	if(ImGui :: Begin("Inspector", NULL, editorGlobals :: windowFlags))
	{
		// GETS ENTITY CURRENTLY SELECTED IN SCENE TREE //
		Entity activeEntity = globals :: curSceneRef -> entities[editorGlobals :: curActiveEntity];

		// WRITES OUT ENTITY DATA //
		ImGui :: Text("Name: %s", activeEntity.name.c_str());

		// RELAYS TRANSFORM COMPONENT DATA //
		if((activeEntity.mask & (1 << TRANS_COMP_ID)) >= 1)
		{
			Transform * curTrans = (Transform *) globals :: curSceneRef -> components[TRANS_COMP_ID]
				[editorGlobals :: curActiveEntity];
			
			std :: string columnNames[3] = { "X", "Y", "Z" };
			std :: string rowNames[3] = { "_p", "_r", "_s" };
			ImGui :: Text("Transform Data\n");
			std :: vector<float> * transVecIndices[3] = 
				{ &(curTrans -> position), &(curTrans -> rotation), &(curTrans -> scale) };
			for(int row = 0; row < 3; row++)
			{
				ImGui :: PushItemWidth(editorGlobals :: sidePanelWidth * 0.2f);
				for(int column = 0; column < 3; column++)
				{
					ImGui :: InputFloat
					(
					 	std :: string(columnNames[column] + rowNames[row]).c_str(), 
						&((*transVecIndices[row])[column]),
						-0.01f, -1.0f, "%.2f"
					);
					if(column != 2)
						ImGui :: SameLine();
				}
				ImGui :: PopItemWidth();
			}
		}

		// RELAYS CAMERA DATA //
		if((activeEntity.mask & (1 << CAMERA_COMP_ID)) >= 1)
		{
			Camera * curCamera = (Camera *) globals :: curSceneRef -> components[CAMERA_COMP_ID]
				[editorGlobals :: curActiveEntity];
			
			ImGui :: Text
			(
			 	"Camera Data\n%.2f\t%.2f\n%.2f\n%s\n%s", 
				curCamera -> near, curCamera -> far, 
				curCamera -> aspectRatio,
				curCamera -> curPipelineRef -> vertShaderName.c_str(),
				curCamera -> curPipelineRef -> fragShaderName.c_str()
			);
		}

		// RELAYS LIGHT DATA //
		if((activeEntity.mask & (1 << LIGHT_COMP_ID)) >= 1)
		{
			Light * curLight = (Light *) globals :: curSceneRef -> components[LIGHT_COMP_ID]
				[editorGlobals :: curActiveEntity];
			ImGui :: Text("Light Data");
			static const char * colorNames[4] = { "r", "g", "b", "a" };
			for(int i = 0; i < 4; i++)
			{
				ImGui :: InputFloat
				(
					colorNames[i], &(curLight -> color[i]),
					-0.01f, -1.0f, "%.2f"
				);
			}
		}

		// RELAYS MESH DATA //
		if((activeEntity.mask & (1 << MESH_COMP_ID)) >= 1)
		{
			Mesh * curMesh = (Mesh *) globals :: curSceneRef -> components[MESH_COMP_ID]
				[editorGlobals :: curActiveEntity];

			ImGui :: Text("Mesh Data\n%s\n%s", curMesh -> name.c_str(), curMesh -> texName.c_str());
		}

		// RELAYS SCRIPT DATA //
		if((activeEntity.mask & (1 << SCRIPT_COMP_ID)) >= 1)
		{
			Script * curScript = (Script *) (globals :: curSceneRef -> components[SCRIPT_COMP_ID]
				[editorGlobals :: curActiveEntity]);
			ImGui :: Text("Script Data\n%s", curScript -> name.c_str());
		}
	} ImGui :: End();

	// DRAWS FILE SYSTEM //

}
