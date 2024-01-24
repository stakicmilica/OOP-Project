#ifndef STRATEGY_H
#define STRATEGY_H
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Strategy {
public:
	virtual void execute(uint16_t station_id_from, uint16_t station_id_to) = 0;
	virtual ~Strategy() = default;

protected:
	uint16_t last_station = 0;
	struct Route {
		vector<pair<uint16_t, string>> stops_;
		size_t getStopsNum() const {
			return stops_.size();
		}
		size_t getLineChangesNum() const;
		string toString();
	};

};

class BaseStrategy : public Strategy {
public:
	void execute(uint16_t station_id_from, uint16_t station_id_to) override;
private:
	bool DFS(uint16_t station_id, string line_id, Route& route, unordered_map<uint16_t, bool>& visited);
};

class LeastStopsStrategy : public Strategy {
public:
	void execute(uint16_t station_id_from, uint16_t station_id_to) override;
private:
	vector<Route> routes;
	bool DFS(uint16_t station_id, string line_id, Route& route, unordered_map<uint16_t, bool>& visited);
};

class LeastLinesStrategy : public Strategy {
public:
	void execute(uint16_t station_id_from, uint16_t station_id_to) override;
private:
	vector<Route> routes;
	bool DFS(uint16_t station_id, string line_id, Route& route, unordered_map<uint16_t, bool>& visited);
};

#endif // !STRATEGY_H