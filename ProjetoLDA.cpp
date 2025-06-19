#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>

using namespace std;

void read_csv(const string &filename, vector<vector<double>> &X, vector<int> &y)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Erro ao abrir arquivo: " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // cabeçalho

    while (getline(file, line))
    {
        stringstream ss(line);
        string cell;
        vector<double> row;
        int col_index = 0;
        int label = -1;

        while (getline(ss, cell, ','))
        {
            if (col_index == 0)
            {
                col_index++;
                continue; // pula ID
            }
            else if (col_index == 451)
            {
                if (cell.find('P') != string::npos)
                    label = 1;
                else if (cell.find('H') != string::npos)
                    label = 0;
                else
                    label = 0;
            }
            else
            {
                try
                {
                    row.push_back(stod(cell));
                }
                catch (...)
                {
                    row.push_back(0.0);
                }
            }
            col_index++;
        }
        if (label != -1)
        {
            X.push_back(row);
            y.push_back(label);
        }
    }
}

vector<vector<double>> read_new_patients(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
        return {};
    string line;
    getline(file, line); // cabeçalho

    vector<vector<double>> new_data;
    while (getline(file, line))
    {
        stringstream ss(line);
        string cell;
        vector<double> row;
        int col_index = 0;

        while (getline(ss, cell, ','))
        {
            if (col_index == 0)
            {
                col_index++;
                continue;
            } // pula ID
            try
            {
                row.push_back(stod(cell));
            }
            catch (...)
            {
                row.push_back(0.0);
            }
            col_index++;
        }
        new_data.push_back(row);
    }
    return new_data;
}

void save_predictions(const string &input_file, const string &output_file, const vector<int> &predictions)
{
    ifstream file(input_file);
    ofstream out(output_file);
    string header;
    getline(file, header);
    out << header << ",predicted_class\n";

    string line;
    int i = 0;
    while (getline(file, line) && i < predictions.size())
    {
        out << line << "," << (predictions[i] == 1 ? "P" : "H") << "\n";
        i++;
    }
}

vector<double> compute_mean(const vector<vector<double>> &X)
{
    int n = X.size(), d = X[0].size();
    vector<double> mean(d, 0.0);
    for (const auto &row : X)
        for (int i = 0; i < d; ++i)
            mean[i] += row[i];
    for (double &val : mean)
        val /= n;
    return mean;
}

vector<vector<double>> compute_covariance(const vector<vector<double>> &X, const vector<double> &mean)
{
    int n = X.size(), d = X[0].size();
    vector<vector<double>> cov(d, vector<double>(d, 0.0));
    for (const auto &row : X)
        for (int i = 0; i < d; ++i)
            for (int j = 0; j < d; ++j)
                cov[i][j] += (row[i] - mean[i]) * (row[j] - mean[j]);
    for (int i = 0; i < d; ++i)
        for (int j = 0; j < d; ++j)
            cov[i][j] /= (n - 1);
    return cov;
}

vector<vector<double>> invert_matrix(const vector<vector<double>> &matrix)
{
    int n = matrix.size();
    vector<vector<double>> A(matrix), inv(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i)
        inv[i][i] = 1.0;

    for (int i = 0; i < n; ++i)
    {
        int max_row = i;
        for (int k = i + 1; k < n; ++k)
            if (abs(A[k][i]) > abs(A[max_row][i]))
                max_row = k;
        if (abs(A[max_row][i]) < 1e-12)
            return {};
        swap(A[i], A[max_row]);
        swap(inv[i], inv[max_row]);

        double diag = A[i][i];
        for (int j = 0; j < n; ++j)
        {
            A[i][j] /= diag;
            inv[i][j] /= diag;
        }

        for (int k = 0; k < n; ++k)
        {
            if (k != i)
            {
                double factor = A[k][i];
                for (int j = 0; j < n; ++j)
                {
                    A[k][j] -= factor * A[i][j];
                    inv[k][j] -= factor * inv[i][j];
                }
            }
        }
    }
    return inv;
}

