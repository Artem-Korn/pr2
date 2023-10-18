#include "Header.h"

int main()
{
	MyBigInt numberA, numberB;
	string hex_array[11] = { 
		"51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4", //A
		"403db8ad88a3932a0b7e8189aed9eeffb8121dfac05c3512fdb396dd73f6331c", //B

		"1182d8299c0ec40ca8bf3f49362e95e4ecedaf82bfd167988972412095b13db8", //XOR
		"403d208400a113220340808088d16a1b10121078400c1002748196dd62460204", //AND
		"51bff8ad9cafd72eabffbfc9befffffffcffbffaffdd779afdf3d7fdf7f73fbc", //OR

		"91fd19319d50ea50af40404a47d16a1b0d11d0733fe9879d72756edb5a3d41c0", //ADD
		"1181a7d68c09c3fc98433d36ea1d8c1b9ced947dbf311d77770e41207250db88", //SUB
		"1181a7d68c09c3fc98433d36ea1d8c1b9ced947dbf311d77770e41207250db88", //MOD

		"1", //DIV
		"14838db3fbf95254ed7fac950c07d63870966ca0c3d64f74cf0e0e9d4e5b806db2ea7c7e4a90f5f94389230c648dcc6e8e0ed99c1b5f5680be402415154845f0", //MUL
		"d99901d7d24d9ea77b3f49854181b1514ce58fe174491f9c2c76bf4b35468c8c294c35d0b5198b576f4df76abdf15190c9358944bd97de1f74abbcb16ded35a617652c128e1161c807f6ba8e705d0104d508b3599420f68607d4bb6849ddc008ca9168e35cfaaabd2edabbdbc250efe9cd6f23a170f591a2c4f84663a78bdce41481960bdf15372f2941d249cf73aaae6f92fe07af531337867f77b9532400" //POW 5
	};

	// Тестування завдання 1
	cout << "Test 1:\n";

	numberA.set_hex(hex_array[0]);
	test("hexTest1", numberA, hex_array[0]);

	numberA.set_hex(hex_array[1]);
	test("hexTest2", numberA, hex_array[1]);

	numberA.set_hex(hex_array[4]);
	test("hexTest3", numberA, hex_array[4]);

	// Тестування завдання 2
	cout << "Test 2:\n";
	numberA.set_hex(hex_array[0]);
	numberB.set_hex(hex_array[1]);

	test("INVTest", numberA.INV().INV(), hex_array[0]);
	/*test("XORTest", numberA.XOR(numberB), hex_array[2]);
	test("ANDTest", numberA.AND(numberB), hex_array[3]);
	test("ORTest", numberA.OR(numberB), hex_array[4]);*/
	
	// Тестування завдання 3
	cout << "Test 3:\n";
	numberA.set_hex(hex_array[0]);
	numberB.set_hex(hex_array[1]);

	test("ADDTest", numberA.ADD(numberB), hex_array[5]);
	test("SUBTest", numberA.SUB(numberB), hex_array[6]);
	test("MODTest", numberA.MOD(numberB), hex_array[7]);
	test("DIVTest", numberA.DIV(numberB), hex_array[8]);
	test("MULTest", numberA.MUL(numberB), hex_array[9]);
	test("powTest", numberA.pow(5), hex_array[10]);

}
