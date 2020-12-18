//
// Created by 李源 on 2020-12-18.
//

#ifndef NEWCPURENDER_DATA_H
#define NEWCPURENDER_DATA_H


#include <fstream>
#include <vector>
#include <cmath>
#include <complex>
#include <iostream>
#include <iomanip> // io manipulator
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>


using namespace std ;

template<class T>
inline T Square(const T &a) { return a*a ; }

// note: double and float, pick float
template<class T>
inline const T& Max(const T& a, const T& b)
{ return b > a ? (b) : (a); }

inline float Max(const double &a, const float &b)
{return b > a ? (b) : float(a);}

inline float Max(const float &a, const double &b)
{return b > a ? float(b) : (a);}

// note: double and float, pick float
template<class T>
inline const T& Min(const T& a, const T& b)
{ return b > a ? (a) : (b); }

inline float Min(const double& a, const float& b)
{ return b > a ? float(a) : (b) ; }

inline float Min(const float& a , const double& b)
{ return b > a ? (a) : float(b) ; }

// note: double and float, pick float
// note: to make a has the same flag with b

template<class T>
inline T Sign(const T& a, const T& b)
{ return b >= 0 ? (a >= 0? (a) : (-a)) :(a >= 0? (-a) : (a)) ;}

inline float Sign(const double& a, const float& b)
{ return float(b >= 0 ? (a >= 0? (a) : (-a)) :(a >= 0? (-a) : (a))) ;}

inline float Sign(const float& a, const double& b)
{ return b >= 0 ? (a >= 0? (a) : (-a)) :(a >= 0? (-a) : (a)) ; }


template<class T>
inline void Swap(T& a, T& b)
{ T t = a ; a = b ; b = t ; }


// note: error Handler

#ifndef _USER_ERROR_CLASS_
#define throw(message) \
{ printf(" ERROR: %s/n   in file %s at line %d.\n", message, __FILE__, __LINE__); throw(1); }
#else

struct Lerror{
    char *message;
    char *file;
    int line;
    Lerror(char *m, char *f, int l) : message(m), file(f), line(l) {}
};

#define throw(message) throw(Lerror(message, __FILE__, __LINE__)) ;
void Lcatch(Lerror err)
{
    printf("ERROR: %s\n    in file %s at line %d\n", err.message, err.file, err.line) ;
    exit(1) ;
}

#endif

#ifdef _USESTDVECTOR_
#define NRvector vector
#else
template<class T>
class Lvector{
private:
    int size_vector ;
    T* data ;

public:
    Lvector() ;
    explicit Lvector(int size) ;
    Lvector(int size, const T& a) ;
    Lvector(int size, const T* a) ;
    Lvector(const Lvector& rhs) ;
    Lvector & operator=(const Lvector& rhs) ;
    typedef T value_type ;
    inline T& operator[](const int i) ; // why is it inline?
    inline const T& operator[](const int i) const ;
    inline int size() ;
    void resize(int newsize) ;
    void assign(int newsize, const T& a) ;
    ~Lvector() ;
};

template <class T>
inline int Lvector<T>::size() { return size_vector; }

template <class T>
Lvector<T>::Lvector() : size_vector(0), data(NULL) {}

// note : if Lvector size less than zero. You will get a nullptr data.
template <class T>
Lvector<T>::Lvector(int size) : size_vector(size), data( size > 0? new T[size]: nullptr) {}


template <class T>
Lvector<T>::Lvector(int size,const T& a) : size_vector(size), data( size > 0? new T[size]: nullptr)
{
    for(int i = 0  ; i < size ; i++)  data[i] = a  ;
}

// note : if put in the stack a pointer, the pointer must be a array pointer.
template <class T>
Lvector<T>::Lvector(int size, const T* a) : size_vector(size), data( size > 0? new T[size]: nullptr)
{
    for(int i = 0 ; i < size ; i++) data[i] = *a++ ;
}

