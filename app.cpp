#include	<iostream>
#include 	<fstream>
#include	<string>
#include	<cstdlib>
#include	"List.h"
#include	"Student.h"

using namespace std;


bool CreateStuList(const char*, List*);
bool DeleteStudent(List*, char*);
bool PrintList(List, int);
bool InsertExamResult(const char* filename, List* list);
bool PrintStatistic(List);
bool FilterStudent(List, List*, char*, int, int);
bool UpdateIDandPhone(List*);
bool FindPotentialFirstClass(List, List*, char*);
int menu();
void removedash(char[]);
void ClearList2(List*);

using namespace std;


int main() {
	char id[8], course[3];
	List list, list2;
	const char* filename;
	int choice, source, year, totalcredit;

	do {
		choice = menu();

		switch (choice) {
		case 1:
			system("CLS");
			filename = "student.txt";
			if (CreateStuList(filename, &list))
				cout << "Successfully read and create student list!" << endl << endl;
			else
				cout << "Unable to read and create student list!" << endl << endl;
			system("pause");
			system("CLS");
			break;

		case 2:
			system("CLS");
			cout << "Enter the ID of the student you want to delete: ";
			cin >> id;
			if (DeleteStudent(&list, id)) {
				cout << "Student with ID " << id << " is successfully deleted." << endl;
			}
			else {
				cout << "Student with ID " << id << " is not found." << endl;
			}
			system("pause");
			system("CLS");
			break;

		case 3:
			system("CLS");
			cout << "***'1' to display on screen or '2' to write to a file*** \n\n";
			cout << "Your choice: ";
			cin >> source;

			//check the validity of variable source
			if (source != 1 && source != 2) {
				cout << "Invalid source. Please enter 1 or 2." << endl;
				return 1;
			}

			if (!PrintList(list, source)) {
				cout << "Failed to print the student list." << endl;
				return 1;
			}
			system("pause");
			system("CLS");
			break;

		case 4:
			system("CLS");
			filename = "exam.txt";
			if (InsertExamResult(filename, &list)) {
				cout << "Exam results successfully inserted into the student list." << endl;
			}
			else {
				cout << "Failed to insert exam results into the student list." << endl;
				return 1;
			}
			system("pause");
			system("CLS");
			break;

		case 5:
			system("CLS");
			cout << "Statistics for Student List:" << endl;
			PrintStatistic(list);
			system("pause");
			system("CLS");
			break;

		case 6:
			system("CLS");
			//filter student based on course,intake year and total credit earned
			cout << "insert the course of student (Upper Cases): ";
			cin >> course;
			cout << "insert the intake year of student: ";
			cin >> year;
			cout << "insert the totalcredit of student: ";
			cin >> totalcredit;
			if (FilterStudent(list, &list2, course, year, totalcredit)) {
				cout << "Successfully print student list" << endl << endl;
				PrintList(list2, 1); //print the student list that match these three condition
				ClearList2(&list2); //clear the list2 after finish printing
			}
			else
				cout << "Unable to print student list!" << endl << endl;
			
			system("pause");
			system("CLS");
			break;

		case 7:
			system("CLS");
			//update id and phone number
			if (UpdateIDandPhone(&list)) {
				cout << "Student ID and phone numbers updated successfully." << endl;
				PrintList(list, 1); // Print the updated list to the screen
			}
			else {
				cout << "Failed to update student ID and phone numbers." << endl;
			}
			system("pause");
			system("CLS");
			break;

		case 8:
			system("CLS");
			//find potential first class student
			cout << "Enter the course (Upper Cases): ";
			cin >> course;

			if (FindPotentialFirstClass(list, &list2, course)) {
				cout << "Potential first class students in " << course << ":" << endl;
				PrintList(list2, 1); // Print list2 to the screen
				ClearList2(&list2);
			}
			else
				cout << "Failed to find potential first class students." << endl;
			system("pause");
			system("CLS");
			break;

		case 9:
			cout << "Exiting program..." << endl;
			break;

		default:
			system("CLS");
			cout << "Invalid choice!" << endl;
			break;
		}
	} while (choice != 9);

	system("pause");
	return 0;
}

