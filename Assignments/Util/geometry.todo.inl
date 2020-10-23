/*
Copyright (c) 2019, Michael Kazhdan
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer. Redistributions in binary form must reproduce
the above copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the distribution. 

Neither the name of the Johns Hopkins University nor the names of its contributors
may be used to endorse or promote products derived from this software without specific
prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/

namespace Util
{
	////////////
	// Matrix //
	////////////
	template< unsigned int Dim >
	Matrix< Dim > Matrix< Dim >::Exp( const Matrix &m , int terms )
	{
		//////////////////////////////////////
		// Compute the matrix exponent here //
		//////////////////////////////////////
		THROW( "method undefined" );
		return Matrix();
	}

	template< unsigned int Dim >
	Matrix< Dim > Matrix< Dim >::closestRotation( void ) const
	{
		///////////////////////////////////////
		// Compute the closest rotation here //
		///////////////////////////////////////
		THROW( "method undefined" );
		return Matrix();
	}

	/////////////////
	// BoundingBox //
	/////////////////
	template< unsigned int Dim >
	BoundingBox< 1 > BoundingBox< Dim >::intersect( const Ray< Dim > &ray ) const
	{
		///////////////////////////////////////////////////////////////
		// Compute the intersection of a BoundingBox with a Ray here //
		///////////////////////////////////////////////////////////////
		//Point3D* p;
		//p[0] = this->center - length / 2;
		//p[1] = this->center + length / 2;
		//TODO: acceleration

		double tmin = -INFINITY, tmax = INFINITY;

		for (int i = 0; i < Dim; i++) {
			//std::cout << "calleddim" << std::endl;
			double t1 = (_p[0][i] - ray.position[i])/ray.direction[i];
			double t2 = (_p[1][i] - ray.position[i])/ray.direction[i];
			tmin = std::max(tmin, std::min(t1, t2));
			tmax = std::min(tmax, std::max(t1, t2));
		}
			//std::cout << " " << std::endl;

		//if (tmin <= 0 && tmax >= 0) {
		//	std::cout << "1" << std::endl;
		//	return BoundingBox<1>();
		//}
 		//if (tmax >= fmin(tmin, 0.0)) {
			 std::cout << "2" << std::endl;
			 BoundingBox1D temp;// = new BoundingBox<1>();
			 temp[0] = Point1D(tmin);
			 temp[1] = Point1D(tmax);//(ray.position + (ray.direction * tmin)).Length();
			 return temp;
			 //eturn (ray.direction * tmin).length();
			//return BoundingBox<1>((ray.direction * tmin).length());
			//return (ray.direction * tmin);
		//} else {
		//	std::cout << "3" << std::endl;
		//	return BoundingBox<1>();
		//}
	}
}