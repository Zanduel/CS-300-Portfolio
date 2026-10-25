//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Zachary Stickman
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : ABCU Advising Assistance Program
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================


// Define a structure to hold course information
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize a node with a course
    Node(Course aCourse) {
        course = aCourse;
    	left = nullptr;
    	right = nullptr;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string courseNumber);
    void deleteTree(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseNumber);
    Course Search(string courseNumber);
};

/**
 * Helper function to recursively destroy the tree
 **/
void BinarySearchTree::deleteTree(Node* node) {
	if (node != nullptr) {
		deleteTree(node->left);
		deleteTree(node->right);
		delete node;
	}
}

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {

    // Initialize root to nullptr for an empty tree
	root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {

    // Delete every node starting from the root
	deleteTree(root);
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {

    // call inOrder function and pass root
	inOrder(root);
}


/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {

    // If the tree is empty, create the root node
	if (root == nullptr) {
		root = new Node(course);
	}
	// Otherwise, recursively add the course to the tree
	else {
		addNode(root, course);
	}
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseNumber) {

    // remove the node matching the course number
	root = removeNode(root, courseNumber);
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNumber) {

    // set current node equal to root
	Node* current = root;

    // Search down the tree until the course is found or the end is reached
	while (current != nullptr) {
        // If a match is found, return the current course
		if (current->course.courseNumber == courseNumber) {
			return current->course;
		}
        // If the course number is smaller, move to the left
		if (courseNumber < current->course.courseNumber) {
			current = current->left;
		}
        // else larger so traverse right
		else {
			current = current->right;
		}
	}

	Course course;
    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {

    // If the new course number is smaller, add it to the left subtree
	if (course.courseNumber < node->course.courseNumber) {

        // if no left node
		if (node->left == nullptr) {
            // this node becomes left
			node->left = new Node(course);
		}
        // else recurse down the left node
		else {
			addNode(node->left, course);
		}
	}
	// Otherwise, add it to the right subtree
	else {
        // if no right node
		if (node->right == nullptr) {
            // this node becomes right
			node->right = new Node(course);
		}
		else {
            // recurse down the right node
			addNode(node->right, course);
		}
	}
}
void BinarySearchTree::inOrder(Node* node) {

    // If the node exists, continue the traversal
	if (node != nullptr) {
		// Visit the left subtree first
		inOrder(node->left);

		// Output the course number and title
		cout << node->course.courseNumber << ": " << node->course.title << endl;

		// Visit the right subtree last
		inOrder(node->right);
	}
}


/**
 * Remove a course from some node recursively
 */
Node* BinarySearchTree::removeNode(Node* node, string courseNumber) {

	// If the node is nullptr, the course was not found
	if (node == nullptr) {
		return node;
	}

	// If the target course number is smaller, search the left subtree
	if (courseNumber < node->course.courseNumber) {
		node->left = removeNode(node->left, courseNumber);
	}
	// If the target course number is larger, search the right subtree
	else if (courseNumber > node->course.courseNumber) {
		node->right = removeNode(node->right, courseNumber);
	}
	else {

		// Case 1: Node has no children
		if (node->left == nullptr && node->right == nullptr) {
		delete node;
		node = nullptr;
		}

		// Case 2: Node has only a left child
		else if (node->left != nullptr && node->right == nullptr) {
			Node* temp = node;
			node = node->left;
			delete temp;
	}

		// Case 3: Node has only a right child
		else if (node->left == nullptr && node->right != nullptr) {
			Node* temp = node;
			node = node->right;
			delete temp;
		}

		// Case 4: Node has two children
		else {
			// Start with the right child
			Node* temp = node->right;
			// Find the smallest node in the right subtree
			while (temp->left != nullptr) {
				temp = temp->left;
			}

			// Replace the current course with the successor course
			node->course = temp->course;
			// Remove the duplicate successor from the right subtree
			node->right = removeNode(node->right, temp->course.courseNumber);
		}
	}

    // return node
	return node;
}



//============================================================================
// Helper functions
//============================================================================

/**
 * Display the course information to the console (std::out)
 *
 * @param course Course containing the course information
 */
void displayCourse(Course course) {
    cout << course.courseNumber << ", " << course.title << endl;

    cout << "Prerequisites: ";

    if (course.prerequisites.empty()) {
    	cout << "None";
    }
    else {
    	for (unsigned int i = 0; i < course.prerequisites.size(); ++i) {
    		cout << course.prerequisites[i];

    		if (i < course.prerequisites.size() - 1) {
    			cout << ", ";
    		}
    	}
    }

    cout << endl;
}

/**
 * Load a CSV file containing courses into the binary search tree
 *
 * @param csvPath The path to the CSV file to load
 * @param bst Binary search tree that will store the courses
 */
bool loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

	// Open the course file
	ifstream inputFile(csvPath);

	if (!inputFile.is_open()) {
			cerr << "Failed to open " << csvPath << endl;
			return false;
	}

	// Read each line from the course file
	string line;
	while (getline(inputFile, line)) {

			// Create a Course object from this row
			Course course;

			stringstream lineStream(line);

			getline(lineStream, course.courseNumber, ',');
			getline(lineStream, course.title, ',');

			string prerequisite;

			while (getline(lineStream, prerequisite, ',')) {
				if (!prerequisite.empty()) {
					course.prerequisites.push_back(prerequisite);
				}
			}

			// Insert this course into the binary search tree
			bst->Insert(course);
		}

	return true;
}


/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseNumber;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
    }

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();

    Course course;

    int choice = 0;
    bool dataLoaded = false;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        // Clear invalid input if the user does not enter a number
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a menu number." << endl;
            continue;
        }

        switch (choice) {

        case 1: {
        	// Load into a new tree so the old data is not duplicated
        	BinarySearchTree* newBst = new BinarySearchTree();

        	bool loadSuccessful = loadCourses(csvPath, newBst);

        	if (loadSuccessful) {
        		delete bst;
        		bst = newBst;

        		dataLoaded = true;

        	    cout << "Course data loaded successfully." << endl;
        	}
        	else {
        	    delete newBst;
        	}
            break;
        }

        case 2:
        	if (dataLoaded) {
        		// Print courses using the binary search tree
        		bst->InOrder();
        	}
        	else {
        		cout << "Please load the course data first." << endl;
        	}
            break;

        case 3:
        	if (dataLoaded) {
				cout << "Enter course number: ";
				cin >> courseNumber;

				course = bst->Search(courseNumber);

				if (!course.courseNumber.empty()) {
					displayCourse(course);
				} else {
					cout << "Course Number " << courseNumber << " not found." << endl;
				}
        	}
        	else {
        		cout << "Please load the course data first." << endl;
        	}
            break;

        	case 9 :
        		break;

			default:
				cout << "Invalid choice. Please enter 1, 2, 3, or 9." << endl;
				break;
        }
    }

    cout << "Good bye." << endl;

    delete bst;

	return 0;
}
