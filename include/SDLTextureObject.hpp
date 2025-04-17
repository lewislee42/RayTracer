#ifndef SDLTEXTUREOBJECT_CLASS_HPP
# define SDLTEXTUREOBJECT_CLASS_HPP

# include <SDL3/SDL.h>
# include <exception>
# include <Vec3.hpp>
# include <Interval.hpp>

class SDLTextureObject {
private:
	SDL_Texture*	_texture;
	Uint32*			_pixels;
	int				_width;
	int				_height;

public:
	SDLTextureObject(SDL_Renderer* renderer);
	~SDLTextureObject();
	
	void lockTexture();
	void unlockTexture();
	void putPixel(int x, int y, Vec3 color);

	SDL_Texture* getTexture();

	class SDLTextureInitializationError: public std::exception {
	public:
		const char* what() const throw();
	};

	class SDLTextureAlreadyLocked: public std::exception {
	public:
		const char* what() const throw();
	};

	class SDLTextureNotLocked: public std::exception {
	public:
		const char* what() const throw();
	};

	class SDLTextureCouldNotLock: public std::exception {
	public:
		const char* what() const throw();
	};

};

#endif /* SDLTEXTUREOBJECT_CLASS_HPP */
