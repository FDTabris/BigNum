// FILE: BigNum.cxx
// CLASS IMPLEMENTED: BigNum (see BigNum.h for documentation)
// DANA FORD

#include "BigNum.h"
#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

namespace dford
{
    BigNum::BigNum()
    {
	base = 10;
	capacity = DEFAULT_CAPACITY;
	digits = new size_t[capacity];
	digits[0] = 0;
	used = 1;
	positive = true;
    }
    	
    BigNum::BigNum(int num, size_t optional_base)
    {
	base = optional_base;
	capacity = DEFAULT_CAPACITY;
	digits = new size_t[capacity];
	used = 0;

	if (num >= 0)
	    positive = true;

	else
	{
	    positive = false;
	    num = num*-1;
	}

// If num is 0, assign other private variables and exit constructor
	if (num == 0)
	{
	    digits[0] = 0;
	    used = 1;
	    return;
	}
	
	while (num != 0)
	{
	    digits[used] = num % 10;
	    num /= 10;
	    ++used;
	}

       if (digits[used - 1] == 0)
	   trim(digits);
	
    }
    
    BigNum::BigNum(const char strin[], size_t optional_base)
    {
	base = optional_base;
	size_t i, j;

	if (strin[0] == '-')
	{   
	    positive = false;
	    used = strlen(strin) - 1;
	    j = used;
	}	    
	    
	else
	{  
	    positive = true;
	    used = strlen(strin);
	    j = used - 1;
	}
	
	if (used <= DEFAULT_CAPACITY)
	    capacity = DEFAULT_CAPACITY;
	else
	    capacity = used;

	digits = new size_t[capacity];
  
// If string is "0" or "000...". Assign digits and exit constructor
	if (strin[0] == '0' && strin[used - 1] == '0')
	{
	    digits[0] = 0;
	    return;
	}	
	
	for (i = 0; i < used; ++i)
	{
	    digits[i] = strin[j] - 0x30;

// NOTE: ON FOLLOWING IF STATEMENT
// Needed if base != 10
	    if (digits[i] >= 10)
		digits[i] -= 7;
	    
	    --j;
	}

// Wrote private trim funciton to handle leading 0's
	if (digits[used - 1] == 0)
	    trim(digits);
    }
    
    BigNum::BigNum(const BigNum& anotherBigNum)
    {
	capacity = anotherBigNum.capacity;
	used = anotherBigNum.used;
	positive = anotherBigNum.positive;
	base = anotherBigNum.base;
	digits = new size_t[anotherBigNum.capacity];
	copy(anotherBigNum.digits, anotherBigNum.digits + used, digits);	
    }
    
    BigNum::~BigNum()
    {
	delete [ ] digits;
    }	

    size_t BigNum::get_digit(size_t index) const
    {
	return digits[index];
    }
    
    size_t BigNum::get_used() const
    {
	return used;
    }
    
    size_t BigNum::get_capacity() const
    {
	return capacity;
    }
    
    size_t BigNum::get_base() const
    {
	return base;
    }
    
    bool BigNum::get_sign() const
    {
	return positive;
    }
    	
    BigNum& BigNum::operator=(const BigNum &anotherBigNum)
    {
	size_t *temp_data;
	
	if (this == &anotherBigNum)
	    return *this;	
	
	if (capacity != anotherBigNum.capacity)
	{
	    capacity = anotherBigNum.capacity;
	    temp_data = new size_t[anotherBigNum.capacity];
	    delete [ ] digits;
	    digits = temp_data;
	}
	
	used = anotherBigNum.used;
	positive = anotherBigNum.positive;
	base = anotherBigNum.base;
	copy(anotherBigNum.digits, anotherBigNum.digits + used, digits);
	return *this;	
    }
    
    BigNum& BigNum::operator+=(const BigNum &addend)
    {
	*this = *this + addend;
	 if (digits[used - 1] == 0)
		trim(digits);
	    
	return *this;
    }
    
    BigNum& BigNum::operator-=(const BigNum &subtractand)
    {
	*this = *this - subtractand;
	 if (digits[used - 1] == 0)
		trim(digits);

	return *this;
    }
    
    BigNum& BigNum::operator*=(const BigNum &multiplicand)
    {
	*this = *this * multiplicand;
	return *this;
    }
    
    BigNum& BigNum::operator/=(const BigNum &divisor)
    {
	*this = *this / divisor;
	return *this;
    }
    
    BigNum& BigNum::operator%=(const BigNum &divisor)
    {
	*this = *this % divisor;
	return *this;
    }
    
