// FILE DEFINITION INCLUDE //
#include <ui.h>

// REDUDANT INCLUDES TO APPEASE BILL GATES //
#include <filesystem>

// FUNCTION IMPLEMENTATIONS //
void uiManagement :: drawEditorUI()
{
	// VARIABLE INITIALIZATION //
	static bool savingScene = false;

	static bool deleteSceneConfirmation = false;
	static std :: string deleteSceneTarget = "";

	static unsigned sidePanelMargin = 12;

	// DRAWS MENU BAR //
	if(ImGui :: BeginMainMenuBar())
	{
		if(ImGui :: BeginMenu("File"))
		{
			if(ImGui :: MenuItem("Save Scene"))
				sceneManagement :: saveScene(globals :: curSceneRef);
			if(ImGui :: MenuItem("Save Scene As..."))
				savingScene = true;

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
			
			if(ImGui :: BeginMenu("Delete Scene"))
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
					{
						deleteSceneTarget = fileName;
						deleteSceneConfirmation = true;
					}
				}
				ImGui :: EndMenu();
			}

			ImGui :: EndMenu();
		}

		if(ImGui :: BeginMenu("Scene"))
		{
			if(ImGui :: MenuItem("Play Scene"))
			{
				appManagement :: compileScripts();
				appManagement :: startScripts(true);
				sceneManagement :: saveScene(globals :: curSceneRef);
				globals :: isPlaying = true;
			}
			if(ImGui :: MenuItem("Stop Scene"))
				globals :: isPlaying = false;
			if(ImGui :: MenuItem("Reset Scene"))
			{
				globals :: isPlaying = false;
				sceneManagement :: changeScene(sceneManagement :: loadScene
						(std :: string(globals :: curSceneRef -> name + ".lscn")));
			}
			ImGui :: EndMenu();
		}

	} ImGui :: EndMainMenuBar();
	float topBarY = 20.0f;

	// DRAWS THE "SAVE SCENE AS" MODAL, IF IT'S ACTIVE //
	if(savingScene)
	{
		ImGui :: OpenPopup("Save Scene As...");
		ImGui :: SetNextWindowSize(ImVec2(globals :: winWidth * 0.4f, globals :: winHeight * 0.1f));
		if(ImGui :: BeginPopupModal("Save Scene As..."), NULL, editorGlobals :: windowFlags)
		{
			static char inputString[LOB_FILE_NAME_MAX] = "";
			if(ImGui :: InputText
					("File Name", inputString, LOB_FILE_NAME_MAX, editorGlobals :: inputTextFlags))
			{
				sceneManagement :: saveScene(globals :: curSceneRef, std :: string(inputString));
				savingScene = false;
			}

			if(ImGui :: Button("Submit"))
			{
				sceneManagement :: saveScene(globals :: curSceneRef, std :: string(inputString));
				savingScene = false;
			}

			ImGui :: SameLine();
			if(ImGui :: Button("Cancel"))
				savingScene = false;
		}
		ImGui :: EndPopup();
	}

	// DRAWS THE "DELETE SCENE" MODAL, IF IT'S ACTIVE //
	if(deleteSceneConfirmation)
	{
		ImGui :: OpenPopup("Delete Scene...");
		ImGui :: SetNextWindowSize(ImVec2(globals :: winWidth * 0.3f, globals :: winHeight * 0.1f));
		if(ImGui :: BeginPopupModal("Delete Scene..."), NULL, editorGlobals :: windowFlags)
		{
			ImGui :: Text("Are you sure you want to delete %s?", deleteSceneTarget.c_str());
			if(ImGui :: Button("Yes"))
			{
				std :: remove(std :: string
					(APIGlobals :: workingPath + "./scenes/" + deleteSceneTarget + ".lscn").c_str());
				deleteSceneConfirmation = false;
			}
			ImGui :: SameLine();
			if(ImGui :: Button("No"))
				deleteSceneConfirmation = false;
		}
		ImGui :: EndPopup();
	}

	// DRAWS ACTIVE USER INTERFACE, IF IN DEVELOPMENT MODE //
	if(globals :: isPlaying)
		return;

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

		// CREATES BUTTON FOR ADDING ENTITIES TO THE SCENE //
		if(ImGui :: Button(uiManagement :: centeredString("Add Entity", sidePanelMargin).c_str()))
		{
			entityID newEntity = sceneManagement :: newEntityID(globals :: curSceneRef, "New Entity");
			sceneManagement :: addComp
			(
				globals :: curSceneRef, newEntity,
				TRANS_COMP_ID, (compPtr) transformHandler :: createTransform()
			);
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
		static std :: string holder = "";
		if(holder.size() != 16)
			holder.resize(16);

		ImGui :: Text("Name: ");
		ImGui :: SameLine();
		if(ImGui :: InputTextWithHint
			("##1", activeEntity.name.c_str(), holder.data(), 16, editorGlobals :: inputTextFlags))
		{ 
			globals :: curSceneRef -> entities[editorGlobals :: curActiveEntity].name = holder; 
			holder = "";
			holder.resize(16);
		}

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
			ImGui :: Separator();
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
			ImGui :: Separator();
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
			ImGui :: Separator();
			Mesh * curMesh = (Mesh *) globals :: curSceneRef -> components[MESH_COMP_ID]
				[editorGlobals :: curActiveEntity];

			ImGui :: Text("Mesh Data\n%s\n%s", curMesh -> name.c_str(), curMesh -> texName.c_str());
		}

		// RELAYS SCRIPT DATA //
		if((activeEntity.mask & (1 << SCRIPT_COMP_ID)) >= 1)
		{
			ImGui :: Separator();
			Script * curScript = (Script *) (globals :: curSceneRef -> components[SCRIPT_COMP_ID]
				[editorGlobals :: curActiveEntity]);
			ImGui :: Text("Script Data\n%s", curScript -> name.c_str());
		}

		// DRAWS BUTTON FOR ADDING NEW COMPONENTS //
		std :: string menuTitle = uiManagement :: centeredString("Add Component", sidePanelMargin);
		if(ImGui :: BeginMenu(menuTitle.c_str()))
		{
			for(unsigned curComp = 0; curComp < CUR_COMP_MAX; curComp++)
			{
				// SKIPS IF ENTITY ALREADY HAS COMPONENT //
				if((activeEntity.mask & (1 << curComp)) >= 1)
					continue;
				// SKIPS OVER INVALID COMPS //
				if(curComp == MESH_COMP_ID || curComp == SCRIPT_COMP_ID)
					continue;

				if(ImGui :: MenuItem(compToString(curComp).c_str()))
				{
					sceneManagement :: addComp
					(
						globals :: curSceneRef,
						editorGlobals :: curActiveEntity,
						curComp, constructComp(curComp)
					);
				}
			}
			ImGui :: EndMenu();
		}

		// DRAWS BUTTON FOR DESTROYING THIS ENTITY //
		ImGui :: Text((centeredString("", sidePanelMargin)).c_str());
		ImGui :: SameLine();
		if(ImGui :: Button("Delete Entity"))
			sceneManagement :: deleteEntity(globals :: curSceneRef, editorGlobals :: curActiveEntity);
	} ImGui :: End();
}

std :: string uiManagement :: centeredString(const char * targetBase, unsigned margin)
{
	std :: string retString = "";
	for(unsigned i = 0; i < margin; i++)
		retString += " ";
	retString += targetBase;
	return retString;
}

