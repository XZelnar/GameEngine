#include "mathUtils.h"

void MathUtils::QuaternionToEuler(const hkQuaternion& _quat, D3DXVECTOR3& euler )
{
	double x = _quat.m_vec.getComponent(0);
	double y = _quat.m_vec.getComponent(1);
	double z = _quat.m_vec.getComponent(2);
	double w = _quat.m_vec.getComponent(3);
	double test = x*y + z*w;
	if (test > 0.499) 
	{ // singularity at north pole
		euler.y = 2 * atan2(x,w);
		euler.z = D3DX_PI/2;
		euler.x = 0;
		return;
	}
	if (test < -0.499) 
	{ // singularity at south pole
		euler.y = -2 * atan2(x,w);
		euler.z = - D3DX_PI/2;
		euler.x = 0;
		return;
	}
    double sqx = x*x;
    double sqy = y*y;
    double sqz = z*z;
    euler.y = atan2(2*y*w-2*x*z , 1 - 2*sqy - 2*sqz);
	euler.z = asin(2*test);
	euler.x = atan2(2*x*w-2*y*z , 1 - 2*sqx - 2*sqz);
}

hkQuaternion& MathUtils::EulerToQuaternion(const D3DXVECTOR3& euler, hkQuaternion& quat) 
{
	double c1 = cos(euler.y/2.0);
	double s1 = sin(euler.y/2.0);
	double c2 = cos(euler.z/2.0);
	double s2 = sin(euler.z/2.0);
	double c3 = cos(euler.x/2.0);
	double s3 = sin(euler.x/2.0);
	double c1c2 = c1*c2;
	double s1s2 = s1*s2;
	double w =c1c2*c3 - s1s2*s3;
  	double x =c1c2*s3 + s1s2*c3;
	double y =s1*c2*c3 + c1*s2*s3;
	double z =c1*s2*c3 - s1*c2*s3;

	quat.set(x, y, z, w);
	return quat;
 }

D3DXVECTOR3& MathUtils::AnyPerpendicularVector(const D3DXVECTOR3& in, D3DXVECTOR3& out)
{
	if (in.x == in.y == in.z == 0)
		return out;

	if (in.x == 0 && in.y == 0)
	{
		out = D3DXVECTOR3(1, 0, 0);
		return out;
	}
	if (in.x == 0 && in.z == 0)
	{
		out = D3DXVECTOR3(1, 0, 0);
		return out;
	}
	if (in.y == 0 && in.z == 0)
	{
		out = D3DXVECTOR3(0, 1, 0);
		return out;
	}


	if (in.x != 0 && in.y != 0)
	{
		out.z = 0;
		out.x = 1;
		out.y = in.x / in.y;
		return out;
	}
	else if (in.x != 0)
	{
		out.y = 0;
		out.x = 1;
		out.z = in.x / in.z;
		return out;
	}
	else
	{
		out.x = 0;
		out.y = 1;
		out.z = in.y / in.z;
		return out;
	}
}

D3DXVECTOR3& MathUtils::VectorCrossProduct(const D3DXVECTOR3& in1, const D3DXVECTOR3& in2, D3DXVECTOR3& out)
{
	out.x = in1.y * in2.z - in1.z * in2.y;
	out.y = in1.z * in2.x - in1.x * in2.z;
	out.z = in1.x * in2.y - in1.y * in2.x;
	return out;
}

D3DXVECTOR3& MathUtils::VectorToEuler(const D3DXVECTOR3& in, D3DXVECTOR3& out)
{
	//out.x = atan2(sqrt(in.y * in.y + in.z * in.z), in.x);
	//out.y = atan2(sqrt(in.z * in.z + in.x * in.x), in.y);
	//out.z = atan2(sqrt(in.x * in.x + in.y * in.y), in.z);
	if (in.y == 0 && in.z == 0)
		out.x = 0;
	else
		out.x = atan2(in.y, in.z);

	if (in.z == 0 && in.x == 0)
		out.y = 0;
	else
		out.y = atan2(-in.z, in.x);

	if (in.x == 0 && in.y == 0)
		out.z = 0;
	else
		out.z = -atan2(in.y, in.x) - D3DX_PI / 2;
	return out;
}