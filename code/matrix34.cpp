////////////////////////////////////////
// matrix34.cpp
////////////////////////////////////////

#include "matrix34.h"

////////////////////////////////////////////////////////////////////////////////

Matrix34 Matrix34::IDENTITY(1.0f,0.0f,0.0f,0.0f,
			    0.0f,1.0f,0.0f,0.0f,
			    0.0f,0.0f,1.0f,0.0f);

////////////////////////////////////////////////////////////////////////////////

Matrix34::Matrix34(float ax,float bx,float cx,float dx,
		   float ay,float by,float cy,float dy,
		   float az,float bz,float cz,float dz) {
	a.Set(ax,ay,az);
	b.Set(bx,by,bz);
	c.Set(cx,cy,cz);
	d.Set(dx,dy,dz);

	pad0=pad1=pad2=0.0f;
	pad3=1.0f;
}

////////////////////////////////////////////////////////////////////////////////

void Matrix34::Identity() {
	a.Set(1.0f,0.0f,0.0f);
	b.Set(0.0f,1.0f,0.0f);
	c.Set(0.0f,0.0f,1.0f);
	d.Set(0.0f,0.0f,0.0f);
}

////////////////////////////////////////////////////////////////////////////////
// Dot
////////////////////////////////////////////////////////////////////////////////

void Matrix34::Dot(const Matrix34 &n,const Matrix34 &m) {
	float ax = m.a.x*n.a.x + m.a.y*n.b.x + m.a.z*n.c.x;
	float ay = m.a.x*n.a.y + m.a.y*n.b.y + m.a.z*n.c.y;
	float az = m.a.x*n.a.z + m.a.y*n.b.z + m.a.z*n.c.z;

	float bx = m.b.x*n.a.x + m.b.y*n.b.x + m.b.z*n.c.x;
	float by = m.b.x*n.a.y + m.b.y*n.b.y + m.b.z*n.c.y;
	float bz = m.b.x*n.a.z + m.b.y*n.b.z + m.b.z*n.c.z;

	float cx = m.c.x*n.a.x + m.c.y*n.b.x + m.c.z*n.c.x;
	float cy = m.c.x*n.a.y + m.c.y*n.b.y + m.c.z*n.c.y;
	float cz = m.c.x*n.a.z + m.c.y*n.b.z + m.c.z*n.c.z;

	float dx = m.d.x*n.a.x + m.d.y*n.b.x + m.d.z*n.c.x + n.d.x;
	float dy = m.d.x*n.a.y + m.d.y*n.b.y + m.d.z*n.c.y + n.d.y;
	float dz = m.d.x*n.a.z + m.d.y*n.b.z + m.d.z*n.c.z + n.d.z;

	a.Set(ax,ay,az);
	b.Set(bx,by,bz);
	c.Set(cx,cy,cz);
	d.Set(dx,dy,dz);
}

////////////////////////////////////////////////////////////////////////////////
// Transform
////////////////////////////////////////////////////////////////////////////////

void Matrix34::Transform(const Vector3 &in,Vector3 &out) const {
	float x=in.x*a.x + in.y*b.x + in.z*c.x + d.x;
	float y=in.x*a.y + in.y*b.y + in.z*c.y + d.y;
	float z=in.x*a.z + in.y*b.z + in.z*c.z + d.z;
	out.Set(x,y,z);
}

////////////////////////////////////////////////////////////////////////////////

void Matrix34::Transform3x3(const Vector3 &in,Vector3 &out) const {
	float x=in.x*a.x + in.y*b.x + in.z*c.x;
	float y=in.x*a.y + in.y*b.y + in.z*c.y;
	float z=in.x*a.z + in.y*b.z + in.z*c.z;
	out.Set(x,y,z);
}

////////////////////////////////////////////////////////////////////////////////
// MakeRotate
////////////////////////////////////////////////////////////////////////////////

void Matrix34::MakeRotateX(float t) {
	float cost=cosf(t);
	float sint=sinf(t);

	a.Set(1.0f, 0.0f, 0.0f);
	b.Set(0.0f, cost, sint);
	c.Set(0.0f,-sint, cost);
	d.Zero();
}

////////////////////////////////////////////////////////////////////////////////

void Matrix34::MakeRotateY(float t) {
	float cost=cosf(t);
	float sint=sinf(t);

	a.Set(cost, 0.0f,-sint);
	b.Set(0.0f, 1.0f, 0.0f);
	c.Set(sint, 0.0f, cost);
	d.Zero();
}

////////////////////////////////////////////////////////////////////////////////

void Matrix34::MakeRotateZ(float t) {
	float cost=cosf(t);
	float sint=sinf(t);

	a.Set( cost, sint, 0.0f);
	b.Set(-sint, cost, 0.0f);
	c.Set( 0.0f, 0.0f, 1.0f);
	d.Zero();
}