// note : the : init list is also ordered.
template <class T>
Lvector<T>::Lvector(const Lvector& rhs) : size_vector(rhs.size_vector), data( size_vector > 0? new T[size_vector]: nullptr)
{
    for(int i = 0 ; i< size_vector ;i++)  data[i] = rhs.data[i] ;
}

//note: resize vector and copy data
template <class T>
Lvector<T>& Lvector<T>::operator=(const Lvector<T> &rhs) {
    if(this != &rhs)
    {
        if(size_vector!=rhs.size_vector)
        {
            if(data!=nullptr)
            {
                delete[] (data) ;
            }

            size_vector = rhs.size_vector ;
            data = size_vector>0 ? new T[size_vector] : NULL ;
        }

        for(int i = 0;  i < size_vector ;i++)
        {
            data[i] = rhs[i] ;
        }
    }

    return *this ;
}


// if you use []， you must has two version. one for [], another for const [].
template <class T>
inline T& Lvector<T>::operator[](const int i)
{
#ifdef _CHECKBOUNDS_
    if(i<0 || i >= size_vector) throw("Lvector subscript out of bounds.")
#endif
    return data[i] ;
}


template <class T>
inline const T & Lvector<T>::operator[](const int i) const	//subscripting
{
#ifdef _CHECKBOUNDS_
    if (i<0 || i>=nn) {
	throw("Lvector subscript out of bounds");
}
#endif
    return data[i];
}

// note: resize would remove all data
template <class T>
void Lvector<T>::resize(int newsize)
{
    if (newsize != size_vector) {
        if (data != NULL) delete[] (data);
        size_vector  = newsize;
        data = size_vector > 0 ? new T[size_vector] : NULL;
    }
}

template <class T>
void Lvector<T>::assign(int newsize, const T& a)
{
    if(newsize != size_vector)
    {
        if(data != nullptr) delete[] data ;
        size_vector = newsize ;
        data = size_vector > 0? new T[size_vector] : nullptr ;
    }
    for( int i = 0; i< size_vector ;i++)  data[i] = a ;
}


template <class T>
Lvector<T>::~Lvector()
{
    if (data != nullptr) delete[] (data);
}

#endif



template <class T>
class Lmatrix {
private:
    int size_matrix_m ;
    int size_matrix_n ;
    T **data ;

public:
    Lmatrix() ;
    Lmatrix(int m, int n) ;
    Lmatrix(int m, int n, const T& a) ;
    Lmatrix(int m, int n, const T* a) ;
    Lmatrix(const Lmatrix& rhs) ;
    Lmatrix & operator=(const Lmatrix &rhs);
    typedef T value_type;
    inline T* operator[](const int i) ;
    inline const T* operator[](const int i) const;
    inline int mrows() const ;
    inline int ncols() const ;
    void resize(int newn, int newm) ;
    void assign(int newn, int newm, const T& a) ;
//    friend std::ostream& operator<<( std::ostream&, const Lmatrix<T>& ) ;
    ~Lmatrix() ;
};


//template<class TA>
//std::ostream& operator<<(std::ostream &os, const Lmatrix<TA>& obj)
//{
//    os<<"Matrix "<<obj.size_matrix_m<<"x"<<obj.size_matrix_n<<std::endl ;
//    for(int i = 0 ; i < obj.size_matrix_m; i++)
//    {
//        for(int j = 0;  j < obj.size_matrix_n; j++)
//        {
//            os<<obj[i][j]<<" ";
//        }
//        os<<std::endl ;
//    }
//    return os ;
//}

template <class T>
Lmatrix<T>::Lmatrix() : size_matrix_m(0), size_matrix_n(0), data(NULL) {}

template<class T>
Lmatrix<T>::Lmatrix(int m, int n ): size_matrix_m(m), size_matrix_n(n) , data(m>0? new T*[m] : nullptr)
{
    int i,nel=m*n;
    if (data) data[0] = nel>0 ? new T[nel] : NULL;
    for (i=1;i<n;i++) data[i] = data[i-1] + m;
}

