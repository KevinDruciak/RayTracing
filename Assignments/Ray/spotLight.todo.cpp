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
	double angle = this->_direction.unit().dot(ray.direction.unit());
	if (angle > cos(this->_cutOffAngle))
	{
		double distance = (iInfo.position - ray.position).length();
		Point3D intensity = this->_ambient * pow(angle, this->_dropOffRate) / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
		return iInfo.material->ambient * intensity;
	}
	return Point3D();
}

Point3D SpotLight::getDiffuse( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	////////////////////////////////////////////////////
	// Get the diffuse contribution of the light here //
	////////////////////////////////////////////////////
	double angle = this->_direction.unit().dot(ray.direction.unit());
	if (angle > cos(this->_cutOffAngle))
	{
		double distance = (iInfo.position - ray.position).length();
		Point3D intensity = this->_diffuse * pow(angle, this->_dropOffRate) / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
		return iInfo.material->diffuse * intensity * (iInfo.normal.dot(-this->_direction) >= 0 ? iInfo.normal.dot(-this->_direction) : 0);
	}
	return Point3D();
}

Point3D SpotLight::getSpecular( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
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

bool SpotLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape* shape ) const
{
	//////////////////////////////////////////////
	// Determine if the light is in shadow here //
	//////////////////////////////////////////////
	RayShapeIntersectionInfo temp; //mac

	return shape->intersect(Ray3D(iInfo.position, (this->_location - iInfo.position).unit())
		, temp, BoundingBox1D(), std::function<bool(double)>()) == INFINITY ? 1 : 0;
}

Point3D SpotLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit ) const
{
	//////////////////////////////////////////////////////////
	// Compute the transparency along the path to the light //
	//////////////////////////////////////////////////////////
	THROW( "method undefined" );
	return Point3D( 1. , 1. , 1. );
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
