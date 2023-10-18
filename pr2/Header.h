#pragma once
#include <iostream>
#include <bitset>
#include <cstdint>
#include <iomanip>
#include <vector>
using namespace std;

class MyBigInt {
	// Вектор для зберігання числа розбитого на блоки по BASE чисел
	vector<unsigned> digits;
	// Кількість чисел на один елемент вектору
	static const int BASE = 1000000000;
	// HEX алфавіт
	static const string HEX;

public:
	MyBigInt();
	MyBigInt(unsigned long long);
	MyBigInt(vector<unsigned>);

	// Сеттери і геттери hex значень
	void set_hex(string);
	string get_hex();

	// Методи для побітових операцій	
	MyBigInt INV();
	MyBigInt XOR(MyBigInt);
	MyBigInt OR(MyBigInt);
	MyBigInt AND(MyBigInt);

	// Методи для арифметичних операцій
	MyBigInt ADD(MyBigInt);
	MyBigInt SUB(MyBigInt);
	MyBigInt MOD(MyBigInt);

	MyBigInt MUL(MyBigInt);
	MyBigInt DIV(MyBigInt);

	// Додаткові методи
	vector<unsigned> get_digits();
	void set_digits(vector<unsigned>);

	bool is_zero();
	void shift_digits_right();
	void remove_leading_zeros();
	int compare(MyBigInt);
	MyBigInt pow(MyBigInt);
	int get_int();

	friend ostream& operator<<(ostream& stream, const MyBigInt& bi);

private:
	bool odd();
	vector<unsigned> naive_mul(vector<unsigned>, vector<unsigned>);
	void extend_vec(vector<unsigned>& v, size_t len);
	vector<unsigned> karatsuba_mul(vector<unsigned>, vector<unsigned>);
	vector<unsigned> digits_sum(vector<unsigned>, vector<unsigned>);
	vector<unsigned> digits_sub(vector<unsigned>, vector<unsigned>);
};

void test(string, MyBigInt, string);