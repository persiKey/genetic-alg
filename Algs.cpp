#include <time.h>
#include <random>
#include "main.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

OBJS_WORTH::OBJS_WORTH() : value { []{
		array<int, OBJS_AMOUNT> v;
		for (size_t i = 0; i < OBJS_AMOUNT; i++)
		{
			v[i] = rand() % UP_WORTH_BOUND + LOW_WORTH_BOUND;
		}

		return std::move(v); }() }
{

}

OBJS_WEIGHT::OBJS_WEIGHT() : value { []{
		array<int, OBJS_AMOUNT> v;
		for (size_t i = 0; i < OBJS_AMOUNT; i++)
		{
			v[i] = rand() % UP_WEIGHT_BOUND + LOW_WEIGHT_BOUND;
		}

		return std::move(v); }() }
{

}

bool Population::Individ::IsAlive()
{
	return weight <= BAG_CAPACITY;
}

void Population::Individ::UpdateWeight(const OBJS_WEIGHT& WEIGHT)
{
	weight = 0;
	for (size_t i = 0; i < OBJS_AMOUNT; ++i)
	{
		weight += genome[i] * WEIGHT.value[i];
	}
}

void Population::Individ::UpdateWorth(const OBJS_WORTH& WORTH)
{
	sum_value = 0;
	for (size_t i = 0; i < OBJS_AMOUNT; ++i)
	{
		sum_value += genome[i] * WORTH.value[i];
	}
}

Population::Population() : task( new Task())
{
	for (size_t i = 0; i < INDS_AMOUNT; i++)
	{
		individs[i].genome[i] = true;
		individs[i].sum_value += task->worth.value[i];
		individs[i].weight += task->weight.value[i];
	}

	best = individs + 0;
	max_sum = individs[0].sum_value;

	pair<int, float> attr[OBJS_AMOUNT];
	for (size_t i = 0; i < OBJS_AMOUNT; i++)
	{
		attr[i].first = i;
		attr[i].second = (float)task->worth.value[i] / task->weight.value[i];
	}
	std::sort(attr + 0, attr + OBJS_AMOUNT,
		[](const pair<int, float>& a, const pair<int, float>& b) 
		{return a.second > b.second; });

	for (size_t i = 0; i < OBJS_AMOUNT; i++)
		attraction[i] = attr[i].first;

}

Population::~Population()
{
	delete task;
}

int Population::GetItertion() const
{
	return iteration;
}

void Population::Iterate()
{
	Individ* parents[2];
	Individ offspring;

	auto cmp = [](const Individ& a, const Individ& b)
	{ return a.sum_value < b.sum_value; };

	{
		int pivot = rand() % INDS_AMOUNT;


		parents[0] = std::max_element(individs + 0, individs + pivot, cmp);

		parents[1] = std::max_element(individs + pivot, individs + INDS_AMOUNT, cmp);
	}

	{
		int pivot1 = rand() % (GENS_AMOUNT - 3) + 1;
		int pivot2 = rand() % (GENS_AMOUNT - pivot1 - 2) + pivot1 + 1;
		int pivot3 = rand() % (GENS_AMOUNT - pivot2 - 1) + pivot2 + 1;
	
		memcpy(offspring.genome, parents[0]->genome, pivot1);
		memcpy(offspring.genome + pivot1, parents[1]->genome + pivot1, pivot2 - pivot1);
		memcpy(offspring.genome + pivot2, parents[0]->genome + pivot2, pivot3 - pivot2);
		memcpy(offspring.genome + pivot3, parents[1]->genome + pivot3, GENS_AMOUNT - pivot3);
	
		offspring.UpdateWeight(task->weight);

		if (!offspring.IsAlive())
		{
			return;
		}
	}

	{
		if (rand() % 100 < MUT_CHANCE)
		{
			int point = rand() % INDS_AMOUNT;
			offspring.weight += (offspring.genome[point] = !offspring.genome[point] ? 1 : -1) * task->weight.value[point];
			if (!offspring.IsAlive())
			{
				offspring.genome[point] = 0;
				offspring.weight -= task->weight.value[point];
			}
		}
	}
	
	{
		int num;

		for (int i = OBJS_AMOUNT - 1; i >= 0; --i)
		{
			num = attraction[i];
			if (offspring.genome[num])
			{
				offspring.genome[num] = false;
				offspring.weight -= task->weight.value[num];
				break;
			}
		}
		int free_space = BAG_CAPACITY - offspring.weight;
		for (size_t i = 0; i < OBJS_AMOUNT; i++)
		{
			num = attraction[i];
			if (!offspring.genome[num] && task->weight.value[num] <= free_space)
			{
				offspring.genome[num] = true;
				offspring.weight += task->weight.value[num];
				break;
			}
		}
	}
	
	{
		offspring.UpdateWorth(task->worth);

		size_t min_index = std::min_element(individs + 0, individs + OBJS_AMOUNT, cmp) - (individs + 0);
		individs[min_index] = offspring;

		if (offspring.sum_value > max_sum)
		{
			max_sum = offspring.sum_value;
			best = individs + min_index;
		}
	}
}

void Population::PrintWorthValue()
{
	using namespace std;

	for (size_t i = 0; i < OBJS_AMOUNT; i++)
	{
		cout << "No\t" << setw(3) << i << '\n';
		cout << "Weight\t" << setw(3) << task->weight.value[i] << '\n';
		cout << "Worth\t" << setw(3) << task->worth.value[i] << '\n';
		cout << "=================\n";

	}
}

void Population::PrintIndivid(int n)
{
	using namespace std;
	if (n < 0 || n >= OBJS_AMOUNT)
	{
		return;
	}
	cout << "No\t" << setw(3) << n << '\n';
	cout << "Weight\t" << setw(3) << individs[n].weight << '\n';
	cout << "Worth\t" << setw(3) << individs[n].sum_value << '\n';
	cout << "=================\n";

	for (int i = 0; i < GENS_AMOUNT; i++)
	{
		cout << i << " : " << individs[n].genome[i] << "; ";
	}
	cout << endl;
}

void Population::PrintObject(int n)
{
	using namespace std;
	if (n < 0 || n >= OBJS_AMOUNT)
	{
		return;
	}
	cout << "No\t" << setw(3) << n << '\n';
	cout << "Weight\t" << setw(3) << task->weight.value[n] << '\n';
	cout << "Worth\t" << setw(3) << task->worth.value[n] << '\n';
	cout << "=================\n";
}

void Population::PrintMax()
{
	PrintIndivid(best - individs);
}

int Population::GetMaxWorth()
{
	return max_sum;
}

