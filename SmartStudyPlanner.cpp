#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

const int MAX_TASKS = 20;
const char FILE_NAME[] = "tasks.txt";

class Task {
public:
    char subject[30];
    char taskName[100];
    char deadline[20];
    char priority[10];
    char status[15];
};

Task tasks[MAX_TASKS];
int taskCount = 0;
void printLine() {
    cout << "-----------------------------------------------------" << endl;
}
void printTask(int i) {
    cout << "  Task #     : " << i + 1 << endl;
    cout << "  Subject    : " << tasks[i].subject << endl;
    cout << "  Task Name  : " << tasks[i].taskName << endl;
    cout << "  Deadline   : " << tasks[i].deadline << endl;
    cout << "  Priority   : " << tasks[i].priority << endl;
    cout << "  Status     : " << tasks[i].status << endl;
    printLine();
}
int priorityValue(const char *p) {
    if (strcmp(p, "High") == 0) return 1;
    if (strcmp(p, "Medium") == 0) return 2;
    return 3;
}
void addTask() {
    if (taskCount >= MAX_TASKS) {
        cout << "  [!] Planner is full! Cannot add more tasks." << endl;
        return;
    }
    printLine();
    cout << "  ADD NEW TASK" << endl;
    printLine();
    cin.ignore();
    cout << "  Enter Subject Name : ";
    cin.getline(tasks[taskCount].subject, 30);
    cout << "  Enter Task Name    : ";
    cin.getline(tasks[taskCount].taskName, 100);
    cout << "  Enter Deadline     : ";
    cin.getline(tasks[taskCount].deadline, 20);

    while (true) {
        cout << "  Enter Priority (High / Medium / Low) : ";
        cin.getline(tasks[taskCount].priority, 10);
        if (strcmp(tasks[taskCount].priority, "High") == 0 ||
            strcmp(tasks[taskCount].priority, "Medium") == 0 ||
            strcmp(tasks[taskCount].priority, "Low") == 0) {
            break;
        }
        cout << "  [!] Invalid priority. Please type exactly: High, Medium, or Low" << endl;
    }
    strcpy(tasks[taskCount].status, "Pending");
    taskCount++;
    cout << "  [OK] Task added successfully!" << endl;
}
void viewAllTasks() {
    printLine();
    cout << "  ALL TASKS (" << taskCount << " total)" << endl;
    printLine();

    if (taskCount == 0) {
        cout << "  No tasks found. Please add a task first." << endl;
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        printTask(i);
    }
}
void markCompleted() {
    printLine();
    cout << "  MARK TASK AS COMPLETED" << endl;
    printLine();

    if (taskCount == 0) {
        cout << "  No tasks available." << endl;
        return;
    }
    viewAllTasks();

    cout << "  Enter Task Number to mark as completed : ";
    int choice;
    cin >> choice;

    if (choice < 1 || choice > taskCount) {
        cout << "  [!] Invalid task number." << endl;
        return;
    }

    int index = choice - 1;

    strcpy(tasks[index].status, "Completed");
    cout << "  [OK] Task marked as Completed!" << endl;
}
void viewHighPriority() {
    printLine();
    cout << "  HIGH PRIORITY TASKS" << endl;
    printLine();

    int found = 0;

    for (int i = 0; i < taskCount; i++) {
        if (strcmp(tasks[i].priority, "High") == 0) {
            printTask(i);
            found++;
        }
    }

    if (found == 0) {
        cout << "  No High priority tasks found." << endl;
    }
}
void progressReport() {
    printLine();
    cout << "  PROGRESS REPORT" << endl;
    printLine();

    int completed = 0;
    int pending = 0;

    for (int i = 0; i < taskCount; i++) {
        if (strcmp(tasks[i].status, "Completed") == 0)
            completed++;
        else
            pending++;
    }

    cout << "  Total Tasks     : " << taskCount << endl;
    cout << "  Completed Tasks : " << completed << endl;
    cout << "  Pending Tasks   : " << pending << endl;

    if (taskCount > 0) {
        int percent = (completed * 100) / taskCount;
        cout << "  Completion      : " << percent << "%" << endl;
    }

    printLine();
}
void saveToFile() {
    ofstream outFile(FILE_NAME);

    if (!outFile) {
        cout << "  [!] Error: Could not open file for saving." << endl;
        return;
    }
    for (int i = 0; i < taskCount; i++) {
        outFile << tasks[i].subject << endl;
        outFile << tasks[i].taskName << endl;
        outFile << tasks[i].deadline << endl;
        outFile << tasks[i].priority << endl;
        outFile << tasks[i].status << endl;
    }

    outFile.close();
    cout << "  [OK] Tasks saved successfully!" << endl;
}
void loadFromFile() {
    ifstream inFile(FILE_NAME);

    if (!inFile) {
        cout << "  [!] No saved file found." << endl;
        return;
    }

    inFile >> taskCount;
    inFile.ignore();

    for (int i = 0; i < taskCount; i++) {
        inFile.getline(tasks[i].subject, 30);
        inFile.getline(tasks[i].taskName, 100);
        inFile.getline(tasks[i].deadline, 20);
        inFile.getline(tasks[i].priority, 10);
        inFile.getline(tasks[i].status, 15);
    }

    inFile.close();
    cout << "  [OK] Tasks loaded successfully!" << endl;
}
void sortByPriority() {
    for (int i = 0; i < taskCount - 1; i++) {
        for (int j = 0; j < taskCount - i - 1; j++) {
            if (priorityValue(tasks[j].priority) > priorityValue(tasks[j + 1].priority)) {
                Task temp = tasks[j];
                tasks[j] = tasks[j + 1];
                tasks[j + 1] = temp;
            }
        }
    }

    cout << "  [OK] Tasks sorted by priority" << endl;
    viewAllTasks();
}
void sortByDeadline() {
    for (int i = 0; i < taskCount - 1; i++) {
        for (int j = 0; j < taskCount - i - 1; j++) {
            if (strcmp(tasks[j].deadline, tasks[j + 1].deadline) > 0) {
                Task temp = tasks[j];
                tasks[j] = tasks[j + 1];
                tasks[j + 1] = temp;
            }
        }
    }

    cout << "  [OK] Tasks sorted by deadline" << endl;
    viewAllTasks();
}
void smartRecommendation() {
    printLine();
    cout << "  SMART TASK RECOMMENDATION" << endl;
    printLine();

    int bestIndex = -1;

    for (int i = 0; i < taskCount; i++) {
        if (strcmp(tasks[i].status, "Pending") != 0) continue;

        if (bestIndex == -1 ||
            priorityValue(tasks[i].priority) < priorityValue(tasks[bestIndex].priority)) {
            bestIndex = i;
        }
    }

    if (bestIndex == -1) {
        cout << "  [OK] Great job! All tasks are completed!" << endl;
    } else {
        cout << "  >>> You should work on this task first: <<<" << endl;
        printTask(bestIndex);
    }
}
void showMenu() {
    cout << endl;
    cout << "  ============================================" << endl;
    cout << "          SMART STUDY PLANNER                 " << endl;
    cout << "  ============================================" << endl;
    cout << "   1.  Add New Task                           " << endl;
    cout << "   2.  View All Tasks                         " << endl;
    cout << "   3.  Mark Task as Completed                 " << endl;
    cout << "   4.  View High Priority Tasks               " << endl;
    cout << "   5.  Progress Report                        " << endl;
    cout << "   6.  Save Tasks to File                     " << endl;
    cout << "   7.  Load Tasks from File                   " << endl;
    cout << "   8.  Sort Tasks by Priority                 " << endl;
    cout << "   9.  Sort Tasks by Deadline                 " << endl;
    cout << "  10.  Smart Task Recommendation              " << endl;
    cout << "   0.  Exit                                   " << endl;
    cout << "  ============================================" << endl;
    cout << "  Enter your choice : ";
}
int main() {
    int choice;

    cout << "Welcome to Smart Study Planner!" << endl;
    cout << "  Tip: Use deadline format YYYY-MM-DD (e.g. 2025-06-20)" << endl;

    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1: addTask(); break;
            case 2: viewAllTasks(); break;
            case 3: markCompleted(); break;
            case 4: viewHighPriority(); break;
            case 5: progressReport(); break;
            case 6: saveToFile(); break;
            case 7: loadFromFile(); break;
            case 8: sortByPriority(); break;
            case 9: sortByDeadline(); break;
            case 10: smartRecommendation(); break;
            case 0: cout << "  Goodbye! Keep studying hard!" << endl; break;
            default: cout << "  Invalid choice." << endl;
        }

    } while (choice != 0);
   return 0;
}
