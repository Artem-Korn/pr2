#include "Header.h"

const string MyBigInt::HEX = "0123456789abcdef";

MyBigInt::MyBigInt()
{
    this->digits.push_back(0);
}

MyBigInt::MyBigInt(unsigned long long value)
{
    while (value != 0) {
        this->digits.push_back(value % MyBigInt::BASE);
        value /= MyBigInt::BASE;
    }
}

MyBigInt::MyBigInt(vector<unsigned> digits)
{
    this->set_digits(digits);
}

void MyBigInt::set_hex(string hex_str)
{
    unsigned len = (unsigned)hex_str.length();
    unsigned j = len;
    MyBigInt result, p_result, hex(16);

    for (unsigned i = 0; i < len; i++) {
        p_result = hex.pow(i);
        result = result.ADD(p_result.MUL(MyBigInt::HEX.find_first_of(hex_str.substr(--j, 1).c_str())));
    }

    this->digits = result.get_digits();
}

string MyBigInt::get_hex()
{
    string result;
    MyBigInt current = *this;
    MyBigInt hex(16);

    while (!current.is_zero()) {
        unsigned temp = current.MOD(hex).get_int();

        if (temp < 10) {
            result = (char)(48 + temp) + result;
        }
        else {
            result = (char)(87 + temp) + result;
        }

        current = current.DIV(hex);
    }

    return result;
}

MyBigInt MyBigInt::INV()
{
    size_t len = this->digits.size();
    vector<unsigned> result(len);
    for (size_t i = 0; i < len; i++) {
        result[i] = ~this->digits[i];
    }
	return MyBigInt(result);
}

MyBigInt MyBigInt::XOR(MyBigInt other)
{
    size_t len = max(this->digits.size(), other.digits.size());
    this->digits.resize(len);
    other.digits.resize(len);

    vector<unsigned> result(len);

    for (size_t i = 0; i < len; i++) {
        result[i] = this->digits[i] ^ other.digits[i];
    }

    return MyBigInt(result);
}

MyBigInt MyBigInt::OR(MyBigInt other)
{
    size_t len = max(this->digits.size(), other.digits.size());
    this->digits.resize(len);
    other.digits.resize(len);

    vector<unsigned> result(len);

    for (size_t i = 0; i < len; i++) {
        result[i] = this->digits[i] | other.digits[i];
    }

    return MyBigInt(result);
}

MyBigInt MyBigInt::AND(MyBigInt other)
{
    size_t len = max(this->digits.size(), other.digits.size());
    this->digits.resize(len);
    other.digits.resize(len);

    vector<unsigned> result(len);

    for (size_t i = 0; i < len; i++) {
        result[i] = this->digits[i] & other.digits[i];
    }

    return MyBigInt(result);
}

MyBigInt MyBigInt::ADD(MyBigInt additive)
{
    return MyBigInt(digits_sum(this->digits, additive.get_digits()));
}

MyBigInt MyBigInt::SUB(MyBigInt negative)
{
    if (this->compare(negative) <= 0)
        return MyBigInt();

    return MyBigInt(digits_sub(this->digits, negative.get_digits()));
}

MyBigInt MyBigInt::MUL(MyBigInt multiplier)
{
    //return MyBigInt(karatsuba_mul(this->digits, multiplier.get_digits()));
    return MyBigInt(naive_mul(this->digits, multiplier.get_digits()));
}

MyBigInt MyBigInt::DIV(MyBigInt divisor)
{
    if (divisor.is_zero()) {
        throw new exception("Divisor is zero!");
    }

    vector<unsigned> result(this->digits.size());
    MyBigInt current;

    for (long long i = static_cast<long long>(this->digits.size()) - 1; i >= 0; i--) {
        current.shift_digits_right();
        current.digits[0] = this->digits[i];
        current.remove_leading_zeros();

        int x = 0, l = 0, r = MyBigInt::BASE;

        while (l <= r) {
            int m = (l + r) / 2;
            MyBigInt t = divisor.MUL(m);
            if (t.compare(current) <= 0) {
                x = m;
                l = m + 1;
            }
            else r = m - 1;
        }

        result[i] = x;
        current = current.SUB(divisor.MUL(x));
    }

    return MyBigInt(result);
}

MyBigInt MyBigInt::pow(MyBigInt n)
{
    MyBigInt result(1), current(*this);

    while (!n.is_zero()) {
        if (n.odd()) {
            result = result.MUL(current);
        }

        current = current.MUL(current);
        n = n.DIV(2);
    }
    return result;
}

MyBigInt MyBigInt::MOD(MyBigInt divisor) 
{
    MyBigInt result = this->SUB((this->DIV(divisor)).MUL(divisor));
    return result;
}

int MyBigInt::get_int()
{
    if (this->digits.size() == 1)
        return this->digits[0];
    return 0;
}

bool MyBigInt::odd()
{
    if (this->digits.size() == 0) return false;
    return this->digits[0] & 1;
}

vector<unsigned> MyBigInt::naive_mul(const vector<unsigned> x, const vector<unsigned> y)
{
    size_t len_l = x.size(), len_r = y.size();
    vector<unsigned> result(len_l + len_r);
    
    for (size_t i = 0; i < len_l; i++) {
        int carry = 0;
        for (size_t j = 0; j < len_r || carry != 0; j++) {
            unsigned long long cur = result[i + j] +
                x[i] * 1LL * (j < len_r ? y[j] : 0) + carry;
            result[i + j] = static_cast<unsigned>(cur % MyBigInt::BASE);
            carry = static_cast<unsigned>(cur / MyBigInt::BASE);
        }
    }

    return result;
}

