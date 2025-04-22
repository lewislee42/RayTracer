#include "SDLTextureObject.hpp"
#include "Utils.hpp"

SDLTextureObject::SDLTextureObject(SDL_Renderer* renderer): _pixels(NULL), _width(0), _height(0) {
	if (!SDL_GetRenderOutputSize(renderer, &this->_width, &this->_height)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s", SDL_GetError());
		throw SDLTextureObject::SDLTextureInitializationError();
	}

    this->_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, this->_width, this->_height);
    if (!this->_texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s", SDL_GetError());
		throw SDLTextureObject::SDLTextureInitializationError();
    }
}

SDLTextureObject::~SDLTextureObject() {
	SDL_DestroyTexture(this->_texture);
}
	

void SDLTextureObject::lockTexture() {
	void *temp;
	int pitch;

	if (this->_pixels != NULL)
		throw SDLTextureObject::SDLTextureAlreadyLocked();

	if (!SDL_LockTexture(this->_texture, NULL, &temp, &this->_pitch)) {
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Could not lock texture: %s", SDL_GetError());
		throw SDLTextureObject::SDLTextureCouldNotLock();
	}
	this->_pixels = (Uint32*)temp;
}

/*
 * Warning the contents of the previous texture might not be provided when running lockTexture
 * meaning that if you lock and immediately unlock it could cause issues
 */
void SDLTextureObject::unlockTexture() {
	SDL_UnlockTexture(this->_texture);
	this->_pixels = NULL;
}

void SDLTextureObject::putPixel(int x, int y, Vec3 color) {
	if (this->_pixels == NULL)
		throw SDLTextureObject::SDLTextureNotLocked();
	if (color.x < 0 || color.y < 0 || color.z < 0)
		return;

	color.x = linearToGamma(color.x);
	color.y = linearToGamma(color.y);
	color.z = linearToGamma(color.z);

	color.x = int(256 * clamp(color.x, 0.0, 0.999));
	color.y = int(256 * clamp(color.y, 0.0, 0.999));
	color.z = int(256 * clamp(color.z, 0.0, 0.999));

	/*color *= 255.999;*/
	
	/*Uint32 colorTemp = (int(color.x) << 24) | (int(color.y) << 16) | (int(color.z) << 8) | 256;*/
	/*std::cout << "color: " << color.x << " " << color.y << " " << color.z << std::endl;*/
	/*this->_pixels[int((y * this->_width) + x)] = colorTemp;*/
	this->_pixels[int((y * this->_width) + x)] =
		SDL_MapRGBA(
			SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32),
			NULL,
			int(color.x),
			int(color.y),
			int(color.z),
			255
		);
	/*Uint8* tempPixels = (Uint8*)this->_pixels + ((y * this->_pitch) + x * 4);*/
	/**((Uint32*)tempPixels) = colorTemp;*/
}

SDL_Texture* SDLTextureObject::getTexture() {
	return this->_texture;
}


// Exceptions
const char* SDLTextureObject::SDLTextureInitializationError::what() const throw() {
	return "could not create texture";
}

const char* SDLTextureObject::SDLTextureAlreadyLocked::what() const throw() {
	return "texture is already locked";
}

const char* SDLTextureObject::SDLTextureNotLocked::what() const throw() {
	return "texture is not locked";
}

const char* SDLTextureObject::SDLTextureCouldNotLock::what() const throw() {
	return "could not lock texture";
}
