Sprint 0 :
	Create UML Diagram for Engine Backend :
		Does the engine function as a state machine?
		How does the render pipeline work?
		How do input interrupts work?

		How does the operation loop function? When does the program control
			execute developer-made scripts?
		How will the concurrency operate?
	
	Design Markups for Engine Frontend UI :
		How similar to Unity should the frontend be? What are the major changes, if any?
		How much time should be dedicated to changing the layout of the editor? Should
			there be significant time dedicated to multiple preset layouts? Moveable
			and rezisable windows? Should the design take a more decentralized approach,
			with multiple windows rather than one window with docked "sections"?

	Research How API Interfaces Function in Engine Design :
		How does the engine compile the script code into a build?
		When does it call it, and how? Is there a dedicated thread for script execution?
		What are the major pitfalls that need to be avoided when designing
			such a system?

	Research Virtual Input System with Tetra
	Determine Layout/Capabilities of a Build Window

Sprint 1 :
	Viewport :
		Multiple modes (Wire frame, shaded, etc.)
		Isometric/Orthographic Mode [STRETCH]
	Create Entities :
		3D Models
		Empties
	Render Prototype 3D Models :
		Create assignable material assets that can be edited directly by the user
	Inspector :
		Read the basic values of an object (position, rotation, script public values, etc.)
		Create hot-loading system so that editing inspector values can change entity
			data in realtime [STRETCH]
	Scene Tree

Sprint 2 :
	File Viewer
	Import Assets
	Utilize Assets :
		Play Audio
		Render Text [STRETCH]
		Render Models :
			Use animations baked into imported models. [STRETCH]
	Compiler :
		Be able to compile and run scenes inside editor context.
	Editor Frontend Documentation

Midterm Goals :
	Base Goals :
		Can create and run a static scene entirely in-editor, using imported models
			and sound effects.
	
	Stretch Goals :
		Can create and run an animated scene entirely in-editor, using imported models,
			sound effects, and animations.

Sprint 3 :
	Create Backend API :
		Can access and call API functions from developer-made scripts.
		Can use automatically called functions (like Start() and Update() in Unity).
		API Documentation
	Make Projects Buildable :
		Add Windows build target.
		Add console-based build targets (for those with open APKs) [STRETCH]

Sprint 4 :
	Add Build Options :
		Debug/Release.
		Executable version number, icon, splashscreens at start.
		Graphics preference presets. [STRETCH]
	Particle Systems :
		Various emitter volumes (spherical, cone-based, etc.)
		Emitter rate, emitter bursts.
		Color, color over time.
		Lifetime of particles, lifetime of emitters.
		Mesh-based particles w/ animations [STRETCH]
	Configurable Shaders :
		Give API capabilities to create new shader objects.
		Change out which shaders objects are currently active.
		Add "render pipeline" presets. [STRETCH]

Sprint 5 :
	Create Prototype Assets
	Design Game

Sprint 6 :
	Build Game


