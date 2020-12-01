#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctime>
#include <thread>
#include <vector>
#include <omp.h>

using namespace std;

vector<int> forest;
vector<tuple<int, int>> tasks;
int number_of_tasks;
const int pooh = 0;

int count_threads()
{
	return (forest.size() / 10) + (forest.size() % 10 > 0 ? 1 : 0);
}

void get_tasks()
{
	int start = 0;
	int finish = 9;
	tasks.emplace_back(make_tuple(0, 9));
	for (size_t i = 0; i < number_of_tasks; i++)
	{
		start = finish + 1 > (int)forest.size() ? -1 : finish + 1;
		finish = finish + 10 > (int)forest.size() ? (forest.size() - 1) : (finish + (size_t)10);
		tasks.emplace_back(make_tuple(start, finish));
	}
}

/// <summary>
/// Search for Winnie the Pooh by one group of bees.
/// </summary>
void search_for_Winnie(tuple<int, int> cut)
{
	try {
		string result = "\nIn the sector [" + to_string(get<0>(cut)) + ';' +
			to_string(get<1>(cut)) + "] of forest Winnie the Pooh is not found.";
		for (int i = get<0>(cut); i < get<1>(cut); i++)
			if (forest[i] == pooh) {
				result = "\n    Winnie the Pooh detected in the sector [" + to_string(get<0>(cut)) + ';'
					+ to_string(get<1>(cut)) +
					"] of forest!\n    He's hiding in " + to_string(i) +
					" tree.\n    Let's start the attack!\n    BzzZZZzzzzZZ..!\n  " +
					"  The wrecker is punished, we return to the base. ";
				break;
			}
		cout << result;
	}
	catch (exception& ex)
	{
		cout << ex.what();
	}
}

/// <summary>
/// Bag of tasks distributed to threads.
/// That is, sections of the forest distributed by groups of bees.
/// </summary>
void run_bag_of_tasks()
{
#pragma omp parallel shared(forest) num_threads(number_of_tasks)
	{
#pragma omp for
		for (int i = 0; i < number_of_tasks; i++)
		{
			// getting a task from the bag
			search_for_Winnie(tasks[i]);
		}
	}
}

/// <summary>
/// Plants a forest and hides Winnie the Pooh.
/// </summary>
/// <param name="length"> The number of trees in the forest. </param>
void create_forest(int length)
{
	int const poohs_shelter = rand() % length;
	for (int i = 0; i < length; i++)
		if (i != poohs_shelter)
			forest.push_back(1);
		else
			forest.push_back(0);
	number_of_tasks = count_threads();
}


/// <summary>
/// Entering a number from the console and checking it.
/// </summary>
/// <returns> The entered number. </returns>
int input_length()
{
	int length = -1;
	string tmp;
	cout << "Enter the number of trees in the forest:\n(a number between 30 and 2147483647)\n";
	try {
		cin >> tmp;
		length = stoi(tmp);
		if (length < 30)
			cout << "\nWrong number of trees in the forest.\nIt should be a number between 30 and 2147483647.\n";
	}
	catch (exception&) {
		cout << "\nInvalid argument. \nIt should be a number between 30 and 2147483647.\n";
	}
	return length;
}

int main()
{
	srand((int)time(0));
	int length = input_length();
	if (length == -1)
	{
		system("pause");
		return 0;
	}
	create_forest(length);

	get_tasks();
	run_bag_of_tasks();

	cout << endl;
	system("pause");
	forest.clear();
	tasks.clear();
	return 0;
}