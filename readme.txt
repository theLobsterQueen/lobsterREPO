// README //
Hey Prof :3

DevBlog link: https://sites.google.com/view/lobsterenginedevblog/home?authuser=0
The dev blog is updated regularly, so be sure to tune in from time to time!

Sources :
	Hudson, James. "Lecture3: Transforms." Computer Graphics 1, 9/13/2022, 
		Shawnee State University.
	Hudson, James. "Lecture4: OpenGL Textures." Computer Graphics 1, 9/13/2022, 
		Shawnee State University.

	https://antongerdelan.net/opengl/index.html
	https://lazyfoo.net/tutorials/SDL/51_SDL_and_modern_opengl/index.php
 	https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html
	https://www.scratchapixel.com/index.html
	http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-9-vbo-indexing/
	https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-
		right-vectors-using-the-rotation-angles
	https://learnopengl.com/Getting-started/Textures
	https://github.com/ocornut/imgui/blob/master/docs/FAQ.md
	https://github.com/ocornut/imgui/wiki
	https://github.com/pybind/pybind11/issues/1201

	https://youtu.be/sXbqwzXtecE?list=PLvv0ScY6vfd9zlZkIIqGDeG5TUWswkMox
	https://youtu.be/md3jFANT3UM
	https://youtu.be/XnUMm-uNmvY
	https://youtu.be/W3gAzLwfIP0?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
	https://youtu.be/fvvWr1jaLyw?si=K1bEC0nxx06WJcc2

	https://www.libsdl.org/
	https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html
	https://pybind11.readthedocs.io/en/stable/
	https://docs.python.org/3/

// KEY //
X: Done
~: Working on it.

// GOALS //
Sprint 0 (PRE-PRODUCTION) :
	Find the Answers to these Questions :
		[X] How am I going to organize my scene structure?
		[X] How will I render objects to the screen?
		[X] How will I organize my main loop?
		[X] How will I will take and handle input?

Sprint 1 (ENGINE BACKEND FUNDAMENTALS) :
	[X] Handle basic SDL2 input events.
	[X] Render entities onto the screen.
	[X] Create foundations for entity component system.
	[X] Create basic file loaders for GLSL shaders.
	[X] Use uniforms and systems logic to make a scene fully interactable.	
	[X] Basic Lighting System

Sprint 2 :
	[X] Save/Load Scenes on Command
	[X] Build out Engine Frontend, Complete With
		[X] Inspector-Analog
			[X] Can Move Entities
			[X] Can change select values of Components
			[X] Can add Components to Entities
		[X] Scene Tree
			[X] Can add Entities
			[X] Can remove Entities
	[X] Base Python Integration 
		[X] Communicate with Precompiled Engine Files over Runtime Python Scripts
		[X] Start/Update Method Analogs
	[~] Create a Python Scripting Library to Interact With
		[X] Camera Component
		[X] Transform Component
		[X] Mesh Component
		[X] Light Component
		[ ] [STRETCH] Allow for "exposed" variables, that could be edited
			directly from the inspector.
	[X] Separate Scene Editing from Running

Sprint 3 :
	[X] Give the Scripting API Additional Capabilities
		[X] Add Components to Existing Entities
		[X] Create New Entities in a Given Scene
		[X] Process Input
		[X] Change the Current Scene
	[ ] Create a Project Manager
		[ ] Can Create new Project Directories
		[ ] Can Manage/List/Delete all Existing Project Directories
	[ ] QoL
		[ ] "Quick-loading" system for loading new meshes/textures/scripts
		[ ] Window resizing
		[ ] Separating the viewport from the UI itself
		[ ] Transform modifier gizmo, highlights for objects without meshes
		[ ] Better error handling
		[ ] Better shader management
