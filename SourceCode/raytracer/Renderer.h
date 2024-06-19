#pragma once
#include <algorithm>
#include <tuple>

#include "CRTTypes.h"
#include "Camera.h"
#include "perlin-noise.h"
#include "Scene.h"
#include "Triangle.h"

struct RendererMetrics {
    int triangleTests = 0;
    int trianglePlaneIntersections = 0;
    int triangleBoundsIntersections = 0;
    int triangleEarlyTerminated = 0; // e.g. distance culling new_t < t
    int triangleBackfaceCulled = 0;

    static constexpr const char* sTriangleTests = "triangleTests";
    static constexpr const char* sTrianglePlaneIntersections = "trianglePlaneIntersections";
    static constexpr const char* sTriangleBoundsIntersections = "triangleBoundsIntersections";
    static constexpr const char* sTriangleEarlyTerminated = "triangleEarlyTerminated";
    static constexpr const char* sTriangleBackfaceCulled = "triangleBackfaceCulled";

    std::string toString() const
    {
        return 
            std::string(sTriangleTests) + ": " + std::to_string(triangleTests) + "\n" +
            std::string(sTrianglePlaneIntersections) + ": " + std::to_string(trianglePlaneIntersections) + "\n" +
            std::string(sTriangleBoundsIntersections) + ": " + std::to_string(triangleBoundsIntersections) + "\n" +
            std::string(sTriangleEarlyTerminated) + ": " + std::to_string(triangleEarlyTerminated) + "\n" +
            std::string(sTriangleBackfaceCulled) + ": " + std::to_string(triangleBackfaceCulled);
    };
};

class Renderer {
public:
    Renderer(Scene* _scene)
        : scene(_scene){}

    void render(Buffer2D& image) const
    {
        for (int y = 0; y < scene->camera.getHeight(); ++y) {
            for (int x = 0; x < scene->camera.getWidth(); ++x) {
                Ray ray = scene->camera.generateRay(x, y);
                // Color color = traceImagePlane(ray); // hw3
                ray.direction = ray.direction.normalize();
                Color color = traceRay(ray); // hw5
                image(x, y) = color;
            }
        }
    }

    void render_dbg(Buffer2D& image) const
    {
            Ray ray;
            ray = {{0.f, 0.f, 0.f}, {-1.f, -1.f, -1.f}};
            ray.direction = ray.direction.normalize();
            traceRay(ray);

            //ray = {{0.f, 0.f, 0.f}, {1.f, -1.f, -1.f}};
            //ray.direction = ray.direction.normalize();
            //traceRay(ray); 

            //ray = {{0.f, 0.f, 0.f}, {-1.f, 1.f, -1.f}};
            //ray.direction = ray.direction.normalize();
            //traceRay(ray);

            //ray = {{0.f, 0.f, 0.f}, {1.f, 1.f, -1.f}};
            //ray.direction = ray.direction.normalize();
            //traceRay(ray); 

            //
            //ray = {{0.f, 0.f, 0.f}, {0.f, 0.f, -1.f}};
            //ray.direction = ray.direction.normalize();
            //traceRay(ray);
    }

private:
    Scene* scene;
    RendererMetrics metrics;

    Color traceRay(const Ray& ray) const
    {
        float t, u, v;
        Vec3 p, n; // intersection Point, triangle Normal
        Color closest = scene->backgroundColor;
        float closest_t = FLT_MAX;
        for (const Triangle& tri : scene->triangles) {
            if (tri.intersect(ray, t, p, n, u, v) && t < closest_t) { // TODO: Separate plane intersection & triangle uv intersection tests for perf.
                closest_t = t;
                closest = shade_uv(p, n, u, v);
            }
        }
        return closest;

    }

    /* hw3. For debugging */
    Color traceImagePlane(const Ray& ray) const
    {
        Vec3 p = ray.origin + ray.direction;
        float scale = -1.f / p.z;
        p = p * scale;
        return shade_abs(p);
    }

    Color shade_dbg_b(Vec3 p) const {
        uint8_t b = static_cast<uint8_t>(fabs(p.z + 1.0f) * 127.5f);
        return Color{ 5,5 , b };
    }

    Color shade_abs(Vec3 p) const {
        uint8_t r = static_cast<uint8_t>(fabs(p.x * 100.f));
        uint8_t g = static_cast<uint8_t>(fabs(p.y * 100.f));
        uint8_t b = static_cast<uint8_t>(fabs(p.z * 100.f));

        return Color{ r, g, b };
    }

    Color shade_perlin(Vec3 p) const
    {
        static const siv::PerlinNoise::seed_type seed = 123456u;
	    static const siv::PerlinNoise perlin{ seed };
	
		const double noise = perlin.octave2D_01(p.x, p.y, 4);
        const uint8_t color = static_cast<uint8_t>(noise * 255);
        return Color{ color, color, color };
    }

    Color shade_uv(const Vec3& p, const Vec3& n, float u, float v) const
    {
        static const siv::PerlinNoise::seed_type seed = 123456u;
	    static const siv::PerlinNoise perlin{ seed };
	
		const double noise = perlin.octave2D_01(u, v, 8, 0.8);
        float brightness = static_cast<uint8_t>(noise * 255);
        const uint8_t r = static_cast<uint8_t>(brightness * n.x);
        const uint8_t g = static_cast<uint8_t>(brightness * n.y);
        const uint8_t b = static_cast<uint8_t>(brightness * n.z);
        return Color{ r, g, b };
    }
};
