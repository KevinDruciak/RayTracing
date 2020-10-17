#include <cmath>
#include <Util/exceptions.h>
#include "pointLight.h"
#include "scene.h"

using namespace Ray;
using namespace Util;

////////////////
// PointLight //
////////////////

Point3D PointLight::getAmbient( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	////////////////////////////////////////////////////
	// Get the ambient contribution of the light here //
	////////////////////////////////////////////////////
	//std::cout << "p: " << ray.position << " i: " << iInfo.position << std::endl;
	const double distance = (iInfo.position - ray.position).length();
	return Point3D(this->_ambient * iInfo.material->ambient / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2)));
	//Point3D position = this->_location;
	//double distance = (position - iInfo.position).length();
	//Point3D AttenColor = this->_ambient / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
	//Point3D point = AttenColor * iInfo.material->ambient;
	//return point;
	//return Point3D(this->_ambient * iInfo.material->ambient / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2)));

}

Point3D PointLight::getDiffuse( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	////////////////////////////////////////////////////
	// Get the diffuse contribution of the light here //
	////////////////////////////////////////////////////
	/*const double distance = (iInfo.position - ray.position).length();
	const Point3D intensity = this->_diffuse / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
	return iInfo.normal.dot(-this->_location) * iInfo.material->diffuse * intensity;*/
	/*double strength = ((this->_location - iInfo.position).unit()).dot(iInfo.normal);
	double dist = (this->_location - iInfo.position).length();
	double intensity = 1 / (this->_constAtten + this->_linearAtten * dist + this->_quadAtten * dist * dist);
	return iInfo.material->diffuse * this->_diffuse * intensity * (strength >= 0 ? strength : 0);*/
	Point3D position = this->_location;
	double dist = (position - iInfo.position).length();
	Point3D AttenColor = this->_diffuse / (this->_constAtten + this->_linearAtten * dist + this->_quadAtten * pow(dist, 2));
	Point3D point = AttenColor * iInfo.normal.dot((position - iInfo.position).unit()) * iInfo.material->diffuse;
	return point;
}

Point3D PointLight::getSpecular( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	/////////////////////////////////////////////////////
	// Get the specular contribution of the light here //
	/////////////////////////////////////////////////////
	/*double projection = (iInfo.normal).dot(ray.position - iInfo.position);
	Point3D reflection = 2.0 * projection * iInfo.normal - (ray.position - iInfo.position);
	double angle = ray.direction.dot(reflection);
	double distance = (this->_location - iInfo.position).length();
	double intensity = 1.0 / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
	return iInfo.material->specular * pow(angle, iInfo.material->specularFallOff) * intensity * this->_specular;*/

	Point3D toLight = (this->_location - iInfo.position).unit();
	Point3D toCamera = (ray.position - iInfo.position).unit();
	double projection = iInfo.normal.dot(toCamera);
	Point3D reflection = iInfo.normal * projection + -(toCamera - iInfo.normal * projection);

	double cos_angle = toLight.dot(reflection.unit());
	if (cos_angle < 0)
		return Point3D();

	double strength = pow(cos_angle, iInfo.material->specularFallOff);

	double dist = (this->_location - iInfo.position).length();
	double intensity = 1 / (this->_constAtten + this->_linearAtten * dist + this->_quadAtten * dist * dist);

	return iInfo.material->specular * this->_specular * intensity * strength;

	/*Point3D position = this->_location;
	double dist = (position - iInfo.position).length();
	Point3D AttenColor = this->_specular / (this->_constAtten + this->_linearAtten * dist + this->_quadAtten * pow(dist, 2));
	Point3D viewerVector = (ray.position - iInfo.position).unit();
	double alpha = iInfo.normal.unit().dot(((position - iInfo.position).unit() + viewerVector).unit());
	Point3D point = iInfo.material->specular * pow(alpha, iInfo.material->specularFallOff) * AttenColor;
	return point;*/
}

bool PointLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape* shape ) const
{
	//////////////////////////////////////////////
	// Determine if the light is in shadow here //
	//////////////////////////////////////////////
	//	RayShapeIntersectionInfo temp; //mac

	//return shape->intersect(Ray3D(iInfo.position, (this->_location - iInfo.position).unit())
	//	, temp, BoundingBox1D(), std::function<bool(double)>()) == INFINITY ? 1 : 0;
	//RayShapeIntersectionInfo temp; //mac
	//return shape->intersect(Ray3D(iInfo.position + -this->_direction * 0.0000000001, -this->_direction)
	//	, temp, BoundingBox1D(), std::function<bool(double)>()) == INFINITY ? 1 : 0;
	//RayShapeIntersectionInfo temp; //mac
	//return shape->intersect(Ray3D(iInfo.position * 0.0000000001, (this->_location - iInfo.position).unit())
	//	, temp, BoundingBox1D(), std::function<bool(double)>()) == INFINITY ? 1 : 0;

	/*Point3D L = (_location - iInfo.position).unit();
	double length = (_location - iInfo.position).length();
	Ray3D iRay = Ray3D(iInfo.position, L);
	RayShapeIntersectionInfo temp;
	double t = shape->intersect(iRay, temp, BoundingBox1D(), std::function<bool(double)>());
	return (t == INFINITY) ? 1 : 0;*/
	Point3D position = this->_location;
	Ray3D ray = Ray3D(iInfo.position , (position - iInfo.position).unit());
	RayShapeIntersectionInfo temp;
	if (shape->intersect(ray, temp, BoundingBox1D(), std::function<bool(double)>()) != INFINITY && shape->intersect(ray, temp, BoundingBox1D(), std::function<bool(double)>()) < (position - iInfo.position + (iInfo.normal * 0.00000001)).length()) {
		return 0;
	}
	return 1;
}

Point3D PointLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit ) const
{
	//////////////////////////////////////////////////////////
	// Compute the transparency along the path to the light //
	//////////////////////////////////////////////////////////
	RayShapeIntersectionInfo inf = RayShapeIntersectionInfo();
	Point3D pointToLight = (this->_location - iInfo.position);

	if (shape.intersect(Ray3D(iInfo.position + pointToLight.unit() * 0.0001, pointToLight.unit()), inf, BoundingBox1D(), std::function<bool(double)>()) == INFINITY) {
		return Point3D(1, 1, 1);
	}
	Point3D trans = inf.normal.dot(-pointToLight) <= 0 ? Point3D(1, 1, 1) : inf.material->transparent;
	return  trans * transparency(inf, shape, cLimit);
}

void PointLight::drawOpenGL( int index , GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();
}

