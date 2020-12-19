//
// Created by 李源 on 2019-11-22.
//

#ifndef TINYRENDER_DATA_H
#define TINYRENDER_DATA_H
#include <math.h>
#include <iostream>
#include <assert.h>
#include <vector>

namespace Vec {


    template<size_t DimCols, size_t DimRows, typename T>
    class mat;

    template<size_t DIM, typename T>
    struct vec {
    private:
        union {
            T data_[DIM];
        };
    public:
        vec() {
            for (size_t i = DIM; i--; data_[i] = T());
        }

        T &operator[](const size_t i) {
            assert(i < DIM);
            return data_[i];
        }

        const T &operator[](const size_t i) const {
            assert (i < DIM);
            return data_[i];
        }
    };

    template<size_t DIM, typename T>
    T operator*(const vec<DIM, T> &lhs, const vec<DIM, T> &rhs) {
        double ret = double(0.0) ;
        for (size_t i = DIM; i--; ret += lhs[i] * rhs[i]);
        return T(ret);
    }

    template<size_t DIM, typename T>
    vec<DIM, T> operator+(vec<DIM, T> lhs, const vec<DIM, T> rhs) {
        vec<DIM, T> ret;
        for (size_t i = DIM; i--; ret[i] = lhs[i] + rhs[i]);
        return ret;

//    for(size_t i=DIM;i--; lhs[i]+=rhs[i]) ;
//    return lhs ;
    }

    template<size_t DIM, typename T, typename U>
    vec<DIM, T> operator+(vec<DIM, T> lhs, const vec<DIM, U> rhs) {
        vec<DIM, T> ret;
        for (size_t i = DIM; i--; ret[i] = lhs[i] + rhs[i]);
        return ret;
    }

    template<size_t DIM, typename T>
    vec<DIM, T> operator-(vec<DIM, T> lhs, const vec<DIM, T> rhs) {

        vec<DIM, T> ret;
        for (size_t i = DIM; i--; ret[i] = lhs[i] - rhs[i]);
        return ret;
    }


    template<size_t LEN, size_t DIM, typename T>
    vec<LEN, T> embed(const vec<DIM, T> &v, T fill = 1) {
        vec<LEN, T> ret;
        for (size_t i = LEN; i--; ret[i] = (i < DIM ? v[i] : fill));
        return ret;
    }

    template<size_t LEN, size_t DIM, typename T>
    vec<LEN, T> proj(const vec<DIM, T> &v) {
        vec<LEN, T> ret;
        for (size_t i = LEN; i--; ret[i] = v[i]);
        return ret;
    }


    template<size_t DIM, typename T>
    std::ostream &operator<<(std::ostream &out, vec<DIM, T> &v) {
        for (unsigned int i = 0; i < DIM; i++) {
            out << v[i] << " ";
        }
        return out;
    }


/////////////////////////////
    template<typename T>
    struct vec<2, T> {
    public:
        union {
            struct {
                T x, y;
            };
            T data_[2];
        };

        vec() : x(T()), y(T()) {}

        vec(T X, T Y) : x(X), y(Y) {}

        template<class U>
        vec<2, T>(const vec<2, U> &v)
        {
            x = T(v.x) ;
            y = T(v.y) ;
        }


        vec<2, T>(const vec<2, T> &v) :
                x(T()),
                y(T()) {
            *this = v;
        }

        vec<2, T> &operator=(const vec<2, T> &v) {
            if (this != &v) {
                x = v.x;
                y = v.y;
            }
            return *this;
        }

        T &operator[](const size_t i) {
            assert(i < 2);
            return i <= 0 ? x : y;
        }

        const T &operator[](const size_t i) const {
            assert(i < 2);
            return i <= 0 ? x : y;
        }

    };


////////////////////////////
    template<typename T>
    struct vec<3, T> {
    public:
        union {
            struct {
                T x, y, z;
            };
            T data_[3];
        };

        vec() : x(T()), y(T()), z(T()) {}

        vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}


        template<class U>
        vec<3, T>(const vec<3, U> &v)
        {
            x = T(v.x) ;
            y = T(v.y) ;
            z = T(v.z) ;
        }


        vec<3, T>(const vec<3, T> &v) :
                x(T()),
                y(T()),
                z(T()) {
            *this = v;
        }

        vec<3, T> &operator=(const vec<3, T> &v) {
            if (this != &v) {
                x = v.x;
                y = v.y;
                z = v.z;
            }
            return *this;
        }


        T &operator[](const size_t i) {
            assert(i < 3);
            return i <= 0 ? x : (1 == i ? y : z);
        }

        const T &operator[](const size_t i) const {
            assert(i < 3);
            return i <= 0 ? x : (1 == i ? y : z);
        }

        T norm() { return sqrt(x * x + y * y + z * z); }

        vec<3, T> &normalize(T l = 1) {
            *this = (*this) * (l / norm());
            return *this;
        }
    };


    template<typename T>
    vec<3, T> cross(vec<3, T> v1, vec<3, T> v2) {
        return vec<3, T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }



/////////////////////////

    template<size_t DIM, typename T, typename U>
    vec<DIM, T> operator*(vec<DIM, T> lhs, const U &rhs) {
        for (size_t i = DIM; i--; lhs[i] *= rhs);
        return lhs;
    }


    template<size_t DIM, typename T, typename U>
    vec<DIM, T> operator/(vec<DIM, T> lhs, const U &rhs) {
        for (size_t i = DIM; i--; lhs[i] /= rhs);
        return lhs;
    }


/////////////////

//对模版类中的模版函数进行定义 给定一个不同的构造函数的定义
//    template<>
//    template<>
//    vec<3, int>::vec(const vec<3, float> &v) {
//        x = int(v[0]);
//        y = int(v[1]);
//        z = int(v[2]);
//    }
//
//    template<>
//    template<>
//    vec<3, float>::vec(const vec<3, int> &v) {
//        std::cout << " round off error may lead to error " << std::endl;
//        x = float(v[0]);
//        y = float(v[1]);
//        z = float(v[2]);
//    }


////////////////////////////
//我太喜欢这个操作了 只需要定义XX<1,Y>里面放一个XX的baseLine 就可以支撑所有的递归操作 也就是要在你的代码中的递归中投放DIM
// 但是这种操作会让行列式的计算变得低效
    template<size_t DIM, typename T>
    struct dt {
        static T det(const mat<DIM, DIM, T> &src) {
            T ret = 0;
            for (size_t i = DIM; i--; ret += src[0][i] * src.cofactor(0, i));
            return ret;
        }
    };


    template<typename T>
    struct dt<1, T> {
        static T det(const mat<1, 1, T> &src) {
            return src[0][0];
        }
    };
///////////////


