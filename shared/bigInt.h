#ifndef BIGINT_H
#define BIGINT_H

// source code from :: https://www.geeksforgeeks.org/cpp/bigint-big-integers-in-c-with-example/
#include <vector>
#include <string>
#include <cstring>
#include <iostream>

class BigInt{
    std::string digits;
public:

    //Constructors:
    BigInt(unsigned long long nr = 0)
    {
        do{
            digits.push_back(nr % 10);
            nr /= 10;
        } while (nr);
    }
    BigInt(std::string & s)
        {
        digits = "";
        int n = s.size();
        for (int i = n - 1; i >= 0;i--){
            if(!isdigit(s[i]))
                throw("ERROR");
            digits.push_back(s[i] - '0');
        }
    }
    BigInt(const char *s)
    {
        digits = "";
        for (int i = strlen(s) - 1; i >= 0;i--)
        {
            if(!isdigit(s[i]))
                throw("ERROR");
            digits.push_back(s[i] - '0');
        }
    }
    BigInt(BigInt & a)
    {
        digits = a.digits;
    }


    friend void divide_by_2(BigInt & a)
    {
        int add = 0;
        for (int i = a.digits.size() - 1; i >= 0;i--){
            int digit = (a.digits[i] >> 1) + add;
            add = ((a.digits[i] & 1) * 5);
            a.digits[i] = digit;
        }
        while(a.digits.size() > 1 && !a.digits.back())
            a.digits.pop_back();
    }
    friend bool Null(const BigInt& a)
    {
        if(a.digits.size() == 1 && a.digits[0] == 0)
            return true;
        return false;
    }
    friend int Length(const BigInt & a)
    {
        return a.digits.size();
    }
    int operator[](const int index)const
    {
        if(digits.size() <= (size_t)index || index < 0)
            throw("ERROR");
        return digits[index];
    }

               /* * * * Operator Overloading * * * */

    //Direct assignment
    BigInt &operator=(const BigInt &a)
    {
        digits = a.digits;
        return *this;
    }

    //Post/Pre - Incrementation
    BigInt &operator++()
    {
        int i, n = digits.size();
        for (i = 0; i < n && digits[i] == 9;i++)
            digits[i] = 0;
        if(i == n)
            digits.push_back(1);
        else
            digits[i]++;
        return *this;
    }
    BigInt operator++(int temp)
    {
        BigInt aux;
        aux = *this;
        ++(*this);
        return aux;
    }

    BigInt &operator--()
    {
        if(digits[0] == 0 && digits.size() == 1)
            throw("UNDERFLOW");
        int i, n = digits.size();
        for (i = 0; digits[i] == 0 && i < n;i++)
            digits[i] = 9;
        digits[i]--;
        if(n > 1 && digits[n - 1] == 0)
            digits.pop_back();
        return *this;
    }
    BigInt operator--(int temp)
    {    
        BigInt aux;
        aux = *this;
        --(*this);
        return aux;
    }

    //Addition and Subtraction
    friend BigInt &operator+=(BigInt &a,const BigInt& b)
    {
        int t = 0, s, i;
        int n = Length(a), m = Length(b);
        if(m > n)
            a.digits.append(m - n, 0);
        n = Length(a);
        for (i = 0; i < n;i++){
            if(i < m)
                s = (a.digits[i] + b.digits[i]) + t;
            else
                s = a.digits[i] + t;
            t = s / 10;
            a.digits[i] = (s % 10);
        }
        if(t)
            a.digits.push_back(t);
        return a;
    }
    friend BigInt operator+(const BigInt &a, const BigInt &b)
    {
        BigInt temp;
        temp = a;
        temp += b;
        return temp;
    }
    friend BigInt operator-(const BigInt &a, const BigInt &b)
    {
        BigInt temp;
        temp = a;
        temp -= b;
        return temp;
    }
    friend BigInt &operator-=(BigInt&a,const BigInt &b)
    {
        if(a < b)
            throw("UNDERFLOW");
        int n = Length(a), m = Length(b);
        int i, t = 0, s;
        for (i = 0; i < n;i++){
            if(i < m)
                s = a.digits[i] - b.digits[i]+ t;
            else
                s = a.digits[i]+ t;
            if(s < 0)
                s += 10,
                t = -1;
            else
                t = 0;
            a.digits[i] = s;
        }
        while(n > 1 && a.digits[n - 1] == 0)
            a.digits.pop_back(),
            n--;
        return a;
    }

    //Comparison operators
    friend bool operator==(const BigInt &a, const BigInt &b)
    {
        return a.digits == b.digits;
    }
    friend bool operator!=(const BigInt & a,const BigInt &b)
    {
        return !(a == b);
    }

