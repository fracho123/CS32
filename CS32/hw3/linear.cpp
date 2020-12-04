#include <cassert>


bool somePredicate(double x) {
	if (x <= 0)
		return false;
	return true;
}



// Return true if the somePredicate function returns false for at
// least one of the array elements; return false otherwise.
bool anyFalse(const double a[], int n)
{	
	if (n <= 0)
		return false;
	
	//if it returns true, recursion
	if (somePredicate(*a))
		return anyFalse(a + 1, n - 1);
	
	//if it returns false, end and return true
	return true;
}

// Return the number of elements in the array for which the
// somePredicate function returns true.
int countTrue(const double a[], int n)
{
	if (n <= 0)
		return 0;
	if (somePredicate(*a))
		return 1 + countTrue(a + 1, n - 1);
	return 0 + countTrue(a + 1, n - 1);
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n)
{
	if (n <= 0)
		return -1;
	if (somePredicate(*a))
		return 0;
	if (firstTrue(a + 1, n - 1) == -1)
		return -1;
	return 1 + firstTrue(a + 1, n - 1);
}

// Return the subscript of the smallest element in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told to examine no
// elements, return -1.
int positionOfSmallest(const double a[], int n)
{
	if (n <= 0)
		return -1;
	if (n == 1)
		return 0;

	int backend = positionOfSmallest(a, n - 1);
	if (a[n-1] < a[backend])
		return n-1;
	return backend;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not contain
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool contains(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0)
		return true;
	if (n1 <= 0) //and n2 is greater than 0
		return false;
	if (*a1 != *a2)
		return contains(a1 + 1, n1 - 1, a2, n2);
	return contains(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
}


int main() {
	double a[] = { -9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,6,7,8,9 };
	double b[] = { 1,2,3,4,5,6,7,8,9 };
	double c[] = { -9,-8,-7,-6,-5,-4,-3,-2,-1,-8,-7,-6,-5,-4,-3,-2,-1,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,6,7,8,9 };
	double d[] = { 10, 50, 40, 20, 50, 40, 30 };
	double e[] = { 50, 20, 30 };
	double f[] = { 50, 40, 40 };
	double g[] = { 50, 30, 20 };
	double h[] = { 10, 20, 20 };

	assert(anyFalse(a, 19));
	assert(!anyFalse(b, 9));
	assert(firstTrue(a, 19) == 10);
	assert(firstTrue(a, 9) == -1);
	assert(firstTrue(b, 9) == 0);
	assert(firstTrue(c, 36) == 26);
	assert(countTrue(a, 19) == 9);
	assert(countTrue(b, 9)== 9);
	assert(positionOfSmallest(a, 19) == 0);
	assert(positionOfSmallest(b, 9) == 0);
	assert(positionOfSmallest(c, 36) == 0);
	assert(positionOfSmallest(d, 7) == 0);
	assert(positionOfSmallest(e, 3) == 1);
	assert(positionOfSmallest(g, 3) == 2);
	assert(contains(d, 7, e, 3));
	assert(contains(d, 7, f, 3));
	assert(!contains(d, 7, g, 3));
	assert(!contains(d, 7, h, 3));
}