#ifndef SDLOBJECT_CLASS_HPP
# define SDLOBJECT_CLASS_HPP

# include <SDL3/SDL.h>
# include <exception>

class SDLObject {
private:
	SDL_Window*		_window;
	SDL_Renderer*	_renderer;
	SDL_Surface*	_surface;
	int				_width;
	int				_height;
	
public:
	SDLObject(Uint16 width, Uint16 height);
	~SDLObject();

	void	clearScreen();
	void	renderTexture(SDL_Texture* texture);

	void			setWidth(Uint16 width);
	int				getWidth() const;
	void			setHeight(Uint16 height);
	int				getHeight() const;
	SDL_Renderer*	getRenderer() const;

	class SDLInitializationError: public std::exception {
	public:
		const char* what() const throw();
	};
};

#endif /* SDLOBJECT_CLASS_HPP */
