// INCLUDES DECLARATION FILES //
#include <globals.h>

bool globals :: isRunning;
bool globals :: isPlaying;
SDL_Window * globals :: winRef;
unsigned short int globals :: winWidth;
unsigned short int globals :: winHeight;
void * globals :: glRef;
Scene * globals :: curSceneRef;
InputState * globals :: inputState;
std :: vector<Pipeline *> globals :: pipelineRefs;
double globals :: deltaTime;
float globals :: clearColor[4];
float globals :: tempScale;
std :: vector<float> globals :: tempRot;
std :: map<std :: string, Mesh *> globals :: meshCache;
std :: map<std :: string, Texture *> globals :: textureCache;

ImGuiWindowFlags editorGlobals :: windowFlags;
ImGuiTreeNodeFlags editorGlobals :: sceneTreeFlags;
ImGuiInputTextFlags editorGlobals :: inputTextFlags;
entityID editorGlobals :: curActiveEntity;
bool editorGlobals :: entitySelected;
bool editorGlobals :: keyInput;
float editorGlobals :: sidePanelWidth;
float editorGlobals :: sidePanelHeight;
float editorGlobals :: bottomPanelWidth;
float editorGlobals :: bottomPanelHeight;
std :: stringstream editorGlobals :: debugText;
Options * editorGlobals :: optionsRef;
