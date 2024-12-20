#include <iostream>
#include <ostream>
#include <cstdlib>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/lib/x64/SDL2.lib" )
#pragma comment( lib, "SDL/lib/x64/SDL2main.lib" )

enum main_states
{
	MAIN_CREATION,  
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = nullptr;

int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	constexpr Uint64 updateTargetTickDuration = 1000 / FPS;
	Uint64 lastUpdateTicks = 0;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			App = new Application();
			LOG("Application Creation --------------")
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("Application Init --------------")
			if (App->Init() == false)
			{
				LOG("Application Init exits with error -----")
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("Application Update --------------")
			}

			break;

		case MAIN_UPDATE:
			{
				const Uint64 currentTicks = SDL_GetTicks();
				const Uint64 deltaTicks = currentTicks - lastUpdateTicks;
				if (deltaTicks > updateTargetTickDuration) {
					int update_return = App->Update(deltaTicks);

					if (update_return == UPDATE_ERROR)
					{
						LOG("Application Update exits with error -----")
						state = MAIN_EXIT;
					}

					if (update_return == UPDATE_STOP)
						state = MAIN_FINISH;

					lastUpdateTicks = currentTicks;
				}
			}
			break;

		case MAIN_FINISH:

			LOG("Application CleanUp --------------")
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with error -----")
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}

	}

	LOG("Bye :)\n")
	delete App;
	return main_return;
}