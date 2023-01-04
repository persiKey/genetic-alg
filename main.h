#pragma once

#include <array>
#include <utility>

using std::array;
using std::pair;

constexpr int BAG_CAPACITY = 250;
constexpr int OBJS_AMOUNT = 100;
constexpr int INDS_AMOUNT = 100;

constexpr int GENS_AMOUNT = OBJS_AMOUNT;

constexpr int LOW_WORTH_BOUND = 2;
constexpr int UP_WORTH_BOUND = 30;

constexpr int LOW_WEIGHT_BOUND = 1;
constexpr int UP_WEIGHT_BOUND = 25;

constexpr int MUT_CHANCE = 5;



struct OBJS_WORTH
{
	const array<int, OBJS_AMOUNT> value;
	OBJS_WORTH();
};

struct OBJS_WEIGHT
{
	const array<int, OBJS_AMOUNT> value;
	OBJS_WEIGHT();
};

struct Task
{
	OBJS_WORTH worth;
	OBJS_WEIGHT weight;
};




class Population
{
private:
	struct Individ
	{
		int sum_value = 0;
		int weight = 0;
		bool genome[OBJS_AMOUNT] = {};

		inline bool IsAlive();
		void UpdateWeight(const OBJS_WEIGHT&);
		void UpdateWorth(const OBJS_WORTH&);
	};

	Individ individs[INDS_AMOUNT];
	int iteration = 0;
	int max_sum = 0;
	Individ* best;
	Task* task;
	int attraction[OBJS_AMOUNT];
public:
	Population();
	int GetItertion() const;

	void Iterate();

	void PrintWorthValue();
	void PrintIndivid(int n);
	void PrintObject(int n);
	void PrintMax();
	int GetMaxWorth();
	~Population();
};