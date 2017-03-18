#ifndef EBA_DUAL_HPP
#       define EBA_DUAL_HPP
#       include <cmath>
namespace eba {
        namespace math {
                template <typename TNumeric> struct dual {
                        TNumeric real, imag;
                        dual(TNumeric a = TNumeric(), TNumeric b = TNumeric()): real(a), imag(b) {}
                        bool operator == (const dual &oth) {
                                return real == oth.real && imag == oth.imag;
                        }
                        bool operator != (const dual &oth) {
                                return real != oth.real || imag != oth.imag;
                        }
                        dual& operator += (const dual &oth) {
                                real += oth.real;
                                imag += oth.imag;
                                return *this;
                        }
                        dual& operator -= (const dual &oth) {
                                real -= oth.real;
                                imag -= oth.imag;
                                return *this;
                        }
                        dual& operator *= (const dual &oth) {
                                TNumeric x, y;
                                x = real * oth.real;
                                y = real * oth.imag + imag * oth.real;
                                real = x, imag = y;
                                return *this;
                        }
                        dual& operator /= (const dual &oth) {
                                TNumeric x, y;
                                x = real / oth.real;
                                y = (imag * oth.real - real * oth.imag)/(oth.real*oth.real);
                                real = x, imag = y;
                                return *this;
                        }
                        dual operator + (const dual & a) {
                                dual copy (*this);
                                copy += a;
                                return copy;
                        }
                        dual operator - (const dual & a) {
                                dual copy (*this);
                                copy -= a;
                                return copy;
                        }
                        dual operator / (const dual & a) {
                                dual copy (*this);
                                copy /= a;
                                return copy;
                        }
                        dual operator * (const dual & a) {
                                dual copy (*this);
                                copy *= a;
                                return copy;
                        }
                };
                template <typename TNumeric>
                dual<TNumeric> conj(const dual<TNumeric>& x) {
                        return dual<TNumeric>(x.real, -x.imag); 
                }
                template <typename TNumeric>
                dual<TNumeric> polar(const TNumeric r, const TNumeric phi) {
                        return dual<TNumeric>(r, r*phi); 
                }
                template <typename TNumeric>
                dual<TNumeric> log(const dual<TNumeric>& x) {
                        return dual<TNumeric>(log(abs(x)), arg(x)); 
                }
                template <typename TNumeric>
                dual<TNumeric> log10(const dual<TNumeric>& x) {
                        return dual<TNumeric>(log(abs(x)) / log(TNumeric(10)), arg(x) / log(TNumeric(10))); 
                }
                template <typename TNumeric>
                dual<TNumeric> exp(const dual<TNumeric>& x) {
                        TNumeric a, b;
                        a = b = exp(x.real);
                        b *= x.imag;
                        return dual<TNumeric>(a, b); 
                }
                template <typename TNumeric>
                dual<TNumeric> sin(const dual<TNumeric>& x) {
                        TNumeric a, b;
                        a = sin(x.real);
                        b = cos(x.real)*x.imag;
                        return dual<TNumeric>(a, b); 
                }
                template <typename TNumeric>
                dual<TNumeric> cos(const dual<TNumeric>& x) {
                        TNumeric a, b;
                        a = cos(x.real);
                        b = sin(x.real)*x.imag;
                        return dual<TNumeric>(a, -b); 
                }
                template <typename TNumeric>
                dual<TNumeric> sinh(const dual<TNumeric>& x) {
                        TNumeric a, b;
                        a = sinh(x.real);
                        b = cosh(x.real)*x.imag;
                        return dual<TNumeric>(a, b); 
                }
                template <typename TNumeric>
                dual<TNumeric> cosh(const dual<TNumeric>& x) {
                        TNumeric a, b;
                        a = cosh(x.real);
                        b = sinh(x.real)*x.imag;
                        return dual<TNumeric>(a, b); 
                }
                template <typename TNumeric>
                TNumeric abs(const dual<TNumeric>& x) {
                        return x.real; 
                }
                template <typename TNumeric>
                TNumeric arg(const dual<TNumeric>& x) {
                        return x.imag / x.real; 
                }
                template <typename TNumeric>
                TNumeric norm(const dual<TNumeric>& x) {
                        return x.imag*x.imag + x.real*x.real; 
                }
                template <typename TNumeric>
                dual<TNumeric> sqrt(const dual<TNumeric>& x) {
                        TNumeric a, b;
                        a = sqrt(x.real);
                        b = x.imag / TNumeric(2);
                        return dual<TNumeric>(a, b/a); 
                }
                template <typename TNumeric>
                dual<TNumeric> pow(const dual<TNumeric>& x, const dual<TNumeric>& pow) {
                        return exp(log(x)*pow);
                }
                template <typename TNumeric>
                dual<TNumeric> tan(const dual<TNumeric>& x) {
                        TNumeric a, b;
                        a = tan(x.real);
                        b = x.imag*(TNumeric(1)+a*a);
                        return dual<TNumeric>(a, b); 
                }
                template <typename TNumeric>
                dual<TNumeric> atan(const dual<TNumeric>& x) {
                        TNumeric a, b;
                        a = atan(x.real);
                        b = x.imag/(TNumeric(1)+x.real*x.real);
                        return dual<TNumeric>(a, b); 
                }
                template <typename TNumeric>
                dual<TNumeric> asin(const dual<TNumeric>& x) {
                        TNumeric a, b;
                        a = asin(x.real);
                        b = x.imag/sqrt(TNumeric(1)-x.real*x.real);
                        return dual<TNumeric>(a, b); 
                }
                template <typename TNumeric>
                dual<TNumeric> acos(const dual<TNumeric>& x) {
                        TNumeric a, b;
                        a = acos(x.real);
                        b = -x.imag/sqrt(TNumeric(1)-x.real*x.real);
                        return dual<TNumeric>(a, b); 
                }
                template <typename TNumeric>
                dual<TNumeric> asinh(const dual<TNumeric>& x) {
                        TNumeric a, b;
                        a = asinh(x.real);
                        b = x.imag/sqrt(TNumeric(1)+x.real*x.real);
                        return dual<TNumeric>(a, b); 
                }
                template <typename TNumeric>
                dual<TNumeric> acosh(const dual<TNumeric>& x) {
                        TNumeric a, b;
                        a = acosh(x.real);
                        b = x.imag/sqrt(x.real*x.real-TNumeric(1));
                        return dual<TNumeric>(a, b); 
                }
                template <typename TNumeric>
                dual<TNumeric> tanh(const dual<TNumeric>& x) {
                        TNumeric a, b;
                        a = tanh(x.real);
                        b = x.imag/(cosh(x.real)*cosh(x.real));
                        return dual<TNumeric>(a, b); 
                }
                template <typename TNumeric>
                dual<TNumeric> atanh(const dual<TNumeric>& x) {
                        TNumeric a, b;
                        a = atanh(x.real);
                        b = x.imag/(TNumeric(1)-x.real*x.real);
                        return dual<TNumeric>(a, b); 
                }

        };
};
#endif
// vim: et:ts=8:sw=8
