#include <cmath>
#include <Util/exceptions.h>
#include "scene.h"

using namespace Ray;
using namespace Util;

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
	/*v = -v.unit();
	n = n.unit();

	Point3D projected = n * v.dot(n);

	return (projected - (v - projected)).unit();*/
	return v - n * (2 * (v).dot(n));

}

bool Scene::Refract( Point3D v , Point3D n , double ir , Point3D& refract )
{
	//////////////////
	// Refract here //
	//////////////////
	refract = v; // uncomment for reference image

	double Ni = 1.0;
	double Nr = ir;

	if (v.dot(n) > 0) { //leaving (not entering) Nr region (if solid) (should avoid looking at triangles from the back)
		Ni = ir;
		Nr = 1;
		n = -n;
	}

	double thetai = acos(n.dot(-v) / v.length());
	double sinthetar = (Ni / Nr) * sin(thetai);
	if (sinthetar < -1 || sinthetar > 1) return 0;
	double thetar = asin(sinthetar);
	refract = n * ((Ni / Nr)*cos(thetai) - cos(thetar)) + v * ((Ni / Nr));
	return 1;
}

Point3D Scene::getColor( Ray3D ray , int rDepth , Point3D cLimit )
{
	////////////////////////////////////////////////
	// Get the color associated with the ray here //
	////////////////////////////////////////////////

	/*RayShapeIntersectionInfo iInfo;
	const double result = this->intersect(ray, iInfo, BoundingBox1D(), std::function< bool(double) >());
	if (result == Infinity) {
		return Point3D();
	}
	Point3D color;

	for (auto* light : this->_globalData.lights)
	{
		//Point3D shadow = light->transparency(iInfo, *this, cLimit);

		RayShapeIntersectionInfo temp = iInfo;
		Point3D trans = Point3D(1, 1, 1);
		if (light->isInShadow(temp, this))
		{
			trans = light->transparency(temp, *this, cLimit).unit();
		}
		color += light->getAmbient(ray, iInfo);
		color += light->getDiffuse(ray, iInfo) * trans; //light->transparency(iInfo, *this, cLimit);
		color += light->getSpecular(ray, iInfo) * trans; // light->transparency(iInfo, *this, cLimit);
	}

	//reflection
	int cutOffReflect = 1;
	for (int i = 0; i < 3; i++) {
		if (iInfo.material->specular[i] > cLimit[i]) {
			cutOffReflect = 0;
		}
	}

	if (rDepth > 0 && !cutOffReflect) {
		Ray3D reflect;
		reflect.direction = Reflect(ray.direction.unit(), iInfo.normal.unit());
		reflect.position = iInfo.position + reflect.direction + (iInfo.normal * 0.000001);
		color += iInfo.material->specular * this->getColor(reflect, rDepth - 1, cLimit / iInfo.material->specular);
	}

	//refraction
	int cutOffRefract = 1;
	for (int i = 0; i < 3; i++)
		if (iInfo.material->transparent[i] > cLimit[i])
			cutOffRefract = 0;

	int belowCriticalAngle = 0;
	if (rDepth > 0 && !cutOffRefract) {
		Ray3D refract;
		belowCriticalAngle = Refract(ray.direction.unit(), iInfo.normal.unit(), iInfo.material->ir, refract.direction);
		if (belowCriticalAngle) {
			refract.position = iInfo.position + refract.direction + (iInfo.normal * 0.000001);
			color += iInfo.material->transparent * this->getColor(refract, rDepth - 1, cLimit / iInfo.material->transparent);
		}
	}

	//clamp
	color += iInfo.material->emissive;
	color[0] = clamp(color[0]);
	color[1] = clamp(color[1]);
	color[2] = clamp(color[2]);

	return color;*/

	static int recursiveCalls = 0;
	static Point3D potentialchange = Point3D(1, 1, 1);

	if (recursiveCalls >= rDepth || potentialchange.length() < cLimit.length()) return Point3D(); //using climit length is close enough

	RayShapeIntersectionInfo iInfo = RayShapeIntersectionInfo();
	double dist = this->intersect(ray, iInfo, BoundingBox1D(), std::function< bool(double) >());

	if (dist == Infinity) {
		return Point3D();
	}
	else {
		Point3D rgb = Point3D();
		rgb = iInfo.material->emissive;// +iInfo.material->ambient * (*this).ambient;

		for (auto* light : this->_globalData.lights) {
			Point3D shadowTerm = light->transparency(iInfo, *this, cLimit);
			Point3D diffuse = light->getDiffuse(ray, iInfo);
			Point3D specular = light->getSpecular(ray, iInfo);
			rgb += light->getAmbient(ray, iInfo);
			rgb += (diffuse + specular)*shadowTerm;
			for (int i = 0; i < 3; i++) {
				if (rgb[i] < 0) rgb[i] = 0;
				if (rgb[i] > 1) rgb[i] = 1;
			}
		}

		Point3D reflected = Point3D();
		if (iInfo.normal.dot(ray.direction) < 0) { //only reflect off of outer surfaces (differs from slides, but in line with reference. Intuitionally the inside is not as reflective)
			Point3D reflectDirection = Reflect(ray.direction, iInfo.normal);
			Ray3D reflectRay = Ray3D(iInfo.position + reflectDirection * 0.00001, reflectDirection);
			potentialchange *= iInfo.material->specular;
			recursiveCalls++;
			reflected = iInfo.material->specular * getColor(reflectRay, rDepth, cLimit);
			recursiveCalls--;
			potentialchange /= iInfo.material->specular;
		}
		rgb += reflected;
		for (int i = 0; i < 3; i++) {
			if (rgb[i] < 0) rgb[i] = 0;
			if (rgb[i] > 1) rgb[i] = 1;
		}

		Point3D transparent = Point3D();
		Point3D refractedDirection;
		if (Refract(ray.direction, iInfo.normal, iInfo.material->ir, refractedDirection)) {
			Ray3D refractedRay = Ray3D(iInfo.position + refractedDirection * 0.00001, refractedDirection);
			potentialchange *= iInfo.material->transparent;
			recursiveCalls++;
			transparent = iInfo.material->transparent * getColor(refractedRay, rDepth, cLimit);
			recursiveCalls--;
			potentialchange /= iInfo.material->transparent;
		}

		Point3D antitransparent = Point3D(1, 1, 1) - iInfo.material->transparent;
		antitransparent = Point3D(1, 1, 1); // uncomment for reference image
		rgb = rgb * antitransparent + transparent;

		for (int i = 0; i < 3; i++) {
			if (rgb[i] < 0) rgb[i] = 0;
			if (rgb[i] > 1) rgb[i] = 1;
		}

		return rgb;
	}
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