    BigNum& BigNum::operator++()
    {
	if (base != 10)
	    convert_to_base10(*this);
	
	capacity += 1;
	size_t i = 1;

	if (positive == false)
	{
		positive = true;
		--*this;
		positive = false;
		if (used == 1 && digits[0] == 0)
			positive = true;
		return *this;
	}

	if (digits[0] != 9)
	{
	    digits[0] += 1;
	    return *this;
	}
	
	digits[0] = 0;
	while (i < used)
	{
	    if (digits[i] != 9)
	    {
		digits[i] += 1;
		return *this;
	    }
	    else
	    {
		digits[i] = 0;
		++i;
	    }
	}

	digits[used] = 1;
	++used;
	return *this;
    }
    
    BigNum& BigNum::operator--()
    {
	if (base != 10)
	    convert_to_base10(*this);
	
	size_t i = 1;

	if (positive == false)
	{
		positive = true;
		++*this;
		positive = false;
		return *this;
	}

	if (used == 1 && digits[0] == 0)
	{
		digits[0] = 1;
		positive = false;
		return *this;
	}
	
	if (digits[0] != 0)
	{
	    digits[0] -= 1;
	    return *this;
	}
	
	digits[0] = 9;
	while (i < used)
	{
	    if (digits[i] != 0)
	    {
		if (i == used - 1 && digits[i] == 1)
		    --used;
		    
		digits[i] -= 1;
		return *this;
	    }
	    else
	    {
		digits[i] = 9;
		++i;
	    }
	}	
    }	
	
    std::ostream& operator <<(std::ostream &os, const BigNum &bignum)
    {
	if (bignum.positive == false)
	    os << '-';

	for (size_t i = 0; i < bignum.used; ++i)
	{
	    switch(bignum.digits[bignum.used - i - 1])
	    {
	        case 10:  os << 'A';
		          break;
	        case 11:  os << 'B';
		          break;
	        case 12:  os << 'C';
		          break;
	        case 13:  os << 'D';
		          break;
	        case 14:  os << 'E';
		          break;
	        case 15:  os << 'F';
		          break;
	        case 16:  os << 'G';
		          break;
	        default:  os << bignum.digits[bignum.used - i - 1];
	    }
	}
	
	return os;
    }
    
    std::istream& operator >>(std::istream &is, BigNum &bignum)
    {
	for (size_t i = 0; i < bignum.get_used(); ++i)
	    is >> bignum.digits[i];
	
	return is;
    }
    
    BigNum operator+(const BigNum &n1, const BigNum &n2)
    {
	BigNum n3(n1);
	BigNum n4(n2);
	BigNum n5;
	
	if (n3.base != n4.base || n3.base != 10)
	{
	    convert_to_base10(n3);
	    convert_to_base10(n4);
	}

// n1 positive and n2 positive
	if (n1.positive == true && n2.positive == true)
	{
	    if (n3 >= n4)
		n5 = n3.sum(n4);		
	    else
		n5 = n4.sum(n3);

	    n5.positive = true;
	    return n5;
	}

// n1 positive and n2 negative
	if (n1.positive == true && n2.positive == false)
	{
	    n4.set_sign(true);
	    
	    if (n3 >= n4)
	    {
		n5 = n3.diff(n4);
		n5.positive = true;
	    }
	    else
	    {
		n5 = n4.diff(n3);
		n5.positive = false;
     	    }
	    
	    if (n5.digits[n5.used - 1] == 0)
		n5.trim(n5.digits);
	    
	    return n5;
	}
	
// n1 negative and n2 positive
	if (n1.positive == false && n2.positive == true)
	{
	    n3.set_sign(true);
	    
	    if (n3 >= n4)
	    {
		n5 = n3.diff(n4);
		n5.positive = false;
	    }
	    else
	    {
		n5 = n4.diff(n3);
		n5.positive = true;
	    }

	    if (n5.digits[n5.used - 1] == 0)
		n5.trim(n5.digits);
	    
	    return n5;
      	}
	
// n1 negative and n2 negative
	if (n1.positive == false && n2.positive == false)
	{
	    n3.set_sign(true);
	    n4.set_sign(true);

	    if (n3 >= n4)
		n5 = n3.sum(n4);
	    else
		n5 = n4.sum(n3);
	
	    n5.positive = false;
	}

	return n5;
    }
    
    
    BigNum operator-(const BigNum &n1, const BigNum &n2)
    {
    	BigNum n3(n1);
	BigNum n4(n2);
	BigNum n5;

	if (n3.base != n4.base || n3.base != 10)
	{
	    convert_to_base10(n3);
	    convert_to_base10(n4);
	}
	
// n1 positive and n2 positive
	if (n1.positive == true && n2.positive == true)
	{
	    if (n3 >= n4)
	    {
		n5 = n3.diff(n4);
		n5.positive = true;
	    }
	    else
	    {
		n5 = n4.diff(n3);
		n5.positive = false;
	    }

	    if (n5.digits[n5.used - 1] == 0)
	    	n5.trim(n5.digits);

	    return n5;
	    
	}

// n1 positive and n2 negative
	if (n1.positive == true && n2.positive == false)
	{
	    n4.set_sign(true);
	    
	    if (n3 >= n4)
		n5 = n3.sum(n4);
	    else
		n5 = n4.sum(n3);

	    n5.positive = true;
	    if (n5.digits[n5.used - 1] == 0)
		n5.trim(n5.digits);

	    return n5;
	}	

// n1 negative and n2 positive
	if (n1.positive == false && n2.positive == true)
	{
	    n3.set_sign(true);

	    if (n3 >= n4)
		n5 = n3.sum(n4);
	    else
		n5 = n4.sum(n3);
	   
	    n5.positive = false;
	    if (n5.digits[n5.used - 1] == 0)
		n5.trim(n5.digits);
	    
	    return n5;
      	}
	
// n1 negative and n2 negative
	if (n1.positive == false && n2.positive == false)
	{
	    n3.set_sign(true);
	    n4.set_sign(true);

	    if (n3 > n4)
	    {
		n5 = n3.diff(n4);
		n5.positive = false;
	    }
	    else
	    {
		n5 = n4.diff(n3);
		n5.positive = true;
	    } 

	    if (n5.digits[n5.used - 1] == 0)
	    	n5.trim(n5.digits);	    
	}
	
	return n5;	
    }
    
