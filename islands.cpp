#include <iostream>
#include <map>
#include <vector>
#include <algorithm>


#define DEBUG 1
#define STEPS 20
using namespace std;

size_t result = 0;


class Island {
public:
	char name;
	vector<int> goods_cost;

	Island(char name, vector<int> &goodsCost)
			: name(name), goods_cost(goodsCost) {}
};

struct Data {
	map<char, Island> islands;
	map<char, map<char, int>> paths;
	int island_number{-1};
	int paths_number{-1};
	int goods_number{-1};
	char starting_island_c{'\0'};

	Data() = default;

	void load() {
		cin >> island_number >> paths_number >> goods_number;

		char c;
		int temp;
		for (int i = 0; i < island_number; i++) {
			cin >> c;
			if (i == 0) {
				starting_island_c = c;
			}
			vector<int> temp_vec;
			for (int j = 0; j < goods_number; j++) {
				cin >> temp;
				temp_vec.push_back(temp);
			}
			paths.insert({c, map<char, int>()});
			islands.insert({c, Island(c, temp_vec)});
		}

		char second_island_c;
		for (int i = 0; i < paths_number; i++) {
			cin >> c >> second_island_c >> temp;
			paths.at(c).insert({second_island_c, temp});

		}

		if (DEBUG) {
			cerr << "Islands: " << island_number << endl;
			cerr << "Paths: " << paths_number << endl;
			cerr << "Goods number: " << goods_number << endl;
			cerr << "Starting island: " << starting_island_c << endl;
		}
	}
};

class Traveler {
public:
	Data &data;
	Island current_island;
	int steps_done;
	int current_money;
	vector<int> goods_amount;
	map<char, bool> visited{};
	int visited_number{1};

	explicit Traveler(Data &data) : data(data), current_island(
			data.islands.at(data.starting_island_c)),
	                                steps_done(0),
	                                current_money(100),
	                                goods_amount(data.goods_number, 0) {
		for (auto &i: data.islands) {
			visited.insert({i.first, false});
		}
		visited.at(current_island.name) = true;
	}

	int cost_between(Island &next) {
		return data.paths.at(current_island.name).at(next.name);
	}

	void sell_all() {
		for (int i = 0; i < data.goods_number; ++i) {
			current_money += current_island.goods_cost[i] * goods_amount[i];
			goods_amount[i] = 0;
		}
	}

	int get_total_money() {
		int total_money = current_money;
		for (int i = 0; i < data.goods_number; ++i) {
			total_money += current_island.goods_cost[i] * goods_amount[i];
		}
		return total_money;
	}

	[[nodiscard]] bool all_visited() const {
		return visited_number == data.island_number;
	}

	[[nodiscard]] bool jurney_done_successfully() const {
		return all_visited() && (current_island.name == data.starting_island_c);
	}

	static void print_status(int total_money) {
		if (total_money > result) {
			result = total_money;
			if (DEBUG) {
				cerr << "Nowy wynik:" << result << endl;
			};
		}

	}

	bool check_next_island(Island &next_island) {
		int travel_cost = data.paths.at(current_island.name).at(
				next_island.name);
		int total_money = get_total_money();
		if (steps_done >= STEPS ||
		    STEPS - steps_done < data.island_number - visited_number) {
			if (jurney_done_successfully()) {
				print_status(total_money);
			}
			return false;
		}
		if (travel_cost > total_money) {
			return false;
		}
		return true;
	}

	void set_visited() {
		if (!visited.at(current_island.name)) {
			visited.at(current_island.name) = true;
			visited_number++;
		}
	}

	void travel_to_next_island(Island &next_island) {
		int travel_cost = cost_between(next_island);
		current_money -= travel_cost;
		current_island = data.islands.at(next_island.name);
		set_visited();
		steps_done++;
	}

	void buy(int material_id, int money_to_keep) {
		if (material_id == -1 || money_to_keep >= current_money) {
			return;
		}
		current_money -= money_to_keep;
		int amount = (current_money / current_island.goods_cost[material_id]);
		goods_amount.at(material_id) += amount;
		current_money -= current_island.goods_cost[material_id] * amount;
		current_money += money_to_keep;
	}

	int find_best_material_id(Island &next_island) const {
		int best_material_id = -1;
		int biggest_difference = -1;
		for (int i = 0; i < data.goods_number; ++i) {
			int difference =
					next_island.goods_cost[i] - current_island.goods_cost[i];
			if (difference > 0 && difference > biggest_difference) {
				biggest_difference = difference;
				best_material_id = i;
			}
		}
		return best_material_id;
	}

	bool can_visit_next_island(Island &next_island) {
		if (!check_next_island(next_island)) {
			return false;
		}
		int travel_cost = cost_between(next_island);
		int material_id = find_best_material_id(next_island);
		sell_all();
		buy(material_id, travel_cost);
		travel_to_next_island(next_island);
		return true;
	}

};

class Simulation {
public:

	static void
	try_to_visit(Traveler traveler, Island &next_island, Data &data) {
		if (traveler.can_visit_next_island(next_island)) {
			visit(traveler, data);
		}
	}

	static void visit(Traveler &traveler, Data &data) {
		auto &current_island = traveler.current_island;
		if (data.paths.at(current_island.name).empty()) {
			return;
		}
		for (auto &map_element: data.paths.at(current_island.name)) {
			auto &next_island = data.islands.at(map_element.first);
			try_to_visit(traveler, next_island, data);
		}
	}

	static void print_result() {
		if (result == 0) {
			printf("Nie istnieje żadne poprawne rozwiązanie.\n");
			return;
		}
		printf("Największy wynik uzyskany metodą zachłanną: %zu\n", result);
	}
};


int main() {

	Data data;
	data.load();
	Traveler new_traveler = Traveler(data);
	Simulation::visit(new_traveler, data);
	Simulation::print_result();
	return 0;
}