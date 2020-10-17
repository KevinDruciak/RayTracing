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
	/*Point3D Il = this->_diffuse;
	Point3D L = -(this->_direction);

	double lightFacingness = iInfo.normal.dot(L.unit());
	if (lightFacingness < 0) return Point3D();
	if ((iInfo.position - ray.position).dot(iInfo.normal) >= 0) return Point3D(); //either an inside surface or grazing shot
	return iInfo.material->diffuse * lightFacingness * Il;*/
}

Point3D DirectionalLight::getSpecular( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	/////////////////////////////////////////////////////
	// Get the specular contribution of the light here //
	/////////////////////////////////////////////////////
	/*double projection = (iInfo.normal).dot(-this->_direction);
	Point3D reflection = 2.0 * projection * iInfo.normal - (-this->_direction);
	double angle = ray.direction.dot(reflection);
	return iInfo.material->specular * pow(angle, iInfo.material->specularFallOff) * this->_specular;*/
	Point3D Il = this->_specular;
	Point3D L = -(this->_direction);
	Point3D R = (-L - iInfo.normal * (2 * (-L).dot(iInfo.normal))).unit();
	Point3D V = (ray.position - iInfo.position).unit();

	double lightFacingness = iInfo.normal.dot(L);
	if (lightFacingness < 0) return Point3D();
	if ((iInfo.position - ray.position).dot(iInfo.normal) >= 0) return Point3D(); //either an inside surface or grazing shot
	double vr = pow(V.dot(R), iInfo.material->specularFallOff);
	return iInfo.material->specular * Il * vr;
}

bool DirectionalLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape* shape ) const
{
	//////////////////////////////////////////////
	// Determine if the light is in shadow here //
	//////////////////////////////////////////////
	RayShapeIntersectionInfo temp = iInfo; //mac
	return shape->intersect(Ray3D(temp.position + -(this->_direction) * 0.0000001, -(this->_direction))
		, temp, BoundingBox1D(), std::function<bool(double)>()) == INFINITY ? 1 : 0;
}

Point3D DirectionalLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit ) const
{
	//////////////////////////////////////////////////////////
	// Compute the transparency along the path to the light //
	//////////////////////////////////////////////////////////

	/*Ray3D ray = Ray3D(iInfo.position, -this->_direction.unit());
	RayShapeIntersectionInfo tempInfo;
	if (shape.intersect(ray, tempInfo, BoundingBox1D(), std::function<bool(double)>()) > 0)
		return tempInfo.material->transparent;
	return Point3D(1, 1, 1);*/

	/*Point3D transparency;
	Point3D contribution = Point3D(1.0, 1.0, 1.0);
	double distance;
	int count = 0;
	RayShapeIntersectionInfo temp = iInfo;

	while(contribution[0] > cLimit[0] && contribution[1] > cLimit[1] && contribution[2] > cLimit[2])
	{
		//std::cout << count << std::endl;

		Ray3D ray = Ray3D(iInfo.position, -this->_direction.unit());
		distance = shape.intersect(Ray3D(temp.position + -this->_direction * 0.0000000001, -this->_direction)
			, temp, BoundingBox1D(), std::function<bool(double)>());
		//std::cout << distance << std::endl;

		if (distance == INFINITY && count == 0)
		{
			//std::cout << Point3D(1.0, 1.0, 1.0) << std::endl;

			return Point3D(1.0, 1.0, 1.0);
		}
		if (distance != INFINITY)
		{
			Point3D kT = temp.material->transparent;
			contribution = kT;
			transparency += contribution;
			count++;
		} else
		{
			break;
		}
	}
	//std::cout << transparency << std::endl;

	return transparency;*/

	/*if (iInfo.material->transparent[0] > cLimit[0] && iInfo.material->transparent[1] > cLimit[1] && iInfo.material->transparent[2] > cLimit[2]) {
		RayShapeIntersectionInfo temp = iInfo;
		Point3D trans = Point3D(1, 1, 1);

		return isInShadow(temp, &shape) ? iInfo.material->transparent * transparency(iInfo, shape, cLimit / iInfo.material->transparent) : trans;
	}
	else {
		return Point3D(1, 1, 1);
	}*/

	RayShapeIntersectionInfo inf = RayShapeIntersectionInfo();
	if (shape.intersect(Ray3D(iInfo.position + ((-this->_direction) * 0.000001), (-this->_direction)), inf, BoundingBox1D(), std::function<bool(double)>()) == INFINITY) {
		return Point3D(1, 1, 1);
	}
	Point3D trans = inf.normal.dot(this->_direction) <= 0 ? Point3D(1, 1, 1) : inf.material->transparent;
	return  trans * transparency(inf, shape, cLimit);
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