bool CreateStuList(const char* filename, List* list) {
	Student student, tmp;
	int i;
	string store;
	ifstream infile(filename);
	bool exist = false;

	if (infile.fail())
	{
		cout << "Unable to open " << filename << endl;
		return 0;
	}
	else
	{
		while (!infile.eof())
		{
			exist = 0;
			infile >> store >> store >> store >> student.id;
			infile >> store >> store;
			infile.ignore();
			infile.getline(student.name, 40);
			infile >> store >> store >> student.course;
			infile >> store >> store >> store >> student.phone_no;

			if (strlen(student.name) == 0)
				break;

			//Check whether the duplicate record exist
			for (i = 1; i <= list->size(); i++)
			{
				list->get(i, tmp);
				if (tmp.compareID(student))
				{
					exist = 1;
					break;
				}
			}
			if (exist)
				cout << student.id << " is a duplicate record.\n";
			else
				list->insert(student);
		}
	}

	if (list->empty()) {

		cout << "The file is empty." << endl;
		return false;

	}

	if (exist)

		cout << endl;

	infile.close();
	return 1;

}

bool DeleteStudent(List* list, char* id) {
	if (list->empty()) {
		cout << "The list is empty.\n";
		return 0;
	}
	Node* cur = list->head;
	Node* prev = NULL;

	while (cur != NULL) {
		if (strcmp(cur->item.id, id) == 0) {
			// Found the node with the specified ID
			if (prev == NULL) {
				list->head = cur->next;
			}
			else {
				prev->next = cur->next;
			}
			delete cur;
			return 1;
		}
		prev = cur;
		cur = cur->next;
	}
	return 0;
}

bool PrintList(List list, int source) {
	if (list.empty()) {
		cout << "The list is empty." << endl;
		return false;
	}

	ofstream outfile;
	if (source == 2) {
		outfile.open("student_result.txt");
		if (outfile.fail()) {
			cout << "Unable to open the file." << endl;
			return false;
		}
	}

	Node* cur = list.head;
	int studentNo = 1;

	while (cur != NULL) {
		//print on screen
		if (source == 1) {
			cout << "******************************************************STUDENT " << studentNo << "******************************************************" << endl;
			cur->item.print(cout);

			if (cur->item.exam_cnt == 0) {
				cout << "THIS STUDENT HAVEN'T TAKEN ANY EXAM YET" << endl;
			}
			else {
				cout << "--------------------------------------------------PAST EXAM RESULT:--------------------------------------------------" << endl;
				for (int i = 0; i < cur->item.exam_cnt; ++i) {
					cur->item.exam[i].print(cout);
					cout << endl;
				}
			}
			cout << "******************************************************STUDENT " << studentNo << "******************************************************" << endl << endl;
		}
		//print the result into "student_result.txt"
		else if (source == 2 && outfile.is_open()) {
			outfile << "******************************************************STUDENT " << studentNo << "******************************************************" << endl;
			cur->item.print(outfile);
			if (cur->item.exam_cnt == 0) {
				outfile << "THIS STUDENT HAVEN'T TAKEN ANY EXAM YET" << endl;
			}
			else {
				outfile << "--------------------------------------------------PAST EXAM RESULT:--------------------------------------------------" << endl;
				for (int i = 0; i < cur->item.exam_cnt; ++i) {
					cur->item.exam[i].print(outfile);
					outfile << endl;
				}
			}
			outfile << "******************************************************STUDENT " << studentNo << "******************************************************" << endl << endl;
		}

		cur = cur->next;
		++studentNo;
	}

	// Close the file if it was opened for writing
	if (source == 2 && outfile.is_open()) {
		outfile.close();
	}

	return true;
}

