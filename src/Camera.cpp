
#include "Utils.hpp"
# include <Camera.hpp>

void Camera::render(const Object3D& world, SDLTextureObject& texture) {
	this->initialize();

	/*std::cout << "Pixels: " << this->imageW << " x " << this->_imageH << "\n";*/
	for (int i = 0; i < this->_imageH; i++) {
		std::cout << "\rScanlines Remaining: " << (this->_imageH - i) << ' ' << std::flush;
		for (int j = 0; j < this->imageW; j++) {

			Vec3 color = {0, 0, 0};
			for (int sample = 0; sample < this->samplePerPixel; sample++) {
				Ray r = this->getRay(j, i);
				color += this->rayColor(r, this->maxDepth, world);
			}
			/*std::cout << "color: " << color.x << ", " << color.y << ", " << color.z << std::endl;*/
			texture.putPixel(j, i, this->_pixelSampleWeight * color);
		}
	}
	std::cout << "\rDone.                  \n";
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
	Vec3 viewportV = viewportH * -this->_v;

	this->_pixelDeltaU = viewportU / this->imageW;
	this->_pixelDeltaV = viewportV / this->_imageH;

	Vec3 viewportUpperLeft =
		this->_center
		- (focalLength * this->_w) 
		- viewportU / 2 
		- viewportV / 2;
	this->_pixel00Loc = viewportUpperLeft + 0.5 * (this->_pixelDeltaU + this->_pixelDeltaV);
}

Vec3 Camera::rayColor(const Ray& r, int depth, const Object3D& world) const {
	HitRecord rec;
	Vec3 color = {0, 0, 0};
	Vec3 attenuationList = {1, 1, 1};
	Ray currentRay = r;

	while (1) {
		if (depth <= 0) {
			return Vec3(0, 0, 0);
		}

		if (world.hit(currentRay, Interval(0.001, infinity), rec)) {
			Ray scattered;
			Vec3 attenuation;
			if (rec.mat->scatter(currentRay, rec, attenuation, scattered)) {
				depth -= 1;
				attenuationList = attenuation * attenuationList;
				currentRay = scattered;
				continue;
			}
			return Vec3(0, 0, 0);
		}
		Vec3 unitDirection = unitVector(r.getDirection());
		float a = 0.5 * (unitDirection.y + 1.0);
		color = (1.0 - a) * Vec3(1, 1, 1) + a * Vec3(0.5, 0.7, 1.0);

		break;
	}
	return color * attenuationList;
}

Ray Camera::getRay(int x, int y) const {
	Vec3 offset = sampleSquare();
	Vec3 pixelSample = this->_pixel00Loc
		+ ((x + offset.x) * this->_pixelDeltaU)
		+ ((y + offset.y) * this->_pixelDeltaV);
	Vec3 rayOrigin = this->_center;
	Vec3 rayDirection = pixelSample - rayOrigin;

	return Ray(rayOrigin, rayDirection);
}

Vec3 Camera::sampleSquare() const {
	return Vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
}

int Camera::getHeight() const {
	return this->_imageH;
}



