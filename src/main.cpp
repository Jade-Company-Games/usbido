#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

void run_ui()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "[usbido] SDL_Init error: " << SDL_GetError() << "\n";
		return;
	}

	SDL_Window *window = SDL_CreateWindow(
		"usbido",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 500,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (!window)
	{
		std::cerr << "[usbido] SDL_CreateWindow error: " << SDL_GetError() << "\n";
		SDL_Quit();
		return;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		std::cerr << "[usbido] SDL_CreateRenderer error: " << SDL_GetError() << "\n";
		SDL_DestroyWindow(window);
		SDL_Quit();
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	bool running = true;
	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				running = false;
		}

		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// placeholder UI
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(io.DisplaySize);
		ImGui::Begin("##main", nullptr,
					 ImGuiWindowFlags_NoTitleBar |
						 ImGuiWindowFlags_NoResize |
						 ImGuiWindowFlags_NoMove |
						 ImGuiWindowFlags_NoCollapse);
		ImGui::Text("usbido");
		ImGui::Separator();
		ImGui::TextDisabled("No drives detected.");
		ImGui::End();

		ImGui::Render();
		SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
		SDL_RenderPresent(renderer);
	}

	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void run_cli()
{
	std::cout << "[usbido] running in CLI mode\n";
	// drive scanning and game launching will go here
}

bool has_flag(const std::vector<std::string> &args, const std::string &flag)
{
	return std::find(args.begin(), args.end(), flag) != args.end();
}

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv + 1, argv + argc);

	if (has_flag(args, "-ui"))
	{
		run_ui();
	}
	else
	{
		run_cli();
	}

	return 0;
}