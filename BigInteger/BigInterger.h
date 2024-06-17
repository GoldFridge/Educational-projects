#ifndef BIGINT_H
#define BIGINT_H
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

const int base = 1000*1000*1000;

class BigInt {
    public:
        vector<int> lnum;
        bool is_negative;

        BigInt () { 
            lnum.clear();
            lnum.push_back(0);
            is_negative = false; 
        };

        BigInt(int number) { 
            lnum.clear();
            lnum.push_back(abs(number));
            is_negative = (number < 0);
        };

        BigInt(const string &number) {
            lnum.clear();
            int i = 0;
            if (number[0] == '-') {
                is_negative = true;
                i = 1;
            } else {
                is_negative = false;
            }
            for (int i=(int)number.length(); i>0; i-=9) {
                if (i < 9) {
                    lnum.push_back (atoi (number.substr (0, i).c_str()));
                }
                else {
                    lnum.push_back (atoi (number.substr (i-9, 9).c_str()));
                }
            }
        };

        BigInt(const BigInt &number) {
            lnum.clear();
            lnum.resize(number.lnum.size());
            for(int i = 0;i < lnum.size();i ++) {
                lnum[i] = number.lnum[i];
            }
            is_negative = number.is_negative;
        };

        friend std::ostream& operator<<(std::ostream &out, const BigInt &b) {
            if(b.is_negative) {
                out << '-';
            }
            out << (b.lnum.empty() ? 0 : b.lnum.back());
            for (int i=(int)b.lnum.size()-2; i>=0; --i)
                out << setw(9) << setfill('0') << b.lnum[i];
            return out;
        };

        friend std::istream& operator>>(std::istream &in, BigInt &b) {
            string s;
            in >> s; 
            b = s;
            return in;
        }

        BigInt operator+(const BigInt &other) const {
            BigInt result = *this;
            if(result.is_negative == other.is_negative) {
                int carry = 0;
                for (size_t i=0; i < max(result.lnum.size(),other.lnum.size()) || carry; ++i) {
                    if (i == result.lnum.size())
                        result.lnum.push_back (0);
                    result.lnum[i] += carry + (i < other.lnum.size() ? other.lnum[i] : 0);
                    carry = result.lnum[i] >= base;
                    if (carry)  result.lnum[i] -= base;
                }
                return result;
            }
            else {
                BigInt a = result , b = other;
                a.is_negative = false;
                b.is_negative = false;
                bool flag = result.is_negative;
                if(a < b) {
                    flag = other.is_negative;
                    swap(a , b);
                }
                int carry = 0;
                for (size_t i=0; i<b.lnum.size() || carry; ++i) {
                    a.lnum[i] -= carry + (i < b.lnum.size() ? b.lnum[i] : 0);
                    carry = a.lnum[i] < 0;
                    if (carry)  a.lnum[i] += base;
                }
                while (a.lnum.size() > 1 && a.lnum.back() == 0)
                    a.lnum.pop_back();
                a.is_negative = flag;
                return a;
            }
        }       
        
        BigInt& operator+=(const BigInt &other) {
            BigInt res = *this + other;
            *this = res;
            return *this;
        }

        BigInt operator-(const BigInt &other) const {
            return *this + (other * (-1)); 
        }

        BigInt& operator-=(const BigInt &other) {
            BigInt res = *this - other;
            *this = res;
            return *this;
        };

        BigInt operator*(const BigInt &other) const{
            BigInt c;
            c.lnum.resize(lnum.size()+other.lnum.size());
            for (size_t i=0; i<lnum.size(); ++i) {
                for (int j=0, carry=0; j<(int)other.lnum.size() || carry; ++j) {
                    long long cur = c.lnum[i+j] + lnum[i] * 1ll * (j < (int)other.lnum.size() ? other.lnum[j] : 0) + carry;
                    c.lnum[i+j] = int (cur % base);
                    carry = int (cur / base);
                }
            }
            while (c.lnum.size() > 1 && c.lnum.back() == 0)
                c.lnum.pop_back();
            if(is_negative == other.is_negative) {
                c.is_negative = false;
            }
            else {
                c.is_negative = true;
            }
            return c;
        };

        BigInt& operator*=(const BigInt &other) {
            BigInt res = *this * other;
            *this = res;
            return *this;
        };

        bool operator==(const BigInt &other) const {
            return (is_negative == other.is_negative) && (lnum == other.lnum);
        }

        bool operator!=(const BigInt &other) const {
            return !(*this == other);
        }

        bool operator<(const BigInt &other) const {
            if (is_negative != other.is_negative) {
                return is_negative;
            }
            if (lnum.size() != other.lnum.size()) {
                return (lnum.size() < other.lnum.size()) ^ is_negative;
            }
            for (int i = lnum.size() - 1; i >= 0; i--) {
                if (lnum[i] != other.lnum[i]) {
                    return (lnum[i] < other.lnum[i]) ^ is_negative;
                }
            }
            return false;
        }

        bool operator<=(const BigInt &other) const {
            return (*this < other) || (*this == other);
        }

        bool operator>(const BigInt &other) const {
            return !(*this <= other);
        }

        bool operator>=(const BigInt &other) const {
            return !(*this < other);
        }

        BigInt operator/(int const &other) {
            BigInt result = *this;
            result.is_negative = false;
            int b = abs(other);
            int carry = 0;
            for (int i=(int)result.lnum.size()-1; i>=0; --i) {
                long long cur = result.lnum[i] + carry * 1ll * base;
                result.lnum[i] = int (cur / b);
                carry = int (cur % b);
            }
            while (result.lnum.size() > 1 && result.lnum.back() == 0)
                result.lnum.pop_back();

            if((is_negative && other < 0) || (!is_negative && other > 0)) {
                result.is_negative = false;
            }
            else {
                result.is_negative = true;
            }
            return result;
        };

        BigInt operator/(const BigInt &other) {
            BigInt a = *this , b = other;
            a.is_negative = false;
            b.is_negative = false;
            BigInt l = 0 , r = a + 1;
            while(r - l > 1) {
                BigInt m = (l + r) / 2;
                if(b * m <= a) {
                    l = m;
                }
                else {
                    r = m;
                }
            }
            if(is_negative != other.is_negative) {
                l.is_negative = true;
            }
            return l;
        };

        BigInt& operator/=(const BigInt &other) {
            BigInt res = *this / other;
            *this = res;
            return *this;
        }

        BigInt operator%(const BigInt &other) {
            BigInt res = *this / other;
            return *this - (other * res); 
        }

        BigInt& operator%=(const BigInt &other) {
            BigInt res = *this % other;
            *this = res;
            return *this;
        };

        static BigInt factorial(int n) {
            if(n == 1) {
                return 1;
            }
            else {
                BigInt k = n;
                return k * factorial(n - 1); 
            }
        };

        BigInt pow(int exponent) {
            BigInt result = *this;
            if(exponent == 0) {
                return 1;
            }
            else if(exponent % 2 == 1) {
                BigInt b = pow(exponent - 1);
                return b * result;
            } 
            else {
                BigInt b = pow(exponent / 2);
                return b * b;
            }
        };

        BigInt gcd(const BigInt &other) {
            BigInt a = *this, b = other;
            while (b != 0) {
                a %= b;
                swap (a, b);
	        }
            return a;
        };

};

#endif
