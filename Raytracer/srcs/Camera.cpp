
#include "Camera.hpp"
#include "RayTracing.hpp"
#include "MetalAdder.hpp"


Camera::Camera(
    uint            screenWidth,
    double          aspectRatio,
    uint            samplePerPixel,
    uint            maxBounces,
    uint			lightSampleAmount,
    uint             vfov,
    Vec3            center,
    Vec3            direction,
    Vec3            vup,
    SDL_Renderer*   renderer
):  screenWidth(screenWidth),
    aspectRatio(aspectRatio),
    samplePerPixel(samplePerPixel),
    maxBounces(maxBounces),
	lightSampleAmount(lightSampleAmount),
    vfov(vfov),
    center(center),
    direction(normalizeVec(direction)),
    vup(vup)
{
    // stuff that needs to initialize once
    _screenHeight = int(screenWidth / aspectRatio);
    if (_screenHeight < 1)
      _screenHeight = 1;

    
    initialize();

    swapchain = (CA::MetalLayer *)SDL_GetRenderMetalLayer(renderer);
    _metalAdder = new MetalAdder(swapchain, screenWidth, _screenHeight);
}


void Camera::render(const ObjectsData& objData, const Object3D* objects, const BVHNode* bvhNodes) {
    initialize();

    // std::cout << "started rendering\n";
    std::chrono::time_point startTime = std::chrono::system_clock::now();
    
    RaytracingMetaData metaData = getMetaData();
    _metalAdder->sendComputeCommand(
        metaData,
        objects,
        bvhNodes,
        objData
    );
    
    std::chrono::time_point endTime = std::chrono::system_clock::now();
    auto difference = endTime - startTime;
    auto differenceMillisecond = std::chrono::duration_cast<std::chrono::milliseconds>(difference);
    long long millisecondCount = differenceMillisecond.count();
    std::cout << "ms: " << millisecondCount << ", FPS: " << static_cast<int>(1000.0f / millisecondCount) << std::endl;
    
}

void Camera::initialize() {
    // Calculating the viewport dimensions
    float focalLength = 1;
    float theta = degreesToRadians(vfov);
    float h = std::tan(theta / 2);
    float viewportH = 2.0 * h * focalLength;
    float viewportW = viewportH * (float(screenWidth) / _screenHeight);

    _w = unitVector(direction * -1);
    _u = unitVector(cross(vup, _w));
    _v = cross(_w, _u);

    Vec3 viewportU = viewportW * _u;
    Vec3 viewportV = viewportH * (_v * -1);

    _pixelDeltaU = viewportU / screenWidth;
    _pixelDeltaV = viewportV / _screenHeight;

    Vec3 viewportUpperLeft = center - (focalLength * _w) - viewportU / 2 - viewportV / 2;
    _pixel00Loc = viewportUpperLeft + 0.5 * (_pixelDeltaU + _pixelDeltaV);
}

RaytracingMetaData Camera::getMetaData() const {
    RaytracingMetaData data = (RaytracingMetaData){
        _pixel00Loc,  _pixelDeltaU,
        _pixelDeltaV, center, samplePerPixel, lightSampleAmount,
        screenWidth, static_cast<uint>(screenWidth / aspectRatio),
		maxBounces
    };
//    std::cout << "data ----" << std::endl <<
//    "pixel00Loc: " << _pixel00Loc << std::endl <<
//    "pixelDeltaU: " << _pixelDeltaU << std::endl <<
//    "pixelDeltaV: " << _pixelDeltaV << std::endl <<
//    "center: " << _center << std::endl <<
//    "samplePerPixel: " << samplePerPixel << std::endl <<
//    "imageW: " << imageW << std::endl <<
//    "imageH: " << int(imageW / aspectRatio) << std::endl;
    return data;
}

