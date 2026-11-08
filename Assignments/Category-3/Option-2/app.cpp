// Option 2 (Advanced): Console Music Player.

#include "menu.h"
#include "DataStructure.h"
#include "TVariable.h"
#include "TLexer.h"
#include "TParser.h"
#include "TExpressionNode.h"
#include "DebugUtils.h"
#include <iostream>
#include <sstream>

int RunApp(int /*argc*/, char* /*argv*/[]) {
	Debug::TLogger::SetLevel(Debug::ELevel::Verbose);

	// Comparator for TVariable* in AVL Tree based on variable name
	auto compareVariables = [](const TVariable* a, const TVariable* b) -> int {
		return a->Name.compare(b->Name);
		};

	// Instantiate AVL Tree as owner of TVariable pointers (true)
	TAVLTree<TVariable*>* symbolTable = new TAVLTree<TVariable*>(compareVariables, true);

	TMenu::PrintHeader("IKT203 - Category 3: Interpreted Calculator");
	TMenu::PrintCalculatorInstructions();

	while (true) {
		std::string input = TMenu::ReadString("calc>");

		// Trim or check exit
		if (input == "exit" || input == "quit") {
			break;
		}
		if (input.empty()) {
			continue;
		}

		if (input == "list") {
			TMenu::PrintHeader("Symbol Table (Variables)");
			symbolTable->TraverseInOrder([](TVariable* var) {
				std::cout << "  " << var->Name << " = " << var->Value << std::endl;
				});
			continue;
		}

		// Handle assignment: e.g., "set x = 10" or "x = 10"
		if (input.rfind("set ", 0) == 0 || input.find('=') != std::string::npos) {
			try {
				std::string assignStr = input;
				if (assignStr.rfind("set ", 0) == 0) {
					assignStr = assignStr.substr(4);
				}

				size_t eqPos = assignStr.find('=');
				if (eqPos == std::string::npos) {
					TMenu::PrintError("Invalid assignment syntax. Use 'set x = 10' or 'x = 10'.");
					continue;
				}

				std::string varName = assignStr.substr(0, eqPos);
				// Trim whitespace around varName
				varName.erase(0, varName.find_first_not_of(" \\t"));
				varName.erase(varName.find_last_not_of(" \\t") + 1);

				std::string exprStr = assignStr.substr(eqPos + 1);

				// Evaluate right-hand side expression
				TLexer lexer(exprStr);
				TQueue<TToken*>* tokens = lexer.Tokenize();
				TParser parser;
				TExpressionNode* ast = parser.Parse(tokens);
				double val = ast->Evaluate(symbolTable);
				delete ast; // Clean up AST

				// Check if variable already exists in symbol table
				TVariable searchDummy(varName, 0.0);
				TVariable* existingVar = symbolTable->Search(&searchDummy);

				if (existingVar != nullptr) {
					existingVar->Value = val;
					TMenu::PrintSuccess(varName + " updated to " + std::to_string(val));
				}
				else {
					TVariable* newVar = new TVariable(varName, val);
					symbolTable->Insert(newVar);
					TMenu::PrintSuccess(varName + " set to " + std::to_string(val));
				}
			}
			catch (const std::exception& e) {
				TMenu::PrintError(e.what());
			}
			continue;
		}

		// Handle calculation: e.g., "calc (x + 5) * 3" or direct expression
		try {
			std::string exprStr = input;
			if (exprStr.rfind("calc ", 0) == 0) {
				exprStr = exprStr.substr(5);
			}

			TLexer lexer(exprStr);
			TQueue<TToken*>* tokens = lexer.Tokenize();
			TParser parser;
			TExpressionNode* ast = parser.Parse(tokens);

			double result = ast->Evaluate(symbolTable);
			delete ast; // Clean up AST

			TMenu::PrintSuccess("Result = " + std::to_string(result));
		}
		catch (const std::exception& e) {
			TMenu::PrintError(e.what());
		}
	}

	// Clean up symbol table (deletes all TVariable pointers because _isDataOwner = true)
	delete symbolTable;

	TMenu::PrintMessage("Exiting Calculator. Goodbye!");
	return 0;
}