    BigNum operator*(const BigNum &n1, const BigNum &n2)
    {
	BigNum n3(n1);
	BigNum n4(n2);
	BigNum n5;
	BigNum temp_array;

	size_t carry = 0;
	size_t temp_num = 0;
	
	if (n3 == 0 || n4 == 0)
	    return n5;

	if (n3.used + n4.used > n5.capacity)
	{
	    temp_array.reserve(n3.used + n4.used);
	    n5.reserve(n3.used + n4.used);
	}
	
	if (n3.positive == false || n4.positive == false)
	{
	    n3.positive = true;
	    n4.positive = true;
	}

	if (n3.base != n4.base || n3.base != 10)
	{
	    convert_to_base10(n3);
	    convert_to_base10(n4);
	}

	n5.used = 0;
	temp_array.used = 0;
	
	if (n3 <= n4)
	{
	    for (size_t i = 0; i < n3.used; ++i)
	    {
		size_t k = i;
		
		for (size_t j = 0; j < n4.used; ++j)
		{
			    
		    temp_num =  n3.digits[i] * n4.digits[j] + carry;
		    if (temp_num >= 10)
		    {
			carry = temp_num / 10;
			temp_num %= 10;
		    }
		    else
		    {
			carry = 0;
		    }
		    

		    temp_array.digits[k] = temp_num;
		    ++temp_array.used;
		    ++k;
		    
		}

		if (carry != 0)
		{
		    temp_array.digits[temp_array.used] = carry;
		    ++temp_array.used;
		}
				
		n5 = n5 + temp_array;
		carry = 0;

		temp_array.used = i + 1;
		for (size_t z = 0; z < temp_array.used; ++z)
		    temp_array.digits[z] = 0;
	    }
	}
	else
	{
	    for (size_t i = 0; i < n4.used; ++i)
	    {
		size_t k = i;
	       
		for (size_t j = 0; j < n3.used; ++j)
		{
		    temp_num =  n4.digits[i] * n3.digits[j] + carry;
		    if (temp_num >= 10)
		    {
			carry = temp_num / 10;
			temp_num %= 10;
		    }
		    else
		    {
			carry = 0;
		    }
		    
		    temp_array.digits[k] = temp_num;
		    ++temp_array.used;
		    ++k;
		    
		}

		if (carry != 0)
		{
		    temp_array.digits[temp_array.used] = carry;
		    ++temp_array.used;
		}
				
		n5 = n5 + temp_array;
		carry = 0;

		temp_array.used = i + 1;
		for (size_t z = 0; z < temp_array.used; ++z)
		    temp_array.digits[z] = 0;
		
	    }
	}
	    
	if (n1.positive != n2.positive)
		n5.positive = false;
		    
	return n5;	
    }
    
