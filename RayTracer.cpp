// RayTracer.cpp : Defines the entry point for the application.
//

#include "RayTracer.h"

#include "camera.h"
#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"



int main()
{
	//Image
	auto aspect_ratio = 16.0 / 9.0;
	int image_width = 400;

	//Calcuate image ratio for ppm
	int image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	// Viewport widths less than one are ok since they are real valued.

	//World

	hittable_list world;

	world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100));

	//Camera
	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;

	cam.render(world);
	
	return 0;
}
