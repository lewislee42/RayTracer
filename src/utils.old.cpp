# include <glm/glm.hpp>
# include <SDLTextureObject.hpp>
# include <SDLObject.hpp>

int edgeCross(const glm::vec2& a, const glm::vec2& b, const glm::vec2& p) {
	glm::vec2 ab = {b.x - a.x, b.y - a.y};
	glm::vec2 ap = {p.x - a.x, p.y - a.y};
	return (ab.x * ap.y) - (ab.y * ap.x);
}

bool isTopLeft(const glm::vec2& start, const glm::vec2& end) {
	glm::vec2 edge = {end.x - start.x, end.y - start.y};

	bool is_top_edge = edge.y == 0 && edge.x > 0;
	bool is_left_edge = edge.y < 0;

	return is_top_edge || is_left_edge;
}

void triangleFill(SDLTextureObject& texture, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec3& color) {
	int x_min = glm::min(glm::min(a.x, b.x), c.x);
	int y_min = glm::min(glm::min(a.y, b.y), c.y);
	int x_max = glm::max(glm::max(a.x, b.x), c.x);
	int y_max = glm::max(glm::max(a.y, b.y), c.y);

	int bias0 = isTopLeft(b, c) ? 0 : -1;
	int bias1 = isTopLeft(c, a) ? 0 : -1;
	int bias2 = isTopLeft(a, b) ? 0 : -1;

	for (int i = y_min; i <= y_max; i++) {
		for (int j = x_min; j <= x_max; j++) {
			glm::vec2 pos = {j, i};

			int w0 = edgeCross(b, c, pos) + bias0;
			int w1 = edgeCross(c, a, pos) + bias1;
			int w2 = edgeCross(a, b, pos) + bias2;

			if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
				/*printf("it actually drew a pixel at pos [%d, %d] with color [%d, %f, %f]\n", j, i, 0xff, color.y, color.z);*/
				texture.putPixel(pos, color);
			}
		}
	}
}

int main() {
	SDL_Event event;
	SDLObject sdlObject(800, 600);
	SDLTextureObject texture(sdlObject.getRenderer());
	glm::vec3 color = {0, 0, 0};
	glm::vec2 triangle[5] = {{60, 0}, {120, 60}, {20, 70}, {7, 20}, {130 ,30}};

	while (1) {
		SDL_PollEvent(&event);
		if (event.type == SDL_EVENT_QUIT) {
			break ;
		}

		sdlObject.clearScreen();
		texture.lockTexture();

		color = {149, 100, 10};
		triangleFill(texture, triangle[0], triangle[1], triangle[2], color);
		color = {30, 100, 10};
		triangleFill(texture, triangle[0], triangle[2], triangle[3], color);
		color = {70, 150, 190};
		triangleFill(texture, triangle[0], triangle[4], triangle[1], color);

		texture.unlockTexture();
		sdlObject.renderTexture(texture.getTexture());
	}

	return 0;

}