    BigNum operator/(const BigNum &n1, const BigNum &n2)
    {
	BigNum n3(n1);
	BigNum n4(n2);
       	BigNum n5;
	
      	size_t divisor = 0;
	size_t multiplier = 1;	
	
	if (n3.capacity > n5.capacity)
	    n5.reserve(n3.capacity);

	if (n3.positive == false || n4.positive == false)
	{
	    n3.positive = true;
	    n4.positive = true;
	}

	if (n3.base != n4.base || n3.base != 10)
	{
	    convert_to_base10(n3);
	    convert_to_base10(n4);
	}
	
	if (n3 < n4)
	    return n5;

	if (n3 == n4)
	{
	    if (n1.positive != n2.positive)
		n5.positive = false;
	    
	    n5.digits[0] = 1;
	    return n5;
	}
	
	while (n4 <= n3)
	{
	    ++divisor;
	    ++multiplier;
	    n4 = n2 * multiplier;
	}

	n5.used = 0;
	while (divisor != 0)
	{
	    n5.digits[n5.used] = divisor % 10;
	    divisor /= 10;
	    ++n5.used;
	}

	if (n1.positive != n2.positive)
	    n5.positive = false;
	    
	return n5;
    }
    
    BigNum operator%(const BigNum &n1, const BigNum &n2)
    {
	BigNum n3(n1);
	BigNum n4(n2);
	BigNum n5;

	BigNum divisor;
	BigNum temp;	

	if (n3.base != n4.base || n3.base != 10)
	{
	    convert_to_base10(n3);
	    convert_to_base10(n4);
	}

	if (n3.positive == false || n4.positive == false)
	{
	    n3.positive = true;
	    n4.positive = true;
	}

	if (n3 < n4)
	    return n3;

	if (n3 == n4)
	    return n5;
	
	divisor = n3/n4;
	temp = n4 * divisor;
	n5 = n3 - temp;
	
	return n5;
    }
    
    BigNum factorial(const BigNum &num)
    {
	BigNum n1(num);
	BigNum n2(num);

	n1.reserve(1000);
	
	if (num.positive == false)
	    n2.positive = true;

	if (num.used == 1 && num.digits[0] == 0)
	{
	    n1.used = 1;
	    n1.digits[0] = 1;
	    return n1;
	}
	
	while (n2.used > 1)
	{   
	    --n2;
	    n1 *= n2;

	}

	while (n2.digits[0] != 1)
	{
	    --n2;
	    n1 *= n2;
	}	

	return n1;
    }
    
    BigNum convert_to_base10(const BigNum &b)
    {
	BigNum n1;
	n1.used = 0;
        size_t base_num_10 = b.digits[0];
	size_t base_num = b.base;

        for (size_t i = 1; i < b.used; ++i)
	{
	    base_num_10 += b.digits[i] * base_num;
	    base_num *= b.base;
	}
	    	    
 	while (base_num_10 != 0)
	{
	    if (n1.used + 1 == n1.capacity)
		n1.reserve(n1.capacity * 2);

	    n1.digits[n1.used] = base_num_10 % 10;
	    base_num_10 /= 10;
	    ++n1.used;
	}

	return n1;
    }
    
    BigNum convert_base10_to_another(const BigNum &b,
						size_t new_base)
    {
	BigNum n1;
	n1.used = 0;

	size_t base_num_10 = 0;
	size_t power_num = 1;

	for (size_t i = 0; i < b.used; ++i)
	{
	    base_num_10 += b.digits[i] * power_num;
	    power_num *= 10;
	}
	
	while (base_num_10 != 0)
	{    
	    if (n1.used + 1 == n1.capacity)
		n1.reserve(n1.capacity * 2);

	    n1.digits[n1.used] = base_num_10 % new_base;
	    base_num_10 /= new_base;
	    ++n1.used;
	}

	n1.base = new_base;	    	
	return n1;
    }
    
    void BigNum::reserve(size_t n)
    {
	size_t *larger_array;
	
	if (n == capacity)
	    return;
	
	if (n < used)
	    n  = used;
       
	larger_array = new size_t[n];
	copy(digits, digits + used, larger_array);
	delete [] digits;
	digits = larger_array;
	capacity = n;	
    }
    
    BigNum BigNum::diff(const BigNum &subtractand) const
    {
	assert (positive && subtractand.positive);
	assert (base == subtractand.base);
	assert (*this >= subtractand);

  	BigNum difference = *this;
	
  	for (size_t i = 0; i < subtractand.used; ++i)
	{
	    if (difference.digits[i] < subtractand.digits[i])
	    {
		difference.digits[i] += 10;
		if (difference.digits[i+1] == 0)
		{
		    size_t j = i + 1;
		    while (j < difference.used && difference.digits[j] == 0)
		    {
			difference.digits[j] = 9;
			++j;
		    }
		    difference.digits[j] -= 1;
		}
		else
		{
		    difference.digits[i+1] -= 1;
		}
		    
	    }

	    difference.digits[i] -= subtractand.digits[i];
	}
	    
	return difference;   
    }
    
