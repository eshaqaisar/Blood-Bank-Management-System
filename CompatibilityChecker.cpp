#include "CompatibilityChecker.h"
#include <iostream>
#include "BloodRequest.h"

using namespace std;


bool CompatibilityChecker::canDonateTo(string donorBlood, string patientBlood) {

	if (donorBlood == "O-") {
		return true;
	}

	if (donorBlood == "O+") {
		return (patientBlood == "O+" || patientBlood == "A+" ||
			patientBlood == "B+" || patientBlood == "AB+");
	}

	if (donorBlood == "A-") {

		return (patientBlood == "A-" || patientBlood == "A+" ||
			patientBlood == "AB+" || patientBlood == "AB-");
	}

	if (donorBlood == "A+") {
		return (patientBlood == "AB+" || patientBlood == "A+");
	}

	if (donorBlood == "B-") {
		return (patientBlood == "B-" || patientBlood == "AB-" ||
			patientBlood == "B+" || patientBlood == "AB+");
	}

	if (donorBlood == "B+") {
		return (patientBlood == "B+" || patientBlood == "AB+");
	}

	if (donorBlood == "AB+") {
		return (patientBlood == "AB+");
	}

	if (donorBlood == "AB-") {
		return (patientBlood == "AB-" || patientBlood == "AB+");
	}


	return false;
}
