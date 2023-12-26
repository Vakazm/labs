#include <iostream>
#include <cmath>

#define MAX_ITERATIONS 10000

using namespace std;

// Функція, яку інтегруємо
long double f (long double x, long double w) {
    return sin (w*x) * exp (- x*x*x);
}

// Метод обрізання границь
int limitrunc (long double w, long double e) {
    long double a = 1;

    while (true) {
        // Частковий інтеграл для обмеження функції
        if (exp (-a) < e || a > MAX_ITERATIONS) { break; }
        a ++;
    }

    return a;
}

// Конвертація точності у кількість ітерацій
int eton (long double e) {
    return min ((int) (1 + 1 / sqrt (e)), MAX_ITERATIONS);
}

// Оцінка залишкових членів
double estimate (double b, int n) {
    double M4 = 24;
    double h = b / n;
    double R = (M4 * pow (h, 4)) / 180 * b;
    return R;
}

// Метод середніх прямокутників
long double squares (long double b, long double w, int n) {
    long double h = b / n;
    long double sum = 0;

    for (int i = 0; i < n; i ++) {
        double xi = i * h + h / 2;
        sum += f (xi, w);
    }

    return h * sum;
}

// Метод Сімпсона
long double simpson (long double b, long double w, int n) {
    long double h = b / n;
    long double sum = 0;

    for (int i = 0; i <= n; i ++) {
        long double xi = i * h;
        long double fxi = f (xi, w);

        if (i == 0 || i == n) {
            sum += fxi;
        }
        else if (i % 2 == 0) {
            sum += 2 * fxi;
        }
        else {
            sum += 4 * fxi;
        }
    }

    return h / 3 * sum;
}

// Таблиця Ромберга
long double romberg (long double b, long double w, int k) {
    long double table [k + 1] [k + 1] = {0};

    for (int i = 0; i <= k; i ++) {
        table [i] [0] = squares (b, w, pow (2, i));
    }

    for (int j = 1; j <= k; j ++) {
        for (int i = j; i <= k; i ++) {
            table [i] [j] = (pow (4, j) * table [i] [j - 1] - 
                table [i - 1] [j - 1]) / (pow (4, j) - 1);
        }
    }
    
    return table [k][k];
}

int main () {
    // Точність та кількість кроків для обчислення інтегралу
    long double e = 0.00001;
    int n = eton (e); 
    cout << "Precision: " << e << ", iterations: " << n << endl;
    
    for (auto w : {1, 10, 20, 50}) {
        cout << "w: " << w << endl;

        // Обчислення верхньої межі інтегрування методом обрізання границь
        long double upper = limitrunc (w, e);
        cout << "Upper integration bound: " << upper << endl;

        // Обчислення інтегралу методом середніх прямокутників
        cout << "Middle squares method: " << squares (upper, w, n) << endl;

        // Обчислення інтегралу методом Сімпсона
        double resultSimpson = simpson (upper, w, n);
        cout << "Simpson method: " << resultSimpson << endl;

        // Обчислення оцінки залишкових членів
        cout << "Residuals estimate: " << estimate (upper, n) << endl;

        // Обчислення інтегралу за допомогою таблиці Ромберга
        cout << "Romberg Table Method: " << romberg (upper, w, 5) << endl;

        cout << endl;
    }

    return 0;
}