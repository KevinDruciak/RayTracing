#include <Util/exceptions.h>
#include "shapeList.h"
#include "triangle.h"

using namespace Ray;
using namespace Util;

////////////////
// Difference //
////////////////
void Difference::updateBoundingBox( void )
{
	///////////////////////////////
	// Set the _bBox object here //
	///////////////////////////////
	THROW( "method undefined" );
}

double Difference::intersect( Util::Ray3D ray , class RayShapeIntersectionInfo &iInfo , BoundingBox1D range , std::function< bool (double) > validityLambda ) const
{
	//////////////////////////////////////////////////////////////////
	// Compute the intersection of the difference with the ray here //
	//////////////////////////////////////////////////////////////////
	THROW( "method undefined" );
	return Infinity;
}

bool Difference::isInside( Util::Point3D p ) const
{
	//////////////////////////////////////////////////////////
	// Determine if the point is inside the difference here //
	//////////////////////////////////////////////////////////
	THROW( "method undefined" );
	return false;
}

///////////////
// ShapeList //
///////////////
double ShapeList::intersect( Ray3D ray , RayShapeIntersectionInfo &iInfo , BoundingBox1D range , std::function< bool (double) > validityLambda ) const
{
	//////////////////////////////////////////////////////////////////
	// Compute the intersection of the shape list with the ray here //
	//////////////////////////////////////////////////////////////////
	
	double distance = INFINITY;
	RayShapeIntersectionInfo info;
	std::vector<ShapeBoundingBoxHit> hits;

	for (auto shape : this->shapes) {
		BoundingBox1D bound = shape->boundingBox().intersect(ray);
		if (bound.isEmpty()) continue;
		hits.push_back(ShapeBoundingBoxHit{bound[0][0], shape});
	}
	std::sort(hits.begin(), hits.end(), ShapeBoundingBoxHit::Compare);
	for (auto hit : hits) {
		double t = hit.shape->intersect(ray, info, range, validityLambda);
		if (t < distance) {
			distance = t;
			iInfo = info;
			break;
		}
	}
	return distance;
}

bool ShapeList::isInside( Point3D p ) const
{
	//////////////////////////////////////////////////////////
	// Determine if the point is inside the shape list here //
	//////////////////////////////////////////////////////////
	THROW( "method undefined" );
	return false;
}

void ShapeList::init( const LocalSceneData &data )
{
	// Initialize the children
	for( int i=0 ; i<shapes.size() ; i++ ) shapes[i]->init( data );

	///////////////////////////////////
	// Do any additional set-up here //
	///////////////////////////////////
}

void ShapeList::updateBoundingBox( void )
{
	///////////////////////////////
	// Set the _bBox object here //
	///////////////////////////////
	this->_bBox = BoundingBox3D();
	for (auto shape : shapes) {
		shape->updateBoundingBox();
		this->_bBox += shape->boundingBox();
	}
}

void ShapeList::initOpenGL( void )
{
	// Initialize the children
	for( int i=0 ; i<shapes.size() ; i++ ) shapes[i]->initOpenGL();

	///////////////////////////
	// Do OpenGL set-up here //
	///////////////////////////
	WARN_ONCE( "remainder of method undefined" );
}

void ShapeList::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();
}

/////////////////
// AffineShape //
/////////////////
double AffineShape::intersect( Ray3D ray , RayShapeIntersectionInfo &iInfo , BoundingBox1D range , std::function< bool (double) > validityLambda ) const
{
	//////////////////////////////////////////////////////////////////////////////////////
	// Compute the intersection of the difference with the affinely deformed shape here //
	//////////////////////////////////////////////////////////////////////////////////////
	RayShapeIntersectionInfo info;
	Ray3D local = this->getInverseMatrix() * ray;
	double ratio = ray.direction.length() / local.direction.length();
	local.direction = local.direction.unit();

	double distance = _shape->intersect(local, info, BoundingBox1D(), std::function< bool(double) >()) * ratio;
	if (distance == INFINITY) return INFINITY;

	iInfo.position = this->getMatrix() * info.position;
	iInfo.material = info.material;
	iInfo.normal = (this->getNormalMatrix() * info.normal).unit();
	iInfo.texture = info.texture;
	return distance;
}

