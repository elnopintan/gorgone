/*
 * RRBVector.h
 *
 *  Created on: 14/01/2012
 *      Author: ignacio
 */

#ifndef RRBVECTOR_H_
#define RRBVECTOR_H_
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <vector>

template <typename T>
class RRBVector {
	class Node {
	public:
		virtual bool isFull(unsigned int parentLevel)=0;
		Node add(const T &data);
	};
	struct Leaf : public Node {
		T _value;
		Leaf(const T & value): _value(value) {};
		bool isFull(unsigned int parentLevel) {return true;}
	};
	struct Branch : public Node {
		typedef boost::shared_ptr<Node> Branch_ptr ;
		std::vector<Branch_ptr> _data;
		std::vector<unsigned int> _index;
		unsigned int _level;
		const Branch & add(const T &value)
		{
			if (_level==0)
			{
				if (_data.size()<32)
				{
					Branch result(*this);
					result._data.push_back(Branch_ptr(new Leaf(value)));
					result._data.push_back(result._data.size());
					return result;
				}
				else
				{
					Branch result;
					result._level=5;
					result._data.push_back(Branch_ptr(new Branch(*this)));
				}
			}
			else
				if (!_data[_data.size()]->isFull(_level))
				{
					Branch result(*this);
					Branch * lastBranch=(Branch *)result._data[_data.size()]->get();
					result._data[_data.size()]=(Branch_ptr(new Branch(lastBranch->add(value))));
					result._index[_data.size()]++;
				}
				else
				{
					Branch result;
					if (_data[_data.size()]< 32)
						result=Branch(*this);
					else
						{
						result._level=_level+5;
						result._data.push_back(Branch_ptr(new Branch(*this)));
						result._index.push_back(_index[_data.size()-1]);
						}
					result._data.push_back(Branch_ptr(new Branch(Branch().add(value))));
					result._index.push_back(result._index[result._data.size()-1]+1);
					return result;
				}

		}
		bool isFull(unsigned int parentLevel) {
			if (_data.size()<32)
				return false;
			if (_data[31]->isFull(_level))
			{
				if (_level== parentLevel-5)
					return true;
				else
					return false;
			}
			return false;
		}
	};

	Branch _root;
	public:
	RRBVector() {
	};

	RRBVector(const Branch &root):
		_root(root)
	{};
	RRBVector(const RRBVector<T> &v):
		_root(v._root)
	{

	};

	RRBVector<T> add(const T &data) {
		return RRBVector<T>(_root.add(data));
	}


};
#endif /* RRBVECTOR_H_ */
