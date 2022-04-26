#include <iostream>
#include <map>
#include <vector>

using namespace std;


struct Island {
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

			islands.insert({c, Island(c, temp_vec)});
		}

		char second_island_c;
		for (int i = 0; i < paths_number; i++) {
			cin >> c >> second_island_c >> temp;
			if (paths.find(c) == paths.end()) {
				auto tempMap = map<char, int>();
				tempMap.insert({second_island_c, temp});
				paths.insert({c, tempMap});
			} else {
				paths.at(c).insert({second_island_c, temp});
			}
		}
	}

};

struct Traveler {
	Data data;
	int steps_done;
	Island current_island;
	map<char, bool> visited{};
	int current_money;
	vector<int> goods_amount;

	explicit Traveler(Data &data) : data(data), current_island(
			data.islands.at(data.starting_island_c)),
	                                steps_done(0),
	                                current_money(100),
	                                goods_amount(data.goods_number, 0) {
		for (auto &i: data.islands) {
			visited.insert({i.first, false});
		}
		//visited.at(current_island.name) = true;// mark current Island as visited
	}


	void sell_all() {
		for (int i = 0; i < data.goods_number; ++i) {
			current_money += current_island.goods_cost[i] * goods_amount[i];
			goods_amount[i] = 0;
		}
	}

	bool check_next_island(Island &next_island) {
		int travel_cost = data.paths.at(current_island.name).at(
				next_island.name);
		if (travel_cost > current_money || steps_done >= 20) {
			return false;
		}
		return true;
	}

	void travel_to_next_island(Island &next_island) {
		int travel_cost = data.paths.at(current_island.name).at(
				next_island.name);
		current_money -= travel_cost;
		current_island = data.islands.at(next_island.name);
		steps_done++;
	}

	void buy(int material_id, int money_to_keep) {
		if (money_to_keep >= current_money) {
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
		for (int i = 0; i < data.goods_number; ++i) {
			int difference =
					next_island.goods_cost[i] - current_island.goods_cost[i];
			if (difference > 0 && difference > best_material_id) {
				best_material_id = difference;
			}
		}
		return best_material_id;
	}


	bool visit_next_island(Island &next_island) {
		if (!check_next_island(next_island)) {
			return false;
		}
		int travel_cost = data.paths.at(current_island.name).at(
				next_island.name);
		int material_id = find_best_material_id(next_island);
		sell_all();
		buy(material_id, travel_cost);
		travel_to_next_island(next_island);
		return true;
	}

};

void generate_result(Traveler traveler) {
	auto &current_island = traveler.current_island;
}


int main() {

	Data data;
	data.load();
	cout << data.islands.size() << endl;
	Traveler new_traveler = Traveler(data);

	generate_result(new_traveler);

	return 0;
}
