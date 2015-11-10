#ifndef PUBLIC_HELPER_H
#define PUBLIC_HELPER_H

#include <cmath>
#include <QString>
#include <QTextStream>

const double EARTH_RADIUS = 6378.137;

inline double degreeToRadian(double degree)
{
    return degree * M_PI / 180.0;
}

//获取两个经纬度之间的距离
inline double getDistance(double lat1, double lng1,
                   double lat2, double lng2)
{
    double radLat1 = degreeToRadian(lat1);
    double radLat2 = degreeToRadian(lat2);
    double a = radLat1 - radLat2;
    double b = degreeToRadian(lng1) - degreeToRadian(lng2);

    double s = 2 * asin(sqrt(pow(sin(a / 2), 2) +
                            cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2)));
    s = s * EARTH_RADIUS;
    s = s * 1000;

    return s;
}

inline void printMessage(const QString &msg)
{
    QTextStream out(stdout);
    out << msg << endl;
}

//////////////////////////////////////////////////////////////////////////

const double a = 6378245.0;
const double ee = 0.00669342162296594323;
const double pi = 3.14159265358979324;
const double xPi = pi * 3000.0 / 180.0;

struct Coordinate {

    Coordinate(): lat(0), lng(0) {}
    Coordinate(double lat, double lng) : lat(lat), lng(lng) {}

    double lat;
    double lng;
};

inline double transformLatWith(double x, double y)
{
    double lat = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y +
            0.2 * sqrt(fabs(x));
    lat += (20.0 * sin(6.0 * x * pi) + 20.0 *sin(2.0 * x * pi)) * 2.0 / 3.0;
    lat += (20.0 * sin(y * pi) + 40.0 * sin(y / 3.0 * pi)) * 2.0 / 3.0;
    lat += (160.0 * sin(y / 12.0 * pi) + 320 * sin(y * pi / 30.0)) * 2.0 / 3.0;
    return lat;
}

inline double transformLngWith(double x, double y)
{
    double lon = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y +
            0.1 * sqrt(fabs(x));
    lon += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
    lon += (20.0 * sin(x * pi) + 40.0 * sin(x / 3.0 * pi)) * 2.0 / 3.0;
    lon += (150.0 * sin(x / 12.0 * pi) + 300.0 * sin(x / 30.0 * pi)) * 2.0 / 3.0;
    return lon;
}

//判断是否在中国
inline bool isOutOfChina(const Coordinate &coor)
{
    if (coor.lng < 72.004 || coor.lng > 137.8347) {
        return true;
    }
    if (coor.lat < 0.8293 || coor.lat > 55.8271) {
        return true;
    }

    return false;
}

inline bool isContains(Coordinate coor, Coordinate coor1,
                       Coordinate coor2)
{
    return (coor.lat >= qMin(coor1.lat, coor2.lat) &&
            coor.lat <= qMax(coor1.lat, coor2.lat) &&
            coor.lng >= qMin(coor1.lng, coor2.lng) &&
            coor.lng <= qMax(coor1.lng, coor2.lng));
}

inline Coordinate baiduLatLngToFakeLatLng(const Coordinate &coor)
{
    double x = coor.lng - 0.0065, y = coor.lat - 0.006;
    double z = sqrt(x * x + y * y) - 0.00002 * sin(y * xPi);
    double theta = atan2(y, x) - 0.000003 * cos(x * xPi);
    Coordinate ret;
    ret.lat = z * sin(theta);
    ret.lng = z * cos(theta);

    return ret;
}

inline Coordinate realLatLngToFakeLatLng(const Coordinate &coor)
{
    Coordinate ret;
    if (isOutOfChina(coor)) {
        ret.lat = coor.lat;
        ret.lng = coor.lng;
    } else {
        double lat = transformLatWith(coor.lng - 105.0, coor.lat - 35.0);
        double lng = transformLngWith(coor.lng - 105.0, coor.lat - 35.0);
        double radLat = coor.lat / 180.0 * pi;
        double magic = sin(radLat);
        magic = 1 - ee * magic * magic;
        double sqrtMagic = sqrt(magic);
        lat = (lat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
        lng = (lng * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
        ret.lat = coor.lat + lat;
        ret.lng = coor.lng + lng;
    }

    return ret;
}

inline Coordinate fakeLatLngToRealLatLng(const Coordinate &coor)
{
    double threshold = 0.00001;

    //the boundary
    double minLat = coor.lat - 0.5;
    double maxLat = coor.lat + 0.5;
    double minLng = coor.lng - 0.5;
    double maxLng = coor.lng + 0.5;

    double delta = 1;
    int maxInteration = 30;
    //binary search
    while(true) {
        Coordinate leftBottom = realLatLngToFakeLatLng(
                    Coordinate(minLat, minLng));
        Coordinate rightBottom = realLatLngToFakeLatLng(
                    Coordinate(minLat, maxLng));
        Coordinate leftUp = realLatLngToFakeLatLng(
                    Coordinate(maxLat, minLng));
        Coordinate midPoint = realLatLngToFakeLatLng(
                    Coordinate((minLat + maxLat) / 2,
                    (minLng + maxLng) / 2));
        delta = fabs(midPoint.lat - coor.lat) +
                fabs(midPoint.lng - coor.lng);
        if (maxInteration-- <= 0 || delta <= threshold) {
            return Coordinate((minLat + maxLat) / 2,
                              (minLng + maxLng) / 2);
        }

        if (isContains(coor, leftBottom, midPoint)) {
            maxLat = (minLat + maxLat) / 2;
            maxLng = (minLng + maxLng) / 2;
        } else if (isContains(coor, rightBottom, midPoint)) {
            maxLat = (minLat + maxLat) / 2;
            minLng = (minLng + maxLng) / 2;
        } else if (isContains(coor, leftUp, midPoint)) {
            minLat = (minLat + maxLat) / 2;
            maxLng = (minLng + maxLng) / 2;
        } else {
            minLat = (minLat + maxLat) / 2;
            minLng = (minLng + maxLng) / 2;
        }
    }
}

//百度坐标转换为真实坐标
inline Coordinate baiduLatLngToRealLatLng(const Coordinate &coor)
{
    Coordinate fakeCoor = baiduLatLngToFakeLatLng(coor);
    return fakeLatLngToRealLatLng(fakeCoor);
}



#endif // PUBLIC_HELPER_H
