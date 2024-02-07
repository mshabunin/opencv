#ifndef OPENCV_CORE_MATEXPR_HPP
#define OPENCV_CORE_MATEXPR_HPP

namespace cv {

//==============================================================================
// Classes

class CV_EXPORTS MatOp
{
public:
    MatOp();
    virtual ~MatOp();

    virtual bool elementWise(const MatExpr& expr) const;
    virtual void assign(const MatExpr& expr, Mat& m, int type=-1) const = 0;
    virtual void roi(const MatExpr& expr, const Range& rowRange, const Range& colRange, MatExpr& res) const;
    virtual void diag(const MatExpr& expr, int d, MatExpr& res) const;
    virtual void augAssignAdd(const MatExpr& expr, Mat& m) const;
    virtual void augAssignSubtract(const MatExpr& expr, Mat& m) const;
    virtual void augAssignMultiply(const MatExpr& expr, Mat& m) const;
    virtual void augAssignDivide(const MatExpr& expr, Mat& m) const;
    virtual void augAssignAnd(const MatExpr& expr, Mat& m) const;
    virtual void augAssignOr(const MatExpr& expr, Mat& m) const;
    virtual void augAssignXor(const MatExpr& expr, Mat& m) const;

    virtual void add(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res) const;
    virtual void add(const MatExpr& expr1, const Scalar& s, MatExpr& res) const;

    virtual void subtract(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res) const;
    virtual void subtract(const Scalar& s, const MatExpr& expr, MatExpr& res) const;

    virtual void multiply(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res, double scale=1) const;
    virtual void multiply(const MatExpr& expr1, double s, MatExpr& res) const;

    virtual void divide(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res, double scale=1) const;
    virtual void divide(double s, const MatExpr& expr, MatExpr& res) const;

    virtual void abs(const MatExpr& expr, MatExpr& res) const;

    virtual void transpose(const MatExpr& expr, MatExpr& res) const;
    virtual void matmul(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res) const;
    virtual void invert(const MatExpr& expr, int method, MatExpr& res) const;

    virtual Size size(const MatExpr& expr) const;
    virtual int type(const MatExpr& expr) const;
};

/** @brief Matrix expression representation
@anchor MatrixExpressions
This is a list of implemented matrix operations that can be combined in arbitrary complex
expressions (here A, B stand for matrices ( Mat ), s for a scalar ( Scalar ), alpha for a
real-valued scalar ( double )):
-   Addition, subtraction, negation: `A+B`, `A-B`, `A+s`, `A-s`, `s+A`, `s-A`, `-A`
-   Scaling: `A*alpha`
-   Per-element multiplication and division: `A.mul(B)`, `A/B`, `alpha/A`
-   Matrix multiplication: `A*B`
-   Transposition: `A.t()` (means A<sup>T</sup>)
-   Matrix inversion and pseudo-inversion, solving linear systems and least-squares problems:
    `A.inv([method]) (~ A<sup>-1</sup>)`,   `A.inv([method])*B (~ X: AX=B)`
-   Comparison: `A cmpop B`, `A cmpop alpha`, `alpha cmpop A`, where *cmpop* is one of
  `>`, `>=`, `==`, `!=`, `<=`, `<`. The result of comparison is an 8-bit single channel mask whose
    elements are set to 255 (if the particular element or pair of elements satisfy the condition) or
    0.
-   Bitwise logical operations: `A logicop B`, `A logicop s`, `s logicop A`, `~A`, where *logicop* is one of
  `&`, `|`, `^`.
-   Element-wise minimum and maximum: `min(A, B)`, `min(A, alpha)`, `max(A, B)`, `max(A, alpha)`
-   Element-wise absolute value: `abs(A)`
-   Cross-product, dot-product: `A.cross(B)`, `A.dot(B)`
-   Any function of matrix or matrices and scalars that returns a matrix or a scalar, such as norm,
    mean, sum, countNonZero, trace, determinant, repeat, and others.
-   Matrix initializers ( Mat::eye(), Mat::zeros(), Mat::ones() ), matrix comma-separated
    initializers, matrix constructors and operators that extract sub-matrices (see Mat description).
-   Mat_<destination_type>() constructors to cast the result to the proper type.
@note Comma-separated initializers and probably some other operations may require additional
explicit Mat() or Mat_<T>() constructor calls to resolve a possible ambiguity.

Here are examples of matrix expressions:
@code
    // compute pseudo-inverse of A, equivalent to A.inv(DECOMP_SVD)
    SVD svd(A);
    Mat pinvA = svd.vt.t()*Mat::diag(1./svd.w)*svd.u.t();

    // compute the new vector of parameters in the Levenberg-Marquardt algorithm
    x -= (A.t()*A + lambda*Mat::eye(A.cols,A.cols,A.type())).inv(DECOMP_CHOLESKY)*(A.t()*err);

    // sharpen image using "unsharp mask" algorithm
    Mat blurred; double sigma = 1, threshold = 5, amount = 1;
    GaussianBlur(img, blurred, Size(), sigma, sigma);
    Mat lowContrastMask = abs(img - blurred) < threshold;
    Mat sharpened = img*(1+amount) + blurred*(-amount);
    img.copyTo(sharpened, lowContrastMask);
@endcode
*/
class CV_EXPORTS MatExpr
{
public:
    MatExpr();
    explicit MatExpr(const Mat& m);