void	Camera::updateMovementBools(const SDL_Event event) {
	if (event.type == SDL_EVENT_KEY_DOWN) {
		if (event.key.scancode == SDL_SCANCODE_W)
			movementBools.keyW = true;
		if (event.key.scancode == SDL_SCANCODE_S)
			movementBools.keyS = true;
		if (event.key.scancode == SDL_SCANCODE_A)
			movementBools.keyA = true;
		if (event.key.scancode == SDL_SCANCODE_D)
			movementBools.keyD = true;
		if (event.key.scancode == SDL_SCANCODE_LEFT)
			movementBools.keyTurnLeft = true;
		if (event.key.scancode == SDL_SCANCODE_RIGHT)
			movementBools.keyTurnRight = true;
		if (event.key.scancode == SDL_SCANCODE_UP)
			movementBools.keyTurnUp = true;
		if (event.key.scancode == SDL_SCANCODE_DOWN)
			movementBools.keyTurnDown = true;
		if (event.key.scancode == SDL_SCANCODE_SPACE)
			movementBools.keyMoveUp = true;
		if (event.key.scancode == SDL_SCANCODE_LCTRL)
			movementBools.keyMoveDown = true;
	}
	else if (event.type == SDL_EVENT_KEY_UP) {
		if (event.key.scancode == SDL_SCANCODE_W)
			movementBools.keyW = false;
		if (event.key.scancode == SDL_SCANCODE_S)
			movementBools.keyS = false;
		if (event.key.scancode == SDL_SCANCODE_A)
			movementBools.keyA = false;
		if (event.key.scancode == SDL_SCANCODE_D)
			movementBools.keyD = false;
		if (event.key.scancode == SDL_SCANCODE_LEFT)
			movementBools.keyTurnLeft = false;
		if (event.key.scancode == SDL_SCANCODE_RIGHT)
			movementBools.keyTurnRight = false;
		if (event.key.scancode == SDL_SCANCODE_UP)
			movementBools.keyTurnUp = false;
		if (event.key.scancode == SDL_SCANCODE_DOWN)
			movementBools.keyTurnDown = false;
		if (event.key.scancode == SDL_SCANCODE_SPACE)
			movementBools.keyMoveUp = false;
		if (event.key.scancode == SDL_SCANCODE_LCTRL)
			movementBools.keyMoveDown = false;
	}
}

void Camera::updateCamera() {
	if (movementBools.keyW == true) {
		center = center + (direction * 0.1);
		std::cout << "W pressed... center: " << center << std::endl;
	}
	if (movementBools.keyS == true) {
		center = center + (-1 * direction * 0.1);
		std::cout << "S pressed... center: " << center << std::endl;
	}
	if (movementBools.keyA == true) {
		center = center + (-1 * cross(direction, (Vec3){0, 1, 0}) *  0.1);
		std::cout << "A pressed... center: " << center << std::endl;
	}
	if (movementBools.keyD == true) {
		center = center + (cross(direction, (Vec3){0, 1, 0}) *  0.1);
		std::cout << "D pressed... center: " << center << std::endl;
	}
	if (movementBools.keyTurnLeft == true) {
		direction = normalizeVec(direction + (cross(direction, (Vec3){0, -1, 0}) * 0.01));
		std::cout << "Left pressed... direction: " << direction << std::endl;
	}
	if (movementBools.keyTurnRight == true) {
		direction = normalizeVec(direction + (cross(direction, (Vec3){0, 1, 0}) * 0.01));
		std::cout << "Right pressed... direction: " << direction << std::endl;
	}
	if (movementBools.keyTurnUp == true) {
		Vec3 right = normalizeVec(cross(direction, vup));
		direction = direction * cos(0.01) + cross(right, direction) * sin(0.01) + right * dot(right, direction) * (1.0f - cos(0.01));
		std::cout << "Up pressed... direction: " << direction << std::endl;
	}
	if (movementBools.keyTurnDown == true) {
		Vec3 right = normalizeVec(cross(direction, vup));
		direction = direction * cos(-0.01) + cross(right, direction) * sin(-0.01) + right * dot(right, direction) * (1.0f - cos(-0.01));
		std::cout << "Down pressed... direction: " << direction << std::endl;
	}
	if (movementBools.keyMoveUp == true) {
		center = center + ((Vec3){0, 1, 0} * 0.1);
	}
}
