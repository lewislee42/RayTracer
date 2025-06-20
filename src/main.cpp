#include <RayTracing.hpp>
#include <RayTracingStructs.hpp>
#include <Utils.hpp>

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include "../libs/metal-cpp/Foundation/Foundation.hpp"
#include "../libs/metal-cpp/Metal/Metal.hpp" 

int main(int argc, char *argv[]) {
    SDL_Event event;

	Camera cam;
	cam.aspectRatio = 16.0 / 9;
	cam.imageW = 800;
	cam.samplePerPixel = 500;
	cam.maxDepth = 50;
	cam.fov = 90;
	cam.lookFrom = (Vec3){0, 0, 0.4};
	cam.lookAt = (Vec3){0, 0, 0};
	cam.vup = (Vec3){0, 1, 0};

	SDLObject sdlObject(cam.imageW, cam.imageW / cam.aspectRatio);
	SDLTextureObject texture(sdlObject.getRenderer());

	std::vector<Object3D> world;
	world.push_back((Object3D){
		(Vec3){0, -100.5, -1.0},
		100,
		(Material){
			MaterialType::LAMBERTIAN,
			(Vec3){0.8, 0.8, 0.0},
			0,
			0
		}
	});

	world.push_back((Object3D){
		(Vec3){0, 0.0, -1.2},
		0.5,
		(Material){
			MaterialType::LAMBERTIAN,
			(Vec3){0.1, 0.2, 0.5},
			0,
			0
		}
	});

	world.push_back((Object3D){
		(Vec3){-1.0, 0.0, -1.0},
		0.5,
		(Material){
			MaterialType::DIELECTRIC,
			(Vec3){0.0, 0.0, 0.0},
			0,
			1.5
		}
	});

	world.push_back((Object3D){
		(Vec3){1.0, 0.0, -1.0},
		0.3,
		(Material){
			MaterialType::METAL,
			(Vec3){0.8, 0.6, 0.2},
			1.0,
			0
		}
	});




	/*shared_ptr<Lambertian> ground_material = make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5));*/
	/*   world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, ground_material));*/
	/**/
	/*   for (int a = -11; a < 11; a++) {*/
	/*       for (int b = -11; b < 11; b++) {*/
	/*           auto choose_mat = randomDouble();*/
	/*           point3 center(a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble());*/
	/**/
	/*           if ((center - point3(4, 0.2, 0)).length() > 0.9) {*/
	/*               shared_ptr<Material> sphere_material;*/
	/**/
	/*               if (choose_mat < 0.8) {*/
	/*                   // diffuse*/
	/*                   auto albedo = randomVector() * randomVector();*/
	/*                   sphere_material = make_shared<Lambertian>(albedo);*/
	/*                   world.add(make_shared<Sphere>(center, 0.2, sphere_material));*/
	/*               } else if (choose_mat < 0.95) {*/
	/*                   // metal*/
	/*                   auto albedo = randomVector(0.5, 1);*/
	/*                   auto fuzz = randomDouble(0, 0.5);*/
	/*                   sphere_material = make_shared<Metal>(albedo, fuzz);*/
	/*                   world.add(make_shared<Sphere>(center, 0.2, sphere_material));*/
	/*               } else {*/
	/*                   // glass*/
	/*                   sphere_material = make_shared<Dielectric>(1.5);*/
	/*                   world.add(make_shared<Sphere>(center, 0.2, sphere_material));*/
	/*               }*/
	/*           }*/
	/*       }*/
	/*   }*/

    /*auto material1 = make_shared<Dielectric>(1.5);*/
    /*world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));*/
    /**/
    /*auto material2 = make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1));*/
    /*world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));*/
    /**/
    /*auto material3 = make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0);*/
    /*world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));*/



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

