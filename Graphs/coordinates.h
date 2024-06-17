#ifndef COORDINATES_H
#define COORDINATES_H

#include <cmath>

#define PI 3.14159265358979323846

using namespace std;

struct Coordinates {
    double latitude;
    double longitude;
};

// Функция для конвертации градусов в радианы
double toRadians(double degrees) {
    return degrees * PI / 180.0;
}

// Функция для вычисления расстояния между двумя географическими координатами в километрах
double distanceBetweenCoordinates(Coordinates& coord1, Coordinates& coord2) {
    double lat1 = toRadians(coord1.latitude);
    double lon1 = toRadians(coord1.longitude);
    double lat2 = toRadians(coord2.latitude);
    double lon2 = toRadians(coord2.longitude);

    // Радиус Земли в километрах
    double radius = 6371.0;

    // Разность между широтами и долготами
    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    // Формула гаверсинусов
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1) * cos(lat2) *
               sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    // Расстояние в километрах
    double distance = radius * c;

    return distance;
}

#endif // COORDINATES_H
