# TobyG

A simple platform independent 3D game development library for ANSI C.

![TobyG](logo.png)

## About

TobyG is a simple platform independent 3D game development library for ANSI C best used for very low definition games (it uses vertex morphing for animation, making it very memory costly for high poly meshes).

My goal was to have a very low level library with just the minimum amount of abstraction. Glancing over a TobyG program an experienced OpenGL programmer might easily guess what's under the hood, which OpenGL calls are bundled and what side effects each function has.

**IMPORTANT:** TobyG is still in development and nothing's set in stone yet. Also, a ton of errors aren't being checked for.

## Usage

The smallest TobyG program would be:

	int main(void) {
		SDL_Init(SDL_INIT_EVERYTHING);
		TobyG_StartTobyG("TobyG", 640, 480);

		/* Game stuff! */

		TobyG_EndTobyG();
		SDL_Quit();
	
		return EXIT_SUCCESS;
	}

This initializes SDL2 (always initialize SDL2 first). Then opens a 640x480 window with a "TobyG" title. Finally it closes the window and quits SDL2.

Note that although the window resolution's 640x480 the actual resolution of a TobyG game will always be 256x224.

Every TobyG function that `Start`s something has a corresponding function that `End`s it. When nesting these types of functions please make sure to reverse call the `End` functions.

### Game Management

Letting TobyG manage the game is entirely optional but might be helpful. TobyG then takes care of game state management (no stacking, only one state at a time can be run).

Game management is initialized with `TobyG_StartGameMgr()` (and destroyed with `TobyG_EndGameMgr()`).

Game states are then added using `TobyG_AddGameMgrState(id, start, stop, loop)` with `id` being a unique state id  and `start`, `stop` and `loop` being functions for the game manager to call when starting the state, stopping the state (on quit or switch) and in every main loop pass.

Use `TobyG_RunGameMgr(id)` to start the main loop with a specific state and call `TobyG_QuitGameMgr()` (in a state) to quit the main loop. `TobyG_ChangeGameMgrState(id)` will stop the current state and start another one.

### Graphics

Start the graphics block in a main loop pass with `TobyG_StartGraphics()` and end with `TobyG_EndGraphics()`.

The 3D coordinate system is right handed with X going positively to the right.

All angles in radiant unless otherwise noted.

#### Camera

Use `TobyG_SetCameraPerspective(fov, near, far)` to set the camera's projection matrix. Use `TobyG_SetCameraPosition(x, y, z)` and `TobyG_SetCameraRotation(x, y, z)` to set up the camera's transformation. Default perspective is 45° FOV, 0.1 near, 100 far.

#### Textures

Read WebP textures with `TobyG_ReadTextureFromWebP(path)` and free them using `TobyG_FreeTexture(texture)`.

Then set the texture for rendering with `TobyG_SetTexture(texture)` in the according rendering block (see Animated Meshes or Mesh Batches).

#### Instances

Instances are for rendering meshes (or later, billboards). They can be positioned, rotated and scaled.

Instances are created using `TobyG_CreateInstance()` and later freed with `TobyG_FreeInstance()`. They're transformed by `TobyG_SetInstancePosition(x, y, z)`, `TobyG_SetInstanceRotation(x, y, z)` and `TobyG_SetInstanceScale(x, y, z)`.

#### Animated Meshes

