
#include "std.h"
#include "volume.h"

Poly Volume::clip( Poly p,int side ){
	for( int k=0;k<size();++k ){
		if( !p.valid() ) return p;
		p=p.clip( plane(k),side );
	}
	return p;
}

void Volume::antiPenumbra( Poly src,Poly dst,Volume &out ){

	float sgn=1;

	for( int q=0;q<2;++q ){

		int src_sz=src.size(),dst_sz=dst.size();

		for( int k=0;k<src_sz;++k ){

			const Vector &v0=src.vertex(k).coords;
			const Vector &v1=src.vertex((k+1)%src_sz).coords;
			if( fabs(v0.distance(v1))<=PLANAR_EPSILON ) continue;

			Line edge( v0,v1-v0 );

			for( int j=0;j<dst_sz;++j ){

				const Vector &t0=dst.vertex(j).coords;

				if( edge.distance( t0 )<=PLANAR_EPSILON ) continue;

				Plane plane( v0,t0,v1 );

				int n;
				for( n=0;n<dst_sz;++n ){
					float d=plane.distance( dst.vertex(n).coords );
					if( fabs(d)<=PLANAR_EPSILON ) continue;
					if( d*sgn<0 ) break;
				}

				if( n==dst_sz ) out.push_back( plane );
			}
		}

		sgn=-1;
		std::swap(src,dst);
	}
}