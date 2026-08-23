# 🌌 IKT203: Algorithms and Data Structures


### **DON'T PANIC!**  
Welcome to the official starter repository for IKT203! 🚀 

If you are reading this, you are about to embark on a journey deep into the bare-metal realm of C++. Here, there are no garbage collectors to save you, and standard library data structures are but a myth. You will forge your own nodes, manage your own memory, and conquer the compiler.

## 📂 The Architecture (Folder Structure)

This repository is pre-configured with a specific architecture. All `CMakeLists.txt` files have been lovingly handcrafted for you, so you can focus entirely on the code. 

Here is where everything lives:

```text
Portfolio 
├───Assignments          <-- The Final Capstone Menu (Classified until late September 🤫)
│   ├───Category-1       
│   │   ├───Option-1     
│   │   └───Option-2     
│   ├───Category-2       
│   │   ├───Option-1     
│   │   └───Option-2     
│   ├───Category-3       
│   │   ├───Option-1     
│   │   └───Option-2     
│   └───Category-4       
│       ├───Option-1     
│       └───Option-2     
├───Common               <-- Your trusty toolkit! 🛠️
│   ├───DataStructures   <-- (Where your generic classes will live)
│   ├───DebugUtils       <-- Free colorful console logging!
│   └───StructureReader  <-- Free file parsers (Do not modify these)
├───Structure            <-- The raw .txt data files you will be parsing
└───Submissions          <-- Your bi-weekly training grounds
    ├───Task-1           
    ├───Task-2           
    ├───Task-3           
    ├───Task-4           
    ├───Task-5           
    └───Task-6           
```

### 📝 Submissions vs. Assignments
*   **`Submissions/` (Task 1 to 6):** These are your mandatory, bi-weekly training missions. They are graded **Pass/Fail**. You must conquer all of these to qualify for the final exam.
*   **`Assignments/` (Category 1 to 4):** This is the final Portfolio capstone (graded A-F). The exact nature of these assignments is currently shrouded in mystery. All will be revealed in due time.

---

## 📜 The Sacred Rules of IKT203

1.  **Manual Memory Management Only:** 
    >**Master Yoda**: *"Use smart pointers, you must not. Manage memory manually, you will."*
    
    You are strictly forbidden from using `std::unique_ptr`, `std::shared_ptr`, or the Standard Template Library (STL) data structures like `std::vector` or `std::list`. You allocate it with `new`, you clean it up with `delete`. 

2.  **Beware the Segfault:**
    > *"Bazinga!" — Your compiler, right after throwing a Segmentation Fault because you dereferenced a null pointer.*
    
    Check your pointers. Then check them again. 

3.  **Keep It Clean (SoC & DRY):**
    Don't repeat yourself, and keep your concerns separated. If your Sorting class is printing text to the console, you have committed a grave architectural sin.

## 🚀 Getting Started

The project is fully CMake-enabled. You can open the root folder in CLion, Visual Studio, or VS Code (with the CMake Tools extension), and it should immediately configure all the sub-projects. 

If it doesn't compile right out of the box... well, as they say in the IT crowd: *Have you tried turning it off and on again?*

## 📚 Using the StructureReaderLib (How to get your data)

Since you are strictly forbidden from using standard containers like `std::vector` to hold your data, we couldn't just hand you a file parser that returns a pre-packaged array. That would be too easy! 

Instead, the `StructureReaderLib` uses **Callbacks**. 

When you tell the library to read a file, you must pass it a function. The library will parse the file line-by-line and "call back" your function for every single record it finds. This gives you the power to instantiate your own objects with `new` and inject them directly into your custom data structures on the fly.

### ⚠️ Instructor Recommendation:
Do not use lambdas! While lambdas are an easy way out for the developer, they hide the underlying mechanics. To truly master C++ in this course, I want you to create dedicated free functions (or static class methods) and pass those as your callbacks. Let's keep it bare-metal.

Here is a quick example of how to read the names file the right way:

```cpp
#include <iostream>
#include "StructureReaderLib.h" // Wait, how does the compiler know where this is?! (CMake magic 🪄)

// 1. Create your dedicated callback function
bool OnNameDiscovered(const int index, const int total, const std::string& first, const std::string& last) {
    
    // Only print the first 5 records to prevent massive console spam! 😂
    if (index < 5) {
        std::cout << "Processing record [" << index + 1 << "/" << total << "]: " << first << " " << last << "\n";
    }
    else if (index == 5) {
        std::cout << "...\n(Reading remaining 9,995 names silently to save your terminal!)\n...\n";
    }
    
    // 🚀 YOUR MISSION HERE: 
    // 1. Create a new object on the heap (e.g., new TPerson(first, last))
    // 2. Insert it into your custom Linked List, Tree, or Graph!
    
    // Return true to keep reading. Return false to abort early.
    return true; 
}  

int main() {
    std::cout << "Initiating data sequence...\n";

    // 2. Pass the function pointer to the reader
    readNamesFromFile(&OnNameDiscovered);

    return 0;
}
```

## 🧠 The IKT203 Philosophy: Lean Code in a "Fat & Slow" World

You might be asking yourself: *"Why are we manually managing pointers and building linked lists from scratch? My gaming rig has 32GB of RAM and a 16-core CPU. Hardware is fast and cheap!"*

To answer that, I will quote the AI that helped me draft this very document: 

> *"The mindset of 'hardware is fast and cheap, so code can be sloppy' is exactly how the industry ends up with simple chat applications that consume 2 Gigabytes of RAM. Teaching you to respect the machine is what separates code mechanics from true software engineers."* 🤖✨

Here is the bare-metal truth you will learn in this course:

*   **The Math Always Wins:** Hardware improvements scale linearly, but bad algorithms scale exponentially. A modern CPU cannot save you if you accidentally write an O(n²) algorithm to process millions of data points. 
*   **The Cache Crisis:** We have massive amounts of RAM today, but the CPU cache remains comparatively tiny. If you write scattered, poorly managed data structures, you trigger constant cache misses, effectively starving your ultra-fast CPU of data.
*   **The Real-World Cost:** In the professional world, "fat and slow" code doesn't just annoy the user; it costs companies real money in server compute time. On mobile, inefficient data processing drains batteries rapidly. Lean code is green code!

By stripping away the "magic" of standard libraries and forcing you to physically allocate every single node, you will realize exactly how much memory bloat you are creating. You are about to gain a superpower: the ability to write lean, highly optimized logic in an industry overflowing with heavy frameworks.

Good luck, have fun, and remember: your towel (and a good debugger) is your best friend. 🤓