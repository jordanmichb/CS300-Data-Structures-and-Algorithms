//============================================================================
// Name        : CourseLoading.cpp
// Author      : Jordan Ballard
// Version     : 1.0
// Date        : August 14, 2022
// Description : Course Loading and Searching Algorithms
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

//============================================================================
// Global definitions
//============================================================================

/**
 * Define a structure to hold course data.
 */
struct Course {
	string courseNumber;
	string courseName;
	vector<string> prerequisites;
	string key;

	bool operator < (const Course& course) {
		return (key < course.key);
	}
};

//============================================================================
// CourseList class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * open, read, and parse a file and build a vector of the
 * file's course information.
 */
class CourseList {
private:
	ifstream fileStream;
	vector<Course> courseList;
	void displayCourse(Course course);

public:
	bool openFile(string userFile);
	bool processFile();
	bool printCourse(string courseNumber);
	void printAll();
};

/**
 * Open the file, load it into the vector, and close the file.
 *
 * @param userFile, the file from user input.
 */
bool CourseList::openFile(string userFile) {
	// Open the file.
	fileStream.open(userFile);
	// If open fails, display error message and return false.
	if (fileStream.fail()) {
		cerr << "Error opening file." << endl;
		return false;
	}
	// If open succeeds, validate and load data into the vector.
	processFile();
	// Close the stream.
	fileStream.close();
	return true;
}

/**
 * Read, parse, validate, and load the file data into the vector.
 */
bool CourseList::processFile() {
	string courseData;
	set<string> courseNums;
	string token;

	// Loop through the file once to gether course numbers for prerequisite validation.
	while (fileStream.good()) {
		// Get the line in the file.
		getline(fileStream, courseData);
		// Use a stringstream to store the line.
		stringstream stream(courseData);
		if (!courseData.empty()) {
			// Get only the first piece of data for each line and store into set.
			if (getline(stream, token, ',')) {
				courseNums.insert(token);
			}
		}
	}

	// Clear error state flags.
	fileStream.clear();
	// Set position back to the beginning of the file.
	fileStream.seekg(0, fileStream.beg);

	// Loop through the file.
	while (fileStream.good()) {
		Course newCourse;
		getline(fileStream, courseData);
		stringstream stream(courseData);
		if (!courseData.empty()) {
			// Get the first piece of data.
			if (getline(stream, token, ',')) {
				// Set course number.
				newCourse.courseNumber = token;
				// Set key for sorting.
				newCourse.key = token;
				// Get second piece of data, reachable only if there is at least one entry.
				if (getline(stream, token, ',')) {
					// Set course name.
					newCourse.courseName = token;
					// Get remaining n pieces of data, reachable only if there are at least two entries.
					while (getline(stream, token, ',')) {
						// If prerequisite is valid, add it to the course's prerequisite vector.
						if (courseNums.count(token)) {
							newCourse.prerequisites.push_back(token);
						}
						else {
							cout << "Error: Invalid prerequisite." << endl;
							return false;
						}
					}
				}
				// Output error at second "if" since all nonempty lines have at least one piece of data.
				else {
					cout << "Error: Course is missing a course number or course name." << endl;
					return false;
				}
			}
			// If line is valid, add course to courseList.
			courseList.push_back(newCourse);
		}
	}
	cout << "Courses loaded." << endl;
	return true;
}

/**
 * Loop over all courses and print one if it is a match.
 *
 * @param courseNumber, the course number to search for.
 */
bool CourseList::printCourse(string courseNumber) {
	for (Course course : courseList) {
		if (course.courseNumber == courseNumber) {
			displayCourse(course);
			return true;
		}
	}
	cout << "Course not found." << endl;
	return false;
}

/**
 * Sort the courses in alphanumeric order and print their information.
 */
void CourseList::printAll() {
	sort(courseList.begin(), courseList.end());
	for (Course course : courseList) {
		displayCourse(course);
	}
}

/**
 * Helper method for displaying course information.
 *
 * @param course, the course to display.
 */
void CourseList::displayCourse(Course course) {
	// Output course number and name.
	cout << course.courseNumber << ": " << course.courseName << endl;
	// If the course has prerequisites, print that info.
	if (course.prerequisites.size() != 0) {
		cout << "\t Prerequisites: ";
		// Loop over and print all prerequisites except last to avoid extra characters at the end.
		for (int i = 0; i < course.prerequisites.size() - 1; i++) {
			cout << course.prerequisites.at(i) << ", ";
		}
		// Print last prerequisite with no extra characters.
		cout << course.prerequisites.at(course.prerequisites.size() - 1) << endl;
	}
}

/**
* The main() method
*/
int main(int argc, char* argv[]) {
	// Instantiate courseList object.
	CourseList* courseList = new CourseList();

	// Display menu and get user input.
	int choice = 0;
	while (choice != 4) {
		cout << "Menu:" << endl;
		cout << " 1. Load Courses" << endl;
		cout << " 2. Print Course List" << endl;
		cout << " 3. Find Course" << endl;
		cout << " 4. Exit" << endl;
		cout << "Choose an option: ";
		cin >> choice;
		cout << endl;

		switch (choice) {

		case 1:
		{
			// Get a file from the user.
			string userFile;
			cout << "Enter a file to load: ";
			cin >> userFile;
			cout << endl;

			// Read, validate, and parse file and load data into vector.
			courseList->openFile(userFile);
			cout << endl;
			break;
		}
		case 2:
			// Print all courses in alphanumeric order.
			courseList->printAll();
			cout << endl;
			break;

		case 3:
		{
			// Get a course number from the user.
			string courseNumber;
			cout << "Enter a course to search for: ";
			cin >> courseNumber;
			cout << endl;

			// Print information for that course.
			courseList->printCourse(courseNumber);
			cout << endl;
			break;
		}
		default:
			cout << "Invalid option." << endl << endl;
			// Clear input state.
			cin.clear();
			// Ignore the rest of the invalid input
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		}
	}
	cout << "Exiting program. Good bye!" << endl;

	return 0;
}