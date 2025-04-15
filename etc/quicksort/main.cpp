#include <iostream>

using namespace std;

//{ 1, 10, 5, 8 ,7 ,6 ,4 ,3 ,2 ,9 };
//{ 3, 7, 8, 1 ,5 ,9 ,6 ,10 ,2 ,4 };
void quicksort(int* data, int start, int end) {
	if (start >= end) {	//���Ұ� �Ѱ��� ���
		return;
	}
	int pivot = start;
	int i = start + 1;
	int j = end;
	int temp;
	while (i <= j) {	//������ ������ �ݺ�
		while (data[i] <= data[pivot] && i <= end) {	//Ű ������ ū ��
			i++;
		}
		while (data[j] >= data[pivot] && j > start) {
			j--;
		}
		if (i > j) {
			temp = data[j];
			data[j] = data[pivot];
			data[pivot] = temp;
		}
		else {
			temp = data[j];
			data[j] = data[i];
			data[i] = temp;
		}
	}
	quicksort(data, start, j - 1);
	quicksort(data, j + 1, end);
}

void main(void) {
	int data[] = { 10, 1, 5, 8 ,7 ,6 ,4 ,3 ,2 ,9 };
	int dataSize = 10;

	quicksort(data, 0, 9);
	for (int i = 0; i < 10; i++) {
		cout << data[i] << ' ';
	}
}