#include <vector>

using namespace std;

struct coord
{
	size_t start; 
	size_t end;
	
	size_t GetSize() const
	{
		return end - start;
	}
};
////////////////////////////////////////////////////////////////////////////////

template<typename T>
void MergeSort(vector<T>& v, int K)
{
	// ������� ���������� ��������� � ������ �����
	int width = v.size() / K; 
	
	if(width * K < v.size())
		width++;
	
	// ������ ������ � ������
	coord mas[K];
	
	#pragma omp parallel for
	for(int i = 0; i < K; i++)
	{	
		mas[i].start = i * width;
		mas[i].end = (i == K - 1) ? v.size() :  (i + 1) * width;
		
		sort(v.begin() + mas[i].start, v.begin() + mas[i].end);
	}
	
	vector<T> temp(v.size());
	
	while( width < v.size() )
	{
		#pragma omp parallel for ordered  
		for(int i = 0; i < K / 2; i++)
		{		
			Merger(v, temp, mas[i * 2], mas[i * 2 + 1]);
			
			#pragma omp ordered
			{
				mas[i].start = mas[i * 2].start;
				mas[i].end = mas[i * 2 + 1].end;
			}
		}
		
		width *= 2;
		K /= 2;
		swap(v, temp);
	}
}

////////////////////////////////////////////////////////////////////////////////
// ������� �������
////////////////////////////////////////////////////////////////////////////////
template<typename T>
void Merger(vector<T>& v, vector<T>& temp, const coord& mas1, const coord& mas2)
{
	// ������� ��������� ��������
	int x1 = mas1.start, x2 = mas2.start, x3 = mas1.start;
	int	y1 = mas1.end - 1, y2 = mas2.end - 1;
	
	// ������� ���� �� �������� �������, ��� �������� ����������� ������ �� ������
	while(x1 <= y1 && x2 <= y2)
	{
		// ������� �� ������� �����, �������� �������� �����
		if(v[x1] <= v[x2])
		{
			swap(temp[x3], v[x1]);
			x3++;
			x1++;
		}
		// ������� �� ������� �����, �������� �������� �����
		else
		{
			swap(temp[x3], v[x2]);
			x3++;
			x2++;
		}
	}
	
	// ���� ������ ����� ��� �� ���������, �� ������� ���������� �������� 
	for(int i = x1; i <= y1; i++)
	{
		swap(temp[x3], v[i]);
		x3++;
	}
	
	// ���� ������ ����� ��� �� ���������, �� ������� ���������� ��������	 
	for(int i = x2; i <= y2; i++)
	{
		swap(temp[x3], v[i]);
		x3++;
	}
}