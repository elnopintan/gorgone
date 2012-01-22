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
		virtual node_ptr add(const T &value)=0;
		virtual unsigned int lastIndex()=0;

	};
	struct LeafBranch: public Node
	{
		T *_data;
		unsigned int _size;
		LeafBranch(unsigned int size):_size(size) {
			_data=new T[size];
		};

		LeafBranch(const T data[], unsigned int n,unsigned int size):_size(size)
		{
			_data=new T[size];
			memcpy(data,_data,n*sizeof(T));
		};
		LeafBranch(const LeafBranch &other) :_size(size){
			_data=new T[other._size];
			memcpy(other._data,_data,_size*sizeof(T));

		}
		virtual ~LeafBranch() {
			delete []_data;
		}

		bool isFull(){
			return _size==32;
		};
		unsigned int size() {return _size;}
		unsigned int level() {return 0;}


		node_ptr add(const T &value) {
				node_ptr result (new LeafBranch(_data, _size,_size+1));
				LeafBranch *newNode= static_cast<LeafBranch *> (result.get());
				newNode->_data[_size]=value;
				return result;
		};
		unsigned int lastIndex() {return _size;};

	};

	struct MidBranch : public Node
	{
		unsigned int _level;
		unsigned int _size;
		unsigned int *_indexes[];
		boost::shared_ptr<Node> *_data;
		bool isFull()
		{
			return _size==32 && _data[31].isFull();
		};
		MidBranch(unsigned int size, unsigned int level):_level(level), _size(size)
		{
			_indexes= new unsigned int[_size];
			_data= new boost::shared_ptr<Node>[_size];
		};

		MidBranch(const boost::shared_ptr<Node> data[],const unsigned int indexes[],unsigned int n, unsigned int size, unsigned int level):
		_size(size), _level(level)
		{
			_indexes= new unsigned int[_size];
			_data= new boost::shared_ptr<Node>[_size];
			for (int i=0;i<n;i++)
			{
				_data[i]=data[i];
				_indexes[i]=indexes[i];
			}
		};
		MidBranch(const MidBranch &other):
		_size(other._size), _level(other._level)
		{

			_indexes= new unsigned int[_size];
			_data= new boost::shared_ptr<Node>[_size];
			for (int i=0;i<other._size;i++) {
				_data[i]=other._data[i];
				_indexes[i]=other._indexes[i];
			}
		}
		virtual ~MidBranch()
		{
			delete []_data;
			delete []_indexes;
		}

		unsigned int size() {return _size;};
		unsigned int level() {return _level;};
		bool isSubnodeFull(Node *node)
		{
			return !((node->level()+5==_level)||(node->isFull()));
		};
		const node_ptr add(const T &value)
		{
			if (isSubnodeFull(_data[_size-1]))
				{
					node_ptr result(new MidBranch(_data,_indexes,_size,_size+1,_level));
					MidBranch *newBranch=static_cast<MidBranch >(result.get());
					LeafBranch nextNode(1);
					nextNode._data[1]=value;
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
					newChild=node_ptr(new MidBranch(1));
					MidBranch *newBranch=static_cast<MidBranch >(newChild.get());
					newBranch->_level=_data[_size-1]->_level+5;
					newBranch->_data[0]=_data[_size-1];
					newBranch->_indexes[0]=_indexes[_size-1];
				}
				else
				{
					newChild=_data[_size-1];
				}
				MidBranch *resultBranch=static_cast<MidBranch>(result.get());
				resultBranch->_data[_size-1]=newChild->add(value);
				resultBranch->_indexes[_size-1]=_indexes[_size-1]+1;
				return result;
			}
		}
	};

	node_ptr _root;
	RRBVector<T> add(const T &value)
	{
		RRBVector<T> newVector;
		if (_root)
		{
			newVector._root=node_ptr(new LeafBranch(1));
			LeafBranch *rootData=newVector._root.get();
			rootData->_data[0]=value;
		}
		else
		{
			if (_root->isFull())
			{
				node_ptr nextRoot(new MidBranch(1));
				MidBranch *rootData=newVector._root.get();
				rootData->_data[0]=_root;
				rootData->_indexes[0]=_root->lastIndex();
				rootData->_level=_root->level()+5;
				newVector._root=nextRoot->add(value);
			}
			else
				newVector._root=_root->add(value);
		}
		return newVector;
	};


};
#endif /* RRBVECTOR_H_ */