    friend bool operator>(const BigInt&a,const BigInt&b)
    {
        return b < a;
    }
    friend bool operator>=(const BigInt&a,const BigInt&b)
    {
        return !(a < b);
    }
    friend bool operator<(const BigInt&a,const BigInt&b)
    {
        int n = Length(a), m = Length(b);
        if(n != m)
            return n < m;
        while(n--)
            if(a.digits[n] != b.digits[n])
                return a.digits[n] < b.digits[n];
        return false;
    }
    friend bool operator<=(const BigInt&a,const BigInt&b)
    {
        return !(a > b);
    }

    //Multiplication and Division
    friend BigInt &operator*=(BigInt &a, const BigInt &b)
    {
        if(Null(a) || Null(b)){
            a = BigInt();
            return a;
        }
        int n = a.digits.size(), m = b.digits.size();
        std::vector<int> v(n + m, 0);
        for (int i = 0; i < n;i++)
            for (int j = 0; j < m;j++){
                v[i + j] += (a.digits[i] ) * (b.digits[j]);
            }
        n += m;
        a.digits.resize(v.size());
        for (int s, i = 0, t = 0; i < n; i++)
        {
            s = t + v[i];
            v[i] = s % 10;
            t = s / 10;
            a.digits[i] = v[i] ;
        }
        for (int i = n - 1; i >= 1 && !v[i];i--)
                a.digits.pop_back();
        return a;
    }
    friend BigInt operator*(const BigInt&a,const BigInt&b)
    {
        BigInt temp;
        temp = a;
        temp *= b;
        return temp;
    }

    friend BigInt &operator/=(BigInt& a,const BigInt &b)
    {
        if(Null(b))
            throw("Arithmetic Error: Division By 0");
        if(a < b){
            a = BigInt();
            return a;
        }
        if(a == b){
            a = BigInt(1);
            return a;
        }
        int i, lgcat = 0, cc;
        int n = Length(a);
        std::vector<int> cat(n, 0);
        BigInt t;
        for (i = n - 1; t * 10 + a.digits[i]  < b;i--){
            t *= 10;
            t += a.digits[i] ;
        }
        for (; i >= 0; i--){
            t = t * 10 + a.digits[i];
            for (cc = 9; cc * b > t;cc--);
            t -= cc * b;
            cat[lgcat++] = cc;
        }
        a.digits.resize(cat.size());
        for (i = 0; i < lgcat;i++)
            a.digits[i] = cat[lgcat - i - 1];
        a.digits.resize(lgcat);
        return a;
    }
    friend BigInt operator/(const BigInt &a,const BigInt &b)
    {
        BigInt temp;
        temp = a;
        temp /= b;
        return temp;
    }


    //Modulo
    friend BigInt operator%(const BigInt &a,const BigInt &b)
    {
        BigInt temp;
        temp = a;
        temp %= b;
        return temp;
    }
    friend BigInt &operator%=(BigInt& a,const BigInt &b)
    {
        if(Null(b))
            throw("Arithmetic Error: Division By 0");
        if(a < b){
            return a;
        }
        if(a == b){
            a = BigInt();
            return a;
        }
        int i, lgcat = 0, cc;
        int n = Length(a);
        std::vector<int> cat(n, 0);
        BigInt t;
        for (i = n - 1; t * 10 + a.digits[i] < b;i--){
            t *= 10;
            t += a.digits[i];
        }
        for (; i >= 0; i--){
            t = t * 10 + a.digits[i];
            for (cc = 9; cc * b > t;cc--);
            t -= cc * b;
            cat[lgcat++] = cc;
        }
        a = t;
        return a;
    }

    //Power Function
    friend BigInt &operator^=(BigInt &a,const BigInt &b)
    {
        BigInt Exponent, Base(a);
        Exponent = b;
        a = 1;
        while(!Null(Exponent)){
            if(Exponent[0] & 1)
                a *= Base;
            Base *= Base;
            divide_by_2(Exponent);
        }
        return a;
    }
    friend BigInt operator^(BigInt & a,BigInt & b)
    {
        BigInt temp(a);
        temp ^= b;
        return temp;
    }
    
    //Square Root Function
    friend BigInt sqrt(BigInt & a)
    {
        BigInt left(1), right(a), v(1), mid, prod;
        divide_by_2(right);
        while(left <= right){
            mid += left;
            mid += right;
            divide_by_2(mid);
            prod = (mid * mid);
            if(prod <= a){
                v = mid;
                ++mid;
                left = mid;
            }
            else{
                --mid;
                right = mid;
            }
            mid = BigInt();
        }
        return v;
    }

    //Read and Write
    friend std::ostream &operator<<(std::ostream &out,const BigInt &a)
    {
        for (int i = a.digits.size() - 1; i >= 0;i--)
            std::cout << (short)a.digits[i];
        return std::cout;
    }

    friend std::istream &operator>>(std::istream &in,BigInt&a)
    {
        std::string s;
        in >> s;
        int n = s.size();
        for (int i = n - 1; i >= 0;i--){
            if(!isdigit(s[i]))
                throw("INVALID NUMBER");
            a.digits[n - i - 1] = s[i];
        }
        return in;
    }


};




#endif