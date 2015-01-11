# Gorgone

Turning mutability into stone.

## Dependencies:

Boost 1.48

This library includes a set of classes that implement persistent collections.
A persistent collection are immutable data that provide modified copies when manipulated.

PersistentVector<T> creates a vector of T objects.

  add(T &data) returns a copy of the PersistentVector with data inserted.

  pop() returns a copy of PersistentVector without the last data

  put(unsigned int pos, T &newData) return a copy of PersistentVector replacing the data in position pos with newData

  size() return vector size

  get(unsigned int pos) returns the data in position pos

main.cpp provides an example:

Creates a PersistentVector of Persistent vectors of long called v2.

Iteratively inserts new vectors into v2, each one copy of the previous with a new data added.

Iteratively calls pop() until v2 is empty.

Writes the ellapsed time between operations;


Repeats this procedure with two kind of Vectors in order to compare times:

TranisentVectorCopy<T>. A transient vector that uses an std::vector<T>.

TransientVectorSPtr<T>. A transient vector that uses an std::vector<shared_ptr <T> >

Repeats the procedures again with a block of 10 chars.


## License

Licensed under the EPL (see the file epl.html).

