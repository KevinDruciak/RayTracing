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
	v = -v.unit();
	n = n.unit();

	Point3D projected = n * v.dot(n);

	return (projected - (v - projected)).unit();
}

bool Scene::Refract( Point3D v , Point3D n , double ir , Point3D& refract )
{
	//////////////////
	// Refract here //
	//////////////////
	/*
	Point3D incidence = -v.unit();
	Point3D normal = n.unit();
	double sin_incidence = n.CrossProduct(v).length() / ir;

	if (sin_incidence > 1) return false;
	double theta_reflected = asin(sin_incidence);

	refract = normal * ((1 / ir) * incidence.dot(normal) - cos(theta_reflected)) - incidence * (1 / ir);
	return true;*/
	return true;
}

Point3D Scene::getColor( Ray3D ray , int rDepth , Point3D cLimit )
{
	////////////////////////////////////////////////
	// Get the color associated with the ray here //
	////////////////////////////////////////////////

	RayShapeIntersectionInfo iInfo;
	const double result = this->intersect(ray, iInfo, BoundingBox1D(), std::function< bool(double) >());
	if (result == Infinity) {
		return Point3D();
	}

	/*Point3D color;
	for (auto* light : (*this)._globalData.lights)
	{
		color += light->getAmbient(ray, iInfo);
		color += light->getDiffuse(ray, iInfo) * light->isInShadow(iInfo, this);
		color += light->getSpecular(ray, iInfo) * light->isInShadow(iInfo, this);
	}

	color += iInfo.material->emissive;
	color[0] = clamp(color[0]);
	color[1] = clamp(color[1]);
	color[2] = clamp(color[2]);

	return color;*/
	// Rays "pass through" the backsides of shapes 
	/*if (ray.direction.dot(iInfo.normal) > 0) {
		ray.position = iInfo.position + ray.direction * 0.00001;
		return getColor(ray, rDepth, cLimit);
	}

	rDepth--;
	Point3D color = iInfo.material->ambient + iInfo.material->emissive;

	// Add reflection
	int cutOffReflect = 1;
	for (int i = 0; i < 3; i++)
		if (iInfo.material->specular[i] > cLimit[i])
			cutOffReflect = 0;


	if (rDepth > 0 && !cutOffReflect) {
		Ray3D reflect;
		reflect.direction = Reflect(ray.direction.unit(), iInfo.normal.unit());
		reflect.position = iInfo.position + reflect.direction * 0.00001;
		color += (Point3D(1,1,1) - iInfo.material->transparent) * iInfo.material->specular * this->getColor(reflect, rDepth, cLimit / iInfo.material->specular);
	}

	for (auto* light : (*this)._globalData.lights) {
		//RayLight& l = *this->lights[i];
		
		Point3D t = Point3D(1,1,1);
		for (int j = 0; j < this->primitiveNum(); j++) {
			t *= light->transparency(iInfo, group->shapes[j], cLimit / iInfo.material->transparent);
		}

		if (iInfo.material->tex == NULL) {
			color += t * l.getDiffuse(camera->position, iInfo);
		}
		else {
			Pixel texVal = Pixel(iInfo.material->tex->img->BilinearSample(iInfo.texCoordinate[0], iInfo.texCoordinate[1]));
			color += t * Point3D(texVal.r, texVal.g, texVal.b);
		}
		color += t * l.getSpecular(camera->position, iInfo);

	}

	for (int i = 0; i < 3; i++) {
		if (color[i] < 0) color[i] = 0;
		else if (color[i] > 1) color[i] = 1;
	}

	return color;*/
/*
	// Add refraction
	// int cutOffRefract = 1;
	// for (int i = 0; i < 3; i++)
	// 	if (iInfo.material->transparent[i] > cLimit[i])
	// 		cutOffRefract = 0;

	// int belowCriticalAngle = 0;
	// if (rDepth > 0 && !cutOffRefract) {
	// 	Ray3D refract;
	// 	belowCriticalAngle = Refract(ray.direction.unit(), iInfo.normal.unit(), iInfo.material->ir, refract.direction);
	// 	if (belowCriticalAngle) {
	// 		refract.position = iInfo.position + refract.direction * 0.00001;
	// 		color += iInfo.material->transparent * this->getColor(refract, rDepth, cLimit / iInfo.material->transparent);
	// 	}
	// }*/
	if (ray.direction.dot(iInfo.normal) > 0) {
		ray.position = iInfo.position + ray.direction * 0.00001;
		return getColor(ray, rDepth, cLimit);
	}

	rDepth--;
	Point3D color = iInfo.material->ambient + iInfo.material->emissive;

	// Add reflection
	int cutOffReflect = 1;
	for (int i = 0; i < 3; i++)
		if (iInfo.material->specular[i] > cLimit[i])
			cutOffReflect = 0;


	if (rDepth > 0 && !cutOffReflect) {
		Ray3D reflect;
		reflect.direction = Reflect(ray.direction.unit(), iInfo.normal.unit());
		reflect.position = iInfo.position + reflect.direction * 0.00001;
		color += iInfo.material->specular * this->getColor(reflect, rDepth, cLimit / iInfo.material->specular);
	}
	for (auto* light : (*this)._globalData.lights) {

	//for (int i = 0; i < this->lightNum; i++) {
	//	RayLight& l = *this->lights[i];

		//Point3D t = Point3D(1,1,1);
		//for (int j = 0; j < group->shapeNum(); j++) {
		//	t *= l.transparency(iInfo, group->shapes[j], cLimit / iInfo.material->transparent);
		//}
		//color += light->getAmbient(ray, iInfo);

		//if (iInfo.material->tex == NULL) {
		color += light->getDiffuse(ray, iInfo) * light->isInShadow(iInfo, this);
		//}
		//else {
		//	Pixel texVal = Pixel(iInfo.material->tex->img->BilinearSample(iInfo.texCoordinate[0], iInfo.texCoordinate[1]));
		//	color += t * Point3D(texVal.r, texVal.g, texVal.b);
		//}
		//color += light->getAmbient(ray, iInfo);
		color += light->getSpecular(ray, iInfo) * light->isInShadow(iInfo, this);

	}

	for (int i = 0; i < 3; i++) {
		if (color[i] < 0) color[i] = 0;
		else if (color[i] > 1) color[i] = 1;
	}
	//color += iInfo.material->emissive;
	//color[0] = clamp(color[0]);
	//color[1] = clamp(color[1]);
	//color[2] = clamp(color[2]);

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