void MyBigInt::extend_vec(vector<unsigned>& v, size_t len) {
    while (len & (len - 1)) {
        ++len;
    }
    v.resize(len);
}

vector<unsigned> MyBigInt::karatsuba_mul(vector<unsigned> x, vector<unsigned> y)
{
    size_t len = max(x.size(), y.size());
    extend_vec(x, len);
    extend_vec(y, len);
    len = x.size();

    vector<unsigned> result(len * 2);

    if (len == 1) {
        return this->naive_mul(x, y);
    }
    
    size_t k = len / 2;
    
    vector<unsigned> Xr {x.begin(), x.end() - k};
    vector<unsigned> Xl {x.begin() + k, x.end()};
    vector<unsigned> Yr {y.begin(), y.end() - k};
    vector<unsigned> Yl {y.begin() + k, y.end()};
    
    vector<unsigned> P1 = karatsuba_mul(Xl, Yl);
    vector<unsigned> P2 = karatsuba_mul(Xr, Yr);
        
    vector<unsigned> Xlr(k);
    vector<unsigned> Ylr(k);
    
    Xlr = digits_sum(Xl, Xr);
    Ylr = digits_sum(Yl, Yr);
    
    vector<unsigned> P3 = karatsuba_mul(Xlr, Ylr);
    
    P3 = digits_sub(P3, P2);
    P3 = digits_sum(P3, P1);
    
    for (unsigned i = 0; i < len; i++) {
        result[i] = P2[i];
    }

    for (size_t i = len; i < (2 * len); i++) {
        result[i] = P1[i - len];
    }

    for (size_t i = 0; i < k; i++) {
        P3.insert(P3.begin(), 0);
    }
    result = digits_sum(result, P3);

    return result;
}

vector<unsigned> MyBigInt::get_digits()
{
    return this->digits;
}

void MyBigInt::set_digits(vector<unsigned> digits)
{
    this->digits = digits;
    this->remove_leading_zeros();
}

void MyBigInt::shift_digits_right()
{
    if (this->digits.size() == 0) {
        this->digits.push_back(0);
        return;
    }
    this->digits.push_back(this->digits[this->digits.size() - 1]);

    for (size_t i = this->digits.size() - 2; i > 0; --i) {
        this->digits[i] = this->digits[i - 1];
    }

    this->digits[0] = 0;
}

bool MyBigInt::is_zero()
{
    return this->digits.empty() || (this->digits.size() == 1 && this->digits.at(0) == 0);
}

void MyBigInt::remove_leading_zeros()
{
    while (this->digits.size() > 1 && this->digits.back() == 0) {
        this->digits.pop_back();
    }
}

int MyBigInt::compare(MyBigInt other)
{
    if (this->is_zero() && other.is_zero()) 
        return 0;

    if (this->digits.size() == other.get_digits().size()) {
        for (int i = (int)this->digits.size() - 1; i >= 0; i--) {
            if (this->digits[i] > other.get_digits()[i]) {
                return 1;
            }
            else if (this->digits[i] < other.get_digits()[i]) {
                return -1;
            }
        }
    } 
    else {
        if (this->digits.size() > other.get_digits().size()) {
            return 1;
        }
        else {
            return -1;
        }
    }

    return 0;
}

ostream& operator<<(ostream& stream, const MyBigInt& bi)
{
    if (bi.digits.empty())
    {
        stream << 0;
    }
    else 
    {
        stream << bi.digits.back();
        char old_fill = stream.fill('0');

        for (long long i = bi.digits.size() - 2; i >= 0; i--) {
            stream << setw(9) << bi.digits[i];
        }

        stream.fill(old_fill);
    }

    return stream;
}

vector<unsigned> MyBigInt::digits_sum(vector<unsigned> x, vector<unsigned> y)
{
    size_t len = max(x.size(), y.size());
    int carry = 0;

    for (size_t i = 0; i < len || carry != 0; i++) {
        if (i == x.size()) 
            x.push_back(0);

        x[i] += carry + (i < y.size() ? y[i] : 0);
        carry = x[i] >= MyBigInt::BASE;

        if (carry != 0) 
            x[i] -= MyBigInt::BASE;
    }
    return x;
}

vector<unsigned> MyBigInt::digits_sub(vector<unsigned> x, vector<unsigned> y)
{
    if (x.size() < y.size())
        return vector<unsigned>(0);

    int p_result;
    int carry = 0;

    for (size_t i = 0; i < y.size() || carry != 0; i++) {
        p_result = x[i] - (carry + (i < y.size() ? y[i] : 0));

        carry = p_result < 0;
        if (carry != 0)
            p_result += MyBigInt::BASE;

        x[i] = p_result;
    }

    return x;
}

void test(string header, MyBigInt result, string expected)
{
    if (result.get_hex().compare(expected) == 0) {
        cout << "- " << header << " completed\n";
    }
    else {
        cout << "- " << header << " failed\n";
        cout << "result: " << result.get_hex() << endl;
        cout << "expected: " << expected << endl;
    }

}