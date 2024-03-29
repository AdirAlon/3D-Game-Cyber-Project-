#pragma once
#include "Vector3D.h"
#include "Vector4D.h"
#include <memory>

class Matrix4x4
{
public:
	Matrix4x4()
	{
	}

	void setIdentity()
	{
		::memset(m_matrix, 0, sizeof(float) * 16);
		m_matrix[0][0] = 1;
		m_matrix[1][1] = 1;
		m_matrix[2][2] = 1;
		m_matrix[3][3] = 1;
	}

	void setTranslation(const Vector3D& translation)
	{
		m_matrix[3][0] = translation.m_x;
		m_matrix[3][1] = translation.m_y;
		m_matrix[3][2] = translation.m_z;
	}

	void setScale(const Vector3D& scale)
	{
		m_matrix[0][0] = scale.m_x;
		m_matrix[1][1] = scale.m_y;
		m_matrix[2][2] = scale.m_z;
	}

	void setRotaionX(float x)
	{
		m_matrix[1][1] = cos(x);
		m_matrix[1][2] = sin(x);
		m_matrix[2][1] = -sin(x);
		m_matrix[2][2] = cos(x);
	}

	void setRotaionY(float y)
	{
		m_matrix[0][0] = cos(y);
		m_matrix[0][2] = -sin(y);
		m_matrix[2][0] = sin(y);
		m_matrix[2][2] = cos(y);
	}

	void setRotaionZ(float z)
	{
		m_matrix[0][0] = cos(z);
		m_matrix[0][1] = sin(z);
		m_matrix[1][0] = -sin(z);
		m_matrix[1][1] = cos(z);
	}

	void operator *=(const Matrix4x4& matrix)
	{
		Matrix4x4 out;
		for (int i = 0; i < 4; i++) 
		{
			for (int j = 0; j < 4; j++)
			{
				out.m_matrix[i][j] = m_matrix[i][0] * matrix.m_matrix[0][j] + m_matrix[i][1] * matrix.m_matrix[1][j] +
					m_matrix[i][2] * matrix.m_matrix[2][j] + m_matrix[i][3] * matrix.m_matrix[3][j];
			}

		}
		::memcpy(m_matrix, out.m_matrix, sizeof(float) * 16);
	}

	void setOrthoLH(float width, float height, float near_plane, float far_plane)
	{
		setIdentity();
		m_matrix[0][0] = 2.0f / width;
		m_matrix[1][1] = 2.0f / height;
		m_matrix[2][2] = 1.0f / (far_plane - near_plane);
		m_matrix[3][2] = -(near_plane / (far_plane - near_plane));
	}

	void setMatrix(const Matrix4x4& matrix)
	{
		::memcpy(m_matrix, matrix.m_matrix, sizeof(float) * 16);
	}

	float getDeterminant()
	{
		Vector4D minor;
		float det;

		Vector4D v1 = Vector4D(this->m_matrix[0][0], this->m_matrix[1][0], this->m_matrix[2][0], this->m_matrix[3][0]);
		Vector4D v2 = Vector4D(this->m_matrix[0][1], this->m_matrix[1][1], this->m_matrix[2][1], this->m_matrix[3][1]);
		Vector4D v3 = Vector4D(this->m_matrix[0][2], this->m_matrix[1][2], this->m_matrix[2][2], this->m_matrix[3][2]);


		minor.cross(v1, v2, v3);
		det = -(this->m_matrix[0][3] * minor.m_x + this->m_matrix[1][3] * minor.m_y + this->m_matrix[2][3] * minor.m_z +
			this->m_matrix[3][3] * minor.m_w);
		return det;
	}

	void inverse()
	{
		int a;
		Matrix4x4 out;
		Vector4D vec[3], v;
		float det = 0.0f;

		det = this->getDeterminant();
		if (!det) return;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i)
						a = a - 1;
					vec[a].m_x = (this->m_matrix[j][0]);
					vec[a].m_y = (this->m_matrix[j][1]);
					vec[a].m_z = (this->m_matrix[j][2]);
					vec[a].m_w = (this->m_matrix[j][3]);

				}
			}
			v.cross(vec[0], vec[1], vec[2]);
			out.m_matrix[0][i] = pow(-1.0f, i) * v.m_x / det;
			out.m_matrix[1][i] = pow(-1.0f, i) * v.m_y / det;
			out.m_matrix[2][i] = pow(-1.0f, i) * v.m_z / det;
			out.m_matrix[3][i] = pow(-1.0f, i) * v.m_w / det;
		}
		this->setMatrix(out);
	}

	void setPerspectiveView(float fov, float aspect, float znear, float zfar)
	{
		float y_scale = 1.0f / tan(fov / 2.0f);
		float x_scale = y_scale / aspect;
		m_matrix[0][0] = x_scale;
		m_matrix[1][1] = y_scale;
		m_matrix[2][2] = zfar / (zfar - znear);
		m_matrix[2][3] = 1.0f;
		m_matrix[3][2] = (-znear * zfar) / (zfar - znear);
	}

	Vector3D getZDirection()
	{
		return Vector3D(m_matrix[2][0], 0, m_matrix[2][2]);
	}

	Vector3D getXDirection()
	{
		return Vector3D(m_matrix[0][0], m_matrix[0][1], m_matrix[0][2]);
	}

	Vector3D getYDirection()
	{
		return Vector3D(m_matrix[1][0], m_matrix[1][1], m_matrix[1][2]);
	}

	Vector3D getTranslation()
	{
		return Vector3D(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);
	}

	~Matrix4x4()
	{
	}
public:
	float m_matrix[4][4] = {};
}; 
