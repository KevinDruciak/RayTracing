#include <cmath>
#include <Util/exceptions.h>
#include "camera.h"
#include "shape.h"

using namespace Ray;
using namespace Util;

////////////
// Camera //
////////////

Ray3D Camera::getRay( int i , int j , int width , int height ) const
{
	/////////////////////////////////////////////////
	// Get the ray through the (i,j)-th pixel here //
	/////////////////////////////////////////////////
	double angle = tan(this->heightAngle / 2.0);
	double ar = static_cast<double>(width) / static_cast<double>(height);

	Point3D x = 2.0 * (this->right) * (i + 0.5) / width * ar * angle;
	Point3D y = 2.0 * (this->up) * (j + 0.5) / height * angle;

	Point3D dir = (this->forward - angle * (this->up + this->right * ar) + x + y).unit();
	return Ray3D(this->position, dir);
}

Ray3D Camera::getRayJitter( double i , double j , int width , int height ) const
{
	/////////////////////////////////////////////////
	// Get the ray through the (i,j)-th pixel here //
	/////////////////////////////////////////////////
	double angle = tan(this->heightAngle / 2.0);
	double ar = static_cast<double>(width) / static_cast<double>(height);

	Point3D x = 2.0 * (this->right) * (i + 0.5) / width * ar * angle;
	Point3D y = 2.0 * (this->up) * (j + 0.5) / height * angle;

	Point3D dir = (this->forward - angle * (this->up + this->right * ar) + x + y).unit();
	return Ray3D(this->position, dir);
}

void Camera::drawOpenGL( void ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

void Camera::rotateUp( Point3D center , float angle )
{
	///////////////////////////////////////////////////
	// Rotate the camera about the up direction here //
	///////////////////////////////////////////////////
	THROW( "method undefined" );
}

void Camera::rotateRight( Point3D center , float angle )
{
	//////////////////////////////////////////////////////
	// Rotate the camera about the right direction here //
	//////////////////////////////////////////////////////
	THROW( "method undefined" );
}

void Camera::moveForward( float dist )
{
	//////////////////////////////////
	// Move the camera forward here //
	//////////////////////////////////
	THROW( "method undefined" );
}

void Camera::moveRight( float dist )
{
	///////////////////////////////////////
	// Move the camera to the right here //
	///////////////////////////////////////
	THROW( "method undefined" );
}

void Camera::moveUp( float dist )
{
	/////////////////////////////
	// Move the camera up here //
	/////////////////////////////
	THROW( "method undefined" );
}
