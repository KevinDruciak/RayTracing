#include <cmath>
#include <Util/exceptions.h>
#include "directionalLight.h"
#include "scene.h"

using namespace Ray;
using namespace Util;

//////////////////////
// DirectionalLight //
//////////////////////

Point3D DirectionalLight::getAmbient( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	////////////////////////////////////////////////////
	// Get the ambient contribution of the light here //
	////////////////////////////////////////////////////
	return iInfo.material->ambient * this->_ambient;
}

Point3D DirectionalLight::getDiffuse( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	////////////////////////////////////////////////////
	// Get the diffuse contribution of the light here //
	////////////////////////////////////////////////////
	return iInfo.material->diffuse * (iInfo.normal).dot(-this->_direction) * this->_diffuse;
}

Point3D DirectionalLight::getSpecular( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	/////////////////////////////////////////////////////
	// Get the specular contribution of the light here //
	/////////////////////////////////////////////////////
	if (iInfo.normal.dot(-this->_direction) < 0 || (iInfo.position - ray.position).dot(iInfo.normal) >= 0) return Point3D();
	Point3D R = (this->_direction - iInfo.normal * 2.0 * (this->_direction.dot(iInfo.normal))).unit();
	Point3D V = (ray.position - iInfo.position).unit();
	return iInfo.material->specular * pow(V.dot(R), iInfo.material->specularFallOff) * this->_specular;
}

bool DirectionalLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape* shape ) const
{
	//////////////////////////////////////////////
	// Determine if the light is in shadow here //
	//////////////////////////////////////////////
	RayShapeIntersectionInfo temp;
	return shape->intersect(Ray3D(iInfo.position + -(this->_direction) * EPSILON, -(this->_direction))
		, temp, BoundingBox1D(), std::function<bool(double)>()) == INFINITY ? true : false;
}

Point3D DirectionalLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit , double radius) const
{
	//////////////////////////////////////////////////////////
	// Compute the transparency along the path to the light //
	//////////////////////////////////////////////////////////
	Point3D one; one[0] = 1; one[1] = 1; one[2] = 1;
    RayShapeIntersectionInfo info;
	if (shape.intersect(Ray3D(iInfo.position + -this->_direction.unit() * EPSILON, -this->_direction.unit()), info, BoundingBox1D(), std::function<bool(double)>()) == INFINITY) return one;
	return info.normal.dot(this->_direction) <= 0 ? one : info.material->transparent * transparency(info, shape, cLimit, radius);
}

void DirectionalLight::drawOpenGL( int index , GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();
}
