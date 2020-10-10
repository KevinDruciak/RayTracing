#include <cmath>
#include <Util/exceptions.h>
#include "scene.h"

using namespace Ray;
using namespace Util;
double clamp(double color)
{
	return std::max(0.0, std::min(1.0, color));
}
///////////
// Scene //
///////////
Point3D Scene::Reflect( Point3D v , Point3D n )
{
	//////////////////
	// Reflect here //
	//////////////////
	v = -v.unit();
	n = n.unit();

	Point3D projected = n * v.dot(n);

	return (projected - (v - projected)).unit();
}

bool Scene::Refract( Point3D v , Point3D n , double ir , Point3D& refract )
{
	//////////////////
	// Refract here //
	//////////////////
	THROW( "method undefined" );
	return false;
}

Point3D Scene::getColor( Ray3D ray , int rDepth , Point3D cLimit )
{
	////////////////////////////////////////////////
	// Get the color associated with the ray here //
	////////////////////////////////////////////////
		//TODO: modify maybe?
	RayShapeIntersectionInfo iInfo;
	const double result = this->intersect(ray, iInfo, BoundingBox1D(), std::function< bool(double) >());
	if (result == Infinity) {
		return Point3D();
	}

	Point3D color;
	for (auto* light : (*this)._globalData.lights)
	{
		color += light->getAmbient(ray, iInfo);
		color += light->getDiffuse(ray, iInfo) * light->isInShadow(iInfo, this);
		color += light->getSpecular(ray, iInfo) * light->isInShadow(iInfo, this);
	}

	color += iInfo.material->emissive;
	color[0] = clamp(color[0]);
	color[1] = clamp(color[1]);
	color[2] = clamp(color[2]);

	return color;
}

//////////////
// Material //
//////////////
void Material::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

/////////////
// Texture //
/////////////
void Texture::initOpenGL( void )
{
	///////////////////////////////////
	// Do OpenGL texture set-up here //
	///////////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