template<class T>
Lmatrix<T>::Lmatrix(int m, int n, const T &a) : size_matrix_m(m), size_matrix_n(n), data(size_matrix_m>0 ? new T*[size_matrix_m] : NULL)
{
    int i,j,nel=size_matrix_m*size_matrix_n ;
    if (data) data[0] = nel>0 ? new T[nel] : NULL;
    for (i=1; i< size_matrix_m ; i++) data[i] = data[i-1] + size_matrix_n;
    for (i=0; i< size_matrix_m ; i++) for (j=0; j<size_matrix_n; j++) data[i][j] = a;
}

template<class T>
Lmatrix<T>::Lmatrix(int m, int n, const T* a) : size_matrix_m(m), size_matrix_n(n), data(size_matrix_m>0 ? new T*[size_matrix_m] : NULL)
{
    int i,j,nel = size_matrix_m*size_matrix_n ;
    if(data) data[0] = nel>0? new T[nel] : NULL ;
    for(int i = 0; i < size_matrix_m; i++) data[i] = data[i-1] + size_matrix_n ;
    for(int i = 0; i < size_matrix_m ; i++) for(int j = 0 ; j< size_matrix_n ; j++) data[i][j] = *a++ ;
}



template<class T>
Lmatrix<T>::Lmatrix(const Lmatrix &rhs) : size_matrix_m(rhs.size_matrix_m), size_matrix_n(rhs.size_matrix_n), data(size_matrix_m>0 ? new T*[size_matrix_m] : NULL)
{

    int i,j,nel= size_matrix_m*size_matrix_n ;
    if (data) data[0] = nel>0 ? new T[nel] : NULL;
    for (i=1; i< size_matrix_m; i++) data[i] = data[i-1] + size_matrix_n;
    for (i=0; i< size_matrix_m; i++) for (j=0; j<size_matrix_n; j++) data[i][j] = rhs[i][j];
}


// other matrix named rhs may has the different size with this matrix
template <class T>
Lmatrix<T> & Lmatrix<T>::operator=(const Lmatrix &rhs)
{
    if(this != rhs)
    {
        int i,j,nel ;
        if(size_matrix_m != rhs.size_matrix_m || size_matrix_n != rhs.size_matrix_n)
        {
            if(data!=nullptr)
            {
                // why we need to delete data first?
                delete[] data[0];
                delete[] data ;
            }

            size_matrix_m = rhs.size_matrix_m ;
            size_matrix_n = rhs.size_matrix_n ;
            data = size_matrix_m > 0 ? new T*[size_matrix_m] : nullptr ;
            nel = size_matrix_m*size_matrix_n ;
            if(data) data[0] = nel >0 ? new T[nel] : nullptr ;
            for(int i = 0;  i < size_matrix_m; i++) data[i] = data[i-1] + size_matrix_n;
        }
        for(int i = 0; i <size_matrix_m; i++) for(int j = 0 ; j< size_matrix_n; j++) data[i][j] = rhs[i][j] ;

    }
    return *this ;
}

template <class T>
inline T* Lmatrix<T>::operator[](const int i)
{
#ifdef _CHECKBOUNDS_
    if (i<0 || i>=size_matrix_m) {
	throw("NRmatrix subscript out of bounds");
}
#endif
    return data[i];

}

template <class T>
inline const T* Lmatrix<T>::operator[](const int i) const
{
#ifdef _CHECKBOUNDS_
    if (i<0 || i>=size_matrix_m) {
	throw("NRmatrix subscript out of bounds");
}
#endif
    return data[i];
}

template <class T>
inline int Lmatrix<T>::mrows() const
{
    return size_matrix_m ;
}


template <class T>
inline int Lmatrix<T>::ncols() const
{
    return size_matrix_n ;
}

