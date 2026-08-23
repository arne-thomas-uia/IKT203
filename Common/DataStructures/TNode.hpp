/// <summary>
/// A node class for a doubly-linked list that stores data of a generic type.
/// </summary>
/// <typeparam name="T">The type of data stored in the node.</typeparam>

template <typename T>
class TNode {
private:
	T data;        // Data of type T (e.g., TSong*)
	TNode* next;   // Pointer to the next node
	TNode* prev;   // Pointer to the previous node
public:
	// Constructor

	TNode(T aData);

	// Destructor
	~TNode();

	// Getters
	T GetData() const;
	TNode* GetNext() const;
	TNode* GetPrev() const;

	// Setters
	void SetNext(TNode* aNextNode);
	void SetPrev(TNode* aPrevNode);
};

// --- Method Implementations ---
// Constructor: Initializes the node with data from the factory function
template <typename T>
TNode<T>::TNode(T aData) : data(aData), next(nullptr), prev(nullptr) {}

// Destructor: Deletes the data pointer
template <typename T>
TNode<T>::~TNode() {
	//Do not delete data, data may exist outside the list
	data = nullptr; // Set data to nullptr to avoid dangling pointer
	next = nullptr; // Set next to nullptr to avoid dangling pointer
	prev = nullptr; // Set prev to nullptr to avoid dangling pointer
}

template <typename T>
T TNode<T>::GetData() const { return data; }

template <typename T>
TNode<T>* TNode<T>::GetNext() const { return next; }

template <typename T>
TNode<T>* TNode<T>::GetPrev() const { return prev; }

template <typename T>
void TNode<T>::SetNext(TNode* aNextNode) { next = aNextNode; }

template <typename T>
void TNode<T>::SetPrev(TNode* aPrevNode) { prev = aPrevNode; }
