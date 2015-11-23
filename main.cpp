#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <math.h>

using namespace std;

// definição de uma série
typedef struct sseries {
	int label;
	vector<double> values;
} series;

// protótipos
void read_file_into_data_structure(ifstream& file, 
	vector<series>& series_vector);
double DTW_distance(vector<double>& a, vector<double>& b);

int main (int argc, char** argv) {
	
	// auxilixares
	vector<series> training_series;
	vector<series> test_series;
	int accuracy_count = 0;

	// valida entrada
    if (argc < 3) {
        cerr <<
        "uso: ./P2 arquivo_de_treino arquivo_de_teste"
        << endl;
        return -1;
    }

    string training_path = argv[1];
    string test_path = argv[2];

	// abre arquivo de treino
	ifstream training_file(training_path);
	if (training_file.is_open()) {
		read_file_into_data_structure(training_file, training_series);	
	} else {
		cerr << "Algo errado com arquivo de treino" << endl;
	}

	training_file.close();

  	// abre arquivo de teste
	ifstream test_file(test_path);
	if (test_file.is_open()) {
		read_file_into_data_structure(test_file, test_series);	
	} else {
		cerr << "Algo errado com arquivo de teste" << endl;
	}

	test_file.close();

	// passa por todas as series de testes
	for (int i = 0; i < test_series.size(); i++) {

		series current_series = test_series[i];
		double shortest_distance = numeric_limits<double>::infinity();
		series shortest_distance_series;

		// compara com todas as series de treino
		for (int j = 0; j < training_series.size(); j++) {

			// calcula distancia dtw
			series current_training_series = training_series[j];
			double current_distance = 
				DTW_distance(
					current_series.values, 
					current_training_series.values
				);

			//cout << current_distance << endl;

			// salva série mais próxima (Nearest Neighbor)
			if (current_distance < shortest_distance) {
				shortest_distance = current_distance;
				shortest_distance_series = current_training_series;
			}
		}

		// checa se o classificador acertou
		if (shortest_distance_series.label == current_series.label) {
			accuracy_count += 1;
			cout << "acertou!" << endl;
		} else { 
			cout << "ERRRROOU" << endl;
		}
	}

	double accuracy = accuracy_count/(double)test_series.size();
	cout << "Acerto: " << 
		accuracy*100.0 << "%" <<
		endl;
 
	return 0;
}

void read_file_into_data_structure(ifstream& file, 
	vector<series>& series_vector) {

	// auxiliares
	string line; 
	double current_value;

	// passa por cada linha
	while (getline(file, line)) {

		series current_series;

		// cria stream a partir da linha lida
		istringstream line_stream(line);
		line_stream >> current_series.label;

		while (line_stream >> current_value) {
			current_series.values.push_back(current_value);
		}

		// adiciona serie atual ao vetor de series
		series_vector.push_back(current_series);
	}

}

double min_of_three(double a, double b, double c) {
	double aux = min(a, b);
	return min (aux, c);
}

double mod(double a) {
	return a < 0 ? -a : a;
}

double DTW_distance(vector<double>& a, vector<double>& b) {

	double alpha[a.size()][b.size()];
	double m[a.size()][b.size()];
	int i, j;


	// matriz das distâncias
	for (i = 0; i < a.size(); i++) {
		for (j = 0; j < b.size(); j++) {

			alpha[i][j] = pow((a.at(i) - b.at(j)), 2);
		}
	}

	// inicialização da matriz de memoização
	for (i = 0; i < a.size(); i++) {
		m[i][0] = numeric_limits<double>::infinity();
	}
	for (j = 0; j < b.size(); j++) {
		m[0][j] = numeric_limits<double>::infinity();
	}
	m[0][0] = 0;

	// dp
	for (i = 1; i < a.size(); i++) {
		for (j = 1; j < b.size(); j++) {
			m[i][j] = alpha[i][j] + min_of_three(m[i-1][j-1], m[i][j-1], m[i-1][j]);
		}
	}
	 
	return m[i][j];
}