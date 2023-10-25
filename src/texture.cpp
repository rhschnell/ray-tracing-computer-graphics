#include "texture.h"
#include "render.h"
#include <framework/image.h>

float squareArea(glm::vec2 v0, glm::vec2 v1)
{
    glm::vec2 v2(v0.x, v1.y);
    return glm::length(v0 - v2) * glm::length(v1 - v2);
}

// TODO: Standard feature
// Given an image, and relevant texture coordinates, sample the texture s.t.
// the nearest texel to the coordinates is acquired from the image.
// - image;    the image object to sample from.
// - texCoord; sample coordinates, generally in [0, 1]
// - return;   the nearest corresponding texel
// This method is unit-tested, so do not change the function signature.
glm::vec3 sampleTextureNearest(const Image& image, const glm::vec2& texCoord)
{
    // TODO: implement this function.
    // Note: the pixels are stored in a 1D array, row-major order. You can convert from (i, j) to
    //       an index using the method seen in the lecture.
    // Note: the center of the first pixel should be at coordinates (0.5, 0.5)
    // Given texcoords, return the corresponding pixel of the image
    // The pixel are stored in a 1D array of row major order
    // you can convert from position (i,j) to an index using the method seen in the lecture
    // Note, the center of the first pixel is at image coordinates (0.5, 0.5)
    return image.pixels[((int)(texCoord.x * image.width)) + ((int)(((1 - texCoord.y) - FLT_MIN) * image.height)) * image.width];
}

// TODO: Standard feature
// Given an image, and relevant texture coordinates, sample the texture s.t.
// a bilinearly interpolated texel is acquired from the image.
// - image;    the image object to sample from.
// - texCoord; sample coordinates, generally in [0, 1]
// - return;   the filter of the corresponding texels
// This method is unit-tested, so do not change the function signature.
glm::vec3 sampleTextureBilinear(const Image& image, const glm::vec2& texCoord)
{
    // TODO: implement this function.
    // Note: the pixels are stored in a 1D array, row-major order. You can convert from (i, j) to
    //       an index using the method seen in the lecture.
    // Note: the center of the first pixel should be at coordinates (0.5, 0.5)
    // Given texcoords, return the corresponding pixel of the image
    // The pixel are stored in a 1D array of row major order
    // you can convert from position (i,j) to an index using the method seen in the lecture
    // Note, the center of the first pixel is at image coordinates (0.5, 0.5)
    float x = texCoord.x * image.width;
    float y = (1 - texCoord.y) * image.height - FLT_MIN;
    glm::vec2 p(x, y);
    // if (x > 0.5 && x < image.width + 0.5 && y > 0.5 && y < image.height + 0.5) {
    int xf = (int)x;
    int yf = (int)y;
    int xSign = 1;
    int ySign = 1;
    if (x - xf <= 0.5)
        xSign = -1;
    if (y - yf <= 0.5)
        ySign = -1;
    float x1 = xf + 0.5;
    float x2 = x1 + xSign;
    float y1 = yf + 0.5;
    float y2 = y1 + ySign;
    glm::vec3 color = image.pixels[(int)x1 + ((int)y1) * image.width] * squareArea(p, glm::vec2(x2, y2))
        + image.pixels[(int)x2 + ((int)y1) * image.width] * squareArea(p, glm::vec2(x1, y2))
        + image.pixels[(int)x1 + ((int)y2) * image.width] * squareArea(p, glm::vec2(x2, y1))
        + image.pixels[(int)x2 + ((int)y2) * image.width] * squareArea(p, glm::vec2(x1, y1));

    return color;
}