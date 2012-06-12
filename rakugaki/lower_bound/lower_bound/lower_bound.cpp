// lower_bound.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

#include <assert.h>

#include <vector>
#include <algorithm>

// lower_bound�����삵�Ă݂��i���Ă��A���S���Y�����T�̃R�s�y�j�B�z��p�B
template <typename Type>
Type* lower_bound(Type* arrayBegin, Type* arrayEnd, Type key)
{
	// �񕪒T��
	std::less<Type> pred;
	size_t left = 0;
	size_t right = arrayEnd - arrayBegin;
	size_t mid;
	while (left < right)
	{
		mid = (left + right) >> 1;
		if (pred(arrayBegin[mid], key))	// [mid] < key <-> [mid] >= key (key�Ɠ�����������key���傫���v�f�̒��ōŏ��̃L�[�����v�f�̈ʒu��Ԃ�)
		{
			left = mid + 1;
		}
		else
		{
			right = mid;
		}
	}

	return arrayBegin + left;
}

// upper_bound
template <typename Type>
Type* upper_bound(Type* arrayBegin, Type* arrayEnd, Type key)
{
	// �񕪒T��
	std::less<Type> pred;
	size_t left = 0;
	size_t right = arrayEnd - arrayBegin;
	size_t mid;
	while (left < right)
	{
		mid = (left + right) >> 1;
		if (pred(arrayBegin[mid], key) || arrayBegin[mid] == key)	// [mid] <= key <-> [mid] > key (key���傫���v�f�̒��ōŏ��̃L�[�����v�f�̈ʒu��Ԃ�)
		{
			left = mid + 1;
		}
		else
		{
			right = mid;
		}
	}

	return arrayBegin + left;

}

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<int> sample;
	sample.push_back(1);
	sample.push_back(2);
	sample.push_back(5);
	sample.push_back(9);
	sample.push_back(10);
	sample.push_back(21);

	int key;
	int* found;
	std::vector<int>::iterator answer;

	key = 1;
	found = lower_bound(&sample[0], &sample[0] + sample.size(), key);
	answer = std::lower_bound(sample.begin(), sample.end(), key);
	assert(answer == sample.end() ? (found == &sample[0]+sample.size()) : (*found == *answer));

	key = 8;
	found = lower_bound(&sample[0], &sample[0] + sample.size(), key);
	answer = std::lower_bound(sample.begin(), sample.end(), key);
	assert(answer == sample.end() ? (found == &sample[0]+sample.size()) : (*found == *answer));

	key = 39;
	found = lower_bound(&sample[0], &sample[0] + sample.size(), key);
	answer = std::lower_bound(sample.begin(), sample.end(), key);
	assert(answer == sample.end() ? (found == &sample[0]+sample.size()) : (*found == *answer));

	key = 2;
	found = upper_bound(&sample[0], &sample[0] + sample.size(), key);
	answer = std::upper_bound(sample.begin(), sample.end(), key);
	assert(answer == sample.end() ? (found == &sample[0]+sample.size()) : (*found == *answer));

	key = 10;
	found = upper_bound(&sample[0], &sample[0] + sample.size(), key);
	answer = std::upper_bound(sample.begin(), sample.end(), key);
	assert(answer == sample.end() ? (found == &sample[0]+sample.size()) : (*found == *answer));

	key = 99;
	found = upper_bound(&sample[0], &sample[0] + sample.size(), key);
	answer = std::upper_bound(sample.begin(), sample.end(), key);
	assert(answer == sample.end() ? (found == &sample[0]+sample.size()) : (*found == *answer));

	return 0;
}