bool InsertExamResult(const char* filename, List* list) {
	ifstream infile(filename);
	if (!infile) {
		cout << "Failed to open file " << filename << endl;
		return false;
	}

	while (!infile.eof()) {

		char id[8];
		int trimester, year, numOfSubjects;
		infile >> id >> trimester >> year >> numOfSubjects;


		Exam exam;
		exam.trimester = trimester;
		exam.year = year;
		exam.numOfSubjects = numOfSubjects;


		for (int i = 0; i < numOfSubjects; ++i) {
			Subject subject;
			infile >> subject.subject_code >> subject.subject_name >> subject.credit_hours >> subject.marks;
			exam.sub[i] = subject;
		}

		Node* cur = list->head;
		while (cur != NULL) {
			if (strcmp(cur->item.id, id) == 0) {
				cur->item.exam[cur->item.exam_cnt++] = exam;
				for (int i = 0; i < cur->item.exam_cnt; i++) {
					cur->item.exam[i].calculateGPA(); //calculate GPA for current exam
					cur->item.calculateCurrentCGPA(); //calculate the student's CGPA
				}
				break;
			}
			cur = cur->next;
		}
	}

	infile.close();
	return true;
}

bool PrintStatistic(List list) {
	if (list.empty()) {
		cout << "The list is empty." << endl;
		return false;
	}

	int totalStudents = list.size();
	int CS = 0, IA = 0, IB = 0, CN = 0, CT = 0;
	double totalCGPA = 0.0, totalSubjects = 0.0, totalCredits = 0.0;

	Node* cur = list.head;
	while (cur != nullptr) {
		// Total the number of students in each course
		if (strcmp(cur->item.course, "CS") == 0) {
			CS++;
		}
		else if (strcmp(cur->item.course, "IA") == 0) {
			IA++;
		}
		else if (strcmp(cur->item.course, "IB") == 0) {
			IB++;
		}
		else if (strcmp(cur->item.course, "CN") == 0) {
			CN++;
		}
		else if (strcmp(cur->item.course, "CT") == 0) {
			CT++;
		}

		totalCGPA += cur->item.current_cgpa;

		for (int i = 0; i < cur->item.exam_cnt; ++i) {
			totalSubjects += cur->item.exam[i].numOfSubjects;
			for (int j = 0; j < cur->item.exam[i].numOfSubjects; ++j) {
				totalCredits += cur->item.exam[i].sub[j].credit_hours;
			}
		}

		cur = cur->next;
	}

	double averageCGPA = totalCGPA / totalStudents;
	double averageSubjectsPerSemester = totalSubjects / totalStudents;
	double averageCreditsPerSemester = totalCredits / (totalStudents * 3); //1 semester = 3 trimesters

	cout << "Total Students: " << totalStudents << endl;
	cout << "CS Students - " << CS << endl;
	cout << "IA Students - " << IA << endl;
	cout << "IB Students - " << IB << endl;
	cout << "CN Students - " << CN << endl;
	cout << "CT Students - " << CT << endl;
	cout << endl;
	cout << "Average CGPA: " << averageCGPA << endl;
	cout << "Average Subjects Taken Per Semester: " << averageSubjectsPerSemester << endl;
	cout << "Average Credits Earned Per Semester: " << averageCreditsPerSemester << endl;

	return true;
}

bool FilterStudent(List list1, List* list2, char* course, int year, int totalcredit) {
	Node* cur = list1.head;

	//return false when list1 is empty or list2 is not empty 
	if (list1.empty() || !list2->empty()) {
		cout << "List 2 is not empty! Please try again" << endl;
		return false;
	}

	while (cur != nullptr) {
		Student& stu = cur->item;
		int enrolledYear = stoi(stu.id) / 100000;  // Extract year from student ID by keep the first 2 digit

		//compare the course,year and total credit
		//keep the last 2 digit of year by -2000
		//the student'totalcredit greater than totalcredit insert
		if (strcmp(stu.course, course) == 0 && enrolledYear == year - 2000 && stu.totalCreditsEarned >= totalcredit) {
			list2->insert(stu);
		}

		cur = cur->next;
	}
	return true;
}

