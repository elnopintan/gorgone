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
	template <int N>
	struct LeafBranch: public Node {
		T _data[N];
		unsigned int _indexes [N];
		LeafBranch() {};

		LeafBranch(const T &data[], unsigned int n)
		{
			memcpy(branches,_branches,n*sizeof(T));
		};
		bool isFull(){
			return N==32;
		};
		unsigned int size() {return N;}
		unsigned int level() {return 0;}


		boost::shared_ptr<Node> add(const T &value) {
			if (N<32)
			{
				boost::shared_ptr<Node> result (new LeafBranch<N+1>(_data,_indexes,N));
				LeafBranch<N+1> *newNode= static_cast<LeafBranch<N+1> *> (result.get());
				newNode->_data[N]=value;
				return result;
			}
			else
			{
				boost::shared_ptr<Node> result (new MidBranch<1> (newNode));
				MidBranch<1> newNode=static_cast<MidBranch<1> *> (result.get());
				newNode->_data[0]=boost::shared_ptr<Node> (new LeafBranch<N>(*this));
				newNode->_indexes[0]=32;
				newNode->_level=5;
				return result;

			}
		};

	};
	struct MidBranch<N> : public Node
	{
		unsigned int _level;
		unsigned int _indexes[N];
		boost::shared_ptr<Node> _data[N];
		bool isFull(){
			return N==32 && _data[31].isFull();
		};
		MidBranch() {};
		MidBranch(const boost::shared_ptr<Node> &data[],const unsigned int &indexes[],unsigned int n) {
			for (int i=0;i<n;i++) {
				_data[i]=data[i];
				_indexes[i]=indexes[i];
			}
		}
		unsigned int size() {return N;};
		unsigned int level() {return _level;};
		const boost::shared_ptr<Node> add(const T &value) {
			if (N<32)
			{
				if (_data[N].isFull() && (_data[N].level()+5)==_level)
				{
					boost::shared_ptr<Node> result(new MidBranch<N+1>(_data,_indexes,N));
					MidBranch<N+1> *newBranch=static_cast<MidBranch<N+1> >(result.get());
					LeafBranch<1> nextNode;
					nextNode._data[1]=value;
					newBranch->_data[N]=boost::shared_ptr<Node> (new LeafBranch<1>(nextNode));
					newBranch->_indexes[N]=_indexes[N-1]+1;
					return result;
				}
				else
				{
					boost::shared_ptr<Node> result(new MidBranch<N>(_data,_indexes,N-1));
					MidBranch<N> *newBranch=static_cast<MidBranch<N> >(result.get());
					newBranch->_data[N-1]=_data[N-1].add(value);
					newBranch->_indexes[N-1]=_indexes[N-1]+1;
					return result;
				}

			}
		};
	};
};
#endif /* RRBVECTOR_H_ */