Animated meshes are read from T3D Animated Meshes (see [T3D](https://github.com/tobchen/T3D#t3d-animated-meshes-t3d)). They use vertex morphing for animation so using high vertex counts is not recommended. For non-animated meshes see Mesh Batches which are optimized for memory usage and draw call count.

Read a mesh with `TobyG_ReadMesh(path)` and later free it with `TobyG_FreeMesh`.

For rendering in the graphics block start the mesh renderer using `TobyG_StartMeshRendering()` to then set the mesh with `TobyG_SetMesh(mesh)` and draw instances by calling `TobyG_DrawMeshAt(instance, frame)` and finally stop mesh rendering with `TobyG_EndMeshRendering()`.

#### Mesh Batches

Mesh batches are a bundle of meshes accessed by index. They are read from T3D Mesh Batches (see [T3D](https://github.com/tobchen/T3D#t3d-mesh-batches-t3db)).

Read a mesh batch with `TobyG_ReadMeshBatch(path)` and later free it with `TobyG_FreeMeshBatch(batch)`. Find indices of meshes by name using `TobyG_GetMeshBatchIndex(batch, name)`.

For rendering in the graphics block start the mesh batch renderer using `TobyG_StartMeshBatchRenderering()` to then set the mesh batch with `TobyG_SetMeshBatch(batch)` and draw instances by calling `TobyG_DrawMeshBatchAt(instance, index)` and finally stop mesh batch rendering with `TobyG_EndMeshBatchRendering()`.

#### Full Example

In this example we load two animated meshes and one mesh batch and render the two animated meshes at two positions each and render two meshes out of the mesh batch at the same position. Animated meshes and mesh batch have different textures each.

So we need: Five instances, two animated meshes, one mesh batch, three textures, one animated mesh renderer, one mesh batch renderer. Just for fun, we also set the camera. SDL2 is already initialized, a TobyG window is already open.

	TobyG_Mesh* mesh1 = TobyG_ReadMesh("mesh1.t3d");
	TobyG_Mesh* mesh2 = TobyG_ReadMesh("mesh2.t3d");
	
	TobyG_MeshBatch* batch = TobyG_ReadMeshBatch("batch.t3db");
	size_t index1 = TobyG_GetMeshBatchIndex(batch, "foo.obj");
	size_t index2 = TobyG_GetMeshBatchIndex(batch, "bar.obj");
	
	TobyG_Texture* tex1 = TobyG_ReadTextureFromWebP("tex1.webp");
	TobyG_Texture* tex2 = TobyG_ReadTextureFromWebP("tex2.webp");
	TobyG_Texture* tex3 = TobyG_ReadTextureFromWebP("tex3.webp");
	
	TobyG_Instance* inst1_1 = TobyG_CreateInstance();
	TobyG_Instance* inst1_2 = TobyG_CreateInstance();
	TobyG_Instance* inst2_1 = TobyG_CreateInstance();
	TobyG_Instance* inst2_2 = TobyG_CreateInstance();
	TobyG_Instance* inst3 = TobyG_CreateInstance();
	
	TobyG_SetInstancePosition(inst1_1, -5, 0, -5);
	TobyG_SetInstancePosition(inst1_2, 5, 0, -5);
	TobyG_SetInstancePosition(inst2_1, 5, 0, 5);
	TobyG_SetInstancePosition(inst2_2, -5, 0, 5);
	TobyG_SetInstancePosition(inst3, 0, 0, 0);
	
	TobyG_SetCameraPerspective(45, 0.1, 100);
	TobyG_SetCameraPosition(0, 0, 20);
	
	for (;;) {
		TobyG_StartGraphics();
		
		TobyG_StartMeshRendering();
		TobyG_SetTexture(tex1);
		TobyG_SetMesh(mesh1);
		TobyG_DrawMeshAt(inst1_1, 1.0);
		TobyG_DrawMeshAt(inst1_2, 9.0);
		TobyG_SetTexture(tex2);
		TobyG_SetMesh(mesh2);
		TobyG_DrawMeshAt(inst2_1, 4.0);
		TobyG_DrawMeshAt(inst2_2, 17.0);
		TobyG_EndMeshRendering();
		
		TobyG_StartMeshBatchRendering();
		TobyG_SetTexture(tex3);
		TobyG_SetMeshBatch(batch);
		TobyG_DrawMeshBatchAt(inst3, index1);
		TobyG_DrawMeshBatchAt(inst3, index2);
		TobyG_EndMeshBatchRendering();
		
		TobyG_EndGraphics();
	}
	
	/* Free everything in reverse order! */

### IO

TobyG provides some help with files and paths.

#### Files

`TobyG_ReadTextFile(path)` reads a text file and returns a null terminated string. It's up to the user to free the string.

`TobyG_ReadBinaryFile(path, *data, *length)` reads a binary file and returns its data (in a byte array) and the data's length (byte array size). It's up to the user to free the data.

#### Paths

In development, very much subject to change.

## Requirements

TobyG needs SDL2, GLEW and WebP to compile. It needs OpenGL 2.1 or higher to run.

## To Do

### IO

- More flexibility in path system

### 3D

- Billboards
- Setting color and alpha for rendering

### 2D

Basically everything, specifically:

* Drawing images
	* Sprite sheet support
* Drawing text
	* Bitmap fonts
	* Vector text (probably never)
* Drawing video
	* WebM
* 3D to 2D space conversion

### Sound

Basically everything and I haven't looked into it at all but from experience with other game engines I'd say:

* Play sounds
	* 2D (custom panning)
	* 3D (probably never)
* Stream music

### Android

Oh my, I have no idea where to begin, but most likely:

1. Simply port to Android
2. Handle life cycle (pause and unpause or restart)
3. Draw virtual input elements

## Version History

### v0.2
TobyG's now exclusively low res, every wriggle room in that direction was removed. IO was developed further.
#### Added
- Path building functions
- File content reading functions
#### Changed
- Virtual resolution now mandatory (always set to 256x224)
#### Removed
- Mesh renderer flexibility (only retro rendering now)
- Mesh batch renderer flexibility (only retro rendering now)

### v0.1
TobyG went public. I held a big party. No one came.

