#include <RayTracing.hpp>
#include <Material.hpp>
#include <Utils.hpp>
#include <memory>

int main(int argc, char *argv[]) {
    SDL_Event event;

	Camera cam;
	cam.aspectRatio = 16.0 / 9;
	cam.imageW = 800;
	cam.samplePerPixel = 100;
	cam.maxDepth = 50;

	SDLObject sdlObject(cam.imageW, cam.imageW / cam.aspectRatio);
	SDLTextureObject texture(sdlObject.getRenderer());

	// World
	Object3DList world;
	shared_ptr<Lambertian> ground = make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0));
	shared_ptr<Lambertian> centerBall = make_shared<Lambertian>(Vec3(0.1, 0.2, 0.5));
	shared_ptr<Dielectric> metalBallLeft = make_shared<Dielectric>(1.0 / 1.33);
	shared_ptr<Metal> metalBallRight = make_shared<Metal>(Vec3(0.8, 0.6, 0.2), 1.0);

	world.add(make_shared<Sphere>(Vec3(0.0, -100.5, -1.0), 100.0, ground));
	world.add(make_shared<Sphere>(Vec3(0.0, 0.0, -1.2), 0.5, centerBall));
	world.add(make_shared<Sphere>(Vec3(-1.0, 0.0, -1.0), 0.5, metalBallLeft));
	world.add(make_shared<Sphere>(Vec3(1.0, 0.0, -1.2), 0.3, metalBallRight));

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
