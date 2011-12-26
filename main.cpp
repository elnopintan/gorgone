/*
 * main.cpp
 *
 *  Created on: 19/12/2011
 *      Author: ignacio
 */
#include "PersistentVector.h"
#include <iostream>
#include <time.h>

struct block {
	char data[10];
};

int main() {
	{
		clock_t start, finish;
		start = clock();

		PersistentVector <long>  v;
		PersistentVector<PersistentVector<long> > v2;
		for (int i=0; i< 10000;i++) {
			if (i>0)
				v=v2.get(i-1);
			v=v.add(i);
			v2=v2.add(v);}


		for (int i=9998; i>=0; i--)
		{
			v2=v2.pop();
		//	std::cout << v2.size() << ": " << v2.get(i).get(i) << std::endl;
		}
		finish = clock();
		std::cout << "PersistentVector with long     >> " << (finish-start) << std::endl;
	}
	{
		clock_t start, finish;
		start = clock();

		TransientVectorCopy <long>  v;
		TransientVectorCopy <TransientVectorCopy<long> > v2;
		for (int i=0; i< 10000;i++) {
			if (i>0)
				v= v2.get(i-1);
			v.add(i);
			v2.add(v);
		}
		for (int i=9998; i>=0; i--)
		{
			v2.pop();
			// std::cout << v2.size() << ": " << v2.get(i).get(i) << std::endl;
		}
		finish = clock();
		std::cout << "TransientVectorCopy with long  >> " << (finish-start) << std::endl;
	}
	{
		clock_t start, finish;
		start = clock();

		TransientVectorSptr <long>  v;
		TransientVectorSptr <TransientVectorSptr<long> > v2;
		for (int i=0; i< 10000;i++) {
			if (i>0)
				v= v2.get(i-1);
			v.add(i);
			v2.add(v);
		}
		for (int i=9998; i>=0; i--)
		{
			v2.pop();
			// std::cout << v2.size() << ": " << v2.get(i).get(i) << std::endl;
		}
		finish = clock();
		std::cout << "TransientVectorSptr with long  >> " << (finish-start) << std::endl;
	}
	{
		clock_t start, finish;
		start = clock();

		PersistentVector <block>  v;
		PersistentVector<PersistentVector<block> > v2;
		for (int i=0; i< 10000;i++) {
			if (i>0)
				v=v2.get(i-1);
			v=v.add(block());
			v2=v2.add(v);}


		for (int i=9998; i>=0; i--)
		{
			v2=v2.pop();
		//	std::cout << v2.size() << ": " << v2.get(i).get(i) << std::endl;
		}
		finish = clock();
		std::cout << "PersistentVector with block    >> " << (finish-start) << std::endl;
	}
	{
		clock_t start, finish;
		start = clock();

		TransientVectorCopy <block>  v;
		TransientVectorCopy <TransientVectorCopy<block> > v2;
		for (int i=0; i< 10000;i++) {
			if (i>0)
				v= v2.get(i-1);
			v.add(block());
			v2.add(v);
		}
		for (int i=9998; i>=0; i--)
		{
			v2.pop();
			// std::cout << v2.size() << ": " << v2.get(i).get(i) << std::endl;
		}
		finish = clock();
		std::cout << "TransientVectorCopy with block >> " << (finish-start) << std::endl;
	}
	{
		clock_t start, finish;
		start = clock();

		TransientVectorSptr <block>  v;
		TransientVectorSptr <TransientVectorSptr<block> > v2;
		for (int i=0; i< 10000;i++) {
			if (i>0)
				v= v2.get(i-1);
			v.add(block());
			v2.add(v);
		}
		for (int i=9998; i>=0; i--)
		{
			v2.pop();
			// std::cout << v2.size() << ": " << v2.get(i).get(i) << std::endl;
		}
		finish = clock();
		std::cout << "TransientVectorSptr with block >> " << (finish-start) << std::endl;
	}


}
