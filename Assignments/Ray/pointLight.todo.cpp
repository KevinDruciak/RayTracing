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
	const double distance = (iInfo.position - ray.position).length();
	return Point3D(this->_ambient * iInfo.material->ambient / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2)));
}

Point3D PointLight::getDiffuse( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	////////////////////////////////////////////////////
	// Get the diffuse contribution of the light here //
	////////////////////////////////////////////////////
	const double distance = (this->_location - iInfo.position).length();
	Point3D color = this->_diffuse / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
	return color * iInfo.normal.dot((this->_location - iInfo.position).unit()) * iInfo.material->diffuse;
}

Point3D PointLight::getSpecular( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	/////////////////////////////////////////////////////
	// Get the specular contribution of the light here //
	/////////////////////////////////////////////////////
	Point3D L = (this->_location - iInfo.position).unit();
	Point3D V = (ray.position - iInfo.position).unit();
	Point3D R = iInfo.normal * iInfo.normal.dot(V) + -(V - iInfo.normal * iInfo.normal.dot(V));
	double theta = L.dot(R.unit());
	if (theta < 0) return Point3D();
	double distance = (this->_location - iInfo.position).length();
	double I = pow(theta, iInfo.material->specularFallOff) / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
	return iInfo.material->specular * I * this->_specular;
}

bool PointLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape* shape ) const
{
	//////////////////////////////////////////////
	// Determine if the light is in shadow here //
	//////////////////////////////////////////////
	Point3D L = (this->_location - iInfo.position).unit();
	RayShapeIntersectionInfo temp = iInfo; //apple
	return shape->intersect(Ray3D(iInfo.position + L * EPSILON, L), temp, BoundingBox1D(), std::function<bool(double)>()) == INFINITY ? true : false;
}

Point3D PointLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit ) const
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

void PointLight::drawOpenGL( int index , GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();
}