bool UpdateIDandPhone(List* list) {
	Node* cur = list->head;
	string temp;
	if (list->empty()) {
		return false;
	}

	while (cur != nullptr) {
		Student& stu = cur->item;
		// Update student ID
		string course(stu.course, 2);
		string newID = "B" + course + stu.id;
		strcpy(stu.id, newID.c_str());

		// Update phone number
		removedash(stu.phone_no); //remove the '-' in phone number
		for (int i = 0; i < 9; i++) {
			temp += stu.phone_no[i];
		}

		if (stu.phone_no[0] % 2 == 1) {
			//if the first phone number is odd
			string newphone = "01" + temp;
			strcpy(stu.phone_no, newphone.c_str());
		}
		else {
			//if the first phone number is even
			string newphone = "02" + temp;
			strcpy(stu.phone_no, newphone.c_str());
		}

		cur = cur->next;
	}

	return true;
}

bool FindPotentialFirstClass(List list1, List* list2, char* course) {
	Student stu;
	Exam exam;
	Subject sub;
	Node* cur = list1.head;
	bool found = false;
	//return false when list1 is empty or list2 is not empty 
	if (list1.empty() || !list2->empty()) {
		cout << "List 2 is not empty! Please try again" << endl;
		return false;
	}

	while (cur != nullptr) {
		stu = cur->item;
		//compare the course and check whether student has taken >3 exams
		if (strcmp(stu.course, course) == 0 && stu.exam_cnt >= 3) {
			int trimesters = 0, total_credit = 0;
			double sum = 0;
			bool LowGPA = false;
			for (int i = 0; i < cur->item.exam_cnt; i++) {
				exam = cur->item.exam[i];
				//calculate the credit hours earned in this trimester
				for (int i = 0; i < exam.numOfSubjects; i++) {
						total_credit += exam.sub[i].credit_hours;
				}
				if (exam.gpa >= 3.75000 && total_credit >= 12) {
					trimesters++;
				}
				else if (exam.gpa < 3.50000) {
					LowGPA = true;
					break;
				}
				total_credit = 0;
			}

			if (trimesters >= 3 && !LowGPA) {
				list2->insert(stu);
				found = true;
			}
		}
		cur = cur->next;
	}

	if (!found) {
		cout << "There is no student in " << course << " that has potential to get first class" << endl;
		return false;
	}
	else
		return true;
}

int menu() {
	int choice;

	do {
		cout << "======= FICT Student Management System =======\n";
		cout << "1. Create student list\n";
		cout << "2. Delete Student\n";
		cout << "3. Print student list\n";
		cout << "4. Insert exam result\n";
		cout << "5. Print Exam Statistic\n";
		cout << "6. Filter Student\n";
		cout << "7. Update Student's ID and Phone\n";
		cout << "8. Find Potential First Class Student\n";
		cout << "9. Exit\n";
		cout << "Enter your choice: ";
		cin >> choice;

		if (choice < 1 || choice > 9) {
			cout << "Invalid choice! Please try again.\n";
		}

	} while (choice < 1 || choice > 9);

	return choice;
}

//remove the '-' in the phone number
void removedash(char phone[]) {
	phone[3] = ' '; //replace the '-' with space
	//move the space to the position after the last number
	for (int i = 3; i < 8; i++) {
		char temp;
		temp = phone[i + 1];
		phone[i + 1] = phone[i];
		phone[i] = temp;
	}
}

//clear list2 after finish print
void ClearList2(List* list2) {
	Node* cur = list2->head;

	// Traverse the list and delete each node
	for(int i=1;i<list2->size();i++){
		list2->remove(i);
		cur = cur->next;
	}
	list2->head = nullptr;
	list2->count = 0;
}
