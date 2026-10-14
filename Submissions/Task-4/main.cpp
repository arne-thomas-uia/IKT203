#include <iostream> 
#include "DebugUtils.h"


/*
Part 1: Implementing the Core Data Structures

1. Implementing a Stack:
  a) Create a simple TStack class that can hold int values.
  b) Use a fixed-size array and a top-of-stack index to manage the data.
  c) Implement the core methods: Push(int item) and Pop().
  d) Add a Peek() method to view the top item without removing it.
  e) Include an IsEmpty() method to check if the stack is empty.

2. Implementing a Queue:
  a) Create a simple TQueue class that can hold int values.
  b) Use a fixed-size array and front/back indices to manage the data.
  c) Implement the core methods: Enqueue(int item) and Dequeue().
  d) Add a Peek() method to view the item at the front without removing it.
  e) Include an IsEmpty() method to check if the queue is empty.
*/

class TStack {

private:
	static const int MAX_SIZE = 100; // Maximum size of the stack
	int stack[MAX_SIZE];              // Array to hold stack elements
	int top;                          // Index of the top element

public:
	TStack() : top(-1) {} // Constructor initializes top to -1 (empty stack)
	void Push(int item) {
		if (top >= MAX_SIZE - 1) {
			Debug::TLogger::Log(Debug::ELevel::Error, "Stack overflow: cannot push " + std::to_string(item));
			return;
		}
		stack[++top] = item; // Increment top and add item
	}
	int Pop() {
		if (IsEmpty()) {
			Debug::TLogger::Log(Debug::ELevel::Error, "Stack underflow: cannot pop from empty stack");
			return -1; // Return an invalid value to indicate error
		}
		return stack[top--]; // Return top item and decrement top
	}
	int Peek() const {
		if (IsEmpty()) {
			Debug::TLogger::Log(Debug::ELevel::Error, "Stack is empty: cannot peek");
			return -1; // Return an invalid value to indicate error
		}
		return stack[top]; // Return the top item without removing it
	}
	bool IsEmpty() const {
		return top == -1; // Stack is empty if top is -1
	}

	bool IsFull() const {
		return top >= MAX_SIZE - 1; // Stack is full if top is at MAX_SIZE - 1
	}
};


class TQueue {
private:
	static const int MAX_SIZE = 100; // Maximum size of the queue
	int queue[MAX_SIZE];              // Array to hold queue elements
	int front;                        // Index of the front element
	int back;                         // Index of the back element
	int count;                        // Number of elements in the queue
public:
	TQueue() : front(0), back(-1), count(0) {} // Constructor initializes indices and count
	void Enqueue(int item) {
		if (count >= MAX_SIZE) {
			Debug::TLogger::Log(Debug::ELevel::Error, "Queue overflow: cannot enqueue " + std::to_string(item));
			return;
		}
		back = (back + 1) % MAX_SIZE; // Circular increment
		queue[back] = item;           // Add item to the back
		count++;                      // Increment count
	}
	int Dequeue() {
		if (IsEmpty()) {
			Debug::TLogger::Log(Debug::ELevel::Error, "Queue underflow: cannot dequeue from empty queue");
			return -1; // Return an invalid value to indicate error
		}
		int item = queue[front];      // Get the front item
		front = (front + 1) % MAX_SIZE; // Circular increment
		count--;                      // Decrement count
		return item;                  // Return the dequeued item
	}
	int Peek() const {
		if (IsEmpty()) {
			Debug::TLogger::Log(Debug::ELevel::Error, "Queue is empty: cannot peek");
			return -1; // Return an invalid value to indicate error
		}
		return queue[front];          // Return the front item without removing it
	}
	bool IsEmpty() const {
		return count == 0;            // Queue is empty if count is 0
	}
};

/*
Part 2: Practical Applications

3. String Reversal with a Stack:
	a) Write a function that takes a string as input and uses your TStack to return the reversed string.
	b) In a short comment, explain why the stack is the perfect tool for this type of task.
4. Recursive Functions with a Stack:
	a) Remember the factorial function you implemented with recursion in Submission 2? Your computer used a hidden "call stack" to make that happen. Now, your task is to re-implement that function without recursion, using your own `TStack` to manage the process.
  b) This is a fantastic exercise that will give you a "eureka" moment about how recursion truly works!
5. Wait Line Simulation with a Queue:
  a) Simulate a simple waiting line, such as a ticket counter.
  b) Use your TQueue to manage a list of people (represented by integer IDs).
  c) People should Enqueue when they arrive and Dequeue when they are served, clearly demonstrating the FIFO principle.
*/

// Part 2.3 - a)
static std::string ReverseString(const std::string& input) {
	TStack stack; // Create a stack instance
	for (char ch : input) {
		stack.Push(ch); // Push each character onto the stack
	}
	std::string reversed;
	while (!stack.IsEmpty()) {
		reversed += static_cast<char>(stack.Pop()); // Pop characters to form the reversed string
	}
	return reversed;
}

// Part 2.3 - b)
// The stack is the perfect tool for string reversal because it follows the Last-In-First-Out (LIFO) principle. When we push characters onto the stack, the last character pushed will be the first one popped off, effectively reversing the order of characters.

