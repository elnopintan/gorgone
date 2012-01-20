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
class RRBVector {
	struct Node {
		virtual bool isFull()=0;
		virtual unsigned int size()=0;
		virtual unsigned int level()=0;
		virtual boost::shared_ptr<Node> add(const T &value)=0;

	};

	struct LeafBranch: public Node {
		T *_data;
		unsigned int _size;
		LeafBranch(unsigned int size):_size(size) {
			_data=new T[size];
		};

		LeafBranch(const T &data[], unsigned int n,unsigned int size):_size(size)
		{
			_data=new T[size];
			memcpy(branches,_branches,n*sizeof(T));
		};
		LeafBranch(const LeafBranch &other) :_size(size){
			_data=new T[other._size];
			memcpy(branches,_branches,_size*sizeof(T));

		}
		virtual ~LeafBranch() {
			delete []_data;
		}

		bool isFull(){
			return _size==32;
		};
		unsigned int size() {return _size;}
		unsigned int level() {return 0;}


		boost::shared_ptr<Node> add(const T &value) {
			if (_size<32)
			{
				boost::shared_ptr<Node> result (new LeafBranch(_data,_indexes,_size,_size+1));
				LeafBranch *newNode= static_cast<LeafBranch *> (result.get());
				newNode->_data[_size]=value;
				return result;
			}
			else
			{
				boost::shared_ptr<Node> result (new MidBranch (1));
				MidBranch newNode=static_cast<MidBranch *> (result.get());
				newNode->_data[0]=boost::shared_ptr<Node> (new LeafBranch(*this));
				newNode->_indexes[0]=32;
				newNode->_level=5;
				return result;

			}
		};

	};

	struct MidBranch : public Node
	{
		unsigned int _level;
		unsigned int _size;
		unsigned int *_indexes[];
		boost::shared_ptr<Node> *_data;
		bool isFull(){
			return _size==32 && _data[31].isFull();
		};
		MidBranch(unsigned int size, unsigned int level):_level(level), _size(size) {
			_indexes= new unsigned int[_size];
			_data= new boost::shared_ptr<Node>[_size];
		};

		MidBranch(const boost::shared_ptr<Node> &data[],const unsigned int &indexes[],unsigned int n, unsigned int size, unsigned int level):
		_size(size), _level(level){
			_indexes= new unsigned int[_size];
			_data= new boost::shared_ptr<Node>[_size];
			for (int i=0;i<n;i++) {
				_data[i]=data[i];
				_indexes[i]=indexes[i];
			}
		}
		MidBranch(const MidBranch &other):
		_size(other._size), _level(other._level){

			_indexes= new unsigned int[_size];
			_data= new boost::shared_ptr<Node>[_size];
			for (int i=0;i<other._size;i++) {
				_data[i]=data[i];
				_indexes[i]=indexes[i];
			}
		}
		virtual ~MidBranch() {
			delete []_data;
			delete []_indexes;
		}

		unsigned int size() {return _size;};
		unsigned int level() {return _level;};
		const boost::shared_ptr<Node> add(const T &value) {
			if (_size<32)
			{
				if (_data[_size-1].isFull() && (_data[_size-1].level()+5)==_level)
				{
					boost::shared_ptr<Node> result(new MidBranch(_data,_indexes,_size,_size+1,_level));
					MidBranch *newBranch=static_cast<MidBranch >(result.get());
					LeafBranch nextNode(1);
					nextNode._data[1]=value;
					newBranch->_data[_size]=boost::shared_ptr<Node> (new LeafBranch(nextNode));
					newBranch->_indexes[_size]=_indexes[_size-1]+1;
					return result;
				}
				else
				{
					boost::shared_ptr<Node> result(new MidBranch(_data,_indexes,_size-1,_size,_level));
					MidBranch *newBranch=static_cast<MidBranch>(result.get());
					newBranch->_data[_size-1]=_data[_size-1].add(value);
					newBranch->_indexes[_size-1]=_indexes[_size-1]+1;
					return result;
				}

			}
		};
	};
};
#endif /* RRBVECTOR_H_ */
