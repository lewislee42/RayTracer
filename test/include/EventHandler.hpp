//
//  EventHandler.h
//  test
//
//  Created by Lewis lee on 10/05/2025.
//

#ifndef EVENTHANDLER_CLASS_HPP
# define EVENTHANDLER_CLASS_HPP

#include <SDL3/SDL.h>

class Camera;

class EventHandler {
	private:
		bool keyMoveForward;
		bool keyMoveBackwards;
		bool keyMoveLeft;
		bool keyMoveRight;
		bool keyMoveUp;
		bool keyMoveDown;
		
		bool keyTurnLeft;
		bool keyTurnRight;
		bool keyTurnUp;
		bool keyTurnDown;
	
	public:
	
		void handleEvents(Camera* cam);

};

#endif