// Part 2.4 - a)
// Implement a recursive function to calculate the factorial of a number using your TStack.
static int Factorial(int n) {
	if (n < 0) {
		Debug::TLogger::Log(Debug::ELevel::Error, "Factorial is not defined for negative numbers.");
		return -1; // Return an invalid value to indicate error
	}
	TStack stack; // Create a stack instance
	int result = 1;
	for (int i = n; i > 1; --i) {
		stack.Push(i); // Push each number onto the stack
	}
	while (!stack.IsEmpty()) {
		result *= stack.Pop(); // Pop numbers and multiply to get the factorial
	}
	return result;
}

// Part 2.5 - a)
static void SimulateWaitLine() {
	TQueue queue; // Create a queue instance
	// Simulate people arriving (IDs 1 to 5)
	for (int i = 1; i <= 5; ++i) {
		queue.Enqueue(i);
		Debug::TLogger::Log(Debug::ELevel::Information, "Person {} has arrived and joined the line.", i);
	}
	// Simulate serving people
	while (!queue.IsEmpty()) {
		int servedPerson = queue.Dequeue();
		Debug::TLogger::Log(Debug::ELevel::Information, "Person {} has been served and left the line.", servedPerson);
		if (servedPerson == 3) {
			queue.Enqueue(2);
			Debug::TLogger::Log(Debug::ELevel::Information, "Person 2 has rejoined the line after being served.");
		}
	}

	Debug::TLogger::Log(Debug::ELevel::Information, "All people have been served and the line is now empty -> {}", queue.IsEmpty());
}


/*
Part 3: Advanced Traversal - Stacks vs. Queues
This is the main event! You will solve the same problem using two different approaches, highlighting the different strengths of stacks and queues. Your recursive functions from Submission 2 actually use an implicit stack, so this task will give you a deeper look into how it all works.
6. Setup: The 100x100 Grid:
	a) Create a 100x100 two-dimensional integer array.
	b) Populate the array with random integer values between 0 and 9.
	c) Choose a random starting cell (row, col).
	d) Create a second 100x100 boolean array to keep track of visited cells.
7. Depth-First Search (DFS) with a Stack:
	a) Write a function that uses your TStack to perform a DFS on the grid, starting from your random cell.
	b) The goal is to find the first occurrence of the number '0'.
	c) In a short comment, explain how the LIFO behavior of the stack guides the search to explore as deeply as possible along one path before backtracking.
9. Breadth-First Search (BFS) with a Queue:
	a) Write a second function that uses your TQueue to perform a BFS on the grid, starting from the same random cell.
	b) The goal is to find the first occurrence of the number '0'.
	c) In a short comment, explain how the FIFO behavior of the queue guides the search to explore the grid level by level.
9. Compare and Contrast:
	a) Write a short paragraph explaining the key differences between the DFS and BFS algorithms.
	b) Describe a scenario where you would choose one algorithm over the other. For example, which one would be better for finding the shortest path between two points?
*/

// part 3.6 - a)
int grid[100][100]; // 100x100 grid
bool visited[100][100]; // 100x100 boolean array to keep track of visited cells
bool gridHasZero = false; // Flag to indicate if '0' is present in the grid

static void InitializeGrid() {
	// Populate the grid with random integers between 0 and 9
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 100; ++j) {
			grid[i][j] = rand() % 10; // Random number between 0 and 9
			visited[i][j] = false; // Initialize visited array to false
			gridHasZero = gridHasZero || (grid[i][j] == 0); // Check if '0' is present
		}
	}
}

static void DFS(int startRow, int startCol) {
	TStack stack; // Create a stack for DFS
	stack.Push(startRow * 100 + startCol); // Push the starting cell onto the stack
	bool found = false; // Flag to indicate if '0' is found
	while (!stack.IsEmpty() && !stack.IsFull() && !found) {
		int cell = stack.Pop();
		int row = cell / 100;
		int col = cell % 100;
		if (row < 0 || row >= 100 || col < 0 || col >= 100 || visited[row][col]) {
			continue; // Skip out-of-bounds or already visited cells
		}
		visited[row][col] = true; // Mark the cell as visited
		if (grid[row][col] == 0) {
			Debug::TLogger::Log(Debug::ELevel::Information, "Found '0' at ({}, {}) using DFS.", row, col);
			found = true;
			return; // Found the target
		}
		// Push neighboring cells onto the stack (up, down, left, right)
		stack.Push((row - 1) * 100 + col); // Up
		stack.Push((row + 1) * 100 + col); // Down
		stack.Push(row * 100 + (col - 1)); // Left
		stack.Push(row * 100 + (col + 1)); // Right
	}
	Debug::TLogger::Log(Debug::ELevel::Information, "No '0' found in the grid using DFS. Grid has zero: {}", gridHasZero);

	// The LIFO behavior of the stack allows DFS to explore as deeply as possible along one path before backtracking, which is useful for finding a solution in scenarios where the solution is located deep within a structure.
}

int main()
{
	Debug::TLogger::SetLevel(Debug::ELevel::Verbose);
	std::string original = "Hello, World!";
	std::string reversed = ReverseString(original);
	Debug::TLogger::Log(Debug::ELevel::Information, "Original: " + original);
	Debug::TLogger::Log(Debug::ELevel::Information, "Reversed: " + reversed);

	int number = 5;
	int fact = Factorial(number);
	Debug::TLogger::Log(Debug::ELevel::Information, "Factorial of {} is {}", number, fact);

	SimulateWaitLine();

	InitializeGrid();
	DFS(rand() % 100, rand() % 100);

}
