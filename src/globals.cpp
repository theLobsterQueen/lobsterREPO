// INCLUDES DECLARATION FILES //
#include <globals.h>

bool globals :: isRunning;
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

ImGuiWindowFlags editorGlobals :: windowFlags;
ImGuiTreeNodeFlags editorGlobals :: sceneTreeFlags;
entityID editorGlobals :: curActiveEntity;
bool editorGlobals :: entitySelected;
float editorGlobals :: sidePanelWidth;
float editorGlobals :: sidePanelHeight;
float editorGlobals :: bottomPanelWidth;
float editorGlobals :: bottomPanelHeight;
