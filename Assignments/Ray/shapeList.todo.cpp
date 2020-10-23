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
	double minimum = INFINITY;
	for (auto* shape : this->shapes)
	{
		auto temp = RayShapeIntersectionInfo();
		auto dist = shape->intersect(ray, temp, range, validityLambda);
		minimum = std::min(dist, minimum);
		if (dist == minimum) iInfo = temp;
	}
	return minimum;


	//TODO: acceleration

	//Ray3D rayPrime = Ray3D(this->getInverseMatrix().multPosition(ray.position), (this->getInverseMatrix().multDirection(ray.direction)).unit());
	//loop through shapes
	
	
	/*std::vector<ShapeBoundingBoxHit> *hits;

	for (auto* shape : this->shapes) {
		BoundingBox1D temp;
		if (!shape->boundingBox().isEmpty()) {
			temp = shape->boundingBox().intersect(ray);
		} else {
			continue;
		}
		//double dist1 = (temp[1] - temp[0]).length();
			std::cout << "got here 1" << std::endl;

		double dist1 = Point1D::Distance(temp[1], temp[0]);
					std::cout << "got here 2" << std::endl;

		if ((dist1 >= 0) && (dist1 != INFINITY)) {
			ShapeBoundingBoxHit temp;
			temp.shape = shape;
			temp.t = dist1;
			hits->push_back(temp);
		}
	}
		std::cout << "got here 3" << std::endl;


	std::sort(hits->begin(), hits->end(), ShapeBoundingBoxHit::Compare);
	//qsort(hits->begin(), totShapeHit, sizeof(ShapeBoundingBoxHit), ShapeBoundingBoxHit::Compare);
	std::cout << "got here 4" << std::endl;
	double minimum = INFINITY;

	for (auto hit : *hits) {
		auto temp = RayShapeIntersectionInfo();
		auto dist = hit.shape->intersect(ray, temp, range, validityLambda);
		minimum = std::min(dist, minimum);
		if (dist == minimum) iInfo = temp;
	}
	return minimum;*/
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
	//WARN_ONCE( "method undefined" );
	//hits->clear();
	//hits->shrink_to_fit();
	//hits->reserve(shapes.size());
	//hits->clear();
}

void ShapeList::updateBoundingBox( void )
{
	///////////////////////////////
	// Set the _bBox object here //
	///////////////////////////////
	BoundingBox3D min;
	for (auto shape : shapes) {
		min += shape->boundingBox();
	}
	this->_bBox = min;
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

	//double result = INFINITY;
	RayShapeIntersectionInfo temp;

	Ray3D local = this->getInverseMatrix() * ray;

	double ratio = ray.direction.length() / local.direction.length();

	local.direction = local.direction.unit();

	double distance = _shape->intersect(local, temp, BoundingBox1D(), std::function< bool(double) >()) * ratio;
	//if (rayDist >= 0 && !(rayDist == INFINITY))
	//{
	//	result = rayDist;
	//}

	iInfo.position = this->getMatrix() * temp.position;
	iInfo.material = temp.material;
	iInfo.normal = (this->getNormalMatrix() * temp.normal).unit();
	iInfo.texture = temp.texture;

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
	//TODO: acceleration
	//this->_bBox = this->getMatrix() * _shape->boundingBox();
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