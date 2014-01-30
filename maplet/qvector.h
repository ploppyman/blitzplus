
#ifndef QVECTOR_H
#define QVECTOR_H

namespace qtl{

int  qavail();
void *qmalloc( int sz );
void qfree( void *q );

template<class T>
class vector{
	T *_beg,*_end,*_cap;
public:
	vector():_beg(0),_end(0),_cap(0){
	}
	vector( const vector<T> &t ){
		_beg=_end=(T*)qmalloc(t.capacity()*sizeof(T));
		_cap=_beg+t.capacity();
		for( int k=0;k<t.size();++k ) push_back( t[k] );
	}
	~vector(){
		clear();
		qfree(_beg);
	}
	T &operator[]( int n ){
		return _beg[n];
	}
	const T &operator[]( int n )const{
		return _beg[n];
	}
	int size()const{
		return _end-_beg;
	}
	int capacity()const{
		return _cap-_beg;
	}
	void clear(){
		while( size() ) pop_back();
	}
	void resize( int n ){
		reserve(n);
		while( size()>n ) pop_back();
		while( size()<n ) new(_end++)T();
	}
	void reserve( int n ){
		if( _cap-_beg>=n ) return;
		T *p=(T*)qmalloc(n*sizeof(T));
		memcpy( p,_beg,size()*sizeof(T) );
		qfree(_beg);
		_cap=p+n;
		_end=p+size();
		_beg=p;
	}
	void push_back( const T &t ){
		if( _end==_cap ) reserve( size()+size()/2+10 );
		new(_end++) T(t);
	}
	void pop_back(){
		--_end;
		_end->~T();
	}
	T &back(){
		return *(_end-1);
	}
	const T &back()const{
		return *(_end-1);
	}
};

}

#endif