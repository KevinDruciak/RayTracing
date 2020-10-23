#include <cmath>
#include  <Util/exceptions.h>
#include "scene.h"
#include "box.h"

using namespace Ray;
using namespace Util;

/////////
// Box //
/////////

void Box::init( const LocalSceneData& data )
{
	// Set the material pointer
	if( _materialIndex<0 ) THROW( "negative material index: %d" , _materialIndex );
	else if( _materialIndex>=data.materials.size() ) THROW( "material index out of bounds: %d <= %d" , _materialIndex , (int)data.materials.size() );
	else _material = &data.materials[ _materialIndex ];

	//////////////////////////////////
	// Do any necessary set-up here //
	//////////////////////////////////
	WARN_ONCE( "method undefined" );
}

void Box::updateBoundingBox( void )
{
	///////////////////////////////
	// Set the _bBox object here //
	///////////////////////////////
	THROW( "method undefined" );
}

void Box::initOpenGL( void )
{
	/////////////////////////////////////////
	// Do any necessary OpenGL set-up here //
	/////////////////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

double Box::intersect( Ray3D ray , RayShapeIntersectionInfo &iInfo , BoundingBox1D range , std::function< bool (double) > validityLambda ) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();

	/////////////////////////////////////////////////////////////
	// Compute the intersection of the shape with the ray here //
	/////////////////////////////////////////////////////////////
		//TODO: acceleration

	Point3D* p;
	p[0] = this->center - length / 2;
	p[1] = this->center + length / 2;
	double tmin = -INFINITY, tmax = INFINITY;
	double tx1 = (p[0][0] - ray.position[0])/ray.direction[0];
	double tx2 = (p[1][0] - ray.position[0])/ray.direction[0];

	tmin = std::max(tmin, std::min(tx1, tx2));
	tmax = std::min(tmax, std::max(tx1, tx2));
	double ty1 = (p[0][1] - ray.position[1])/ray.direction[1];
	double ty2 = (p[1][1] - ray.position[1])/ray.direction[1];

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));
	double tz1 = (p[0][2] - ray.position[2])/ray.direction[2];
	double tz2 = (p[1][2] - ray.position[2])/ray.direction[2];

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));
	if (tmax < tmin) {
		return INFINITY;
	}
	Point3D intersection = ray.position + ray.direction.unit() * tmin;
	double dist = (intersection - ray.position).length();
	if (dist != INFINITY) {
		iInfo.position = intersection;
		iInfo.material = this->_material;
		if (iInfo.position[0] == p[0][0]) {
			Point3D pPrime = Point3D(center[0] - length[0], center[1], center[2]);
			iInfo.normal = (pPrime - center).unit();
		}
		if (iInfo.position[0] == p[1][0]) {
			Point3D pPrime = Point3D(center[0] + length[0], center[1], center[2]);
			iInfo.normal = (pPrime - center).unit();
		}
		if (iInfo.position[1] == p[0][1]) {
			Point3D pPrime = Point3D(center[0], center[1] - length[1], center[2]);
			iInfo.normal = (pPrime - center).unit();
		}
		if (iInfo.position[1] == p[1][1]) {
			Point3D pPrime = Point3D(center[0], center[1] + length[1], center[2]);
			iInfo.normal = (pPrime - center).unit();
		}
		if (iInfo.position[2] == p[0][2]) {
			Point3D pPrime = Point3D(center[0], center[1], center[2] - length[2]);
			iInfo.normal = (pPrime - center).unit();
		}
		if (iInfo.position[2] == p[1][2]) {
			Point3D pPrime = Point3D(center[0], center[1], center[2] - length[2]);
			iInfo.normal = (pPrime - center).unit();
		}
		return (intersection - ray.position).length();
	} else {
		return INFINITY;
	}
}

bool Box::isInside( Point3D p ) const
{
	///////////////////////////////////////////////////
	// Determine if the point is inside the box here //
	///////////////////////////////////////////////////
	THROW( "method undefined" );
	return false;
}

void Box::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