////////////////////////////////////////////////////////////////////////////////

void Matrix34::MakeRotateUnitAxis(const Vector3 &v,float t) {
	float cost=cosf(t);
	float sint=sinf(t);
	float omc=1.0f-cost;

	a.x=omc*v.x*v.x+cost;
	b.y=omc*v.y*v.y+cost;
	c.z=omc*v.z*v.z+cost;
	a.y=omc*v.x*v.y+sint*v.z;
	b.x=omc*v.x*v.y-sint*v.z;
	a.z=omc*v.x*v.z-sint*v.y;
	c.x=omc*v.x*v.z+sint*v.y;
	b.z=omc*v.y*v.z+sint*v.x;
	c.y=omc*v.y*v.z-sint*v.x;
	d.Zero();
}

////////////////////////////////////////////////////////////////////////////////
// Euler angles
////////////////////////////////////////////////////////////////////////////////

void Matrix34::FromEulers(float ang1, float ang2, float ang3, int order) {
	float sx=sinf(ang1),cx=cosf(ang1);
	float sy=sinf(ang2),cy=cosf(ang2);
	float sz=sinf(ang3),cz=cosf(ang3);

	switch(order) {
		case EULER_XYZ:
			a.Set(cy*cz,          cy*sz,         -sy);
			b.Set(sx*sy*cz-cx*sz, sx*sy*sz+cx*cz, sx*cy);
			c.Set(cx*sy*cz+sx*sz, cx*sy*sz-sx*cz, cx*cy);
			break;
		case EULER_XZY:
			a.Set( cz*cy,           sz,    -cz*sy);
			b.Set(-cx*sz*cy+sx*sy,  cx*cz,  cx*sz*sy+sx*cy);
			c.Set( sx*sz*cy+cx*sy, -sx*cz, -sx*sz*sy+cx*cy);
			break;
		case EULER_YXZ:
			a.Set( cy*cz-sy*sx*sz, cy*sz+sy*sx*cz, -sy*cx);
			b.Set(-cx*sz,          cx*cz,           sx);
			c.Set( sy*cz+cy*sx*sz, sy*sz-cy*sx*cz,  cy*cx); 
			break;
		case EULER_YZX:
			a.Set(cy*cz, cy*sz*cx+sy*sx, cy*sz*sx-sy*cx);
			b.Set(  -sz, cz*cx,          cz*sx);
			c.Set(sy*cz, sy*sz*cx-cy*sx, sy*sz*sx+cy*cx);
			break;
		case EULER_ZXY:
			a.Set( cz*cy+sz*sx*sy, sz*cx, -cz*sy+sz*sx*cy);
			b.Set(-sz*cy+cz*sx*sy, cz*cx,  sz*sy+cz*sx*cy);
			c.Set( cx*sy,         -sx,     cx*cy);
			break;
		case EULER_ZYX:
			a.Set( cz*cy, sz*cx+cz*sy*sx, sz*sx-cz*sy*cx);
			b.Set(-sz*cy, cz*cx-sz*sy*sx, cz*sx+sz*sy*cx);
			c.Set( sy,   -cy*sx,          cy*cx);
			break;
		default:
			printf("ERROR: Matrix34::FromEulers()- Bad order specified\n");
			break;
	}

	d.Zero();
}

////////////////////////////////////////////////////////////////////////////////