template <class T>
void Lmatrix<T>::resize(int newm, int newn)
{
    int i,nel;
    if (newm != size_matrix_m || newn != size_matrix_n) {
        if (data != NULL) {
            delete[] (data[0]);
            delete[] (data);
        }
        size_matrix_m = newm ;
        size_matrix_n = newn ;
        data = size_matrix_m>0 ? new T*[size_matrix_m] : NULL;
        nel = size_matrix_m*size_matrix_n;
        if (data) data[0] = nel>0 ? new T[nel] : NULL;
        for (i=1; i< size_matrix_m; i++) data[i] = data[i-1] + size_matrix_n;
    }
}


template <class T>
void Lmatrix<T>::assign(int newm, int newn, const T& a)
{
    int i,j,nel ;
    if ( newm!=size_matrix_m || newn != size_matrix_n)
    {
        if (data != NULL) {
            delete[] (data[0]);
            delete[] (data);
        }

        size_matrix_m = newm ;
        size_matrix_n - newn ;
        data = size_matrix_m>0 ? new T*[size_matrix_m] : NULL;
        nel = size_matrix_m*size_matrix_n;
        if (data) data[0] = nel>0 ? new T[nel] : NULL;
        for (i=1; i< size_matrix_m; i++) data[i] = data[i-1] + size_matrix_n;
        for (i=0; i< size_matrix_m; i++) for (j=0; j<size_matrix_n; j++) data[i][j] = a;
    }
}

template <class T>
Lmatrix<T>::~Lmatrix()
{
    if (data != NULL) {
        delete[] (data[0]);
        delete[] (data);
    }
}



template <class T>
class LMat3d {
private:
    int nn;
    int mm;
    int kk;
    T ***v;
public:
    LMat3d();
    LMat3d(int n, int m, int k);
    inline T** operator[](const int i);	//subscripting: pointer to row i
    inline const T* const * operator[](const int i) const;
    inline int dim1() const;
    inline int dim2() const;
    inline int dim3() const;
    ~LMat3d();

};

template <class T>
LMat3d<T>::LMat3d(): nn(0), mm(0), kk(0), v(NULL) {}

template <class T>
LMat3d<T>::LMat3d(int n, int m, int k) : nn(n), mm(m), kk(k), v(new T**[n])
{
    int i,j;
    v[0] = new T*[n*m];
    v[0][0] = new T[n*m*k];
    for(j=1; j<m; j++) v[0][j] = v[0][j-1] + k;
    for(i=1; i<n; i++) {
        v[i] = v[i-1] + m;
        v[i][0] = v[i-1][0] + m*k;
        for(j=1; j<m; j++) v[i][j] = v[i][j-1] + k;
    }
}


template <class T>
inline T** LMat3d<T>::operator[](const int i) //subscripting: pointer to row i
{
    return v[i];
}

template <class T>
inline const T* const * LMat3d<T>::operator[](const int i) const
{
    return v[i];
}

template <class T>
inline int LMat3d<T>::dim1() const
{
    return nn;
}

template <class T>
inline int LMat3d<T>::dim2() const
{
    return mm;
}

template <class T>
inline int LMat3d<T>::dim3() const
{
    return kk;
}

template <class T>
LMat3d<T>::~LMat3d()
{
    if (v != NULL) {
        delete[] (v[0][0]);
        delete[] (v[0]);
        delete[] (v);
    }
}




// define datatype
typedef int Int; // 32 bit integer
typedef unsigned int Uint;


// MSVC provide int 64  other provide long long
#ifdef _MSC_VER
typedef __int64 Llong; // 64 bit integer
typedef unsigned __int64 Ullong;
#else
// other compiler provide all
typedef long long int Llong; // 64 bit integer
typedef unsigned long long int Ullong;
#endif

typedef char Char; // 8 bit integer
typedef unsigned char Uchar;

typedef double Doub; // default floating type
typedef long double Ldoub;

typedef complex<double> Complex; // default complex type

typedef bool Bool;


