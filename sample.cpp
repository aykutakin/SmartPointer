/**
 * MIT License
 *
 * Copyright (c) 2018 Aykut Akin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "SmartPointer.h"
#include <iostream>

using namespace std;

class dummy {
public:
  int i;
	int j;
	dummy() {i=0;j=0;}
	~dummy() {}
};

class derived : public dummy {
public:
	int x;
	int y;
	derived() {x=1;y=1;}
	~derived() {}
};

void test_kernel();
void test_features();
void test_equality();
void test_complicated();

int main() {
	test_kernel();
	test_features();
	test_equality();
	test_complicated();

	return 0;
}

void test_kernel() {
	SmartPointer<dummy> ptr1(1);
	SmartPointer<dummy> ptr_null;
	SmartPointer<dummy> ptr_arr1(2);

	(*ptr1).i = 2;
//	(*ptr_null).i = 2; //Error
	(*ptr_arr1).i = 2;
	cout << "(*ptr1).i:" << (*ptr1).i /*<< "(*ptr_null).i:" << (*ptr_null).i*/ << "\t(*ptr_arr1).i:" << (*ptr_arr1).i << endl;

	ptr1->i = 3;
//	ptr_null->i = 3; //Error
	ptr_arr1->i = 3;
	cout << "ptr1->i:" << ptr1->i /*<< "ptr_null->i:" << ptr_null->i*/ << "\tptr_arr1->i:" << ptr_arr1->i << endl;

	ptr1[0].i = 4;
//	ptr_null[0].i = 4; //Error
	ptr_arr1[0].i = 4;
	cout << "ptr1[0].i:" << ptr1[0].i /*<< " ptr_null[0].i:" << ptr_null[0].i*/ << "\tptr_arr1[0].i:" << ptr_arr1[0].i << endl;

//	ptr1[1].i = 5; //Error
//	ptr_null[1].i = 5; //Error
//	ptr_arr1[3].i = 5; //Error
//	cout << "ptr1[1].i:" << ptr1[1].i << " ptr_null[1].i:" << ptr_null[1].i << " ptr_arr1[3].i:" << ptr_arr1[3].i << endl;

	(ptr_arr1 + 1).i = 3;
	cout << ptr_arr1[1].i << endl;
}

void test_features() {
	SmartPointer<dummy> ptr1(1);
	SmartPointer<dummy> ptr_null;
	SmartPointer<dummy> ptr_arr1(2);

	ptr1.extend(2);
	ptr_null.extend(2);
	ptr_arr1.extend(3);
	ptr1[1].i = 1;
	ptr_null[1].i = 1;
	ptr_arr1[1].i = 1;
	cout << "ptr1[1].i:" << ptr1[1].i << "\tptr_null[1].i:" << ptr_null[1].i << "\tptr_arr1[1].i:" << ptr_arr1[1].i << endl;

	ptr1.shrink(1,0);
	ptr_null.shrink(0,0);
	ptr_arr1.shrink(2,1);
	cout << "ptr1[0].j:" << ptr1[0].j /*<< "\tptr_null[1].i:" << ptr_null[1].i*/ << "\tptr_arr1[1].j:" << ptr_arr1[1].j << endl;

	cout << ptr1.isNull() << endl;
	cout << ptr_null.isNull() << endl;
}

void test_equality() {
	SmartPointer<dummy> ptr1(1);
	SmartPointer<dummy> ptr_null;
	SmartPointer<dummy> ptr_arr1(2);
	
	{
		SmartPointer<dummy> ptr2(ptr1);
		SmartPointer<dummy> ptr3(1);
		SmartPointer<dummy> ptr4(1);
		
		ptr1->i = 2;
		ptr1->j = 2;
		ptr3 = ptr1;
		ptr1 = ptr4;
	}

	ptr_null = ptr1;
	ptr_null = ptr_arr1;
	ptr_null.setSmartPtr(new dummy,1);
	ptr_null->i = 5;
	dummy xx;
	ptr_null.setSmartPtr(&xx,1,false);
}

void func(SmartPointer<dummy> xxx) {
	cout << xxx->i << endl;
}

void test_complicated() {
	SmartPointer<dummy> ptr1(1);
	SmartPointer<dummy> ptr_null;
	SmartPointer<dummy> ptr_arr1(2);

	ptr_null = ptr1;
	ptr1.extend(2);
	ptr_null->i = 5;
	ptr_null.destroy();
	ptr_arr1.destroy();

	ptr_null.setSmartPtr(new derived,1);

	SmartPointer<derived> derived_ptr(1);
	derived_ptr->i = 15;
	func(derived_ptr);

	ptr_null = derived_ptr;
	SmartPointer<derived> der_ptr2;
	der_ptr2 = ptr_null;
	cout << der_ptr2->i << endl;
}
