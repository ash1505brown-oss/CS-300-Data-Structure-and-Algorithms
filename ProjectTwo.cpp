#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Structure to store course information
struct Course {
	string courseNumber;
	string courseTitle;
	vector<string> prerequisites;
};

// Node structure for the binary serach tree recommended in project one
struct Node {
	Course course;
	Node* left;
	Node* right;

	Node(Course newCourse) {
		course = newCourse;
		left = nullptr;
		right = nullptr;
	}
};

// Binary search tree to organize course information
class BinarySearchTree {
private:
	Node* root;

	// Recursively insert a course into the tree
	Node* insertNode(Node* node, Course course) {
		if (node == nullptr) {
			return new Node(course);
		}
		if (course.courseNumber < node->course.courseNumber) {
			node->left = insertNode(node->left, course);
		}
		else if (course.courseNumber > node->course.courseNumber) {
			node->right = insertNode(node->right, course);
		}

		return node;
	}

	// Print courses in alphanumeric order
	void inOrder(Node* node) {
		if (node == nullptr) {
			return;
		}
		// Visit the left subtree
		inOrder(node->left);

		// Print the current course
		cout << node->course.courseNumber << ", "
			<< node->course.courseTitle << endl;

		//Visit the right subtree
		inOrder(node->right);
	}

	// Search for a course by its course number
	Node* searchNode(Node* node, string courseNumber) {
		if (node == nullptr) {
			return nullptr;
		}
		if (courseNumber == node->course.courseNumber) {
			return node;
		}
		if (courseNumber < node->course.courseNumber) {
			return searchNode(node->left, courseNumber);
		}
		else {
			return searchNode(node->right, courseNumber);
		}
	}

public:
	// Constructor initializes an empty tree
	BinarySearchTree() {
		root = nullptr;
	}

	//Public function to insert a course
	void insert(Course course) {
		root = insertNode(root, course);
	}

	// Display all courses in alphanumeric order
	void printCourseList() {
		if (root == nullptr) {
			cout << "No courses have been loaded." << endl;
			return;
		}

		cout << "Here is a sample schedule:" << endl;
		inOrder(root);
	}

	//Search for a course by its course Number
	Course* search(string courseNumber) {
		Node* result = searchNode(root, courseNumber);

		if (result == nullptr) {
			return nullptr;
		}
		return &(result->course);
	}
};

// Display course information and prerequisites
void printCourseInformation(BinarySearchTree& tree, string courseNumber) {
	Course* course = tree.search(courseNumber);

	if (course == nullptr) {
		cout << "Course not found." << endl;
		return;
	}
	cout << course->courseNumber << ", "
		<< course->courseTitle << endl;

	cout << "Prerequisites: ";

	if (course->prerequisites.empty()) {
		cout << "None" << endl;
	}
	else {
		cout << endl;

		for (size_t i = 0; i < course->prerequisites.size(); i++) {
			string prereqNumber = course->prerequisites[i];

			Course* prereqCourse = tree.search(prereqNumber);

			cout << prereqNumber;

			if (prereqCourse != nullptr) {
				cout << ", " << prereqCourse->courseTitle;
			}
			else {
				cout << " (course information unavailable)";
			}

			cout << endl;
		}
	}
}

// Load course information from the CSV file provided
bool loadCourses(string fileName, BinarySearchTree& tree) {
	ifstream file(fileName);

	if (!file.is_open()) {
		cout << "Error: Unable to open file." << endl;
		return false;
	}

	string line;

	while (getline(file, line)) {
		stringstream ss(line);
		string value;
			Course course;

		// Read the course number
		getline(ss, course.courseNumber, ',');

		// Read the course title
		getline(ss, course.courseTitle, ',');

		//Read the prerequisites
		while (getline(ss, value, ',')) {
			if (!value.empty()) {
				course.prerequisites.push_back(value);
			}
		}

		// insert the course into the binary search tree
		tree.insert(course);
	}

	file.close();

	cout << "Course data loaded successfully!" << endl;
	return true;
}

// Main function to run the course planner
int main() {
	BinarySearchTree tree;
	string fileName;
	string courseNumber;
	int choice = 0;

	cout << "Welcome to the course planner!" << endl;

	while (choice != 9) {
		cout << endl;
		cout << "1. Load Data Structure." << endl;
		cout << "2. Print Course List." << endl;
		cout << "3. Print Course." << endl;
		cout << "9. Exit" << endl;
		cout << endl;
		cout << "What would you like to do? ";

		if (!(cin >> choice)) {
			cout << "Invalid input. Please enter a number." << endl;
			cin.clear();
			cin.ignore(10000, '\n');
			continue;
		}

		if (choice == 1) {
			cout << "Enter the file name: ";
			getline(cin >> ws, fileName);
			loadCourses(fileName, tree);
		}

		else if (choice == 2) {
			tree.printCourseList();
		}

		else if (choice == 3) {
			cout << "What course do you want to know about? ";
			cin >> courseNumber;
			printCourseInformation(tree, courseNumber);
		}

		else if (choice == 9) {
			cout << "Thank you for using the course planner!" << endl;
		}

		else {
			cout << choice << " is not a valid option." << endl;
		}
	}

	return 0;
}