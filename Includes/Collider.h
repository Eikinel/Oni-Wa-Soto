#pragma once

/* A collider is a shape (rectangle, sphere, etc.) contained by an entity.
** When a collision occure, effects will vary depending on what the entity in cause should do. */

#include "Constants.h"

enum				eColliderType
{
	HEAD,
	ARM,
	BODY,
	FOOT,
	CASE,
	OTHER
};

template <typename T>
class					ICollider
{
public:
	T&					getShape()
	{
		return (this->_shape);
	}

	const eColliderType	getType() const
	{
		return (this->_type);
	}

protected:
	T				_shape;
	eColliderType	_type;
};

class				BoxCollider : public ICollider<sf::RectangleShape>
{
public:
	BoxCollider(const sf::RectangleShape& rect, const eColliderType type = OTHER)
	{
		std::cout << "Add box collider at position [" << rect.getGlobalBounds().left << " ; " << rect.getGlobalBounds().top << "] and size [" << rect.getGlobalBounds().width << " ; " << rect.getGlobalBounds().height << "]." << std::endl;
		this->_shape = rect;
		this->_type = type;
	};

	BoxCollider(const sf::FloatRect& rect, const eColliderType type = OTHER)
	{
		std::cout << "Add box collider at position [" << rect.left << " ; " << rect.top << "] and size [" << rect.width << " ; " << rect.height << "]." << std::endl;
		this->_shape.setPosition(sf::Vector2f(rect.left, rect.top));
		this->_shape.setSize(sf::Vector2f(rect.width, rect.height));
		this->_type = type;
	};

	BoxCollider(float x, float y, int width, int height, const eColliderType type = OTHER)
	{
		std::cout << "Add box collider at position [" << x << " ; " << y << "] and size [" << width << " ; " << height << "]." << std::endl;
		this->_shape.setPosition(sf::Vector2f(x, y));
		this->_shape.setSize(sf::Vector2f(width, height));
		this->_type = type;
	}

	template <typename U> BoxCollider(const sf::Vector2<U>& pos, const sf::Vector2<U>& size, const eColliderType type = OTHER)
	{
		std::cout << "Add box collider at position [" << pos.x << " ; " << pos.y << "] and size [" << size.x << " ; " << size.y << "]." << std::endl;
		this->_shape.setPosition(pos);
		this->_shape.setSize(size);
		this->_type = type;
	}

	virtual ~BoxCollider() {}
};

class			ParallelogramCollider : public ICollider<sf::ConvexShape>
{
public:
	ParallelogramCollider(const sf::ConvexShape& other, const eColliderType type = OTHER)
	{
		sf::FloatRect	box = other.getGlobalBounds();

		std::cout << "Add parallelogram collider at position [" << box.left << " ; " << box.top << "] and size [" << box.width << " ; " << box.height << "]." << std::endl;
		this->_shape.setPointCount(4);
		this->_shape.setPoint(0, toIsometric(sf::Vector2f(box.left, box.top)));
		this->_shape.setPoint(1, toIsometric(sf::Vector2f(box.left + box.width, box.top)));
		this->_shape.setPoint(2, toIsometric(sf::Vector2f(box.left + box.width, box.top + box.height)));
		this->_shape.setPoint(3, toIsometric(sf::Vector2f(box.left, box.top + box.height)));
		this->_type = type;
	};

	ParallelogramCollider(float x, float y, int width, int height, const eColliderType type = OTHER)
	{
		std::cout << "Add parallelogram collider at position [" << x << " ; " << y << "] and size [" << width << " ; " << height << "]." << std::endl;
		this->_shape.setPointCount(4);
		this->_shape.setPoint(0, toIsometric(sf::Vector2f(x, y)));
		this->_shape.setPoint(1, toIsometric(sf::Vector2f(x + width, y)));
		this->_shape.setPoint(2, toIsometric(sf::Vector2f(x + width, y + height)));
		this->_shape.setPoint(3, toIsometric(sf::Vector2f(x, y + height)));
		this->_shape.setFillColor(sf::Color::Transparent);
		this->_type = type;
	};

	template <typename U> ParallelogramCollider(const sf::Vector2<U>& pos, const sf::Vector2<U>& size, const eColliderType type = OTHER)
	{
		std::cout << "Add parallelogram collider at position [" << pos.x << " ; " << pos.y << "] and size [" << size.x << " ; " << size.y << "]." << std::endl;
		this->_shape.setPointCount(4);
		this->_shape.setPoint(0, toIsometric(sf::Vector2f(pos.x, pos.y)));
		this->_shape.setPoint(1, toIsometric(sf::Vector2f(pos.x + size.x, pos.y)));
		this->_shape.setPoint(2, toIsometric(sf::Vector2f(pos.x + size.x, pos.y + size.y)));
		this->_shape.setPoint(3, toIsometric(sf::Vector2f(pos.x, pos.y + size.y)));
		this->_shape.setFillColor(sf::Color::Transparent);
		this->_type = type;
	}

	virtual bool	contains(const sf::Vector2f& point)
	{
		sf::FloatRect	box = this->_shape.getGlobalBounds();

		if (point.x < box.left || point.x > box.left + box.width ||
			point.y < box.top || point.y > box.top + box.height)
			return (false);

		int				ct = 0;
		sf::Vector2f	A(this->_shape.getPoint(0));
		sf::Vector2f	B(this->_shape.getPoint(1));
		sf::Vector2f	vect_dir(A.x - B.x, A.y - B.y);

		vect_dir /= vect_dir.y;

		for (float y = box.top; y < box.top + box.height; y++)
		{
			if (B.x + ct * vect_dir.x <= A.x)
			{
				ct = -ct;
				B = sf::Vector2f(this->_shape.getPoint(3));
				vect_dir = sf::Vector2f(B.x - A.x, B.y - A.y);
				vect_dir /= vect_dir.y;
			}

			for (float x = B.x + ct * vect_dir.x; x < B.x + (-ct) * vect_dir.x; x++)
			{
				if ((x >= point.x && x <= point.x) &&
					(y >= point.y && y <= point.y))
					return (true);
			}

			ct++;
		}

		return (false);
	}

	virtual ~ParallelogramCollider() {}
};