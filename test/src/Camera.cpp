
#include <Camera.hpp>
#include <RayTracing.hpp>
#include <MetalAdder.hpp>


Camera::Camera(
    uint            screenWidth,
    double          aspectRatio,
    uint            samplePerPixel,
    uint            maxBounces,
    uint			lightSampleAmount,
    int             vfov,
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
    vup(vup),
    _frameCount(0)
{
    // stuff that needs to initialize once
    _screenHeight = int(screenWidth / aspectRatio);
    if (_screenHeight < 1)
      _screenHeight = 1;

    _pixelSampleWeight = 1.0 / samplePerPixel;
    
    initialize();

    swapchain = (CA::MetalLayer *)SDL_GetRenderMetalLayer(renderer);
    _metalAdder = new MetalAdder(swapchain, screenWidth, _screenHeight);
}


void Camera::render(const std::vector<Object3D> world) {
    initialize();

    // std::cout << "started rendering\n";
    std::chrono::time_point startTime = std::chrono::system_clock::now();
    
    _frameCount++;
    CameraData camData = getCameraData();
    _metalAdder->sendComputeCommand(
        camData,
        world.data(),
        static_cast<uint>(world.size()),
        maxBounces
    );
    
    std::chrono::time_point endTime = std::chrono::system_clock::now();
    auto difference = endTime - startTime;
    auto differenceMillisecond = std::chrono::duration_cast<std::chrono::milliseconds>(difference);
    long long millisecondCount = differenceMillisecond.count();
    std::cout << "Finished randering... Took: " << millisecondCount << " µs\n";
    
	if (_frameCount == 1000000)
		_frameCount = 0;
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

CameraData Camera::getCameraData() const {
    CameraData data = (CameraData){
        _pixel00Loc,  _pixelDeltaU,
        _pixelDeltaV, center, samplePerPixel, lightSampleAmount,
        screenWidth,  int(screenWidth / aspectRatio),
        _frameCount
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
