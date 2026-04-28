#include "Patient.h"

Patient::Patient(QString n, int a, QString c, QString bg, QString hName)
    : Person(n, a, c, bg), hospitalName(hName) {

}