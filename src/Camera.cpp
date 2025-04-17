
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
	this->_center = {0, 0, 0};

	float focalLength = 1.0;
	float viewportH = 2.0;
	float viewportW = viewportH * (double(this->imageW) / this->_imageH);

	Vec3 viewportU = {viewportW, 0, 0};
	Vec3 viewportV = {0, -viewportH, 0};

	this->_pixelDeltaU = viewportU / this->imageW;
	this->_pixelDeltaV = viewportV / this->_imageH;

	Vec3 viewportUpperLeft =
		this->_center
		- Vec3(0, 0, focalLength)
		- viewportU / 2 
		- viewportV / 2;
	this->_pixel00Loc = viewportUpperLeft + 0.5 * (this->_pixelDeltaU + this->_pixelDeltaV);
}

Vec3 Camera::rayColor(const Ray& r, int depth, const Object3D& world) const {
	HitRecord rec;
	Vec3 color = {0, 0, 0};

	if (depth <= 0)
		return Vec3(0, 0, 0);

	if (world.hit(r, Interval(0.001, infinity), rec)) {
		Vec3 direction = randomOnHemisphere(rec.normal);
		return 0.5 * this->rayColor(Ray(rec.p, direction), depth - 1, world);
	}
	Vec3 unitDirection = unit_vector(r.getDirection());

	double a = 0.5 * (unitDirection.y + 1.0);

	color = (1.0 - a) * Vec3(1, 1, 1) + a * Vec3(0.5, 0.7, 1.0);
	return color;
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
