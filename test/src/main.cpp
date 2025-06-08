#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include <RayTracing.hpp>
#include <RayTracingStructs.hpp>


int main(int argc, char *argv[]) {
    SDL_Event event;
    double aspectRatio = 16.0 / 9.0;
    uint   imageW      = 800;
    
    
    SDLObject sdlObject(imageW, imageW / aspectRatio);
    
    std::cout << "Vec direction: " << ((Vec3){0, 0, 0} - (Vec3){13, 2, 3}) << std::endl;
	Camera cam(
        imageW,
        aspectRatio,
        5000, // sample per pixel
        50, // bounces
        50, // sample light
        30, // fov
        Vec3(12.8, 7.4, 11.3), // center
        Vec3(-0.7, -0.3, -0.6), // direction
        Vec3(0 , 1 , 0), // up
        sdlObject.getRenderer()
    );


	std::vector<Object3D> world;

    world.push_back((Object3D){
        (Vec3){0, -1000, 0},
        1000,
        (Material){
            MaterialType::LAMBERTIAN,
            (Vec3){0.5, 0.5, 0.5},
            0,
            0
        },
        0
    });
    
//    for (int a = -11; a < 11; a++) {
//        for (int b = -11; b < 11; b++) {
//            float chooseMat = randomDouble();
//            Vec3 center(a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble());
//
//            if (length(center - (Vec3){4, 0.2, 0}) > 0.9) {
//                Material sphereMaterial;
//
//                if (chooseMat < 0.8) {
//                    // diffuse
//                    Vec3 albedo = randomVector() * randomVector();
//                    sphereMaterial = (Material){
//                        MaterialType::LAMBERTIAN,
//                        albedo,
//                        0,
//                        0
//                    };
//                    world.push_back(Object3D(center, 0.2, sphereMaterial));
//                } else if (chooseMat < 0.95) {
//                    // metal
//                    Vec3 albedo = randomVector(0.5, 1, Vec3(1 * b + a, b - a * 2, a * b - 0.129));
//                    float fuzz = 0.5 * randomDouble();
//                    sphereMaterial = Material(MaterialType::METAL, albedo, fuzz, 0);
//                    world.push_back(Object3D(center, 0.2, sphereMaterial));
//                } else {
//                    // glass
//                    sphereMaterial = Material(MaterialType::METAL, Vec3(), 0, 1.5);
//                    world.push_back(Object3D(center, 0.2, sphereMaterial));
//                }
//            }
//        }
//    }
//	CameraData camData = cam.getCameraData();
//	camData.frameCount++;
//	for (int a = 0; a < 223; a++) {
//		for (int b = 0; b < 223; b++) {
//			int pixelPos = (a * camData.imageW) + b;
//			Vec3 position = randomUnitVector((Vec3){
//				float((33.72f + b) / (91.14 + pixelPos) * camData.frameCount / 10.14),
//				float((18.94f + a) / (41.25 + pixelPos) * camData.frameCount / 29.67),
//				float((10.13f + a) / b) / ((1.3f + pixelPos) * camData.frameCount)
//			});
//			std::cout << "position: " << position << std::endl;
//			std::cout << "seed: " << (Vec3){
//				float((33.72f + b) / (91.14 + pixelPos) * camData.frameCount / 10.14),
//				float((18.94f + a) / (41.25 + pixelPos) * camData.frameCount / 29.67),
//				float((10.13f + a) / b) / ((1.3f + pixelPos) * camData.frameCount)
//			} << std::endl;
//			std::cout << "a b: " << a << " " << b << std::endl;
//			position = 0.7 * position + Vec3(3, 3, -2.4);
//			Material material = Material(MaterialType::LAMBERTIAN, Vec3(1, 0, 0), 0, 0);
//			world.push_back(Object3D(position, 0.009, material, 0));
//			
//		}
//	}

    Material material1 = Material(MaterialType::DIELECTRIC, Vec3(), 0, 1.5);
    world.push_back(Object3D(Vec3(0, 1, 0), 1.0, material1));
//    
    Material material2 = Material(MaterialType::LAMBERTIAN, Vec3(1, 1, 1), 0, 0);
    world.push_back(Object3D(Vec3(-4, 1, 0), 1.0, material2, 0));
    
    Material material3 = Material(MaterialType::METAL, Vec3(0.7, 0.6, 0.5), 0.1, 0);
    world.push_back(Object3D(Vec3(4, 1, 0), 1.0, material3, 0));
//    
    Material material4 = Material(MaterialType::DIFFUSE_LIGHT, Vec3(1, 0, 0), 0, 0);
    world.push_back(Object3D(Vec3(3, 1, 2), 0.5, material4, 0));
//    
    Material material5 = Material(MaterialType::DIFFUSE_LIGHT, Vec3(0, 1, 0), 0, 0);
    world.push_back(Object3D(Vec3(3, 0.7, 4), 0.3, material5));
    
    Material material6 = Material(MaterialType::DIFFUSE_LIGHT, Vec3(5, 5, 5), 0, 0);
    world.push_back(Object3D(Vec3(3, 3, -2.4), 0.7, material6));
//    
    Material material7 = Material(MaterialType::DIFFUSE_LIGHT, Vec3(0, 1, 1), 0, 0);
    world.push_back(Object3D(Vec3(-4, 1, -2), 0.7, material7));

    std::sort(world.begin(), world.end(), [](const Object3D o1, const Object3D o2) {
        return o1.mat.type > o2.mat.type;
    });
    for (int i = 0; i < world.size(); i++) {
        world[i].id = i;
    }
    std::cout << "size = " << world.size() << std::endl;
    

    
    
    bool keyW = false;
    bool keyS = false;
    bool keyA = false;
    bool keyD = false;
    bool keyMoveUp = false;
    bool keyMoveDown = false;
    bool renderRealTime = true;
    
    bool keyTurnLeft = false;
    bool keyTurnRight = false;
    bool keyTurnUp = false;
    bool keyTurnDown = false;
    
    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            break;
        } else if (event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                break;
			}
            
            if (event.key.scancode == SDL_SCANCODE_R) {
                cam.render(world);
            }
            if (event.key.scancode == SDL_SCANCODE_W) {
                keyW = true;
            }
            if (event.key.scancode == SDL_SCANCODE_S) {
                keyS = true;
            }
            if (event.key.scancode == SDL_SCANCODE_A) {
                keyA = true;
            }
            if (event.key.scancode == SDL_SCANCODE_D) {
                keyD = true;
            }
            if (event.key.scancode == SDL_SCANCODE_LEFT) {
                keyTurnLeft = true;
            }
            if (event.key.scancode == SDL_SCANCODE_RIGHT) {
                keyTurnRight = true;
            }
            if (event.key.scancode == SDL_SCANCODE_UP) {
                keyTurnUp = true;
            }
            if (event.key.scancode == SDL_SCANCODE_DOWN) {
                keyTurnDown = true;
            }
            if (event.key.scancode == SDL_SCANCODE_SPACE) {
                keyMoveUp = true;
            }
            if (event.key.scancode == SDL_SCANCODE_LCTRL) {
                keyMoveDown = true;
            }
        } else if (event.type == SDL_EVENT_KEY_UP) {
            if (event.key.scancode == SDL_SCANCODE_W) {
                keyW = false;
            }
            if (event.key.scancode == SDL_SCANCODE_S) {
                keyS = false;
            }
            if (event.key.scancode == SDL_SCANCODE_A) {
                keyA = false;
            }
            if (event.key.scancode == SDL_SCANCODE_D) {
                keyD = false;
            }
            if (event.key.scancode == SDL_SCANCODE_LEFT) {
                keyTurnLeft = false;
            }
            if (event.key.scancode == SDL_SCANCODE_RIGHT) {
                keyTurnRight = false;
            }
            if (event.key.scancode == SDL_SCANCODE_UP) {
                keyTurnUp = false;
            }
            if (event.key.scancode == SDL_SCANCODE_DOWN) {
                keyTurnDown = false;
            }
            if (event.key.scancode == SDL_SCANCODE_SPACE) {
                keyMoveUp = false;
            }
            if (event.key.scancode == SDL_SCANCODE_LCTRL) {
                keyMoveDown = false;
            }
        }
        
        if (keyW == true) {
            cam.center = cam.center + (cam.direction * 0.1);
            std::cout << "W pressed... center: " << cam.center << std::endl;
        }
        if (keyS == true) {
            cam.center = cam.center + (-1 * cam.direction * 0.1);
            std::cout << "S pressed... center: " << cam.center << std::endl;
        }
        if (keyA == true) {
            cam.center = cam.center + (-1 * cross(cam.direction, (Vec3){0, 1, 0}) *  0.1);
            std::cout << "A pressed... center: " << cam.center << std::endl;
        }
        if (keyD == true) {
            cam.center = cam.center + (cross(cam.direction, (Vec3){0, 1, 0}) *  0.1);
            std::cout << "D pressed... center: " << cam.center << std::endl;
        }
        if (keyTurnLeft == true) {
            cam.direction = normalizeVec(cam.direction + (cross(cam.direction, (Vec3){0, -1, 0}) * 0.01));
            std::cout << "Left pressed... direction: " << cam.direction << std::endl;
        }
        if (keyTurnRight == true) {
            cam.direction = normalizeVec(cam.direction + (cross(cam.direction, (Vec3){0, 1, 0}) * 0.01));
            std::cout << "Right pressed... direction: " << cam.direction << std::endl;
        }
        if (keyTurnUp == true) {
            Vec3 right = normalizeVec(cross(cam.direction, cam.vup));
            cam.direction = cam.direction * cos(0.01) + cross(right, cam.direction) * sin(0.01) + right * dot(right, cam.direction) * (1.0f - cos(0.01));
            std::cout << "Up pressed... direction: " << cam.direction << std::endl;
        }
        if (keyTurnDown == true) {
            Vec3 right = normalizeVec(cross(cam.direction, cam.vup));
            cam.direction = cam.direction * cos(-0.01) + cross(right, cam.direction) * sin(-0.01) + right * dot(right, cam.direction) * (1.0f - cos(-0.01));
            std::cout << "Down pressed... direction: " << cam.direction << std::endl;
        }
        if (keyMoveUp == true) {
            cam.center = cam.center + ((Vec3){0, 1, 0} * 0.1);
        }
//        if (keyMoveDown == true) {
//            cam.center = cam.center + ((Vec3){0, -1, 0} * 0.1);
//        }
//        
//        if (renderRealTime == true) {
//            cam.render(world);
//        }
//        std::cout << "center: " << cam.center << std::endl;
//        std::cout << "direction: " << cam.direction << std::endl;
    }
    

    return 0;
}

