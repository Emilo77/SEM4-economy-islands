#include <iostream>
#include <map>
#include <vector>
using namespace std;

struct island {
	char name;
	vector<int> goods_cost;

	island(char name, vector<int> &goodsCost)
	    : name(name), goods_cost(goodsCost) {}
};

map<char, island> islands;
map<char, map<char, int>> paths;
int island_number;
int paths_number;
int goods_number;

struct traveler {
	int steps_done;
	island current_island;
	map<char, bool> visited{};
	int current_money;
	vector<int> goods_amount;

	explicit traveler(island &currentIsland) : current_island(currentIsland), steps_done(0),
	                                  current_money(100), goods_amount(goods_number, 0) {
		for(auto &i : islands) {
			visited.insert({i.first, false});
		}
		visited.at(currentIsland.name) = true;  // mark current island as visited
	}


	void sell_all() {
		for(int i = 0; i < goods_number; ++i) {
			current_money += current_island.goods_cost[i] * goods_amount[i];
			goods_amount[i] = 0;
		}
	}

	bool check_next_island(island &next_island) {
		int travel_cost = paths.at(current_island.name).at(next_island.name);
		if (travel_cost > current_money || steps_done >= 20) {
			return false;
		}
		return true;
	}

	void travel_to_next_island(island &next_island) {
		int travel_cost = paths.at(current_island.name).at(next_island.name);
		current_money -= travel_cost;
		current_island = islands.at(next_island.name);
		steps_done++;
	}

	void buy(char material, int money_to_keep) {
		if (money_to_keep >= current_money) {
			return;
		}
		int amount;
		current_money -= money_to_keep;
		switch (material) {
			case 'w':
				amount = (current_money / current_island.w_cost);
				current_money -= amount * current_island.w_cost;
				w_material += amount;
				break;
			case 'x':
				amount = (current_money / current_island.x_cost);
				current_money -= amount * current_island.x_cost;
				x_material += amount;
				break;
			case 'y':
				amount = (current_money / current_island.y_cost);
				current_money -= amount * current_island.y_cost;
				y_material += amount;
				break;
			case 'z':
				amount = (current_money / current_island.z_cost);
				current_money -= amount * current_island.z_cost;
				z_material += amount;
				break;
			default:
				break;
		}
		current_money += money_to_keep;
	}

	char find_best_material(island &next_island) const {
		int w_diff = next_island.w_cost - current_island.w_cost;
		int x_diff = next_island.x_cost - current_island.x_cost;
		int y_diff = next_island.y_cost - current_island.y_cost;
		int z_diff = next_island.z_cost - current_island.z_cost;

		int biggest_diff = max(w_diff, max(x_diff, max(y_diff, z_diff)));
		if (biggest_diff < 0) {
			return '-';
		}
		if (biggest_diff == w_diff) {
			return 'w';
		} else if (biggest_diff == x_diff) {
			return 'x';
		} else if (biggest_diff == y_diff) {
			return 'y';
		} else {
			return 'z';
		}
	}

	bool visit_next_island(island &next_island) {
		if (!check_next_island(next_island)) {
			return false;
		}
		int travel_cost = paths.at(current_island.name).at(next_island.name);
		char material = find_best_material(next_island);
		sell_all();
		buy(material, travel_cost);
		travel_to_next_island(next_island);
		return true;
	}
};


void generate_result(traveler traveler) {
	auto &current_island = traveler.current_island;

}

void load_data() {

	cin >> island_number >> paths_number;

	char c;
	char second_island_c;
	int temp;
	for (int i = 0; i < island_number; i++) {
		cin >> c;
		vector<int> temp_vec;
		for(int j = 0; j < goods_number; j++) {
			cin >> temp;
			temp_vec.push_back(temp);
		}

		islands.insert({c, island(c, temp_vec)});
	}

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

int main() {

	load_data();
	cout << islands.size() << endl;
	traveler new_traveler = traveler(islands.at('A'));

	generate_result(new_traveler);

	return 0;
}
