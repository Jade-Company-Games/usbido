#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

struct AppTexture
{
    SDL_Texture* texture = nullptr;
    int width = 0;
    int height = 0;
};

AppTexture load_texture(SDL_Renderer* renderer, const char* path)
{
    AppTexture result;
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (!surface)
    {
        std::cerr << "[usbido] SDL_LoadBMP error: " << SDL_GetError() << "\n";
        return result;
    }
    result.texture = SDL_CreateTextureFromSurface(renderer, surface);
    result.width = surface->w;
    result.height = surface->h;
    SDL_DestroySurface(surface);
    return result;
}

void run_ui()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "[usbido] SDL_Init error: " << SDL_GetError() << "\n";
        return;
    }

    SDL_Window* window = SDL_CreateWindow("usbido", 800, 500, SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        std::cerr << "[usbido] SDL_CreateWindow error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        std::cerr << "[usbido] SDL_CreateRenderer error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    SDL_SetRenderVSync(renderer, 1);

    AppTexture logo = load_texture(renderer, "assets/usbido.bmp");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("##main", nullptr,
            ImGuiWindowFlags_NoTitleBar  |
            ImGuiWindowFlags_NoResize    |
            ImGuiWindowFlags_NoMove      |
            ImGuiWindowFlags_NoCollapse
        );

        if (logo.texture)
        {
            float target_height = 64.0f;
            float aspect = (float)logo.width / (float)logo.height;
            ImVec2 logo_size(target_height * aspect, target_height);

            float window_width = ImGui::GetContentRegionAvail().x;
            ImGui::SetCursorPosX((window_width - logo_size.x) * 0.5f);
            ImGui::Image((ImTextureID)(intptr_t)logo.texture, logo_size);
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
        }

        ImGui::TextDisabled("No drives detected.");
        ImGui::End();

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    if (logo.texture)
        SDL_DestroyTexture(logo.texture);

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void run_cli()
{
    std::cout << "[usbido] running in CLI mode\n";
}

bool has_flag(const std::vector<std::string>& args, const std::string& flag)
{
    return std::find(args.begin(), args.end(), flag) != args.end();
}

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv + 1, argv + argc);

    if (has_flag(args, "-ui"))
        run_ui();
    else
        run_cli();

    return 0;
}