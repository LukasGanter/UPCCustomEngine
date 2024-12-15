#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#include "ModuleModel.h"
#include "SDL/include/SDL.h"
#include "imgui/imgui_impl_sdl2.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput() : Module(), mouse({ 0, 0 }), mouse_motion({ 0,0 })
{
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
	//RELEASE_ARRAY(keyboard);
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system")
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError())
		ret = false;
	}

	return ret;
}

bool ModuleInput::Start()
{
	return true;
}

// Called each loop iteration
update_status ModuleInput::PreUpdate()
{
	static SDL_Event event;

	mouse_motion = { 0, 0 };
	memset(windowEvents, false, WE_COUNT * sizeof(bool));

	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (auto& mouse_button : mouse_buttons)
	{
		if (mouse_button == KEY_DOWN)
			mouse_button = KEY_REPEAT;

		if (mouse_button == KEY_UP)
			mouse_button = KEY_IDLE;
	}

	mouseWheelMotion = 0;
	while (SDL_PollEvent(&event) != 0)
	{

		ImGui_ImplSDL2_ProcessEvent(&event);
		const ImGuiIO& io = ImGui::GetIO();

		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			case SDL_WINDOWEVENT_RESIZED:
				App->GetWindowModule()->UpdateWindowWidth(event.window.data1);
				App->GetWindowModule()->UpdateWindowHeight(event.window.data2);
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (!io.WantCaptureMouse)
			{
				mouse_buttons[event.button.button - 1] = KEY_DOWN;
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if (!io.WantCaptureMouse)
			{
				mouse_buttons[event.button.button - 1] = KEY_UP;
			}
			break;

		case SDL_MOUSEMOTION:
			if (!io.WantCaptureMouse)
			{
				mouse_motion.x = event.motion.xrel / 2;
				mouse_motion.y = event.motion.yrel / 2;
				mouse.x = event.motion.x / 2;
				mouse.y = event.motion.y / 2;
			}
			break;
		case SDL_MOUSEWHEEL:
			if (!io.WantCaptureMouse)
			{
				mouseWheelMotion = event.wheel.y;
			}
			break;
		case SDL_DROPFILE:
			char* fullPath = event.drop.file;
			LOG("Dropped file: %s", fullPath)
			App->GetModelModule()->LoadAssets(fullPath);
			SDL_free(fullPath);
			break;
		}
	}

	if (GetWindowEvent(EventWindow::WE_QUIT) == true || GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{


	LOG("Quitting SDL event subsystem")
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool ModuleInput::GetWindowEvent(EventWindow ev) const
{
	return windowEvents[ev];
}

const iPoint& ModuleInput::GetMousePosition() const
{
	return mouse;
}

const iPoint& ModuleInput::GetMouseMotion() const
{
	return mouse_motion;
}