void Matrix34::ToEulers(float* ang1, float* ang2, float* ang3,int order) {
	switch(order) {
		case EULER_XYZ:
			*ang1 = atan2f(b.z,c.z);
			*ang2 = asinf(-a.z);
			*ang3 = atan2f(a.y,a.x);
			break;
		case EULER_XZY:
			*ang1 = atan2f(-c.y,b.y);
			*ang2 = atan2f(-a.z,a.x);
			*ang3 = asinf(a.y);
			break;
		case EULER_YXZ:
			*ang1 = asinf(b.z);
			*ang2 = atan2f(-a.z,c.z);
			*ang3 = atan2f(-b.x,b.y);
			break;
		case EULER_YZX:
			*ang1 = atan2f(b.z,b.y);
			*ang2 = atan2f(c.x,a.x);
			*ang3 = asinf(-b.x);
			break;
		case EULER_ZXY:
			*ang1 = asinf(-c.y);
			*ang2 = atan2f(c.x,c.z);
			*ang3 = atan2f(a.y,b.y);
			break;
		case EULER_ZYX:
			*ang1 = atan2f(-c.y,c.z);
			*ang2 = asinf(c.x);
			*ang3 = atan2f(-b.x,a.x);
			break;
		default:
			printf("ERROR: Matrix34::ToEulers()- Bad order specified\n");
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Scale
////////////////////////////////////////////////////////////////////////////////

void Matrix34::MakeScale(float x,float y,float z) {
	a.Set(x,0.0f,0.0f);
	b.Set(0.0f,y,0.0f);
	c.Set(0.0f,0.0f,z);
	d.Zero();
}

////////////////////////////////////////////////////////////////////////////////
// Inverse, Transpose
////////////////////////////////////////////////////////////////////////////////

inline float Det22(float ax,float ay,float bx,float by) {return ax*by-ay*bx;}

bool Matrix34::Inverse() {
	float subx, suby, subz;
	subx=Det22( b.y, b.z, c.y, c.z );
	suby=Det22( b.x, b.z, c.x, c.z );
	subz=Det22( b.x, b.y, c.x, c.y );

	float Det = a.x*subx - a.y*suby + a.z*subz; // Determinant
	if(Det==0.0f) {
		printf("ERROR: Matrix34::Inverse()- Matrix not invertible\n");
		Identity();
		return false;
	}
	Det=1.0f/Det;		// Inverse of the determinant

	Matrix34 Temp;
	Temp.a.x = subx*Det;
	Temp.b.x = -suby*Det;
	Temp.c.x = subz*Det;
	Temp.d.x = - (d.x*Temp.a.x + d.y*Temp.b.x + d.z*Temp.c.x);

	subx = Det22( a.y, a.z, c.y, c.z );
	suby = Det22( a.x, a.z, c.x, c.z );
	subz = Det22( a.x, a.y, c.x, c.y );

	Temp.a.y = -subx*Det;
	Temp.b.y = suby*Det;
	Temp.c.y = -subz*Det;
	Temp.d.y = - (d.x*Temp.a.y + d.y*Temp.b.y + d.z*Temp.c.y);

	subx = Det22( a.y, a.z, b.y, b.z );
	suby = Det22( a.x, a.z, b.x, b.z );
	subz = Det22( a.x, a.y, b.x, b.y );

	Temp.a.z = subx*Det;
	Temp.b.z = -suby*Det;
	Temp.c.z = subz*Det;
	Temp.d.z = - (d.x*Temp.a.z + d.y*Temp.b.z + d.z*Temp.c.z);

	(*this)=Temp;
	return true;
}

////////////////////////////////////////////////////////////////////////////////

void Matrix34::FastInverse() {
	Matrix34 tmp;

	tmp.a.Set(a.x,b.x,c.x);
	tmp.b.Set(a.y,b.y,c.y);
	tmp.c.Set(a.z,b.z,c.z);
	tmp.d.x=-d.Dot(a);
	tmp.d.y=-d.Dot(b);
	tmp.d.z=-d.Dot(c);

	(*this)=tmp;
}

////////////////////////////////////////////////////////////////////////////////

void Matrix34::Transpose() {
	float t;
	t=b.x; b.x=a.y; a.y=t;
	t=c.x; c.x=a.z; a.z=t;
	t=c.y; c.y=b.z; b.z=t;
}

////////////////////////////////////////////////////////////////////////////////
// Viewing
////////////////////////////////////////////////////////////////////////////////

void Matrix34::LookAt(const Vector3 &from,const Vector3 &to) {
	d=from;
	c = d - to;
	c.Normalize();
	a.Set (c.z,0.0f,-c.x);
	a.Normalize();			// BUG: this can blow up
	b.Cross(c,a);
}

////////////////////////////////////////////////////////////////////////////////

void Matrix34::PolarView(float dist,float azm,float inc,float twst) {
	FromEulers(-inc,azm,twst,EULER_ZXY);
	d = c;
	d *= dist;
}

////////////////////////////////////////////////////////////////////////////////
// Misc functions
////////////////////////////////////////////////////////////////////////////////

float Matrix34::Determinant3x3() const {
	return a.x*b.y*c.z+a.y*b.z*c.x+a.z*b.x*c.y-a.x*b.z*c.y-a.y*b.x*c.z-a.z*b.y*c.x;
}

////////////////////////////////////////////////////////////////////////////////

void Matrix34::Print(const char *s) const {
	if(s) printf("%s={\n",s);
	else printf("matrix={\n");
	a.Print();
	b.Print();
	c.Print();
	d.Print();
	printf("}\n");
}

////////////////////////////////////////////////////////////////////////////////
void Matrix34::setEqual(Matrix34 mat){
    a.Set(mat.a.x, mat.a.y, mat.a.z);
     b.Set(mat.b.x, mat.b.y, mat.b.z);
     c.Set(mat.c.x, mat.c.y, mat.c.z);
     d.Set(mat.d.x, mat.d.y, mat.d.z);
}
