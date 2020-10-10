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
	double projection = (iInfo.normal).dot(-this->_direction);
	Point3D reflection = 2.0 * projection * iInfo.normal - (-this->_direction);
	double angle = ray.direction.dot(reflection);
	return iInfo.material->specular * pow(angle, iInfo.material->specularFallOff) * this->_specular;
}

bool DirectionalLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape* shape ) const
{
	//////////////////////////////////////////////
	// Determine if the light is in shadow here //
	//////////////////////////////////////////////
	return shape->intersect(Ray3D(iInfo.position + -this->_direction * 0.0000000001, -this->_direction)
		, RayShapeIntersectionInfo(), BoundingBox1D(), std::function<bool(double)>()) == INFINITY ? 1 : 0;
}

Point3D DirectionalLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit ) const
{
	//////////////////////////////////////////////////////////
	// Compute the transparency along the path to the light //
	//////////////////////////////////////////////////////////
	THROW( "method undefined" );
	return Point3D( 1. , 1. , 1. );
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
