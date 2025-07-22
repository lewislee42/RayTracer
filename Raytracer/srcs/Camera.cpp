
#include "Camera.hpp"
#include "RayTracing.hpp"
#include "MetalAdder.hpp"


Camera::Camera(
    uint            screenWidth,
    double          aspectRatio,
    uint            samplePerPixel,
    uint            maxBounces,
    uint			lightSampleAmount,
    uint            vfov,
    Vec3            center,
    Vec3            direction,
    Vec3            vup
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

    updateCameraData();
}

void Camera::updateCameraData() {
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
        _pixel00Loc,
		_pixelDeltaU,
        _pixelDeltaV,
        center,
        samplePerPixel,
        lightSampleAmount,
        screenWidth,
        static_cast<uint>(screenWidth / aspectRatio),
		maxBounces
    };
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

void Camera::updateCameraPosDir(float deltaTime) {
	if (movementBools.keyW == true) {
		center = center + (direction * MOVE_SPEED * deltaTime);
		std::cout << "W pressed... center: " << center << std::endl;
	}
	if (movementBools.keyS == true) {
		center = center + (-1 * direction * MOVE_SPEED * deltaTime);
		std::cout << "S pressed... center: " << center << std::endl;
	}
	if (movementBools.keyA == true) {
		center = center + (-1 * cross(direction, (Vec3){0, 1, 0}) * MOVE_SPEED * deltaTime);
		std::cout << "A pressed... center: " << center << std::endl;
	}
	if (movementBools.keyD == true) {
		center = center + (cross(direction, (Vec3){0, 1, 0}) * MOVE_SPEED * deltaTime);
		std::cout << "D pressed... center: " << center << std::endl;
	}
	if (movementBools.keyTurnLeft == true) {
		direction = normalizeVec(direction + (cross(direction, (Vec3){0, -1, 0}) * TURN_SPEED * deltaTime));
		std::cout << "Left pressed... direction: " << direction << std::endl;
	}
	if (movementBools.keyTurnRight == true) {
		direction = normalizeVec(direction + (cross(direction, (Vec3){0, 1, 0}) * TURN_SPEED * deltaTime));
		std::cout << "Right pressed... direction: " << direction << std::endl;
	}
	if (movementBools.keyTurnUp == true) {
		Vec3 right = normalizeVec(cross(direction, vup));
		float turnAngle = TURN_SPEED * deltaTime;
		direction = direction * cos(turnAngle) + cross(right, direction) * sin(turnAngle) + right * dot(right, direction) * (1.0f - cos(turnAngle));
		std::cout << "Up pressed... direction: " << direction << std::endl;
	}
	if (movementBools.keyTurnDown == true) {
		Vec3 right = normalizeVec(cross(direction, vup));
		float turnAngle = -1 * TURN_SPEED * deltaTime;
		direction = direction * cos(turnAngle) + cross(right, direction) * sin(turnAngle) + right * dot(right, direction) * (1.0f - cos(turnAngle));
		std::cout << "Down pressed... direction: " << direction << std::endl;
	}
	if (movementBools.keyMoveUp == true) {
		center = center + ((Vec3){0, 1, 0} * MOVE_SPEED * deltaTime);
	}
}
