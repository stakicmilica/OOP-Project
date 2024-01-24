#include <fstream>
#include <iostream>
#include <sstream>
#include "PublicTraffic.h"

using namespace std;

int tests_public_passed = 0;


void showStationInfo(PublicTraffic& public_traffic) {
	uint16_t station_id;
	cout << string(80, '_') << "\n\n";
	cout << "Unesite sifru zeljenog stajalista (npr 154): ";
	cin >> station_id;
	public_traffic.showStationInfo(station_id);
}

void showBasicLineInfo(PublicTraffic& public_traffic) {
	string line_id;
	cout << string(80, '_') << "\n\n";
	cout << "Unesite oznaku linije (npr EKO): ";
	cin >> line_id;
	public_traffic.showBasicLineInfo(line_id);
}

void showStatisticLineInfo(PublicTraffic& public_traffic) {
	string line_id;
	cout << string(80, '_') << "\n\n";
	cout << "Unesite oznaku linije (npr EKO): ";
	cin >> line_id;
	public_traffic.showStatisticLineInfo(line_id);
}

void showPathBetweenTwoStations(PublicTraffic& public_traffic) {
	int station_id_from;
	int station_id_to;
	cout << string(80, '_') << "\n\n";
	cout << "Unesite sifru pocetnog stajalista: ";
	cin >> station_id_from;
	cout << "Unesite sifru krajnjeg stajalista: ";
	cin >> station_id_to;
	public_traffic.showPathBetweenTwoStations(station_id_from, station_id_to);
}


int main() {

	try {
		int choosen = -1;

		string info = "Dobrodosli u simulator mreze gradskog prevoza. ";
		string choose_opt = "Molimo Vas, odaberite jednu od opcija:\n";
		string info_opt1 = "\t1. Ucitavanje podataka o mrezi gradskog prevoza\n";
		string end_opt = "\t0. Kraj rada\n";

		string bus_net_opt1 = "\t1. Prikaz informacija o stajalistu.\n";
		string bus_net_opt2 = "\t2. Prikaz osnovnih informacija o liniji gradskog prevoza.\n";
		string bus_net_opt3 = "\t3. Prikaz statistickih informacija o liniji gradskog prevoza.\n";
		string bus_net_opt4 = "\t4. Pronalazak putanje izmedju dva stajalista.\n";

		cout << info;
		cout << "\n" << string(80, '_') << "\n\n";
		cout << choose_opt << info_opt1 << end_opt;
		cin >> choosen;

		while (choosen < 0 || choosen > 1)
		{
			cout << "Opcija " << to_string(choosen) << " nije validna. " << choose_opt << info_opt1 << end_opt;
			cin >> choosen;
		}

		if (choosen == 0) return 0;
		//if (choosen == 1)	

		PublicTraffic& public_traffic = PublicTraffic::getInstance();
		public_traffic.loadStations();
		public_traffic.loadLines();

		while (true) {

			cout << string(80, '_') << "\n\n";
			cout << choose_opt << bus_net_opt1 << bus_net_opt2 << bus_net_opt3 << bus_net_opt4 << end_opt;
			cin >> choosen;

			while (choosen < 0 || choosen > 4)
			{
				cout << "Opcija " << to_string(choosen) << " nije validna. " << choose_opt;
				cout << bus_net_opt1 << bus_net_opt2 << bus_net_opt3 << bus_net_opt4 << end_opt;
				cin >> choosen;
			}

			if (choosen == 0) return 0;

			else if (choosen == 1) {
				showStationInfo(public_traffic);
			}
			else if (choosen == 2) {
				showBasicLineInfo(public_traffic);
			}
			else if (choosen == 3) {
				showStatisticLineInfo(public_traffic);
			}
			else if (choosen == 4) {
				showPathBetweenTwoStations(public_traffic);
			}
		}
	}
	catch (string err) {
		cout << err << "\n";
	}

	return 0;
}
