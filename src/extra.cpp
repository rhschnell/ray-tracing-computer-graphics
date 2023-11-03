#include "extra.h"
#include "bvh.h"
#include "light.h"
#include "recursive.h"
#include "shading.h"
#include <framework/trackball.h>

// TODO; Extra feature
// Given the same input as for `renderImage()`, instead render an image with your own implementation
// of Depth of Field. Here, you generate camera rays s.t. a focus point and a thin lens camera model
// are in play, allowing objects to be in and out of focus.
// This method is not unit-tested, but we do expect to find it **exactly here**, and we'd rather
// not go on a hunting expedition for your implementation, so please keep it here!
void renderImageWithDepthOfField(const Scene& scene, const BVHInterface& bvh, const Features& features, const Trackball& camera, Screen& screen)
{
    if (!features.extra.enableDepthOfField) {
        return;
    }

    // ...
}

// TODO; Extra feature
// Given the same input as for `renderImage()`, instead render an image with your own implementation
// of motion blur. Here, you integrate over a time domain, and not just the pixel's image domain,
// to give objects the appearance of "fast movement".
// This method is not unit-tested, but we do expect to find it **exactly here**, and we'd rather
// not go on a hunting expedition for your implementation, so please keep it here!
void renderImageWithMotionBlur(const Scene& scene, const BVHInterface& bvh, const Features& features, const Trackball& camera, Screen& screen)
{
    if (!features.extra.enableMotionBlur) {
        return;
    }

}

// TODO; Extra feature
// Given a rendered image, compute and apply a bloom post-processing effect to increase bright areas.
// This method is not unit-tested, but we do expect to find it **exactly here**, and we'd rather
// not go on a hunting expedition for your implementation, so please keep it here!
void postprocessImageWithBloom(const Scene& scene, const Features& features, const Trackball& camera, Screen& image)
{
    if (!features.extra.enableBloomEffect) {
        return;
    }

    // ...
}


// TODO; Extra feature
// Given a camera ray (or reflected camera ray) and an intersection, evaluates the contribution of a set of
// glossy reflective rays, recursively evaluating renderRay(..., depth + 1) along each ray, and adding the
// results times material.ks to the current intersection's hit color.
// - state;    the active scene, feature config, bvh, and sampler
// - ray;      camera ray
// - hitInfo;  intersection object
// - hitColor; current color at the current intersection, which this function modifies
// - rayDepth; current recursive ray depth
// This method is not unit-tested, but we do expect to find it **exactly here**, and we'd rather
// not go on a hunting expedition for your implementation, so please keep it here!
void renderRayGlossyComponent(RenderState& state, Ray ray, const HitInfo& hitInfo, glm::vec3& hitColor, int rayDepth)

    {
        // Generate an initial specular ray, and base secondary glossies on this ray
        Ray reflection;
        reflection.origin = ray.origin + ray.t * ray.direction;
        reflection.direction = glm::normalize(ray.direction - 2 * glm::dot(hitInfo.normal, ray.direction) * hitInfo.normal);
        reflection.origin = reflection.origin + 0.0001f * reflection.direction;
        HitInfo dummy;
        state.bvh.intersect(state, reflection, dummy );
        glm::vec3 center = reflection.origin + reflection.direction * reflection.t;
        glm::vec3 copy((reflection.direction.x - 1) / 5, (reflection.direction.y - 1) / 7, (reflection.direction.z - 1) / 3);
        if (reflection.direction == glm::vec3(0, 0, 0))
            return;
        glm::vec3 normal1 = glm::normalize(cross(reflection.direction, copy));
        glm::vec3 normal2 = glm::normalize(cross(reflection.direction, normal1));
        center = center + normal1 + normal2;
        std::vector<Ray> rays;
        float shineC = glm::atan(hitInfo.material.shininess/64/reflection.t);
        auto numSamples = state.features.extra.numGlossySamples;
        for (int i = 0; i < numSamples * 8; i++) {
            Ray rayP;
            rayP.direction = reflection.direction;
            rayP.origin = reflection.origin;
            glm::vec2 angle = state.sampler.next_2d();
            float sign = state.sampler.next_1d();
            if (true) {

                glm::mat4 rotationMat(1);
                sign = pow(-1, int(sign * 2));
                rotationMat = glm::rotate(rotationMat, angle.y * glm::pi<float>(), reflection.direction);
                rotationMat = glm::rotate(rotationMat, angle.x * sign * shineC, normal1);
                rayP.direction = glm::normalize(glm::vec3(rotationMat * glm::vec4(rayP.direction, 1.0f)));
                float angl = glm::acos(glm::dot(hitInfo.normal, rayP.direction));
                if (angl < glm::pi<float>() / 2)

                    // glm::vec3 offset = center + coords.x * normal1 + coords.y * normal2;
                    // rayP.direction = offset - rayP.origin;
                    // rayP.t = FLT_MAX;
                    // rayP.direction = glm::normalize(rayP.direction);
                    // if (glm::length(center - offset) <= 1.f)
                    rays.push_back(rayP);
                // drawRay(rayP, glm::vec3(0, 1, 0));
            }
        }
        glm::vec3 color(0, 0, 0);
        float size = std::min((float)rays.size(), (float)numSamples);
        for (int i = 0; i < size; i++)
            color = color + renderRay(state, rays[i], rayDepth + 1);
        hitColor = hitColor + color / (float)size * hitInfo.material.ks;

        // ...
    }


// TODO; Extra feature
// Given a camera ray (or reflected camera ray) that does not intersect the scene, evaluates the contribution
// along the ray, originating from an environment map. You will have to add support for environment textures
// to the Scene object, and provide a scene with the right data to supply this.
// - state; the active scene, feature config, bvh, and sampler
// - ray;   ray object
// This method is not unit-tested, but we do expect to find it **exactly here**, and we'd rather
// not go on a hunting expedition for your implementation, so please keep it here!
glm::vec3 sampleEnvironmentMap(RenderState& state, Ray ray)
{
    if (state.features.extra.enableEnvironmentMap) {
        // Part of your implementation should go here
        return glm::vec3(0.f);
    } else {
        return glm::vec3(0.f);
    }
}


// TODO: Extra feature
// As an alternative to `splitPrimitivesByMedian`, use a SAH+binning splitting criterion. Refer to
// the `Data Structures` lecture for details on this metric.
// - aabb;       the axis-aligned bounding box around the given triangle set
// - axis;       0, 1, or 2, determining on which axis (x, y, or z) the split must happen
// - primitives; the modifiable range of triangles that requires splitting
// - return;     the split position of the modified range of triangles
// This method is unit-tested, so do not change the function signature.
size_t splitPrimitivesBySAHBin(const AxisAlignedBox& aabb, uint32_t axis, std::span<BVH::Primitive> primitives)
{
    using Primitive = BVH::Primitive;

    return 0; // This is clearly not the solution
}