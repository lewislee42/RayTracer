#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include "RayTracing.hpp"

#include "BVH.h"

int main(int argc, char *argv[]) {
    SDL_Event event;
    double aspectRatio = 16.0 / 9.0;
    uint   imageW      = 800;
    
    
    SDLObject sdlObject(imageW, imageW / aspectRatio);
    
    // std::cout << "Vec direction: " << ((Vec3){0, 0, 0} - (Vec3){13, 2, 3}) << std::endl;
	Camera cam(
        imageW,
        aspectRatio,
        5, // sample per pixel
		5, // bounces
        1, // sample light
        30, // fov
        Vec3(3.62, 5.76, 11.82), // center
        Vec3(-0.26, -0.35, -0.89), // direction
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
    world.push_back(Object3D(Vec3(-4, 1, 0), 1.0, material2, 0));
    
    Material material3 = Material(MaterialType::METAL, Vec3(0.7, 0.6, 0.5), 0, 0);
    world.push_back(Object3D(Vec3(4, 1, 0), 1.0, material3, 0));
 
    Material material4 = Material(MaterialType::DIFFUSE_LIGHT, Vec3(1, 1, 0), 0, 0);
    world.push_back(Object3D(Vec3(3, 1, 2), 0.5, material4, 0));
    
    Material material5 = Material(MaterialType::DIFFUSE_LIGHT, Vec3(0, 1, 0), 0, 0);
    world.push_back(Object3D(Vec3(3, 0.7, 4), 0.3, material5));
//

//    Material material7 = Material(MaterialType::DIFFUSE_LIGHT, Vec3(0, 1, 1), 0, 0);
//    world.push_back(Object3D(Vec3(-4, 1, -2), 0.7, material7));

    Material material6 = Material(MaterialType::DIFFUSE_LIGHT, Vec3(5, 5, 5), 0, 0);
    world.push_back(Object3D(Vec3(3, 3, -2.4), 0.7, material6));
//
    Material material7 = Material(MaterialType::DIFFUSE_LIGHT, Vec3(0, 1, 1), 0, 0);
    world.push_back(Object3D(Vec3(-4, 1, -2), 0.7, material7));

    
    
    BoundingBox boundingBox;
    int lightCount = 0;
    for (uint i = 0; i < world.size(); i++) {
        world[i].id = i;
        growBoundingBoxSphere(boundingBox, world[i]);
        
		if (world[i].mat.type == MaterialType::DIFFUSE_LIGHT)
			lightCount++;
    }
    
    std::vector<BVHNode> nodes;
	BVHNode root;
	root.triangleCount = static_cast<int>(world.size());
	root.boundingBox = boundingBox;
	nodes.push_back(root);
	root.childIndex = static_cast<int>(nodes.size());

    Split(nodes, world, &root);
//    for (auto node : nodes) {
//		std::cout << "triangleIndex: " << node.triangleIndex << " triangleCount: " << node.triangleCount << " childIndex: " << node.childIndex << std::endl;
//	}
	
	ObjectsData objData = ObjectsData{
		static_cast<unsigned int>(world.size()),
		static_cast<unsigned int>(lightCount),
		static_cast<unsigned int>(nodes.size())
	};
	
//    std::cout << "size = " << world.size() << std::endl;

    while (1) {
        SDL_PollEvent(&event);
        if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
			break;
		}
		cam.updateMovementBools(event);
		cam.updateCamera();
		cam.render(objData, world.data(), nodes.data());
//        std::cout << "center: " << cam.center << std::endl;
//        std::cout << "direction: " << cam.direction << std::endl;
    }
    

    return 0;
}

