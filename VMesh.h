#pragma once
#include "Vector2D.h"
#include "Vector3D.h"

class VMesh
{
public:
	VMesh() :m_pos(), m_t_cords(), m_normal()
	{
	}

	VMesh(Vector3D pos, Vector2D t_cords, Vector3D normal) :m_pos(pos), m_t_cords(t_cords), m_normal(normal)
	{
	}

	VMesh(const VMesh& vertex) :m_pos(vertex.m_pos), m_t_cords(vertex.m_t_cords), m_normal(vertex.m_normal)
	{
	}

	~VMesh()
	{
	}
public:
	Vector3D m_pos;
	Vector2D m_t_cords;
	Vector3D m_normal;
};