bool AffineShape::isInside( Point3D p ) const
{
	///////////////////////////////////////////////////////////////////////
	// Determine if the point is inside the affinely deformed shape here //
	///////////////////////////////////////////////////////////////////////
	THROW( "method undefined" );
	return _shape->isInside( p );
}

void AffineShape::updateBoundingBox( void )
{
	///////////////////////////////
	// Set the _bBox object here //
	///////////////////////////////
	_shape->updateBoundingBox();
	this->_bBox = this->getMatrix() * _shape->boundingBox();
}

void AffineShape::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	WARN_ONCE( "method undefined" );
	_shape->drawOpenGL( glslProgram );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();
}

//////////////////
// TriangleList //
//////////////////
double TriangleList::intersect( Ray3D ray , RayShapeIntersectionInfo &iInfo , BoundingBox1D range , std::function< bool (double) > validityLambda ) const
{
	////////////////////////////////////////////////////////////////////////////
	// Compute the intersection of the difference with the triangle list here //
	////////////////////////////////////////////////////////////////////////////
	auto temp = RayShapeIntersectionInfo();
	const double intersection = this->_shapeList.intersect(ray, temp, range, validityLambda);
	if (intersection != INFINITY) {
		iInfo.material = this->_material;
		iInfo.normal = temp.normal;
		iInfo.position = temp.position;
		iInfo.texture = temp.texture;
	}
	return intersection;
}

void TriangleList::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();
}

void TriangleList::init( const LocalSceneData &data )
{
	// Set the vertex and material pointers
	_vertices = &data.vertices[0];
	_vNum = (unsigned int)data.vertices.size();
	if( _materialIndex>=data.materials.size() ) THROW( "shape specifies a material that is out of bounds: %d <= %d" , _materialIndex , (int)data.materials.size() );
	else if( _materialIndex<0 ) THROW( "negative material index: %d" , _materialIndex );
	else _material = &data.materials[ _materialIndex ];

	_shapeList.init( data );

	///////////////////////////////////
	// Do any additional set-up here //
	///////////////////////////////////
	//WARN_ONCE( "method undefined" );
}

void TriangleList::initOpenGL( void )
{
	_shapeList.initOpenGL();

	///////////////////////////
	// Do OpenGL set-up here //
	///////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();
}

///////////
// Union //
///////////
double Union::intersect( Ray3D ray , RayShapeIntersectionInfo &iInfo , BoundingBox1D range , std::function< bool (double) > validityLambda ) const
{
	/////////////////////////////////////////////////////////////
	// Compute the intersection of the union with the ray here //
	/////////////////////////////////////////////////////////////
	THROW( "method undefined" );
	return Infinity;
}

void Union::init( const LocalSceneData &data )
{
	_shapeList.init( data );

	///////////////////////////////////
	// Do any additional set-up here //
	///////////////////////////////////
	WARN_ONCE( "method undefined" );
}

void Union::updateBoundingBox( void )
{
	///////////////////////////////
	// Set the _bBox object here //
	///////////////////////////////
	THROW( "method undefined" );
}

bool Union::isInside( Point3D p ) const
{
	/////////////////////////////////////////////////////
	// Determine if the point is inside the union here //
	/////////////////////////////////////////////////////
	THROW( "method undefined" );
	return false;
}

//////////////////
// Intersection //
//////////////////
double Intersection::intersect( Ray3D ray , RayShapeIntersectionInfo &iInfo , BoundingBox1D range , std::function< bool (double) > validityLambda ) const
{
	/////////////////////////////////////////////////////////////////////////////////////
	// Compute the intersection of the difference with the intersection of shapes here //
	/////////////////////////////////////////////////////////////////////////////////////
	THROW( "method undefined" );
	return -1;
}

void Intersection::init( const LocalSceneData &data )
{
	_shapeList.init( data );

	///////////////////////////////////
	// Do any additional set-up here //
	///////////////////////////////////
	WARN_ONCE( "method undefined" );
}

void Intersection::updateBoundingBox( void )
{
	///////////////////////////////
	// Set the _bBox object here //
	///////////////////////////////
	THROW( "method undefined" );
}

bool Intersection::isInside( Point3D p ) const
{
	///////////////////////////////////////////////////////////////////////
	// Determine if the point is inside the instersection of shapes here //
	///////////////////////////////////////////////////////////////////////
	THROW( "method undefined" );
	return false;
}