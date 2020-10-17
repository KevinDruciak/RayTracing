#include <cmath>
#include <Util/exceptions.h>
#include "scene.h"
#include "spotLight.h"

using namespace Ray;
using namespace Util;

///////////////
// SpotLight //
///////////////
Point3D SpotLight::getAmbient( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	////////////////////////////////////////////////////
	// Get the ambient contribution of the light here //
	////////////////////////////////////////////////////

	/*double angle = this->_direction.unit().dot(ray.direction.unit());
	if (angle > cos(this->_cutOffAngle))
	{
		double distance = (iInfo.position - ray.position).length();
		Point3D intensity = this->_ambient * pow(angle, this->_dropOffRate) / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
		return iInfo.material->ambient * intensity;
	}
	return Point3D();*/

	double strength = ((this->_location - iInfo.position).unit()).dot(iInfo.normal);
	double dist = (this->_location - iInfo.position).length();

	double cos_angle = this->_direction.unit().dot((iInfo.position - this->_location).unit());
	//if (cos_angle <= cos(this->_cutOffAngle))
	//	return Point3D();
	double intensity = pow(cos_angle, this->_dropOffRate) / (this->_constAtten + this->_linearAtten * dist + this->_quadAtten * dist * dist);
	return iInfo.material->ambient * intensity * (strength >= 0 ? strength : 0);


	/*Point3D AttenColor;
	Point3D position = this->_location;
	Point3D v = (iInfo.position - v).unit();
	Point3D d = this->_direction;
	double dist = (iInfo.position - position).length();
	double angle = pow(d.dot(v), this->_dropOffRate);
	if (angle > cos(this->_cutOffAngle)) {
		AttenColor = this->_ambient * angle / (this->_constAtten + this->_linearAtten * dist + this->_quadAtten * pow(dist, 2));
	}
	else {
		AttenColor = Point3D(0, 0, 0);
	}
	Point3D point = AttenColor * iInfo.material->ambient;
	return point;*/
	//return this->_ambient;
}

Point3D SpotLight::getDiffuse( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	////////////////////////////////////////////////////
	// Get the diffuse contribution of the light here //
	////////////////////////////////////////////////////
	/*double angle = this->_direction.unit().dot(ray.direction.unit());
	if (angle > cos(this->_cutOffAngle))
	{
		double distance = (iInfo.position - ray.position).length();
		Point3D intensity = this->_diffuse * pow(angle, this->_dropOffRate) / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
		return iInfo.material->diffuse * intensity * (iInfo.normal.dot(-this->_direction) >= 0 ? iInfo.normal.dot(-this->_direction) : 0);
	}
	return Point3D();*/

	double strength = ((this->_location - iInfo.position).unit()).dot(iInfo.normal);
	double dist = (this->_location - iInfo.position).length();

	double cos_angle = this->_direction.unit().dot((iInfo.position - this->_location).unit());
	if (cos_angle <= cos(this->_cutOffAngle))
		return Point3D();
	double intensity = pow(cos_angle, this->_dropOffRate) / (this->_constAtten + this->_linearAtten * dist + this->_quadAtten * dist * dist);

	return iInfo.material->diffuse * this->_diffuse * intensity * (strength >= 0 ? strength : 0);

	/*Point3D AttenColor;
	Point3D position = this->_location;
	Point3D v = (iInfo.position - v).unit();
	Point3D d = this->_direction;
	double dist = (iInfo.position - position).length();
	double angle = pow(d.dot(v), this->_dropOffRate);
	if (angle > cos(this->_cutOffAngle)) {
		AttenColor = this->_diffuse * angle / (this->_constAtten + this->_linearAtten * dist + this->_quadAtten * pow(dist, 2));
	}
	else {
		AttenColor = Point3D(0, 0, 0);
	}
	Point3D point = AttenColor * iInfo.normal.dot((position - iInfo.position).unit()) * iInfo.material->diffuse;
	return point;*/

}

Point3D SpotLight::getSpecular( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
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

	double spec_cos_angle = toLight.dot(reflection.unit());
	if (spec_cos_angle < 0)
		return Point3D();

	double strength = pow(spec_cos_angle, iInfo.material->specularFallOff);

	double dist = (this->_location - iInfo.position).length();
	double cos_angle = this->_direction.unit().dot((iInfo.position - this->_location).unit());
	if (cos_angle <= cos(this->_cutOffAngle))
		return Point3D();
	double intensity = pow(cos_angle, this->_dropOffRate) / (this->_constAtten + this->_linearAtten * dist + this->_quadAtten * dist * dist);

	return iInfo.material->specular * this->_specular * intensity * strength;

	/*Point3D AttenColor;
	Point3D position = this->_location;
	Point3D v = (iInfo.position - v).unit();
	Point3D d = this->_direction;
	double dist = (iInfo.position - position).length();
	double angle = pow(d.dot(v), this->_dropOffRate);
	if (angle > cos(this->_cutOffAngle)) {
		AttenColor = this->_specular * angle / (this->_constAtten + this->_linearAtten * dist + this->_quadAtten * pow(dist, 2));
	}
	else {
		AttenColor = Point3D(0, 0, 0);
	}

	Point3D viewerVector = (ray.position - iInfo.position).unit();
	double alpha = iInfo.normal.unit().dot(((position - iInfo.position).unit() + viewerVector).unit());
	Point3D point = iInfo.material->specular * pow(alpha, iInfo.material->specularFallOff) * AttenColor;
	return point;*/

}

bool SpotLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape* shape ) const
{
	//////////////////////////////////////////////
	// Determine if the light is in shadow here //
	//////////////////////////////////////////////
	/*RayShapeIntersectionInfo temp; //mac

	return shape->intersect(Ray3D(iInfo.position, (this->_location - iInfo.position).unit())
		, temp, BoundingBox1D(), std::function<bool(double)>()) == INFINITY ? 1 : 0;*/

	Point3D position = this->_location;
	Ray3D ray = Ray3D(iInfo.position + (iInfo.normal * 0.00000001), (position - iInfo.position + (iInfo.normal * 0.00000001)).unit());
	RayShapeIntersectionInfo temp;
	if (shape->intersect(ray, temp, BoundingBox1D(), std::function<bool(double)>()) != INFINITY && shape->intersect(ray, temp, BoundingBox1D(), std::function<bool(double)>()) < (position - iInfo.position + (iInfo.normal * 0.00000001)).length()) {
		return 0;
	}
	return 1;

	/*RayShapeIntersectionInfo inf = RayShapeIntersectionInfo();
	Point3D pointToLight = (this->_location - iInfo.position);

	if (shape->intersect(Ray3D(iInfo.position - pointToLight.unit() * 0.0000001, pointToLight.unit()), inf, BoundingBox1D(), std::function<bool(double)>()) == INFINITY) {
		return 0;
	}
	return 1;*/

}

Point3D SpotLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit ) const
{
	//////////////////////////////////////////////////////////
	// Compute the transparency along the path to the light //
	//////////////////////////////////////////////////////////
	RayShapeIntersectionInfo inf = RayShapeIntersectionInfo();
	Point3D pointToLight = (this->_location - iInfo.position);

	if (shape.intersect(Ray3D(iInfo.position + pointToLight.unit() * .0000001, pointToLight.unit()), inf, BoundingBox1D(), std::function<bool(double)>()) == INFINITY) {
		return Point3D(1, 1, 1);
	}
	Point3D trans = inf.normal.dot(-pointToLight) <= 0 ? Point3D(1, 1, 1) : inf.material->transparent;
	return  trans * transparency(inf, shape, cLimit);
}

void SpotLight::drawOpenGL( int index , GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();
}