vector<double> multiply(const vector<vector<double>> &A, const vector<double> &x)
{
    int n = A.size(), m = x.size();
    vector<double> result(n, 0.0);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            result[i] += A[i][j] * x[j];
    return result;
}

double dot(const vector<double> &a, const vector<double> &b)
{
    return inner_product(a.begin(), a.end(), b.begin(), 0.0);
}

void train_lda(const vector<vector<double>> &X, const vector<int> &y, vector<double> &w, double &threshold)
{
    vector<vector<double>> X0, X1;
    for (size_t i = 0; i < y.size(); ++i)
    {
        if (y[i] == 0)
            X0.push_back(X[i]);
        else
            X1.push_back(X[i]);
    }

    auto mean0 = compute_mean(X0), mean1 = compute_mean(X1);
    auto cov0 = compute_covariance(X0, mean0), cov1 = compute_covariance(X1, mean1);

    int d = mean0.size();
    vector<vector<double>> Sw(d, vector<double>(d, 0.0));
    for (int i = 0; i < d; ++i)
        for (int j = 0; j < d; ++j)
            Sw[i][j] = cov0[i][j] + cov1[i][j];

    double lambda = 1e-4;
    for (int i = 0; i < d; ++i)
        Sw[i][i] += lambda;

    auto Sw_inv = invert_matrix(Sw);
    if (Sw_inv.empty())
        return;

    vector<double> mean_diff(d);
    for (int i = 0; i < d; ++i)
        mean_diff[i] = mean1[i] - mean0[i];

    w = multiply(Sw_inv, mean_diff);
    threshold = 0.5 * dot(w, mean0) + 0.5 * dot(w, mean1);
}

int predict(const vector<double> &x, const vector<double> &w, double threshold)
{
    return dot(w, x) > threshold ? 1 : 0;
}

void evaluate_model(const vector<int> &y_true, const vector<int> &y_pred)
{
    int TP = 0, TN = 0, FP = 0, FN = 0;

    for (size_t i = 0; i < y_true.size(); ++i)
    {
        if (y_true[i] == 1 && y_pred[i] == 1)
            TP++;
        else if (y_true[i] == 0 && y_pred[i] == 0)
            TN++;
        else if (y_true[i] == 0 && y_pred[i] == 1)
            FP++;
        else if (y_true[i] == 1 && y_pred[i] == 0)
            FN++;
    }

    double accuracy = static_cast<double>(TP + TN) / y_true.size();
    double error_rate = 1.0 - accuracy;
    double precision = (TP + FP) ? static_cast<double>(TP) / (TP + FP) : 0.0;
    double recall = (TP + FN) ? static_cast<double>(TP) / (TP + FN) : 0.0;
    double f1_score = (precision + recall) ? 2.0 * (precision * recall) / (precision + recall) : 0.0;

    cout << "\n--- Avaliação do Modelo ---\n";
    cout << "Matriz de Confusão:\n";
    cout << "TP: " << TP << "  FP: " << FP << "\n";
    cout << "FN: " << FN << "  TN: " << TN << "\n";

    cout << "Acurácia:        " << accuracy << endl;
    cout << "Erro de previsão:" << error_rate << endl;
    cout << "Precisão:        " << precision << endl;
    cout << "Recall:          " << recall << endl;
    cout << "F1-Score:        " << f1_score << endl;
}

class LinearDiscriminantAnalysis
{
public:
    vector<double> coef;
    double intercept;
    vector<double> means;
    vector<double> prior_probs;

