#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include "RayTracing.hpp"

#include "Engine.hpp"

int main(int argc, char *argv[]) {
    double			aspectRatio	= 16.0 / 9.0;
    unsigned int	screenWidth	= 800;
    
    Engine engine(screenWidth, static_cast<unsigned int>(screenWidth / aspectRatio), aspectRatio);
    engine.run();
    

    return 0;
}

