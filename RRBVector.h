/*
 * RRBVector.h
 *
 *  Created on: 14/01/2012
 *      Author: ignacio
 */

/*
 *    r
 *    _data
 * 15-1                                                               2-p
 *    _data                                                           _data
 * 10-1               2               3               4               1               2
 *    _data           _data           _data           _data           _data           _data
 *  5-1   2   3   4   1   2   3   4   1   2   3   4   1   2   3   4   1   2   3   4   1
 *                                                                                   _d
 *  0-123412341234123412341234123412341234123412341234123412341234123412341234123412341234
 *
 */

#ifndef RRBVECTOR_H_
#define RRBVECTOR_H_
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string.h>

template <typename T>
class RRBVector
{
	struct Node;
	typedef boost::shared_ptr<Node> node_ptr;
	struct Node
	{
		virtual bool isFull()=0;
		virtual unsigned int size()=0;
		virtual unsigned int level()=0;
		virtual const node_ptr add(const T &value)=0;
		virtual const node_ptr pop()=0;
		virtual unsigned int lastIndex()=0;
		virtual const T &get(unsigned int pos)=0;
		virtual bool hasOnlyOne()=0;
		virtual ~Node() {};

	};
	struct LeafBranch: public Node
	{
		T *_data;
		unsigned int _size;
		LeafBranch(unsigned int size):_size(size) {
			_data=new T[size];
		};

		LeafBranch(const T data[], unsigned int n,unsigned int size): _size(size)
		{
			_data=new T[size];
			for (unsigned int i=0;i<n;i++)
				_data[i]=data[i];
		};

		LeafBranch(const LeafBranch &other) :_size(other._size){
			_data=new T[other._size];
			for (unsigned int i=0;i<other._size;i++)
					_data[i]=other._data[i];

		}

		LeafBranch &operator=(const LeafBranch &other) {
			delete[]_data;
			_size=other._size;
			_data=new T[other._size];
			for (unsigned int i=0;i<other._size;i++)
					_data[i]=other._data[i];
			return *this;
		}



		virtual ~LeafBranch() {
			delete []_data;
		}

		bool isFull(){
			return _size==32;
		};
		bool hasOnlyOne() {
			return _size==1;
		}
		unsigned int size() {return _size;}
		unsigned int level() {return 0;}


		const node_ptr add(const T &value) {
				node_ptr result (new LeafBranch(_data, _size,_size+1));
				LeafBranch *newNode= static_cast<LeafBranch *> (result.get());
				newNode->_data[_size]=value;
				return result;
		};

		const node_ptr pop() {
			if (_size==1)
				return node_ptr();
			return node_ptr(new LeafBranch(_data,_size-1,_size-1));
		};

		unsigned int lastIndex() {return _size;};
		const T & get(unsigned int pos) {
			return _data[pos];
		}

	};

	struct MidBranch : public Node
	{
		unsigned int _level;
		unsigned int _size;
		unsigned int *_indexes;
		boost::shared_ptr<Node> *_data;
		bool isFull()
		{
			return _size==32 && _data[31]->isFull();
		};
		MidBranch(unsigned int size, unsigned int level):_level(level), _size(size)
		{
			_indexes= new unsigned int[_size];
			_data= new boost::shared_ptr<Node>[_size];
		};

		MidBranch(const boost::shared_ptr<Node> data[],const unsigned int indexes[],unsigned int n, unsigned int size, unsigned int level):
		 _level(level),_size(size)
		{
			_indexes= new unsigned int[_size];
			_data= new boost::shared_ptr<Node>[_size];
			for (unsigned int i=0;i<n;i++)
			{
				_data[i]=data[i];
				_indexes[i]=indexes[i];
			}
		};

		MidBranch(const MidBranch &other):
			_level(other._level), _size(other._size)
		{

			_indexes= new unsigned int[_size];
			_data= new boost::shared_ptr<Node>[_size];
			for (int i=0;i<other._size;i++) {
				_data[i]=other._data[i];
				_indexes[i]=other._indexes[i];
			}
		}

		virtual MidBranch & operator=(const MidBranch &other)
		{
			for (unsigned int i=0;i<_size;i++)
							_data[i].reset();
						delete []_data;
						delete []_indexes;
			_level=other._level;
			_size=other._size;
			_indexes= new unsigned int[_size];
			_data= new boost::shared_ptr<Node>[_size];
			for (unsigned int i=0;i<other._size;i++) {
				_data[i]=other._data[i];
				_indexes[i]=other._indexes[i];
			}
			return *this;
		}