    void fit(const vector<vector<double>> &X, const vector<int> &y)
    {
        vector<vector<double>> X0, X1;
        for (size_t i = 0; i < y.size(); ++i)
        {
            if (y[i] == 0)
                X0.push_back(X[i]);
            else
                X1.push_back(X[i]);
        }

        auto mean0 = compute_mean(X0), mean1 = compute_mean(X1);
        means = mean0; // só por guardar

        auto cov0 = compute_covariance(X0, mean0), cov1 = compute_covariance(X1, mean1);

        int d = mean0.size();
        vector<vector<double>> Sw(d, vector<double>(d, 0.0));
        for (int i = 0; i < d; ++i)
            for (int j = 0; j < d; ++j)
                Sw[i][j] = cov0[i][j] + cov1[i][j];

        double lambda = 1e-4;
        for (int i = 0; i < d; ++i)
            Sw[i][i] += lambda;

        auto Sw_inv = invert_matrix(Sw);
        if (Sw_inv.empty())
            return;

        vector<double> mean_diff(d);
        for (int i = 0; i < d; ++i)
            mean_diff[i] = mean1[i] - mean0[i];

        coef = multiply(Sw_inv, mean_diff);
        intercept = 0.5 * dot(coef, mean0) + 0.5 * dot(coef, mean1);
    }

    int predict(const vector<double> &x) const
    {
        return dot(coef, x) > intercept ? 1 : 0;
    }

    void save_model(const string &filename) const
    {
        ofstream file(filename);
        if (!file.is_open())
        {
            cerr << "Erro ao abrir o arquivo para salvar o modelo.\n";
            return;
        }

        file << means.size() << "\n";
        for (const auto &mean : means)
            file << mean << " ";
        file << "\n";

        file << coef.size() << "\n";
        for (const auto &c : coef)
            file << c << " ";
        file << "\n";

        file << intercept << "\n";

        file << prior_probs.size() << "\n";
        for (const auto &p : prior_probs)
            file << p << " ";
        file << "\n";

        file.close();
    }

    void load_model(const string &filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "Erro ao abrir o arquivo para carregar o modelo.\n";
            return;
        }

        int size;

        file >> size;
        means.resize(size);
        for (int i = 0; i < size; ++i)
            file >> means[i];

        file >> size;
        coef.resize(size);
        for (int i = 0; i < size; ++i)
            file >> coef[i];

        file >> intercept;

        file >> size;
        prior_probs.resize(size);
        for (int i = 0; i < size; ++i)
            file >> prior_probs[i];

        file.close();
    }
};

int main()
{
    vector<vector<double>> X;
    vector<int> y;
    read_csv("alzheimer_data.csv", X, y);

    vector<int> indices(X.size());
    iota(indices.begin(), indices.end(), 0);
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(indices.begin(), indices.end(), default_random_engine(seed));

    vector<vector<double>> X_train, X_test;
    vector<int> y_train, y_test;
    size_t split_idx = static_cast<size_t>(0.8 * X.size());
    for (size_t i = 0; i < X.size(); ++i)
    {
        if (i < split_idx)
        {
            X_train.push_back(X[indices[i]]);
            y_train.push_back(y[indices[i]]);
        }
        else
        {
            X_test.push_back(X[indices[i]]);
            y_test.push_back(y[indices[i]]);
        }
    }

    LinearDiscriminantAnalysis lda;
    string model_file = "lda_model.txt";
    bool carregar = false; // altere se quiser carregar modelo treinado

    if (carregar)
    {
        lda.load_model(model_file);
    }
    else
    {
        lda.fit(X_train, y_train);
        lda.save_model(model_file);
    }
    vector<double> w;
    double threshold;
    train_lda(X_train, y_train, w, threshold);

    vector<int> y_pred;
    for (size_t i = 0; i < X_test.size(); ++i)
    {
        y_pred.push_back(predict(X_test[i], w, threshold));
    }
    evaluate_model(y_test, y_pred);

    auto new_patients = read_new_patients("novos_pacientes.csv");
    vector<int> new_predictions;
    for (const auto &patient : new_patients)
        new_predictions.push_back(predict(patient, w, threshold));

    save_predictions("novos_pacientes.csv", "saida.csv", new_predictions);
    return 0;
}
