#include "Strategy.h"
#include "PublicTraffic.h"

size_t Strategy::Route::getLineChangesNum() const {
	size_t num = 1;
	for (size_t i = 0; i < stops_.size() - 1; i++) {
		if (stops_[i].second != stops_[i + 1].second) num++;
	}
	return num;
}

string Strategy::Route::toString() {
	string ret = ("->" + stops_[0].second + "\n" + to_string(stops_[0].first));
	for (size_t i = 1; i < stops_.size(); i++) {
		ret.append(" " + to_string(stops_[i].first));

		if (stops_[i].second != stops_[i - 1].second) {
			ret.append("\n" + stops_[i - 1].second + "->" + stops_[i].second + "\n" + to_string(stops_[i].first));
		}
	}

	ret.append("\n");
	return ret;
}


/*********************** BASE STRATEGY ***********************/

void BaseStrategy::execute(uint16_t from, uint16_t to) {
	if (from == to) {
		cout << "*** Nevalidna stajalista. ***\n";
		return;
	}
	last_station = to;
	PublicTraffic& pt = PublicTraffic::getInstance();

	unordered_map<uint16_t, bool> visited;
	for (const auto& it : pt.stations_) {
		visited.insert({ it.first, false });
	}

	bool find = false;
	for (const auto& it : pt.stations_[from].next_stop_) {
		Route r;
		if (DFS(from, it.first, r, visited)) {
			ofstream file;
			file.open("putanja_" + to_string(from) + "_" + to_string(to) + ".txt");
			file << r.toString();
			file.close();
			cout << "*** Putanja izmedju stajalista " << from << " i " << to;
			cout << " nalazi se u fajlu " << "'putanja_" << from << "_" << to << ".txt'. ***\n\n";
			find = true;
			break;
		}
	}
	if (!find) cout << "*** Nevalidna stajalista. ***\n";
}


bool BaseStrategy::DFS(uint16_t station_id, string line_id, Route& route, unordered_map<uint16_t, bool>& visited) {

	visited[station_id] = true;
	PublicTraffic& pt = PublicTraffic::getInstance();

	route.stops_.push_back({ station_id, line_id });

	uint16_t next = pt.stations_[station_id].next_stop_[line_id];
	if (visited[next]) return false;
	if (next == last_station) {
		route.stops_.push_back({ next, line_id });
		return true;
	}

	for (const auto& it : pt.stations_[next].next_stop_) {
		if (DFS(next, it.first, route, visited)) {
			return true;
		}
		else {
			visited[next] = false;
			route.stops_.pop_back();
		}
	}

	return false;
}



/*********************** LEAST STOPS STRATEGY ***********************/

void LeastStopsStrategy::execute(uint16_t from, uint16_t to) {
	if (from == to) {
		cout << "*** Nevalidna stajalista. ***\n";
		return;
	}
	last_station = to;
	PublicTraffic& pt = PublicTraffic::getInstance();

	unordered_map<uint16_t, bool> visited;
	for (const auto& it : pt.stations_) {
		visited.insert({ it.first, false });
	}

	for (const auto& it : pt.stations_[from].next_stop_) {
		Route r;
		DFS(from, it.first, r, visited);
	}

	sort(routes.begin(), routes.end(), [](Route const &a, Route const &b) {
		return a.getStopsNum() < b.getStopsNum();
	});

	if (!routes.empty()) {
		ofstream file;
		file.open("putanja_" + to_string(from) + "_" + to_string(to) + ".txt");
		file << routes.at(0).toString();
		file.close();
		cout << "*** Putanja izmedju stajalista " << from << " i " << to;
		cout << " nalazi se u fajlu " << "'putanja_" << from << "_" << to << ".txt'. ***\n\n";
	}
	else cout << "*** Nevalidna stajalista. ***\n";
}


bool LeastStopsStrategy::DFS(uint16_t station_id, string line_id, Route& route, unordered_map<uint16_t, bool>& visited) {

	visited[station_id] = true;
	PublicTraffic& pt = PublicTraffic::getInstance();

	route.stops_.push_back({ station_id, line_id });

	uint16_t next = pt.stations_[station_id].next_stop_[line_id];
	if (visited[next]) return false;
	if (next == last_station) {
		route.stops_.push_back({ next, line_id });
		routes.push_back(route);

		route.stops_.pop_back();
		return true;
	}

	for (const auto& it : pt.stations_[next].next_stop_) {
		DFS(next, it.first, route, visited);
		visited[next] = false;
		route.stops_.pop_back();
	}

	return false;
}


/*********************** LEAST LINES STRATEGY ***********************/

void LeastLinesStrategy::execute(uint16_t from, uint16_t to) {
	if (from == to) {
		cout << "*** Nevalidna stajalista. ***\n";
		return;
	}
	last_station = to;
	PublicTraffic& pt = PublicTraffic::getInstance();

	unordered_map<uint16_t, bool> visited;
	for (const auto& it : pt.stations_) {
		visited.insert({ it.first, false });
	}

	for (const auto& it : pt.stations_[from].next_stop_) {
		Route r;
		DFS(from, it.first, r, visited);
	}

	sort(routes.begin(), routes.end(), [](Route const &a, Route const &b) {
		return a.getLineChangesNum() < b.getLineChangesNum();
	});

	if (!routes.empty()) {
		ofstream file;
		file.open("putanja_" + to_string(from) + "_" + to_string(to) + ".txt");
		file << routes.at(0).toString();
		file.close();
		cout << "*** Putanja izmedju stajalista " << from << " i " << to;
		cout << " nalazi se u fajlu " << "'putanja_" << from << "_" << to << ".txt'. ***\n\n";
	}
	else cout << "*** Nevalidna stajalista. ***\n";
}


bool LeastLinesStrategy::DFS(uint16_t station_id, string line_id, Route& route, unordered_map<uint16_t, bool>& visited) {

	visited[station_id] = true;
	PublicTraffic& pt = PublicTraffic::getInstance();

	route.stops_.push_back({ station_id, line_id });

	uint16_t next = pt.stations_[station_id].next_stop_[line_id];

	if (visited[next]) return false;

	if (next == last_station) {
		route.stops_.push_back({ next, line_id });
		routes.push_back(route);

		route.stops_.pop_back();
		return true;
	}

	for (const auto& it : pt.stations_[next].next_stop_) {
		DFS(next, it.first, route, visited);
		visited[next] = false;
		route.stops_.pop_back();
	}

	return false;
}
