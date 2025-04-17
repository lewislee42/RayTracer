#include <RayTracing.hpp>

int main(int argc, char *argv[]) {
    SDL_Event event;

	Camera cam;
	cam.aspectRatio = 16.0 / 9;
	cam.imageW = 800;
	cam.samplePerPixel = 100;
	cam.maxDepth = 25;

	SDLObject sdlObject(cam.imageW, cam.imageW / cam.aspectRatio);
	SDLTextureObject texture(sdlObject.getRenderer());

	// World
	Object3DList world;
	world.add(make_shared<Sphere>(Vec3(0, 0, -1), 0.5));
	world.add(make_shared<Sphere>(Vec3(0, -100.5, -1), 100));

    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            break;
        } else if (event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.scancode == SDL_SCANCODE_R) {
				sdlObject.clearScreen();

				texture.lockTexture();

				cam.render(world, texture);

				texture.unlockTexture();
			}
		}
		sdlObject.renderTexture(texture.getTexture());
    }

    return 0;
}
