#include <cmath>
#include <Util/exceptions.h>
#include "scene.h"
#include "sphere.h"

using namespace Ray;
using namespace Util;

////////////
// Sphere //
////////////

void Sphere::init( const LocalSceneData &data )
{
	// Set the material pointer
	if( _materialIndex<0 ) THROW( "negative material index: %d" , _materialIndex );
	else if( _materialIndex>=data.materials.size() ) THROW( "material index out of bounds: %d <= %d" , _materialIndex , (int)data.materials.size() );
	else _material = &data.materials[ _materialIndex ];

	///////////////////////////////////
	// Do any additional set-up here //
	///////////////////////////////////
	//WARN_ONCE( "method undefined" );
}
void Sphere::updateBoundingBox( void )
{
	///////////////////////////////
	// Set the _bBox object here //
	///////////////////////////////
		//TODO: acceleration

	double xmax = this->center[0] + radius;
	double ymax = this->center[1] + radius;
	double zmax = this->center[2] + radius;
	double xmin = this->center[0] - radius;
	double ymin = this->center[1] - radius;
	double zmin = this->center[2] - radius;
	this->_bBox[0] = Point3D(xmin, ymin, zmin);
	this->_bBox[1] = Point3D(xmax, ymax, zmax);
}
void Sphere::initOpenGL( void )
{
	///////////////////////////
	// Do OpenGL set-up here //
	///////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

double Sphere::intersect( Ray3D ray , RayShapeIntersectionInfo &iInfo , BoundingBox1D range , std::function< bool (double) > validityLambda ) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();

	//////////////////////////////////////////////////////////////
	// Compute the intersection of the sphere with the ray here //
	//////////////////////////////////////////////////////////////
	Polynomial1D<2> equation;
	equation.coefficient(0) = (ray.position - this->center).squareNorm() - pow(this->radius, 2);
	equation.coefficient(1) = 2.0 * ray.direction.dot(ray.position - this->center);
	equation.coefficient(2) = 1.0;

	auto *roots = new double[2];
	const auto numRoots = equation.roots(roots);

	if (numRoots == 0) return INFINITY;

	const auto distance = (roots[0] >= 0 && roots[1] < 0) || (roots[0] < 0 && roots[1] >= 0)
		? std::max(roots[0], roots[1]) : (roots[0] >= 0 && roots[1] >= 0)
		? std::min(roots[0], roots[1]) : INFINITY;

	if (distance == INFINITY) return INFINITY;

	iInfo.position = ray(distance);
	iInfo.normal = (iInfo.position - this->center).unit();
	iInfo.material = this->_material;
	//Point3D intersection = ray.position + ray.direction * distance;
	//iInfo.position = intersection;
	//iInfo.normal = (intersection - this->center).unit();
	//iInfo.material = this->_material;

	// Texture mapping
	/*float theta = atan2(-(intersection[2] - this->center[2]), intersection[0] - this->center[0]);
	float u = (theta + Pi) / (2.0 * Pi);
	float gamma = acos(-(intersection[1] - this->center[1]) / this->radius);
	float v = gamma / Pi;

	iInfo.texture[0] = u;
	iInfo.texture[1] = v;*/

	return distance;
}

bool Sphere::isInside( Point3D p ) const
{
	//////////////////////////////////////////////////////
	// Determine if the point is inside the sphere here //
	//////////////////////////////////////////////////////
	THROW( "method undefined" );
	return false;
}

void Sphere::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