    MatExpr(const MatOp* _op, int _flags, const Mat& _a = Mat(), const Mat& _b = Mat(),
            const Mat& _c = Mat(), double _alpha = 1, double _beta = 1, const Scalar& _s = Scalar());

    operator Mat() const;
    template<typename _Tp> operator Mat_<_Tp>() const;

    Size size() const;
    int type() const;

    MatExpr row(int y) const;
    MatExpr col(int x) const;
    MatExpr diag(int d = 0) const;
    MatExpr operator()( const Range& rowRange, const Range& colRange ) const;
    MatExpr operator()( const Rect& roi ) const;

    MatExpr t() const;
    MatExpr inv(int method = DECOMP_LU) const;
    MatExpr mul(const MatExpr& e, double scale=1) const;
    MatExpr mul(const Mat& m, double scale=1) const;

    Mat cross(const Mat& m) const;
    double dot(const Mat& m) const;

    void swap(MatExpr& b);

    const MatOp* op;
    int flags;

    Mat a, b, c;
    double alpha, beta;
    Scalar s;
};

inline
MatExpr::MatExpr()
    : op(0), flags(0), a(Mat()), b(Mat()), c(Mat()), alpha(0), beta(0), s()
{}

inline
MatExpr::MatExpr(const MatOp* _op, int _flags, const Mat& _a, const Mat& _b,
                 const Mat& _c, double _alpha, double _beta, const Scalar& _s)
    : op(_op), flags(_flags), a(_a), b(_b), c(_c), alpha(_alpha), beta(_beta), s(_s)
{}

inline
MatExpr::operator Mat() const
{
    Mat m;
    op->assign(*this, m);
    return m;
}

template<typename _Tp> inline
MatExpr::operator Mat_<_Tp>() const
{
    Mat_<_Tp> m;
    op->assign(*this, m, traits::Type<_Tp>::value);
    return m;
}

//==============================================================================
// Operators

//! @relates cv::MatExpr
//! @{

static inline
Mat& operator += (Mat& a, const MatExpr& b)
{
    b.op->augAssignAdd(b, a);
    return a;
}

static inline
const Mat& operator += (const Mat& a, const MatExpr& b)
{
    b.op->augAssignAdd(b, (Mat&)a);
    return a;
}

template<typename _Tp> static inline
Mat_<_Tp>& operator += (Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignAdd(b, a);
    return a;
}

template<typename _Tp> static inline
const Mat_<_Tp>& operator += (const Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignAdd(b, (Mat&)a);
    return a;
}

static inline
Mat& operator -= (Mat& a, const MatExpr& b)
{
    b.op->augAssignSubtract(b, a);
    return a;
}

static inline
const Mat& operator -= (const Mat& a, const MatExpr& b)
{
    b.op->augAssignSubtract(b, (Mat&)a);
    return a;
}

template<typename _Tp> static inline
Mat_<_Tp>& operator -= (Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignSubtract(b, a);
    return a;
}

template<typename _Tp> static inline
const Mat_<_Tp>& operator -= (const Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignSubtract(b, (Mat&)a);
    return a;
}

static inline
Mat& operator *= (Mat& a, const MatExpr& b)
{
    b.op->augAssignMultiply(b, a);
    return a;
}

static inline
const Mat& operator *= (const Mat& a, const MatExpr& b)
{
    b.op->augAssignMultiply(b, (Mat&)a);
    return a;
}

template<typename _Tp> static inline
Mat_<_Tp>& operator *= (Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignMultiply(b, a);
    return a;
}

template<typename _Tp> static inline
const Mat_<_Tp>& operator *= (const Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignMultiply(b, (Mat&)a);
    return a;
}

static inline
Mat& operator /= (Mat& a, const MatExpr& b)
{
    b.op->augAssignDivide(b, a);
    return a;
}

static inline
const Mat& operator /= (const Mat& a, const MatExpr& b)
{
    b.op->augAssignDivide(b, (Mat&)a);
    return a;
}

template<typename _Tp> static inline
Mat_<_Tp>& operator /= (Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignDivide(b, a);
    return a;
}

template<typename _Tp> static inline
const Mat_<_Tp>& operator /= (const Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignDivide(b, (Mat&)a);
    return a;
}

//! @} relates cv::MatExpr

//==============================================================================
// Functions

CV_EXPORTS MatExpr min(const Mat& a, const Mat& b);
CV_EXPORTS MatExpr min(const Mat& a, double s);
CV_EXPORTS MatExpr min(double s, const Mat& a);
template<typename _Tp> static inline
MatExpr min(const Mat_<_Tp>& a, const Mat_<_Tp>& b) { return cv::min((const Mat&)a, (const Mat&)b); }
template<typename _Tp> static inline
MatExpr min(const Mat_<_Tp>& a, double s) { return cv::min((const Mat&)a, s); }
template<typename _Tp> static inline
MatExpr min(double s, const Mat_<_Tp>& a) { return cv::min((const Mat&)a, s); }
template<typename _Tp, int m, int n> static inline
MatExpr min (const Mat& a, const Matx<_Tp, m, n>& b) { return min(a, Mat(b)); }
template<typename _Tp, int m, int n> static inline
MatExpr min (const Matx<_Tp, m, n>& a, const Mat& b) { return min(Mat(a), b); }

CV_EXPORTS MatExpr max(const Mat& a, const Mat& b);
CV_EXPORTS MatExpr max(const Mat& a, double s);
CV_EXPORTS MatExpr max(double s, const Mat& a);
template<typename _Tp> static inline
MatExpr max(const Mat_<_Tp>& a, const Mat_<_Tp>& b) { return cv::max((const Mat&)a, (const Mat&)b); }
template<typename _Tp> static inline
MatExpr max(const Mat_<_Tp>& a, double s) { return cv::max((const Mat&)a, s); }
template<typename _Tp> static inline
MatExpr max(double s, const Mat_<_Tp>& a) { return cv::max((const Mat&)a, s); }
template<typename _Tp, int m, int n> static inline
MatExpr max (const Mat& a, const Matx<_Tp, m, n>& b) { return max(a, Mat(b)); }
template<typename _Tp, int m, int n> static inline
MatExpr max (const Matx<_Tp, m, n>& a, const Mat& b) { return max(Mat(a), b); }

CV_EXPORTS MatExpr abs(const Mat& m);
CV_EXPORTS MatExpr abs(const MatExpr& e);
template<typename _Tp> static inline
MatExpr abs(const Mat_<_Tp>& m) { return cv::abs((const Mat&)m); }

static inline
void swap(MatExpr& a, MatExpr& b) { a.swap(b); }


} // cv::

#endif // OPENCV_CORE_MATEXPR_HPP