    BigNum BigNum::sum(const BigNum &addend) const
    {
	assert (positive && addend.positive);
	assert (base == addend.base);
	assert (*this >= addend);

	BigNum summation;
	summation.used = 0;	
	summation.reserve(capacity + 2);
	size_t carry = 0;
   
	for (size_t i = 0; i < used; ++i)
	{
	    if (i < addend.used)
	    {
		summation.digits[i] = digits[i] + addend.digits[i] + carry;
		if (summation.digits[i] >= 10)
		{
		    summation.digits[i] %= 10;
		    carry = 1;
		}
		else
		{
		    carry = 0;
		}			
		
	    }
	    else
	    {	
		summation.digits[i] = digits[i] + carry;
		if (summation.digits[i] >= 10)
		{
		    summation.digits[i] %= 10;
		    carry = 1;
		}
		else
		{
		    carry = 0;
		}	
	    }
	    summation.used++;
	}
	
	if (carry == 1)
	{
	    summation.digits[summation.used] = 1;
	    summation.used++;
	}
	
	return summation;		
    }
    
    void BigNum::set_digit(size_t digit, size_t index)
    {
	if (index >= used)
	    reserve(used);
       	digits[index] = digit;     
    }
  
    void BigNum::set_used(size_t new_used)
    {
	assert (new_used < capacity);
       	used = new_used;
    }

    void BigNum::set_base(size_t new_base)
    {
	base = new_base;
    }

    void BigNum::set_sign(bool pos_or_neg)
    {
	if (pos_or_neg == true)
	    positive = true;
	else
	    positive = false;
    }  

    void BigNum::trim(size_t smaller_digits[])
    {
	size_t index = used - 1;

	if (smaller_digits[0] == 0)
	    used = 1;
	else
	{
	    while (smaller_digits[index] == 0)
	    {
		smaller_digits[index] = smaller_digits[index - 1];
		--index;
		--used;
	    }
	}
	
    }
      
    bool operator>(const BigNum &n1, const BigNum &n2)
    {
	return (n1 >= n2 && n1 != n2);
    }
    
    bool operator>=(const BigNum &n1, const BigNum &n2)
    {
	return !(n1 < n2);
    }
    
    bool operator<(const BigNum &n1, const BigNum &n2)
    {
	BigNum n3(n1);
	BigNum n4(n2);
	
	if (n3.get_base() != n4.get_base())
	{
	    convert_to_base10(n3);
	    convert_to_base10(n4);
	}

	if (n3.get_sign() == false)
	{
	    if (n4.get_sign() == true)
		return true;
	    if (n3.get_used() > n4.get_used())
		return true;
	    if (n3.get_used() < n4.get_used())
		return false;
	}
	
	if (n3.get_sign() == true)
	{
	    if (n4.get_sign() == false)
		return false;
	    if (n3.get_used() > n4.get_used())
		return false;
	    if (n3.get_used() < n4.get_used())
		return true;
	}

	int i = n3.get_used() - 1;
	
	if (n3.get_sign() == false)
	{
	    while (i >= 0)
	    {
		if (n3.get_digit(i) > n4.get_digit(i))
		    return true;
		if (n3.get_digit(i) < n4.get_digit(i))
		    return false;
		--i;
	    }

	    return false;	    
	}
	else
	{
	    while (i >= 0)
	    {
		if (n3.get_digit(i) < n4.get_digit(i))
		    return true;
		if (n3.get_digit(i) > n4.get_digit(i))
		    return false;
		--i;
	    }

	    return false;  
	}	
    }
    
    bool operator<=(const BigNum &n1, const BigNum &n2)
    {
	return !(n1 > n2);
    }
    
    bool operator==(const BigNum &n1, const BigNum &n2)
    {
	BigNum n3(n1);
	BigNum n4(n2);
	
	if (n3.get_base() != n4.get_base())
	{
	    convert_to_base10(n3);
	    convert_to_base10(n4);
	}
	
	if (n3.get_sign() != n4.get_sign())
	    return false;

	if (n3.get_used() != n4.get_used())
	    return false;
	
	for (size_t i = 0; i < n3.get_used(); ++i)
	{
	    if (n3.get_digit(i) != n4.get_digit(i))
		return false;
	}
	
	return true;
    }
    
    bool operator!=(const BigNum &n1, const BigNum &n2)
    {
	return !(n1 == n2);
    }
    
}
