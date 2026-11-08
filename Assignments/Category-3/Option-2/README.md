## **The Hook: Why Choose the Calculator?**

"Choosing the standard Employee Directory BST gets you 200 points. It teaches you how to store data. But choosing the Interpreted Calculator gets you 300 points, and more importantly, it teaches you how to **build a software engine**.

If you have ever wondered how Python executes your scripts, how Unreal Engine parses Blueprints, or how a web browser reads JavaScript, this is it. You are going to build an Abstract Syntax Tree (AST). You aren't just storing data anymore; you are making data *do* things."

## **1\. Tokens & The Lexer (Bringing Order to Chaos)**

**The Problem:** The user types `"calc (x + 5) * 3"`. To the computer, this is just a chaotic, meaningless array of characters. It doesn't know that `5` is a number, `+` is math, and `x` is a variable.

* **WHAT it is:** A Lexer (Lexical Analyzer) is a scanner. A Token is a labeled package of meaning.  
* **WHY we need it (SoC):** We must separate *reading* the text from *doing* the math (Separation of Concerns). If your tree tries to read `std::string` characters directly, you have failed the architecture. The Lexer's only job is to group characters into logical `TToken` objects (e.g., `[Identifier: "x"]`, `[Operator: "+"]`, `[Number: 5]`).  
* **HOW we build it:** We iterate through the string one `char` at a time. If we see a digit, we keep reading until the number ends, package it into a `TToken`, and push it into our custom `TQueue<TToken*>`. We completely bypass `std::vector` to prove we control the memory.  
* **WHERE this is used in the real world:**  
  * **Compilers (C++, Java, C\#):** The very first step of compiling your code is lexing it into tokens.  
  * **JSON/XML Parsers:** Whenever an app reads a configuration file or a web API response.  
  * **AI & Large Language Models:** When you type a prompt into ChatGPT, it is instantly "tokenized" before the neural network processes it.

## **2\. The Parser (The Master of Rules & PEMDAS)**

**The Problem:** Now we have a clean queue of tokens: `(`, `x`, `+`, `5`, `)`, `*`, `3`. But a queue doesn't understand that multiplication happens before addition.

* **WHAT it is:** The Parser is the grammar architect. It consumes the queue of tokens and builds a hierarchical tree based on strict mathematical rules (Recursive Descent).  
* **WHY we need it:** We need to enforce order of operations. Without a parser, `3 + 5 * 2` evaluates left-to-right as `16`. A parser ensures it evaluates as `13`.  
* **HOW we build it:** We use functions that call other functions to organically build a tree.  
  * `ParseExpression()` looks for `+` and `-`. It asks `ParseTerm()` for the numbers.  
  * `ParseTerm()` looks for `*` and `/`. It asks `ParsePower()` for the numbers.  
  * By structuring it this way, multiplication is forced *deeper* into the tree than addition, meaning it gets calculated first.  
* **WHERE this is used in the real world:**  
  * **SQL Databases:** When you type `SELECT * FROM Users WHERE Age > 18`, a parser breaks that down into a query execution tree.  
  * **Web Browsers:** Parsing HTML tags into the Document Object Model (DOM) tree.

## **3\. The Expression Node / AST (Polymorphism in Action)**

**The Problem:** We need a tree where the nodes aren't just storing data—they need to actively evaluate themselves. But a "Number" evaluates differently than a "Plus Sign" or a "Sine Function."

* **WHAT it is:** The Abstract Syntax Tree (AST). This is where the **A.P.I.E.** pillars (specifically Polymorphism and Encapsulation) shine.  
* **WHY we need it:** If we use a standard `TTreeNode` with giant `if/else` or `switch` statements to figure out if it's a number, variable, or operator, our code becomes a procedural nightmare. We want the tree to be smart.  
* **HOW we build it:**  
  * We create an abstract base class `TExpressionNode` with a pure virtual `virtual double Evaluate() = 0;`.  
  * We create concrete derived classes: `TNumberNode`, `TOperatorNode`, `TVariableNode`.  
  * To calculate the answer, we simply call `_root->Evaluate()`.  
  * This triggers a **Post-Order Traversal**: The `+` operator node says, "I can't add until I know my left and right." It calls `Evaluate()` on its children. The numbers bubble up from the bottom (the leaves) to the top (the root).  
* **WHERE this is used in the real world:**  
  * **Game Engines (Unity/Unreal):** Visual scripting (Node blueprints) is literally just a giant AST. When the game runs, it traverses the nodes to execute game logic.  
  * **Spreadsheet Software (Excel):** When you type `=SUM(A1:A5) * B2`, Excel builds an AST to calculate the cell's value.