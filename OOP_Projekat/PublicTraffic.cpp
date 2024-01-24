#include "PublicTraffic.h"
#include "Strategy.h"

void PublicTraffic::loadStations() {

	string out = "\nMolimo Vas, unesite putanju do fajla sa stajalistima:\n";
	string stations_path;
	cout << string(80, '_') << "\n";
	cout << out;
	cin >> stations_path;
	//stations_path = "D:\\DESKTOP\\OOP_PROJ_2020_21_JavniTest\\javni_testovi\\test1\\stajalista.txt";


	ifstream file;
	file.open(stations_path, ios::in);

	while (!file.good()) {
		cout << "Putanja do fajla '" << stations_path << "' je nevalidna. " << out;
		cin >> stations_path;
		file.open(stations_path, ios::in);
	}

	string line;
	regex line_reg("(\\d+) (.+)");
	uint16_t i = 1;
	while (getline(file, line)) {
		smatch match;
		if (regex_match(line, match, line_reg)) {
			uint16_t station_id_ = stoi(match[1].str());
			string station_name_ = match[2].str();

			if (!stations_.count(station_id_)) {
				stations_.insert({ station_id_, station_name_ });
			}
			else {
				throw "GRESKA!!! Stajaliste '" + to_string(station_id_) + "' vec definisano!";
			}
		}
		else {
			throw "GRESKA!!! Nevalidan format zapisa na liniji broj " + to_string(i) + ".";
		}
		i++;
	}
	file.close();
	cout << "*** Uspesno ucitana gradska stajalista! ***\n";
}

void PublicTraffic::loadLines() {
	string out = "\nMolimo Vas, unesite putanju do fajla sa linijama gradskog prevoza:\n";
	string lines_path;
	cout << string(80, '_') << "\n";
	cout << out;
	cin >> lines_path;

	//lines_path = "D:\\DESKTOP\\OOP_PROJ_2020_21_JavniTest\\javni_testovi\\test1\\linije.txt";

	ifstream file;
	file.open(lines_path, ios::in);

	while (!file.good()) {
		cout << "Putanja do fajla '" << lines_path << "' je nevalidna. " << out;
		cin >> lines_path;
		file.open(lines_path, ios::in);
	}

	string line;
	regex line_reg("(\\w+) (.+)#(.+)");
	uint16_t i = 1;
	try {
		while (getline(file, line)) {
			smatch match;
			if (regex_match(line, match, line_reg)) {
				string line_name_ = match[1].str();
				string route_a_ = match[2].str();
				string route_b_ = match[3].str();

				if (lines_.count(line_name_)) {
					throw "GRESKA!!! Linija '" + line_name_ + "' vec definisana!";
				}

				vector<uint16_t> stations_a_ = splitRoute(route_a_);
				vector<uint16_t> stations_b_ = splitRoute(route_b_);

				lines_.insert({ line_name_, {stations_a_, stations_b_} });

				insertStops(line_name_, stations_a_);
				insertStops(line_name_, stations_b_);
			}
			else {
				throw "GRESKA!!! Nevalidan format zapisa na liniji broj " + to_string(i) + ".";
			}
			i++;
		}
		file.close();
		cout << "*** Uspesno ucitane gradske linije! ***\n";
	}
	catch (string err) {
		file.close();
		throw err;
	}
	catch (exception e) {
		file.close();
		throw "GRESKA!!! Nevalidan format zapisa na liniji broj " + to_string(i) + ".";
	}

}

vector<uint16_t> PublicTraffic::splitRoute(string& route) const {

	string delimiter = " ";
	vector<uint16_t> stations;
	size_t pos = 0;
	while ((pos = route.find(delimiter)) != string::npos) {
		uint16_t token = stoi(route.substr(0, pos));
		stations.push_back(token);
		route.erase(0, pos + 1);
	}
	stations.push_back(stoi(route));
	return stations;
}

void PublicTraffic::insertStops(const string& line_name, const vector<uint16_t>& stops) {

	for (size_t i = 0; i < stops.size() - 1; i++) {
		auto result = stations_[stops[i]].next_stop_.insert({ line_name, stops[i + 1] });
		if (!result.second) {
			// insert vraca pair {iterator, bool}
			throw "GRESKA!!! Za stajaliste broj '" + to_string(stops[i]) + "' vec ucitana linija '" + line_name + "'.";
		}
	}
}


