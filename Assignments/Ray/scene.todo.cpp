#include <cmath>
#include <Util/exceptions.h>
#include "scene.h"

using namespace Ray;
using namespace Util;
using namespace Image;

double clamp(double color)
{
	return std::max(0.0, std::min(1.0, color));
}

///////////
// Scene //
///////////
Point3D Scene::Reflect( Point3D v , Point3D n )
{
	//////////////////
	// Reflect here //
	//////////////////
	return v - n * (2.0 * (v).dot(n));
}

bool Scene::Refract( Point3D v , Point3D n , double ir , Point3D& refract )
{
	//////////////////
	// Refract here //
	//////////////////
	refract = v; 

	double ni = 1.0;
	double nr = ir;

	if (v.dot(n) > 0) { 
		ni = ir;
		nr = 1.0;
		n = -n;
	}

	double ti = acos(n.dot(-v) / v.length());
	double critical_angle = (ni / nr) * sin(ti); 
	if (critical_angle > 1) return false;
	double tr = asin(ni * sin(ti) / nr);
	refract = n * ((ni / nr)*cos(ti) - cos(tr)) + v * ((ni / nr));
	return true;
}

Point3D Scene::getColor( Ray3D ray , int rDepth , Point3D cLimit, double radius )
{
	////////////////////////////////////////////////
	// Get the color associated with the ray here //
	////////////////////////////////////////////////
	if (rDepth <= 0) return Point3D();
	Point3D one; one[0] = 1; one[1] = 1; one[2] = 1;
	RayShapeIntersectionInfo iInfo;
	double dist = this->intersect(ray, iInfo, BoundingBox1D(), std::function< bool(double) >());
	if (dist == INFINITY) return Point3D();
	Point3D color = iInfo.material->emissive;
	for (auto* light : this->_globalData.lights) {
		Point3D shadowTerm = light->transparency(iInfo, *this, cLimit, radius);
		Point3D diffuse = light->getDiffuse(ray, iInfo);
		Point3D specular = light->getSpecular(ray, iInfo);
		if (iInfo.material->tex == NULL) {
			color += (diffuse + specular) * (shadowTerm);
			color += light->getAmbient(ray, iInfo);
		} else {				
			Pixel32 texVal = iInfo.material->tex->_image.bilinearSample(iInfo.texture);
			Point3D tex{texVal.r / 255.0, texVal.g / 255.0, texVal.b / 255.0};
			color += (diffuse + specular) * shadowTerm * tex;
			color += light->getAmbient(ray, iInfo) * tex;
		}
	}
	
	//Reflection
	bool shouldReflect = false;
	for (int i = 0; i < 3; i++) {
		if (iInfo.material->specular[i] > cLimit[i]) {
			shouldReflect = true;
		}
	}
	if (rDepth > 0 && shouldReflect) {
		if(iInfo.normal.dot(ray.direction) < 0) {
			Ray3D reflect;
			reflect.direction = Reflect(ray.direction.unit(), iInfo.normal.unit());
			reflect.position = iInfo.position + reflect.direction * EPSILON;
			color += iInfo.material->specular * this->getColor(reflect, rDepth - 1, cLimit / iInfo.material->specular, radius);
		}
	}
	color[0] = clamp(color[0]);
	color[1] = clamp(color[1]);
	color[2] = clamp(color[2]);
	
	//Refraction
	bool shouldRefract = false;
	for (int i = 0; i < 3; i++) {
		if (iInfo.material->transparent[i] > cLimit[i]) {
			shouldRefract = true;
		}
	}
	Point3D transparent;
	Point3D refractedDirection;	
	if (rDepth > 0 && shouldRefract) {
		Point3D refractedDirection;
		if (Refract(ray.direction, iInfo.normal, iInfo.material->ir, refractedDirection)) {
			Ray3D refractedRay = Ray3D(iInfo.position + refractedDirection * EPSILON, refractedDirection);
			color += (iInfo.material->transparent * this->getColor(refractedRay, rDepth - 1, cLimit / iInfo.material->transparent, radius));
		}
	}
	color[0] = clamp(color[0]);
	color[1] = clamp(color[1]);
	color[2] = clamp(color[2]);
	return color;
}

//////////////
// Material //
//////////////
void Material::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();
}

/////////////
// Texture //
/////////////
void Texture::initOpenGL( void )
{
	///////////////////////////////////
	// Do OpenGL texture set-up here //
	///////////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();
}
