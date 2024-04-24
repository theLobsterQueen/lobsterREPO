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
			if(ImGui :: MenuItem("New Scene"))
				sceneManagement :: changeScene(appManagement :: createBaseScene());
			ImGui :: Separator();
			if(ImGui :: MenuItem("Save Scene"))
				sceneManagement :: saveScene(globals :: curSceneRef);
			if(ImGui :: MenuItem("Save Scene As..."))
				savingScene = true;
			ImGui :: Separator();

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
				sceneManagement :: play();
			if(ImGui :: MenuItem("Stop Scene"))
				sceneManagement :: stop();
			if(ImGui :: MenuItem("Reset Scene"))
			{
				sceneManagement :: stop();
				sceneManagement :: changeScene(sceneManagement :: loadScene
						(std :: string(globals :: curSceneRef -> name + ".lscn")));
			}
			ImGui :: EndMenu();
		}

		if(ImGui :: BeginMenu("Options"))
		{
			if(ImGui :: MenuItem("Hide UI"))
			{
				editorGlobals :: optionsRef -> hideUIWhilePlaying =
					!editorGlobals :: optionsRef -> hideUIWhilePlaying;
				windowManagement :: changeSize(globals :: winWidth, globals :: winHeight);
			}
			if(editorGlobals :: optionsRef -> hideUIWhilePlaying)
			{
				ImGui :: SameLine();
				ImGui :: Text("X");
			}

			ImGui :: Separator();
			if(ImGui :: BeginMenu("Resize Window"))
			{
				if(ImGui :: MenuItem("1600x900"))
					windowManagement :: changeSize(1600, 900);
				if(ImGui :: MenuItem("1366x768"))
					windowManagement :: changeSize(1366, 768);
				ImGui :: EndMenu();
			}

			ImGui :: Separator();
			if(ImGui :: BeginMenu("Vertex Shader"))
			{
				std :: filesystem :: directory_iterator dirIt;
				try
				{ 
					dirIt = std :: filesystem :: begin
						(std :: filesystem :: directory_iterator("./shaders")); 
				}

				catch(std :: filesystem :: filesystem_error err)
				{
					dirIt = std :: filesystem :: begin
						(std :: filesystem :: directory_iterator("./../../shaders")); 
				}

				for(auto& p : dirIt)
				{

					std :: string pathName = p.path().string();
					if(pathName.find(".vert") == std :: string :: npos 
						|| pathName.find(".swp") != std :: string ::npos)
					{
						continue;
					}

					std :: string fileName = p.path().stem().string();
					if(ImGui :: MenuItem(fileName.c_str()))
					{
						graphicManagement :: loadShader
							(globals :: pipelineRefs[0], GL_VERTEX_SHADER, (fileName + ".vert").c_str());
						graphicManagement :: compileProgram(globals :: pipelineRefs[0]);
					}
				}
				ImGui :: EndMenu();
			}
			if(ImGui :: BeginMenu("Fragment Shader"))
			{
				std :: filesystem :: directory_iterator dirIt;
				try
				{ 
					dirIt = std :: filesystem :: begin
						(std :: filesystem :: directory_iterator("./shaders")); 
				}

				catch(std :: filesystem :: filesystem_error err)
				{
					dirIt = std :: filesystem :: begin
						(std :: filesystem :: directory_iterator("./../../shaders")); 
				}

				for(auto& p : dirIt)
				{

					std :: string pathName = p.path().string();
					if(pathName.find(".frag") == std :: string :: npos 
						|| pathName.find(".swp") != std :: string ::npos)
					{
						continue;
					}

					std :: string fileName = p.path().stem().string();
					if(ImGui :: MenuItem(fileName.c_str()))
					{
						graphicManagement :: loadShader
							(globals :: pipelineRefs[0], GL_FRAGMENT_SHADER, (fileName + ".frag").c_str());
						graphicManagement :: compileProgram(globals :: pipelineRefs[0]);
						graphicManagement :: printPipeline(globals :: pipelineRefs[0]);
					}
				}
				ImGui :: EndMenu();
			}
			ImGui :: EndMenu();
		}

	} ImGui :: EndMainMenuBar();
	float topBarY = 20.0f;

	// DRAWS THE "SAVE SCENE AS" MODAL, IF IT'S ACTIVE //
	if(savingScene)
	{
		std :: string saveName = "";
		if(uiManagement :: getInputString("Save Scene as...", saveName))
		{
		   if(saveName != "")	
			   sceneManagement :: saveScene(globals :: curSceneRef, saveName); 
		   savingScene = false;
		}
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
	if(globals :: isPlaying && editorGlobals :: optionsRef -> hideUIWhilePlaying)
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
		if
		(
		 	ImGui :: InputTextWithHint
			(
				std :: to_string(editorGlobals :: curActiveEntity).c_str(), activeEntity.name.c_str(), 
				holder.data(), 16, editorGlobals :: inputTextFlags
			)
		)

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
	
			std :: string rowNames[3] = { "P", "R", "S" };
			std :: string columnNames[3] = { "_x", "_y", "_z" };
			ImGui :: Text("Transform Data");
			std :: vector<float> * transVecIndices[3] = 
				{ &(curTrans -> position), &(curTrans -> rotation), &(curTrans -> scale) };
			for(int row = 0; row < 3; row++)
			{
				ImGui :: PushItemWidth(editorGlobals :: sidePanelWidth * 0.2f);
				for(int column = 0; column < 3; column++)
				{
					ImGui :: InputFloat
					(
					 	std :: string(rowNames[row] + columnNames[column]).c_str(), 
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
			
			ImGui :: Text("Camera Data");
			uiManagement :: deleteButton(CAMERA_COMP_ID);
			ImGui :: Text
			(
			 	"\n%.2f\t%.2f\n%.2f\n%s\n%s", 
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
			uiManagement :: deleteButton(LIGHT_COMP_ID);
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

			ImGui :: Text("Mesh Data");
			uiManagement :: deleteButton(MESH_COMP_ID);
			ImGui :: SameLine();
			if(ImGui :: Button("Clear Cache"))
			{
				globals :: meshCache.clear();
				globals :: textureCache.clear();
			}

			static std :: string meshHolder = "";
			static std :: string texHolder  = "";

			if(meshHolder.size() != 16)
				meshHolder.resize(16);
			if(ImGui :: InputTextWithHint
				("##2", curMesh -> name.c_str(), meshHolder.data(), 16, editorGlobals :: inputTextFlags))
			{ 
				Mesh * mesh = meshHandler :: getMeshFromPLY(meshHolder);
				if(mesh != nullptr)
				{
					sceneManagement :: addComp
					(
						globals :: curSceneRef, editorGlobals :: curActiveEntity,
						MESH_COMP_ID, (compPtr) (meshHandler :: getMeshFromPLY(meshHolder))
					);
				}
				meshHolder = "";
				meshHolder.resize(17);
			}
			ImGui :: SameLine();
			if(ImGui :: Button("Clear"))
				sceneManagement :: addComp
				(
					globals :: curSceneRef, editorGlobals :: curActiveEntity,
					MESH_COMP_ID, (compPtr) (meshHandler :: getMeshFromPLY(""))
				);			

			if(texHolder.size() != 16)
				texHolder.resize(16);
			if(ImGui :: InputTextWithHint
				("##3", curMesh -> texName.c_str(), texHolder.data(), 16, editorGlobals :: inputTextFlags))
			{
				Texture * tex = textureHandler :: createTexture(texHolder);
				if(tex != nullptr)
					meshHandler :: setTexture(curMesh, textureHandler :: createTexture(texHolder));
				texHolder = "";
				texHolder.resize(16);
			}
			ImGui :: SameLine();
			if(ImGui :: Button("Clear"))
				meshHandler :: setTexture(curMesh, textureHandler :: createTexture(""));
		}

		// RELAYS COLLIDER DATA //
		if((activeEntity.mask & (1 << COLLIDE_COMP_ID)) >= 1)
		{
			ImGui :: Separator();
			Collider * curCollider = (Collider *) (globals :: curSceneRef -> components
				[COLLIDE_COMP_ID][editorGlobals :: curActiveEntity]);
			ImGui :: Text("Collider Data");
			float * collideMatrix[9] =
			{
				&(curCollider -> rWidth), &(curCollider -> lWidth),
				&(curCollider -> uHeight), &(curCollider -> dHeight),
				&(curCollider -> fDepth), &(curCollider -> bDepth)
			};

			std :: string nameMatrix[9] = 
			{ 
				"W_r", "W_l",
				"H_u", "H_d",
				"D_f", "D_b" 
			};
			uiManagement :: deleteButton(COLLIDE_COMP_ID);
			for(int row = 0; row < 3; row++)
			{
				ImGui :: PushItemWidth(editorGlobals :: sidePanelWidth * 0.2f);
				for(int column = 0; column < 2; column++)
				{
					ImGui :: InputFloat
					(
					 	nameMatrix[(row * 2) + column].c_str(), 
						&(*collideMatrix[(row * 2) + column]),
						-0.01f, -1.0f, "%.2f"
					);
					if(column != 1)
						ImGui :: SameLine();
				}
				ImGui :: PopItemWidth();
			}
		}

		// RELAYS SCRIPT DATA //
		if((activeEntity.mask & (1 << SCRIPT_COMP_ID)) >= 1)
		{
			ImGui :: Separator();
			Script * curScript = (Script *) (globals :: curSceneRef -> components[SCRIPT_COMP_ID]
				[editorGlobals :: curActiveEntity]);
			ImGui :: Text("Script Data");
			uiManagement :: deleteButton(SCRIPT_COMP_ID);
			ImGui :: SameLine();
			if(ImGui :: Button("Reload"))
				appManagement :: compileScripts();

			// GETS INPUT FOR LOADING IN NEW SCRIPT //
			static unsigned scriptNameSize = 32;
			static std :: string scriptHolder = "";
			if(scriptHolder.size() != scriptNameSize)
				scriptHolder.resize(scriptNameSize);

			if
			(
				ImGui :: InputTextWithHint
				(
					"##4", curScript -> name.c_str(), scriptHolder.data(), 
					scriptNameSize, editorGlobals :: inputTextFlags
				)
			)

			{ 
				Script * script = scriptHandler :: 
					createScript(scriptHolder, editorGlobals :: curActiveEntity);
				if(script != nullptr)
				{
					sceneManagement :: addComp
					(
					 	globals :: curSceneRef, editorGlobals :: curActiveEntity,
						SCRIPT_COMP_ID, (compPtr) (script)
					);
				}
				scriptHolder = "";
				scriptHolder.resize(scriptNameSize);
			}
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

				// CREATES BASIC ITEMS WITH NON-PARAMETERIZED CONSTRUCTORS //
				if(ImGui :: MenuItem(compToString(curComp).c_str()))
				{
					// IF NEW COMPONENT REQUIRES A FILE TO BE CORRECTLY CONSTRUCTED, //
						// SETS LOADING BOOL AND CONTINUES //
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
		ImGui :: Text((centeredString("", sidePanelMargin / 3)).c_str());
		ImGui :: SameLine();
		if(ImGui :: Button("Copy Entity"))
			sceneManagement :: copyEntity(globals :: curSceneRef, editorGlobals :: curActiveEntity);
		ImGui :: SameLine();
		ImGui :: Text((centeredString("", sidePanelMargin / 3)).c_str());
		ImGui :: SameLine();
		if(ImGui :: Button("Delete Entity"))
			sceneManagement :: deleteEntity(globals :: curSceneRef, editorGlobals :: curActiveEntity);
	} ImGui :: End();

	ImGui :: SetNextWindowPos(ImVec2
		(editorGlobals :: sidePanelWidth, globals :: winHeight - editorGlobals :: bottomPanelHeight));
	ImGui :: SetNextWindowSize(ImVec2
		(editorGlobals :: bottomPanelWidth, editorGlobals :: bottomPanelHeight));
	if(ImGui :: Begin("Debug Log", NULL, editorGlobals :: windowFlags))
	{
		std :: string textString = editorGlobals :: debugText.str();
		if(textString.size() > 2048)
			textString.erase(0, textString.size() - 2048);
		ImGui :: TextWrapped(textString.c_str());
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

bool uiManagement :: getInputString(std :: string labelString, std :: string& outputData)
{
	// VARIABLE INITIALIZATION //
	bool retValue = false;

	ImGui :: OpenPopup(labelString.c_str());
	ImGui :: SetNextWindowSize(ImVec2(globals :: winWidth * 0.4f, globals :: winHeight * 0.1f));
	if(ImGui :: BeginPopupModal(labelString.c_str()), NULL, editorGlobals :: windowFlags)
	{
		static char inputString[LOB_FILE_NAME_MAX] = "";
		if(ImGui :: InputText
			(labelString.c_str(), inputString, LOB_FILE_NAME_MAX, editorGlobals :: inputTextFlags))
		{ 
			outputData = std :: string(inputString);
			retValue = true;
		}

		if(ImGui :: Button("Submit"))
		{ 
			outputData = std :: string(inputString);
			retValue = true;
		}

		ImGui :: SameLine();
		if(ImGui :: Button("Cancel"))
		{
			outputData = "";
			retValue = true;
		}
	}
	ImGui :: EndPopup();
	return retValue;
}

void uiManagement :: deleteButton(componentID compID)
{
	ImGui :: SameLine();
	if(ImGui :: Button(std :: string("Delete " + compToString(compID)).c_str()))
	{
		std :: cout << "REMOVING " << compToString(compID) << "!" << std :: endl;
		sceneManagement :: removeComp(globals :: curSceneRef, editorGlobals :: curActiveEntity, compID);
	}
}