#define DEFAULT_ALLOC  4  ;


    template<size_t DimRows, size_t DimCols, typename T>
    class mat {
        vec<DimCols, T> rows[DimRows];
    public:
        mat() {}

        vec<DimCols, T> &operator[](const size_t i) {
            assert(i < DimRows);
            return rows[i];
        }

        const vec<DimCols, T> &operator[](const size_t i) const {
            assert(i < DimRows);
            return rows[i];
        }

        vec<DimRows, T> col(const size_t idx) {
            vec<DimRows, T> ret;
            for (size_t i = DimRows; i--; ret[i] = rows[i][idx]);
            return ret;
        }


        void set_col(const size_t idx, vec<DimRows, T> setdata) {
            assert(idx < DimCols);
            for (size_t i = DimRows; i--; rows[i][idx] = setdata[i]);
        }


        static mat<DimRows, DimCols, T> indentity() {
            mat<DimRows, DimCols, T> ret;
            for (int i = 0; i < DimRows; i++) {
                for (int j = 0; j < DimCols; j++) {
                    ret[i][j] = (i == j ? 1.f : 0.f);
                }
            }

            ret[0][0] = 1;
            return ret;
        }

        T det() const {
            return dt<DimCols, T>::det(*this);
        }


        mat<DimRows - 1, DimCols - 1, T> get_minor(size_t row, size_t col) const {
            mat<DimRows - 1, DimCols - 1, T> ret;
            for (size_t i = DimRows - 1; i--;)
                for (size_t j = DimCols - 1; j--; ret[i][j] = rows[i < row ? i : i + 1][j < col ? j : j + 1]);

            return ret;
        }


        mat<DimRows, DimCols, T> adjugate() const {
            mat<DimRows, DimCols, T> ret;
            for (size_t i = DimRows; i--;)
                for (size_t j = DimCols; j--; ret[i][j] = cofactor(i, j));
            return ret;
        }


        T cofactor(size_t i, size_t j) const {
            assert(i < DimRows);
            assert(j < DimCols);

            return get_minor(i, j).det() * ((i + j) % 2 ? -1 : 1);
        }


        mat<DimCols, DimRows, T> inverse_transpose() {
            mat<DimRows, DimCols, T> ret = adjugate();
            T tmp = ret[0] * rows[0];
            return ret / tmp;
        }

        mat<DimRows, DimCols, T> transpose() {
            mat<DimCols, DimRows, T> ret;
            for (size_t i = DimCols; i--; ret[i] = this->col(i));
            return ret;
        }

        inline int get_cols() { return DimCols; }

        inline int get_rows() { return DimRows; }


        inline mat<DimCols, DimRows, T> &operator=(const mat<DimRows, DimCols, T> &rhs) {
            if (this != &rhs) {
                for (size_t i = DimRows; i--; (*this)[i] = rhs[i]);
            }
            return *this;
        }


    };




/////////////////

    template<size_t DimRows, size_t DimCols, typename T>
    vec<DimRows, T> operator*(const mat<DimRows, DimCols, T> &lhs, const vec<DimCols, T> &rhs) {
        vec<DimRows, T> ret;
        for (size_t i = DimRows; i--; ret[i] = lhs[i] * rhs);
        return ret;
    }

    template<size_t R1, size_t C1, size_t C2, typename T>
    mat<R1, C2, T> operator*(const mat<R1, C1, T> &lhs, mat<C1, C2, T> &rhs) {
        mat<R1, C2, T> result;
        for (size_t i = R1; i--;)
            for (size_t j = C2; j--;) {
                vec<C1, T> temp1 = lhs[i];
                vec<C1, T> temp2 = rhs.col(j);
                result[i][j] = temp1 * temp2;
            }
        return result;
    }

    template<size_t DimRows, size_t DimCols, typename T>
    mat<DimCols, DimRows, T> operator/(mat<DimRows, DimCols, T> lhs, const T &rhs) {
        for (size_t i = DimRows; i--; lhs[i] = lhs[i] / rhs);
        return lhs;
    }


    template<size_t DimRows, size_t DimCols, class T>
    std::ostream &operator<<(std::ostream &out, mat<DimRows, DimCols, T> &m) {
        for (size_t i = 0; i < DimRows; i++) out << m[i] << std::endl;
        return out;
    }

    vec<3,float> min(vec<3,float> left, vec<3,float> right)
    {
        vec<3,float> result ;
        result[0] = std::min(left[0],right[0]) ;
        result[1] = std::min(left[1],right[1]) ;
        result[2] = std::min(left[2],right[2]) ;
    }

}

////////////////


typedef Vec::vec<2,  float> Vec2f;
typedef Vec::vec<2,  int>   Vec2i;
typedef Vec::vec<3,  float> Vec3f;
typedef Vec::vec<3,  int>   Vec3i;
typedef Vec::vec<4,  float> Vec4f;
typedef Vec::mat<4,4,float> Matrix44;
typedef Vec::mat<4,1,float> Matrix41;




#endif //TINYRENDER_DATA_H
