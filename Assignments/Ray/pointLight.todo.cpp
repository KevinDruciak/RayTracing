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
	const double distance = (iInfo.position - ray.position).length();
	const Point3D intensity = this->_diffuse / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
	return iInfo.normal.dot(-this->_location) * iInfo.material->diffuse * intensity;
}

Point3D PointLight::getSpecular( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	/////////////////////////////////////////////////////
	// Get the specular contribution of the light here //
	/////////////////////////////////////////////////////
	double projection = (iInfo.normal).dot(ray.position - iInfo.position);
	Point3D reflection = 2.0 * projection * iInfo.normal - (ray.position - iInfo.position);
	double angle = ray.direction.dot(reflection);
	double distance = (this->_location - iInfo.position).length();
	double intensity = 1.0 / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
	return iInfo.material->specular * pow(angle, iInfo.material->specularFallOff) * intensity * this->_specular;
}

bool PointLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape* shape ) const
{
	//////////////////////////////////////////////
	// Determine if the light is in shadow here //
	//////////////////////////////////////////////
	return shape->intersect(Ray3D(iInfo.position, (this->_location - iInfo.position).unit())
		, RayShapeIntersectionInfo(), BoundingBox1D(), std::function<bool(double)>()) == INFINITY ? 1 : 0;
}

Point3D PointLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit ) const
{
	//////////////////////////////////////////////////////////
	// Compute the transparency along the path to the light //
	//////////////////////////////////////////////////////////
	THROW( "method undefined" );
	return Point3D( 1. , 1. , 1. );
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