void PublicTraffic::showStationInfo(uint16_t station_id) {

	while (!stations_.count(station_id)) {
		cout << "Stajaliste broj " + to_string(station_id) + " je nevalidno.\n";
		cout << "Unesite sifru zeljenog stajalista (npr 154): ";
		cin >> station_id;
	}

	ofstream file;
	file.open("stajaliste_" + to_string(station_id) + ".txt");

	file << station_id << " " << stations_[station_id].station_name_ << " [";

	uint16_t i = 0;

	for (auto& it : stations_[station_id].next_stop_) {
		file << it.first;
		file << (++i != stations_[station_id].next_stop_.size() ? " " : "]");
	}

	file.close();

	cout << "*** Informacije o stajalistu " << station_id << " nalaze se u fajlu " << "'stajaliste_" << station_id << ".txt'. ***\n\n";
}

void PublicTraffic::showBasicLineInfo(string& line_id) {

	while (!lines_.count(line_id)) {
		cout << "Linija " + line_id + " je nevalidna.\n";
		cout << "Unesite oznaku linije (npr EKO): ";
		cin >> line_id;
	}

	ofstream file;
	file.open("linija_" + line_id + ".txt");

	file << line_id << " ";

	auto last = lines_[line_id].first.size() - 1;

	file << stations_[lines_[line_id].first.at(0)].station_name_ << "->";
	file << stations_[lines_[line_id].first.at(last)].station_name_;

	for (uint16_t it : lines_[line_id].first) {
		file << "\n" << it << " " << stations_[it].station_name_;
	}

	for (uint16_t it : lines_[line_id].second) {
		file << "\n" << it << " " << stations_[it].station_name_;
	}

	file.close();

	cout << "*** Informacije o liniji " << line_id << " nalaze se u fajlu " << "'linija_" << line_id << ".txt'. ***\n\n";
}


void PublicTraffic::showStatisticLineInfo(string& line_id) {

	while (!lines_.count(line_id)) {
		cout << "Linija " + line_id + " je nevalidna.\n";
		cout << "Unesite oznaku linije (npr EKO): ";
		cin >> line_id;
	}

	ofstream file;
	file.open("statistika_" + line_id + ".txt");

	file << line_id << "\n";

	map<string, uint16_t> shared_stations_;

	for (auto& it : lines_) {
		if (it.first != line_id) shared_stations_.insert({ it.first, 0 });
	}

	for (auto& it : stations_) {
		if (!it.second.next_stop_.count(line_id)) continue;

		for (auto& i : it.second.next_stop_) {
			if (i.first != line_id) {
				shared_stations_[i.first]++;
			}
		}
	}

	uint16_t i = 0, max = 0;
	string max_line_id_ = "";

	for (auto& it : shared_stations_) {
		if (it.second) {
			file << (!i++ ? "" : " ") << it.first;
			max_line_id_ = it.second > max ? it.first : max_line_id_;
			max = it.second > max ? it.second : max;
		}
	}
	file << "\n" << max_line_id_;

	file.close();

	cout << "*** Statistika o liniji " << line_id << " nalazi se u fajlu " << "'statistika_" << line_id << ".txt'. ***\n\n";
}

void PublicTraffic::showPathBetweenTwoStations(uint16_t station_id_from, uint16_t station_id_to) {
	uint16_t strategy_id;
	cout << "Odaberite strategiju:\n\t1. Osnovna\n\t2. Najmanje stajalista\n\t3. Najmanje presedanja\n\n";
	cin >> strategy_id;

	while (strategy_id < 0 || strategy_id > 3) {
		cout << "Nevalidna opcija.\n";
		cout << "Odaberite strategiju:\n\t1. Osnovna\n\t2. Najmanje stajalista\n\t3. Najmanje presedanja\n\n";
		cin >> strategy_id;
	}

	if (strategy_id == 1) {
		BaseStrategy bs;
		bs.execute(station_id_from, station_id_to);
	}
	else if (strategy_id == 2) {
		LeastStopsStrategy bs;
		bs.execute(station_id_from, station_id_to);
	}
	else if (strategy_id == 3) {
		LeastLinesStrategy bs;
		bs.execute(station_id_from, station_id_to);
	}

}