		virtual ~MidBranch()
		{
			for (unsigned int i=0;i<_size;i++)
				_data[i].reset();
			delete []_data;
			delete []_indexes;
		}

		unsigned int size() {return _size;};
		unsigned int level() {return _level;};
		unsigned int lastIndex() {
			return _indexes[_size-1];
		}
		bool isSubnodeFull(Node *node)
		{
			return (node->level()+5==_level)&&(node->isFull());
		};

		const node_ptr add(const T &value)
		{
			if (isSubnodeFull(_data[_size-1].get()))
				{
					node_ptr result(new MidBranch(_data,_indexes,_size,_size+1,_level));
					MidBranch *newBranch=static_cast<MidBranch *>(result.get());
					LeafBranch nextNode(1);
					nextNode._data[0]=value;
					newBranch->_data[_size]=node_ptr (new LeafBranch(nextNode));
					newBranch->_indexes[_size]=_indexes[_size-1]+1;
					return result;
				}
			else
			{
				node_ptr result(new MidBranch(_data,_indexes,_size-1,_size,_level));
				node_ptr newChild;
				if (_data[_size-1]->isFull())
				{
					///Shift children node
					newChild=node_ptr(new MidBranch(1,_data[_size-1]->level()+5));
					MidBranch *newBranch=static_cast<MidBranch *>(newChild.get());
					newBranch->_data[0]=_data[_size-1];
					unsigned int prevIndex=_size-1==0?0:_indexes[_size-2];
					newBranch->_indexes[0]=_indexes[_size-1]-prevIndex;
				}
				else
				{
					newChild=_data[_size-1];
				}
				MidBranch *resultBranch=static_cast<MidBranch *>(result.get());
				resultBranch->_data[_size-1]=newChild->add(value);
				resultBranch->_indexes[_size-1]=_indexes[_size-1]+1;
				return result;
			}
		};

		bool hasOnlyOne() {
			return _size==1;
		}

		const node_ptr pop()
		{
			node_ptr newChild=_data[_size-1]->pop();
			if (newChild)
			{
				node_ptr result(new MidBranch(_data,_indexes,_size-1,_size,_level));
				MidBranch *newBranch=static_cast<MidBranch *>(result.get());
				newBranch->_data[_size-1]=newChild;
				newBranch->_indexes[_size-1]=_indexes[_size-1]-1;
				return result;
			}
			if (_size==1)
				return node_ptr();
			if ((_size==2)&&(_data[_size-2]->level()>0))
				{
					return _data[0];
				}
			return node_ptr(new MidBranch(_data,_indexes,_size-1,_size-1,_level));
		};

		const T & get(unsigned int pos) {
			unsigned int branch;
			for (branch=(pos >> _level) & 0x1f; _indexes[branch] <pos;branch++);
			unsigned int nextPos= branch? _indexes[branch-1]:0;
			return _data[branch]->get(pos-nextPos);
		};
	};

	node_ptr _root;
	public:
	RRBVector<T> add(const T &value)
	{
		RRBVector<T> newVector;
		if (!_root)
		{
			newVector._root=node_ptr(new LeafBranch(1));
			LeafBranch *rootData=static_cast<LeafBranch *> (newVector._root.get());
			rootData->_data[0]=value;
		}
		else
		{
			if (_root->isFull())
			{
				node_ptr nextRoot(new MidBranch(1,_root->level()+5));
				MidBranch *rootData=static_cast<MidBranch *>(nextRoot.get());
				rootData->_data[0]=_root;
				rootData->_indexes[0]=_root->lastIndex();
				newVector._root=nextRoot->add(value);
			}
			else
				newVector._root=_root->add(value);
		}
		return newVector;
	};

	RRBVector<T> pop() {
		RRBVector<T> result;
		if (!_root)
			throw "";
			result._root=_root->pop();
		return result;
	}
	unsigned int size() {
		if (!_root) return 0;
		return _root->lastIndex();
	}

	const T & get(unsigned int pos) const {
		if (!_root)
			throw "";
		if (pos<_root->lastIndex())
			return _root->get(pos);
		throw "";
	}


};
#endif /* RRBVECTOR_H_ */
