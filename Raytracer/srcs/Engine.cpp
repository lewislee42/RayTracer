//
//  Engine.cpp
//  Raytracer
//
//  Created by Lewis lee on 22/07/2025.
//

#include "Engine.hpp"

void	initObjects(std::vector<Object3D>& objBuffer, std::vector<BVHNode>& bvhNodeBuffer, unsigned int& lightCount) {

	objBuffer.push_back((Object3D){
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
    
//    for (int a = -5; a < 5; a++) {
//        for (int b = -5; b < 5; b++) {
//            float chooseMat = randomFloat();
//            Vec3 center(a + 0.9*randomFloat(), 0.2, b + 0.9*randomFloat());
//
//            if (length(center - (Vec3){4, 0.2, 0}) > 0.9) {
//                Material sphereMaterial;
//
//                if (chooseMat <= 0.99) {
//                    Vec3 albedo = randomVector() * randomVector();
////                    sphereMaterial = (Material){
////                        MaterialType::LAMBERTIAN,
////                        albedo,
////                        0,
////                        0
////                    };
////                    world.push_back(Object3D(center, 0.2, sphereMaterial));
//                } else if (chooseMat <= 0.66) {
//                    // metal
//                    Vec3 albedo = randomVector(0.5, 1, Vec3(1 * b + a, b - a * 2, a * b - 0.129));
//                    float fuzz = 0.5 * randomFloat();
////                    sphereMaterial = Material(MaterialType::METAL, albedo, fuzz, 0);
////                    world.push_back(Object3D(center, 0.2, sphereMaterial));
//                } else {
//                    // glass
//					sphereMaterial = Material(MaterialType::DIELECTRIC, Vec3(), 0, 1.5);
//                    world.push_back(Object3D(center, 0.2, sphereMaterial));
//                }
//            }
//        }
//    }
//	RaytracingMetaData camData = cam.getMetaData();
//	for (int a = 0; a < 10; a++) {
//		for (int b = 0; b < 10; b++) {
//			int pixelPos = (a * camData.imageW) + b;
//			Vec3 position = randomUnitVector((Vec3){
//				float((33.72f + b) / (91.14 + pixelPos) * 10.14),
//				float((18.94f + a) / (41.25 + pixelPos) * 29.67),
//				float((10.13f + a) / b) / ((1.3f + pixelPos))
//			});
//			std::cout << "position: " << position << std::endl;
//			std::cout << "seed: " << (Vec3){
//				float((33.72f + b) / (91.14 + pixelPos) * 10.14),
//				float((18.94f + a) / (41.25 + pixelPos) * 29.67),
//				float((10.13f + a) / b) / ((1.3f + pixelPos))
//			} << std::endl;
//			std::cout << "a b: " << a << " " << b << std::endl;
//			position = position * (Vec3){static_cast<float>(a), 1.0f, static_cast<float>(b)};
//			position.y = 1.0f;
//			Material material = Material(MaterialType::LAMBERTIAN, Vec3(1, 1, 1), 0, 0);
//			world.push_back(Object3D(position, 0.3f, material, 0));
//			
//		}
//	}


//    Material material1 = Material(MaterialType::DIELECTRIC, Vec3(), 0, 1.5);
//    world.push_back(Object3D(Vec3(0, 1, 0), 1.0, material1));

    Material material2 = Material(MaterialType::LAMBERTIAN, Vec3(1, 0, 0), 0, 0);
    objBuffer.push_back(Object3D(Vec3(-4, 1, 0), 1.0, material2, 0));
    
    Material material3 = Material(MaterialType::METAL, Vec3(0.7, 0.6, 0.5), 0, 0);
    objBuffer.push_back(Object3D(Vec3(4, 1, 0), 1.0, material3, 0));
 
    Material material4 = Material(MaterialType::DIFFUSE_LIGHT, Vec3(1, 1, 0), 0, 0);
    objBuffer.push_back(Object3D(Vec3(3, 1, 2), 0.5, material4, 0));
    
    Material material5 = Material(MaterialType::DIFFUSE_LIGHT, Vec3(0, 1, 0), 0, 0);
    objBuffer.push_back(Object3D(Vec3(3, 0.7, 4), 0.3, material5));

    Material material6 = Material(MaterialType::DIFFUSE_LIGHT, Vec3(5, 5, 5), 0, 0);
    objBuffer.push_back(Object3D(Vec3(3, 3, -2.4), 0.7, material6));

    Material material7 = Material(MaterialType::DIFFUSE_LIGHT, Vec3(0, 1, 1), 0, 0);
    objBuffer.push_back(Object3D(Vec3(-4, 1, -2), 0.7, material7));


	// Grow the bounding box for BVH
	BoundingBox boundingBox;
    for (uint i = 0; i < objBuffer.size(); i++) {
        objBuffer[i].id = i;
        growBoundingBoxSphere(boundingBox, objBuffer[i]);
        
		if (objBuffer[i].mat.type == MaterialType::DIFFUSE_LIGHT)
			lightCount++;
    }


	// BVH
	BVHNode root;
	root.triangleCount = static_cast<int>(objBuffer.size());
	root.boundingBox = boundingBox;
	bvhNodeBuffer.push_back(root);
	root.childIndex = static_cast<int>(bvhNodeBuffer.size());

    Split(bvhNodeBuffer, objBuffer, &root);
}

Engine::Engine(
	unsigned int screenWidth,
	unsigned int screenHeight,
	double aspectRatio
):	screenWidth(screenWidth),
	screenHeight(screenHeight),
	aspectRatio(aspectRatio),
	camera(
		screenWidth,
        aspectRatio,
        10,							// sample per pixel
		10,							// bounces
        1,							// sample light
        30,							// fov
        Vec3(3.62, 5.76, 11.82),	// center
        Vec3(-0.26, -0.35, -0.89),	// direction
        Vec3(0 , 1 , 0)				// up
	)
{
	// Init window
	{
		if (!SDL_Init(SDL_INIT_VIDEO)) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
			throw SDLInitializationError();
		}

		if (!SDL_CreateWindowAndRenderer("Hello SDL", this->screenWidth, this->screenHeight, SDL_WINDOW_MAXIMIZED, &this->window, &this->renderer)) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
			throw SDLInitializationError();
		}

		this->surface = SDL_CreateSurface(this->screenWidth, this->screenHeight, SDL_PIXELFORMAT_RGBA32);
		if (!this->surface) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
			throw SDLInitializationError();
		}
    }


	// Init Metal Interface
	swapchain = (CA::MetalLayer *)SDL_GetRenderMetalLayer(renderer);
    metalAdder = new MetalAdder(swapchain, screenWidth, screenHeight);


	// Init Objects & BVH
	unsigned int lightCount = 0;
	initObjects(objBuffer, bvhNodeBuffer, lightCount);
	
	objData = ObjectsData{
		static_cast<unsigned int>(objBuffer.size()),
		static_cast<unsigned int>(lightCount),
		static_cast<unsigned int>(bvhNodeBuffer.size())
	};
	
	
	// Debug messages
	std::cout << "[INFO] initialized " << objData.objectAmount << " Objects" << std::endl;
	std::cout << "[INFO] initialized " << objData.nodesCount << " Bvh Nodes" << std::endl;
	std::cout << "[INFO] counted " << objData.lightAmount << " lights" << std::endl;
}


