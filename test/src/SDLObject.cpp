#include "SDLObject.hpp"

SDLObject::SDLObject(Uint16 width, Uint16 height): _width(width), _height(height) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		throw SDLInitializationError();
    }

    if (!SDL_CreateWindowAndRenderer("Hello SDL", this->_width, this->_height, SDL_WINDOW_MAXIMIZED, &this->_window, &this->_renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
		throw SDLInitializationError();
    }

	this->_surface = SDL_CreateSurface(this->_width, this->_height, SDL_PIXELFORMAT_RGBA32);
    if (!this->_surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
		throw SDLInitializationError();
    }
}

SDLObject::~SDLObject() {
    SDL_DestroyRenderer(this->_renderer);
    SDL_DestroyWindow(this->_window);
    SDL_Quit();
}


// Member functions
void SDLObject::clearScreen() {
	SDL_SetRenderDrawColor(this->_renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(this->_renderer);
}

void SDLObject::renderTexture(SDL_Texture* texture) {
	SDL_RenderTexture(this->_renderer, texture, NULL, NULL);
	SDL_RenderPresent(this->_renderer);
}


// Getters & Setters
void SDLObject::setWidth(Uint16 width) {
	this->_width = width;
}

int SDLObject::getWidth() const {
	return this->_width;
}

void SDLObject::setHeight(Uint16 height) {
	this->_height= height;
}

int SDLObject::getHeight() const {
	return this->_height;
}

SDL_Renderer* SDLObject::getRenderer() const {
	return this->_renderer;
}


// Exceptions
const char* SDLObject::SDLInitializationError::what() const throw() {
	return "Could not initialize SDL components";
}