// note: three method for getting NaN
//note: uncomment one of the following 3 methods of defining a global NaN
// you can test by verifying that (NaN != NaN) is true
static const Doub NaN = numeric_limits<Doub>::quiet_NaN();

//Uint proto_nan[2]={0xffffffff, 0x7fffffff};
//double NaN = *( double* )proto_nan;

//Doub NaN = sqrt(-1.);


// const for I
typedef const Lvector<Int> VecInt_I ;
typedef Lvector<Int> VecInt, VecInt_O, VecInt_IO;

typedef const Lvector<Uint> VecUint_I;
typedef Lvector<Uint> VecUint, VecUint_O, VecUint_IO;


typedef const Lvector<Llong> VecLlong_I;
typedef Lvector<Llong> VecLlong, VecLlong_O, VecLlong_IO;

typedef const Lvector<Ullong> VecUllong_I;
typedef Lvector<Ullong> VecUllong, VecUllong_O, VecUllong_IO;

typedef const Lvector<Char> VecChar_I;
typedef Lvector<Char> VecChar, VecChar_O, VecChar_IO;

typedef const Lvector<Char*> VecCharp_I;
typedef Lvector<Char*> VecCharp, VecCharp_O, VecCharp_IO;

typedef const Lvector<Uchar> VecUchar_I;
typedef Lvector<Uchar> VecUchar, VecUchar_O, VecUchar_IO;

typedef const Lvector<Doub> VecDoub_I;
typedef Lvector<Doub> VecDoub, VecDoub_O, VecDoub_IO;

typedef const Lvector<Doub*> VecDoubp_I;
typedef Lvector<Doub*> VecDoubp, VecDoubp_O, VecDoubp_IO;

typedef const Lvector<Complex> VecComplex_I;
typedef Lvector<Complex> VecComplex, VecComplex_O, VecComplex_IO;

typedef const Lvector<Bool> VecBool_I;
typedef Lvector<Bool> VecBool, VecBool_O, VecBool_IO;

// matrix types

//
typedef const Lmatrix<Int> MatInt_I;
typedef Lmatrix<Int> MatInt, MatInt_O, MatInt_IO;

typedef const Lmatrix<Uint> MatUint_I;
typedef Lmatrix<Uint> MatUint, MatUint_O, MatUint_IO;

typedef const Lmatrix<Llong> MatLlong_I;
typedef Lmatrix<Llong> MatLlong, MatLlong_O, MatLlong_IO;

typedef const Lmatrix<Ullong> MatUllong_I;
typedef Lmatrix<Ullong> MatUllong, MatUllong_O, MatUllong_IO;

typedef const Lmatrix<Char> MatChar_I;
typedef Lmatrix<Char> MatChar, MatChar_O, MatChar_IO;

typedef const Lmatrix<Uchar> MatUchar_I;
typedef Lmatrix<Uchar> MatUchar, MatUchar_O, MatUchar_IO;

typedef const Lmatrix<Doub> MatDoub_I;
typedef Lmatrix<Doub> MatDoub, MatDoub_O, MatDoub_IO;


typedef const Lmatrix<Bool> MatBool_I;
typedef Lmatrix<Bool> MatBool, MatBool_O, MatBool_IO;

// 3D matrix types

typedef const LMat3d<Doub> Mat3DDoub_I;
typedef LMat3d<Doub> Mat3DDoub, Mat3DDoub_O, Mat3DDoub_IO;


#ifdef _TURNONFPES_
#ifdef _MSC_VER
struct turn_on_floating_exceptions {
	turn_on_floating_exceptions() {
		int cw = _controlfp( 0, 0 );
		cw &=~(EM_INVALID | EM_OVERFLOW | EM_ZERODIVIDE );
		_controlfp( cw, MCW_EM );
	}
};
turn_on_floating_exceptions yes_turn_on_floating_exceptions;
#endif /* _MSC_VER */
#endif /* _TURNONFPES */


#endif //NEWCPURENDER_DATA_H