Engine::~Engine() {
	delete this->metalAdder;
	SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}




void	Engine::render() {
    std::chrono::time_point startTime = std::chrono::system_clock::now();
    
    RaytracingMetaData metaData = camera.getMetaData();
    metalAdder->sendComputeCommand(
        metaData,
        objBuffer.data(),
        bvhNodeBuffer.data(),
        objData
    );
    
    std::chrono::time_point endTime = std::chrono::system_clock::now();
    auto difference = endTime - startTime;
    auto differenceMillisecond = std::chrono::duration_cast<std::chrono::milliseconds>(difference);
    long long millisecondCount = differenceMillisecond.count();
	deltaTime = millisecondCount / 1000.0f;
    std::cout << "ms: " << millisecondCount << ", FPS: " << static_cast<int>(1000.0f / millisecondCount) << std::endl;
}


void	Engine::run() {
	while (1) {
        SDL_PollEvent(&event);
        if (event.key.scancode == SDL_SCANCODE_ESCAPE)
			break;
		
		camera.updateMovementBools(event);
		camera.updateCameraPosDir(deltaTime);
		camera.updateCameraData();
		render();
    }
}


/* ---- Exceptions ---- */
const char* Engine::SDLInitializationError::what() const throw() {
	return "Could not initialize SDL components";
}
