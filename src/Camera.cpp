
#include "Utils.hpp"
#include <Camera.hpp>
#include <RayTracing.hpp>
#include <MetalAdder.hpp>

Camera::Camera() {
  this->initialize();
  this->_device = MTL::CreateSystemDefaultDevice();
  this->_metalAdder =
      new MetalAdder(this->_device, this->imageW, this->_imageH);
  this->_pixels = new uint8_t[(this->imageW * this->_imageH * 3)];
  bzero(this->_pixels, sizeof(uint8_t) * (this->imageW * this->_imageH * 3));
}

void Camera::render(const std::vector<Object3D> world,
                    SDLTextureObject &texture) {
  this->initialize();
  /*uint8_t wholearray;*/

  std::cout << "started rendering\n";
  CameraData camData = this->getCameraData();
  this->_metalAdder->sendComputeCommand(camData, world.data(), world.size(),
                                        this->maxDepth, this->_pixels);

  std::cout << "Pixels: " << this->imageW << " x " << this->_imageH << "\n";
  for (int i = 0; i < this->_imageH; i++) {
    std::cout << "\rScanlines Remaining: " << (this->_imageH - i) << ' '
              << std::flush;
    for (int j = 0; j < this->imageW; j++) {

      Vec3 color = {0, 0, 0};
      /*for (int sample = 0; sample < this->samplePerPixel; sample++) {*/
      /*	Ray r = this->getRay(j, i);*/
      /*	color = color + this->rayColor(r, this->maxDepth, world,
       * world.size());*/
      /*}*/
      int currentPos = ((i * this->imageW) + j) * 3;
      color.x = this->_pixels[currentPos];
      color.y = this->_pixels[currentPos + 1];
      color.z = this->_pixels[currentPos + 2];
      /*std::cout << "color: " << color << std::endl;*/
      texture.putPixel(j, i, this->_pixelSampleWeight * color);
    }
  }
  std::cout << "\rDone.                  \n";

  std::cout << "finished rendering\n";
}

void Camera::initialize() {
  this->_imageH = int(this->imageW / this->aspectRatio);
  if (this->_imageH < 1)
    this->_imageH = 1;

  this->_pixelSampleWeight = 1.0 / this->samplePerPixel;
  this->_center = this->lookFrom;

  // Calculating the viewport dimensions
  float focalLength = 1.0;
  float theta = degreesToRadians(this->fov);
  float h = std::tan(theta / 2);
  float viewportH = 2.0 * h * focalLength;
  float viewportW = viewportH * (float(this->imageW) / this->_imageH);

  this->_w = unitVector(this->lookFrom - this->lookAt);
  this->_u = unitVector(cross(vup, this->_w));
  this->_v = cross(this->_w, this->_u);

  Vec3 viewportU = viewportW * this->_u;
  Vec3 viewportV = viewportH * this->_v * -1;

  this->_pixelDeltaU = viewportU / this->imageW;
  this->_pixelDeltaV = viewportV / this->_imageH;

  Vec3 viewportUpperLeft =
      this->_center - (focalLength * this->_w) - viewportU / 2 - viewportV / 2;
  this->_pixel00Loc =
      viewportUpperLeft + 0.5 * (this->_pixelDeltaU + this->_pixelDeltaV);
}

bool gotHit(const Ray &r, const Interval rayT, HitRecord *rec,
            const std::vector<Object3D> &objects, const int objectAmount) {
  HitRecord tempRec;
  bool hitAnything = false;
  float closestSoFar = rayT.max;

  for (int i = 0; i < objectAmount; i++) {
    Interval tempInterval = (Interval){rayT.min, closestSoFar};
    if (hit(objects[i], r, tempInterval, &tempRec)) {
      hitAnything = true;
      closestSoFar = tempRec.t;
      *rec = tempRec;
    }
  }

  return hitAnything;
}

Vec3 Camera::rayColor(const Ray &r, int depth,
                      const std::vector<Object3D> &world,
                      const int &objectAmount) const {
  HitRecord rec;
  Vec3 color = {0, 0, 0};
  Vec3 attenuationList = {1, 1, 1};
  Ray currentRay = r;

  while (1) {
    if (depth <= 0) {
      return (Vec3){0, 0, 0};
    }

    if (gotHit(currentRay, (Interval){0.001, infinity}, &rec, world,
               world.size())) {
      Ray scattered;
      Vec3 attenuation = (Vec3){0, 0, 0};
      if (scatter(rec.mat, currentRay, rec, &attenuation, &scattered)) {
        depth -= 1;
        attenuationList = attenuation * attenuationList;
        currentRay = scattered;
        continue;
      }
      return (Vec3){0, 0, 0};
    }
    Vec3 unitDirection = unitVector(currentRay.direction);
    float a = 0.5 * (unitDirection.y + 1.0);
    color = (1.0 - a) * (Vec3){1, 1, 1} + a * (Vec3){0.5, 0.7, 1.0};

    break;
  }
  /*std::cout << "attenuation: " << attenuationList << " color: " << color <<
   * std::endl;*/
  return color * attenuationList;
}

Ray Camera::getRay(int x, int y) const {
  Vec3 offset = sampleSquare();
  Vec3 pixelSample = this->_pixel00Loc + ((x + offset.x) * this->_pixelDeltaU) +
                     ((y + offset.y) * this->_pixelDeltaV);
  Vec3 rayOrigin = this->_center;
  Vec3 rayDirection = pixelSample - rayOrigin;

  return (Ray){rayOrigin, rayDirection};
}

Vec3 Camera::sampleSquare() const {
  return (Vec3){randomDouble() - 0.5f, randomDouble() - 0.5f, 0};
}

int Camera::getHeight() const { return this->_imageH; }

CameraData Camera::getCameraData() const {
  return (CameraData){
      this->_pixel00Loc,  this->_pixelDeltaU,
      this->_pixelDeltaV, this->_center,
      this->imageW,       int(this->imageW / this->aspectRatio)};
}
