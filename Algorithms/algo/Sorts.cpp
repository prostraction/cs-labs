#include <iostream>
#include <algorithm>

void show(int arr[], int size)
{
	for (int i = 0; i < size; i++)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
}

void selection_sort(int arr[], int size)
{
	int smallest_index;
	for (int start = 0; start < size - 1; start++)
	{
		smallest_index = start;
		for (int current = start + 1; current < size; current++)
		{
			if (arr[current] < arr[smallest_index])
				smallest_index = current;
		}
		std::swap(arr[start], arr[smallest_index]);
	}
}

void shell_sort(int arr[], int size)
{
	int temp;
	int k, j, i;
	for(k = size / 2; k > 0; k = k / 2)
        for(i = k; i < size; i++)
        {
            temp = arr[i];
            for(j = i; j >= k; j = j - k)
            {
                if(temp < arr[j - k])
                    arr[j] = arr[j - k];
                else
                    break;
            }
            arr[j] = temp;
        }
}

int main()
{
	int array[12]; 
	for (int i = 0; i < 12; i++)
		array[i] = 12 - i;
	show(array, 12);
	selection_sort(array, 12); show(array, 12);
	for (int i = 0; i < 12; i++)
		array[i] = 12 - i;
	std::cout << "\n";
	show(array, 12);
	shell_sort(array, 12);
	show(array, 12);
}