#include <iostream>
#include <vector>
#include <math.h>
#include <nlohmann/json.hpp>
#include <fstream>

using namespace std;

using json = nlohmann::json;

struct Point {
	double x;
	double y;
};


struct PolynomialMultiplier {
	double xI;
	double xK;

	double GetMultiplierValue(double x) const {
		double value = (x - xI) / (xK - xI);

		return value;
	}
};

struct Polynomial {
	vector <PolynomialMultiplier> polynomialMultipliers;

	void CreateMultiplier(double xI, double xK) {
		polynomialMultipliers.push_back({xI, xK});
	}

	double GetValue(double x) const {
		double value = 1;
		for (auto& fraction : polynomialMultipliers) {
			value *= fraction.GetMultiplierValue(x);
		}

		return value;
	}
};



struct PolynomialFunciton {
	vector <Polynomial> LangrangePolynomials;
	vector <double> yInitialValues;

	void Initialize(vector <Point> initialPoints) {

		for (int k = 0; k < initialPoints.size(); k++) {

			yInitialValues.push_back(initialPoints[k].y);

			Polynomial polynomial;
			for (int i = 0; i < initialPoints.size(); i++) {
				if (k != i) polynomial.CreateMultiplier(initialPoints[i].x, initialPoints[k].x);
			}

			LangrangePolynomials.push_back(polynomial);
		}
	}

	double GetValue(double approxX) const {

		double value = 0;

		for (int i = 0; i < LangrangePolynomials.size(); i++) {
			value += yInitialValues[i] * LangrangePolynomials[i].GetValue(approxX);
		}

		return value;
	}

};

double GetInitialFuncValue(double x) {
	return pow(x, 3) - 5 * pow(x, 2); // x^3 - 5 * x^2
}


vector <Point> GetInitialPoints(double a, double b, double m) {

	vector <Point> points;

	for (int i = 1; i <= m; i++) {

		Point point;
		point.x =  a + (i - 1) * (b - a) / (m - 1); // 
		point.y = GetInitialFuncValue(point.x);

		points.push_back(point);
	}

	return points;
}

vector<double> GetApproximationXValues(double a, double b) {
	
	vector<double> xValues;
	
	for (int j = 1; j <= 21; j++) {
		double x = a + (j - 1) * (b - a) / 20;
		xValues.push_back(x);
	}

	return xValues;
}

void PrintPoints(vector<Point> points) {

	for (auto& point : points) {
		cout << "X = " << point.x << "; Y = " << point.y << endl;
	}

}

void to_json(json& j, const Point& point) {
	j = json{ {"x", point.x}, {"y", point.y} };
}



int main()
{
	PolynomialFunciton polynomialFunction;
	double a, b, m, n;
	cout << "Print a, b, m, n" << endl;
	cin >> a >> b >> m >> n;
	
	const vector <Point> initialPoints = GetInitialPoints(a, b, m);
	cout << "Initial points: " << endl;
	PrintPoints(initialPoints);


	const vector <double> xApproxValues = GetApproximationXValues(a, b);

	polynomialFunction.Initialize(initialPoints);

	vector <Point> polynomialFunctionPoints;
	vector <Point> initialFunctionPoints;
	vector <Point> diff;


	for (auto xApproxValue : xApproxValues) {

		polynomialFunctionPoints.push_back({ xApproxValue , polynomialFunction.GetValue(xApproxValue) });
		initialFunctionPoints.push_back({ xApproxValue, GetInitialFuncValue(xApproxValue) });

		double evalDiff = initialFunctionPoints.back().y - polynomialFunctionPoints.back().y;
		diff.push_back({ xApproxValue, evalDiff });
	}

	cout << "Initial function: " << endl;
	PrintPoints(initialFunctionPoints);

	cout << "Polynomial function: " << endl;
	PrintPoints(polynomialFunctionPoints);

	cout << "Difference between functions: " << endl;
	PrintPoints(diff);

	json j;
	j["initial function"] = initialFunctionPoints;
	j["polynomial function"] = polynomialFunctionPoints;

	ofstream file("points.json");
	if (file.is_open()) {
		file << j.dump();
		file.close();

		system("python plots.py");
	}
}
