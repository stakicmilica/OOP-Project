#ifndef PUBLIC_TRAFFIC_H
#define PUBLIC_TRAFFIC_H
#include <fstream>
#include <iostream>
#include <regex>
#include <map>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class PublicTraffic {
public:
	void loadStations();
	void loadLines();

	void showStationInfo(uint16_t station_id);
	void showBasicLineInfo(string& line_id);
	void showStatisticLineInfo(string& line_id);
	void showPathBetweenTwoStations(uint16_t station_id_from, uint16_t station_id_to);

	static PublicTraffic& getInstance() {
		static PublicTraffic pt;
		return pt;
	}

private:
	PublicTraffic() {}
	PublicTraffic(const PublicTraffic&) = delete;
	PublicTraffic& operator=(const PublicTraffic&) = delete;

	struct Station {
		Station(string station_name = "") : station_name_(station_name) {}
		string station_name_;
		map<string, uint16_t> next_stop_;
	};

	unordered_map<uint16_t, Station> stations_;
	unordered_map<string, pair<vector<uint16_t>, vector<uint16_t>>> lines_;

	vector<uint16_t> splitRoute(string& route) const;
	void insertStops(const string& line_name, const vector<uint16_t>& stops);

	friend class BaseStrategy;
	friend class LeastStopsStrategy;
	friend class LeastLinesStrategy;
};

#endif // !PUBLIC_TRAFFIC_H#pragma once
