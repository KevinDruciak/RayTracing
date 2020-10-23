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
	double angle = this->_direction.unit().dot((iInfo.position - this->_location).unit());
	if (angle <= cos(this->_cutOffAngle)) return Point3D();
	double I = (this->_location - iInfo.position).unit().dot(iInfo.normal);
	double distance = (this->_location - iInfo.position).length();
	double intensity = pow(angle, this->_dropOffRate) / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
	return iInfo.material->ambient * intensity * I;
}

Point3D SpotLight::getDiffuse( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	////////////////////////////////////////////////////
	// Get the diffuse contribution of the light here //
	////////////////////////////////////////////////////
	double angle = this->_direction.unit().dot((iInfo.position - this->_location).unit());
	if (angle <= cos(this->_cutOffAngle)) return Point3D();
	double I = (this->_location - iInfo.position).unit().dot(iInfo.normal);
	double distance = (this->_location - iInfo.position).length();
	double intensity = pow(angle, this->_dropOffRate) / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
	return iInfo.material->diffuse * this->_diffuse * intensity * I;
}

Point3D SpotLight::getSpecular( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	/////////////////////////////////////////////////////
	// Get the specular contribution of the light here //
	/////////////////////////////////////////////////////
	Point3D L = (this->_location - iInfo.position).unit();
	Point3D V = (ray.position - iInfo.position).unit();
	Point3D R = iInfo.normal * iInfo.normal.dot(V) + -(V - iInfo.normal * iInfo.normal.dot(V));
	double theta = L.dot(R);
	if (theta < 0) return Point3D();
	double I = pow(theta, iInfo.material->specularFallOff);
	double angle = this->_direction.unit().dot((iInfo.position - this->_location).unit());
	if (angle <= cos(this->_cutOffAngle)) return Point3D();
	double distance = (this->_location - iInfo.position).length();
	double intensity = pow(angle, this->_dropOffRate) / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
	return iInfo.material->specular * this->_specular * intensity * I;
}

bool SpotLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape* shape ) const
{
	//////////////////////////////////////////////
	// Determine if the light is in shadow here //
	//////////////////////////////////////////////
	Point3D L = (this->_location - iInfo.position).unit();
	RayShapeIntersectionInfo temp = iInfo; //apple
	return shape->intersect(Ray3D(iInfo.position + L * EPSILON, L), temp, BoundingBox1D(), std::function<bool(double)>()) == INFINITY ? true : false;
}

Point3D SpotLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit ) const
{
	//////////////////////////////////////////////////////////
	// Compute the transparency along the path to the light //
	//////////////////////////////////////////////////////////
	//apple
	Point3D one; one[0] = 1; one[1] = 1; one[2] = 1;
	RayShapeIntersectionInfo temp;

	Point3D L = (this->_location - iInfo.position);
	if (shape.intersect(Ray3D(iInfo.position + L.unit() * EPSILON, L.unit()), temp, BoundingBox1D(), std::function<bool(double)>()) == INFINITY) return one;
	return temp.normal.dot(-L) <= 0 ? one : temp.material->transparent * transparency(temp, shape, cLimit);
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
