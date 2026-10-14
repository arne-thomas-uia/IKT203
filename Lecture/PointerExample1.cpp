
//***************************************************************
// Pointer Example 2: Using old C++ style function pointers
//***************************************************************

// 1. The concrete compare function
int compareInt(int val1, int val2) {
	return (val1 > val2) - (val1 < val2);
}

// 2. Declare the pointer type
typedef int (*CompareFunc)(int, int);

// 3. Usage: "borrow" the address
CompareFunc FCompare = &compareInt;

// 4. Call the function through the pointer
int result = FCompare(5, 10);

//***************************************************************
// Pointer Example 2: Using old C++ style function pointers
//***************************************************************

// 1. The concrete compare function
int compareFloat(float val1, float val2) {
	return (val1 > val2) - (val1 < val2);
}

// 2. Declare the pointer type
using CompareFuncFloat = int (*)(float, float);

// 3. Usage: "borrow" the address
CompareFuncFloat FCompareFloat = &compareFloat;

// 4. Call the function through the pointer
int resultFloat = FCompareFloat(5.0f, 10.0f);

//***************************************************************
// Pointer Example 3: Pointer to Member Function
//***************************************************************

// 1. A simple class with a member function
class TPlayer {
public:
  void SetHealth(int val) {
    // . . . .
  }
};

int main() {
  // 2. Declare the member pointer and assign it 
  // (Notice we must use TPlayer:: to specify WHICH class it belongs to)
  void (TPlayer:: * actionPtr)(int) = &TPlayer::SetHealth;

  // 3. Create the actual objects (the necessary "this" context)
  TPlayer myPlayer;               // Stack object
  TPlayer* playerPtr = &myPlayer; // Heap/Pointer object

  // 4. Invocation! We MUST combine the object and the pointer
  (myPlayer.*actionPtr)(100);     // Using .* for direct objects
  (playerPtr->*actionPtr)(100);   // Using ->* for object pointers

  return 0;
}

//***************************************************************
// Pointer Example 4: The Static Wrapper: C++'s Native .bind(this)
//***************************************************************

// 1. Generic C-style system expects a raw callback and a context
typedef void (*RawCallback)(void* ctx, int val);

void TriggerEvent(RawCallback callback, void* context) {
  callback(context, 100);
}

class TPlayer {
public:
  void TakeDamage(int amount) {
    // Real OOP logic happens here!
  }

  // 2. The Static Wrapper (Our manual "Bind")
  static void DamageWrapper(void* ctx, int amount) {
    // 3. Recover the object instance
    TPlayer* player = static_cast<TPlayer*>(ctx);
    // 4. Invoke the real method
    player->TakeDamage(amount);
  }
};

int main() {
  TPlayer myPlayer;

  // 5. Pass the wrapper, and smuggle 'myPlayer' as the void* context
  TriggerEvent(&TPlayer::DamageWrapper, &myPlayer);

  return 0;
}